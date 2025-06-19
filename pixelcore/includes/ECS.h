#pragma once

#include <vector>
#include <unordered_map>
#include <utility>

namespace px
{
    using Entity = size_t;

    struct ecs;

    inline ecs* global_ecs_ptr = nullptr;

    template<typename T>
    struct ComponentPool
    {
        std::vector<T> _data;
        std::unordered_map<Entity, int> entityToIndex;

        int add(Entity e, T& component) { int index = _data.size(); _data.push_back(std::move(component)); entityToIndex[e] = index; return index; }
        T& get(Entity e) { return _data[entityToIndex[e]]; }
    };

    struct ecs
    {
        private:
            Entity _nextEntity = 0;

        public:
            ecs() { global_ecs_ptr = this; }

            template<typename T>
            ComponentPool<T>& get_pool() { static ComponentPool<T> pool; return pool; }

            Entity make() { return _nextEntity++; }

            template<typename T>
            int add(Entity e, T& component) { return get_pool<T>().add(e, component); }

            template<typename T>
            T& component(Entity e) { return get_pool<T>().get(e); }

            struct view
            {
                Entity id;
                ecs* owner;

                view() : owner(global_ecs_ptr), id(global_ecs_ptr ? global_ecs_ptr->make() : 0) {}
                template<typename... Components>
                view(Components&&... components) : owner(global_ecs_ptr), id(global_ecs_ptr ? global_ecs_ptr->make() : 0)
                { (add(std::forward<Components>(components)), ...); }

                template<typename T>
                T& component() { return owner->component<T>(id); }

                template<typename T>
                int add(T&& component) { return owner->add<T>(id, component); }
            };
    };
}
