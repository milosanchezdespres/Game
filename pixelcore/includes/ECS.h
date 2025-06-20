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
            static void unregister_pool(IPool* pool) { auto it = std::remove(_pools.begin(), _pools.end(), pool); if (it != _pools.end()) _pools.erase(it, _pools.end()); }

            template<typename T>
            struct ComponentPool : IPool
            {
                std::vector<T> _data;
                std::unordered_map<size_t, int> entityToIndex;
                std::vector<size_t> indexToEntity;

                ComponentPool() { ecs::register_pool(this); }
                ~ComponentPool() { ecs::unregister_pool(this); }

                int add(size_t i, T& component)
                {
                    if (entityToIndex.find(i) != entityToIndex.end()) return entityToIndex[i];
                    int index = _data.size();

                    _data.push_back(std::move(component));

                    entityToIndex[i] = index;
                    indexToEntity.push_back(i);

                    return index;
                }

                T& get(size_t i) { return _data[entityToIndex[i]]; }

                void remove(size_t i)
                {
                    auto it = entityToIndex.find(i);
                    if (it == entityToIndex.end()) return;

                    int index = it->second;
                    int last = _data.size() - 1;

                    if (index != last)
                    {
                        _data[index] = std::move(_data[last]);
                        size_t moved = indexToEntity[last];
                        entityToIndex[moved] = index;
                        indexToEntity[index] = moved;
                    }

                    _data.pop_back();
                    indexToEntity.pop_back();

                    entityToIndex.erase(i);
                }

                void clear() override { _data.clear(); entityToIndex.clear(); indexToEntity.clear(); }
            };

            template<typename T>
            static ComponentPool<T>& get_pool() { static ComponentPool<T> pool; return pool; }

            struct view
            {
                EntityID id;
                ecs* owner;

                view() : owner(global_ecs_ptr), id(global_ecs_ptr ? global_ecs_ptr->make() : EntityID{0, 0}) {}

                template<typename... Components>
                view(Components&&... components) : owner(global_ecs_ptr), id(global_ecs_ptr ? global_ecs_ptr->make() : EntityID{0, 0})
                { (add(std::forward<Components>(components)), ...); }

                template<typename T>
                T& component() { return owner->component<T>(id); }

                template<typename T>
                int add(T&& component) { return owner->add<T>(id, component); }

                template<typename T>
                void remove() { return owner->remove<T>(id); }

                bool valid() const { return owner && owner->valid(id); }
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
    };
}
