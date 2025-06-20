#pragma once

#include "../core/core.h"

namespace px
{
    struct EntityID
    {
        size_t index;
        size_t version;
        bool operator==(const EntityID& other) const { return index == other.index && version == other.version; }
    };

    struct ecs;
    inline ecs* global_ecs_ptr = nullptr;

    struct IPool
    {
        virtual void clear() = 0;
        virtual void remove(size_t index) = 0;
        virtual ~IPool() {}
    };

    struct ecs
    {
        private:
            size_t _nextIndex = 0;
            std::vector<size_t> _versions;
            std::vector<size_t> _freeIndices;
            static inline std::vector<IPool*> _pools;

        public:
            ecs() { global_ecs_ptr = this; }

            static void register_pool(IPool* pool) { _pools.push_back(pool); }
            static void unregister_pool(IPool* pool)
            {
                auto it = std::remove(_pools.begin(), _pools.end(), pool);
                if (it != _pools.end()) _pools.erase(it, _pools.end());
            }

            template<typename T>
            struct ComponentPool : IPool
            {
                T* _data = nullptr;
                size_t _capacity = 0;
                size_t _size = 0;
                std::vector<size_t> dense;
                std::vector<size_t> sparse;
                std::vector<size_t> freeSlots;

                ComponentPool()
                {
                    ecs::register_pool(this);
                    _capacity = 1024;
                    _data = (T*) ::operator new(sizeof(T) * _capacity);
                }

                ~ComponentPool()
                {
                    clear();
                    ::operator delete(_data);
                    ecs::unregister_pool(this);
                }

                void clear() override
                {
                    for (size_t i = 0; i < _size; i++) _data[i].~T();
                    dense.clear();
                    sparse.clear();
                    sparse.resize(0);
                    freeSlots.clear();
                    _size = 0;
                }

                bool has(size_t entity)
                {
                    if (entity >= sparse.size()) return false;
                    size_t index = sparse[entity];
                    return index < _size && index < dense.size() && dense[index] == entity;
                }

                void grow()
                {
                    size_t newCapacity = _capacity * 2;
                    T* newData = (T*) ::operator new(sizeof(T) * newCapacity);
                    for (size_t i = 0; i < _size; i++) new (newData + i) T(std::move(_data[i]));
                    for (size_t i = 0; i < _size; i++) _data[i].~T();
                    ::operator delete(_data);
                    _data = newData;
                    _capacity = newCapacity;
                }

                int add(size_t entity, T& component)
                {
                    if (has(entity)) return (int)sparse[entity];

                    if (entity >= sparse.size()) sparse.resize(entity + 1, size_t(-1));

                    if (!freeSlots.empty())
                    {
                        size_t slot = freeSlots.back();
                        freeSlots.pop_back();

                        new(_data + slot) T(std::move(component));

                        if (slot >= dense.size()) dense.resize(slot + 1);
                        dense[slot] = entity;
                        sparse[entity] = slot;

                        return (int)slot;
                    }

                    if (_size >= _capacity) grow();

                    new(_data + _size) T(std::move(component));
                    if (_size >= dense.size()) dense.resize(_size + 1);
                    dense[_size] = entity;

                    sparse[entity] = _size;

                    return (int)(_size++);
                }

                T& get(size_t entity) { return _data[sparse[entity]]; }

                void remove(size_t entity)
                {
                    if (!has(entity)) return;

                    size_t index = sparse[entity];
                    _data[index].~T();

                    size_t last = _size - 1;
                    if (index != last)
                    {
                        _data[index] = std::move(_data[last]);
                        dense[index] = dense[last];
                        sparse[dense[index]] = index;
                    }

                    dense.pop_back();
                    sparse[entity] = size_t(-1);
                    _size--;

                    freeSlots.push_back(index);
                }
            };

            template<typename T>
            static ComponentPool<T>& get_pool() { static ComponentPool<T> pool; return pool; }

            struct view
            {
                EntityID id = { size_t(-1), size_t(-1) };
                ecs* owner = nullptr;

                view() { if (!global_ecs_ptr) return; owner = global_ecs_ptr; id = owner->make(); }

                view(std::nullptr_t)
                    : id{size_t(-1), size_t(-1)}, owner(nullptr)
                {}

                template<typename... Components>
                view(Components&&... components)
                {
                    if (!global_ecs_ptr) return;
                    owner = global_ecs_ptr;
                    id = owner->make();
                    if (valid()) (add(std::forward<Components>(components)), ...);
                }

                view(const view& other)
                    : id(other.id), owner(other.owner)
                {}

                view& operator=(const view& other)
                {
                    if (this == &other) return *this;
                    id = other.id;
                    owner = other.owner;
                    return *this;
                }

                view(view&& other) noexcept
                    : id(other.id), owner(other.owner)
                {
                    other.id = { size_t(-1), size_t(-1) };
                    other.owner = nullptr;
                }

                view& operator=(view&& other) noexcept
                {
                    if (this != &other)
                    {
                        id = other.id;
                        owner = other.owner;
                        other.id = { size_t(-1), size_t(-1) };
                        other.owner = nullptr;
                    }
                    return *this;
                }

                template<typename T>
                T& component()
                {
                    if (!valid()) throw std::runtime_error("invalid view component access");
                    return owner->component<T>(id);
                }

                template<typename T>
                int add(T&& component)
                {
                    if (!valid()) throw std::runtime_error("invalid view add");
                    return owner->add<T>(id, component);
                }

                template<typename T>
                void remove()
                {
                    if (!valid()) return;
                    owner->remove<T>(id);
                }

                bool valid() const
                {
                    return owner && owner->valid(id);
                }
            };

            EntityID make()
            {
                if (!_freeIndices.empty())
                {
                    size_t i = _freeIndices.back();
                    _freeIndices.pop_back();
                    return {i, _versions[i]};
                }

                size_t i = _nextIndex++;
                _versions.push_back(0);
                return {i, 0};
            }

            static void destroy(EntityID e)
            {
                if (!global_ecs_ptr) return;
                if (e.index < global_ecs_ptr->_versions.size())
                {
                    for (auto* pool : _pools) pool->remove(e.index);
                    global_ecs_ptr->_versions[e.index]++;
                    global_ecs_ptr->_freeIndices.push_back(e.index);
                }
            }

            static void destroy(const view& v) { destroy(v.id); }

            static bool valid(EntityID e)
            {
                if (!global_ecs_ptr) return false;
                return e.index < global_ecs_ptr->_versions.size() && global_ecs_ptr->_versions[e.index] == e.version;
            }

            static bool valid(const view& v) { return valid(v.id); }

            template<typename T>
            static int add(EntityID e, T& component) { return get_pool<T>().add(e.index, component); }

            template<typename T>
            static T& component(EntityID e) { return get_pool<T>().get(e.index); }

            template<typename T>
            static void remove(EntityID e) { get_pool<T>().remove(e.index); }

            static void clear() { for (auto* pool : _pools) pool->clear(); }

            template<typename... Components>
            struct query
            {
                using Pools = std::tuple<ComponentPool<Components>*...>;
                Pools pools;

                query()
                    : pools(&get_pool<Components>()...)
                {}

                struct iterator
                {
                    query* parent;
                    size_t pos = 0;
                    size_t end = 0;

                    iterator(query* q, size_t start)
                        : parent(q), pos(start)
                    {
                        end = std::get<0>(parent->pools)->_size;
                        for (auto pool_ptr : { std::get<ComponentPool<Components>*>(parent->pools)... })
                            if (pool_ptr->_size < end)
                                end = pool_ptr->_size;
                        advance_to_valid();
                    }

                    void advance_to_valid()
                    {
                        while (pos < end)
                        {
                            size_t entity = std::get<0>(parent->pools)->dense[pos];
                            bool all_have = true;
                            ((all_have = all_have && std::get<ComponentPool<Components>*>(parent->pools)->has(entity)), ...);
                            if (all_have) break;
                            ++pos;
                        }
                    }

                    bool operator!=(const iterator& other) const { return pos != other.pos; }

                    void operator++()
                    {
                        ++pos;
                        advance_to_valid();
                    }

                    auto operator*()
                    {
                        size_t entity = std::get<0>(parent->pools)->dense[pos];
                        return std::forward_as_tuple(std::get<ComponentPool<Components>*>(parent->pools)->_data[std::get<ComponentPool<Components>*>(parent->pools)->sparse[entity]]...);
                    }
                };

                iterator begin() { return iterator(this, 0); }
                iterator end() { return iterator(this, std::get<0>(pools)->_size); }
            };

            template<typename... Components> query<Components...> make_query() { return query<Components...>(); }

            struct EntityViewFactory
            {
                struct ArgsBase { virtual ~ArgsBase() {} };

                void bake(ecs::view& out_view)
                {
                    out_view = ecs::view();
                    _on_base_bake_(out_view);
                    _on_bake_(out_view, nullptr);
                }

                template<typename... Args>
                void bake(ecs::view& out_view, Args&&... args)
                {
                    struct ArgsImpl : ArgsBase
                    {
                        std::tuple<std::decay_t<Args>...> data;
                        ArgsImpl(Args&&... args) : data(std::forward<Args>(args)...) {}
                    };

                    ArgsImpl packed(std::forward<Args>(args)...);

                    out_view = ecs::view();
                    _on_base_bake_(out_view);
                    _on_bake_(out_view, &packed);
                }

                void render(ecs::view& out_view) { _on_render_(out_view); }

            protected:
                virtual void _on_base_bake_(ecs::view& out_view) {}
                virtual void _on_bake_(ecs::view& out_view, ArgsBase* args) = 0;
                virtual void _on_render_(ecs::view& out_view) {}
            };

    };

    inline ecs global_ecs_instance;
}

/* example of iteration usages :
auto q = ecs::make_query<Component1, Component2>();

for (auto [comp1, comp2] : q)
{
    // Access the components directly
} */