#pragma once

#include "singleton.h"

#include <iostream>
#include <vector>

#include <unordered_map>
#include <typeindex>

#include <algorithm>
#include <functional>

#define ECS ecs::instance()

namespace px
{
//--------------------------------------
    using std::cout, std::endl;
    using std::vector, std::unordered_map;
    using std::type_index;

    using Entity = size_t;
    using RemoveFunc = std::function<void(Entity)>;

    const int component_pool_max_capacity = 1000;

//--------------------------------------
    template <typename T>
    struct ComponentPool
    {
        private:
            int size;

            vector<T> components;
            vector<bool> alive;
        
        public:
            vector<Entity> actives;
            
            ComponentPool()
            {
                components.resize(component_pool_max_capacity);
                alive.resize(component_pool_max_capacity, false);

                size = components.size();
            }

            void add(Entity id) 
            {
                if (id >= components.size())
                {
                    components.resize(id + 1);
                    alive.resize(id + 1, false);

                    size = components.size();
                }

                if (!alive[id])
                {
                    components[id] = T{};
                    alive[id] = true;
                    actives.push_back(id);
                }
            }

            T* get(Entity id)
            {
                if (id < components.size() && alive[id])  return &components[id];
                return nullptr;
            }

            void remove(Entity id)
            {
                if (id < size)
                {
                    alive[id] = false;

                    auto it = std::find(actives.begin(), actives.end(), id);
                    if (it != actives.end())
                    {
                        std::iter_swap(it, actives.end() - 1);
                        actives.pop_back();
                    }
                }
            }
    };

//--------------------------------------
    struct ecs : public Singleton<ecs>
    {
        friend class Singleton<ecs>;

        private:
            ecs() = default;

            size_t previous_entity_id = -1;
            unordered_map<type_index, void*> pools;

            vector<RemoveFunc> component_removers;
            unordered_map<type_index, std::function<void(void*)>> pool_deleters;

        public:
            int create_entity()
            {
                previous_entity_id++;
                return previous_entity_id;
            }

            template <typename T>
            void add_component(Entity entity)
            {
                type_index typeId = typeid(T);

                if (pools.find(typeId) == pools.end())
                {
                    pools[typeId] = new ComponentPool<T>();

                    component_removers.push_back([this](Entity e) { this->remove<T>(e); });

                    pool_deleters[typeId] = [](void* ptr) { delete static_cast<ComponentPool<T>*>(ptr); };
                }

                static_cast<ComponentPool<T>*>(pools[typeId])->add(entity);
            }

            template <typename T>
            T* component(Entity entity)
            {
                type_index typeId = typeid(T);
                if (pools.find(typeId) == pools.end()) return nullptr;
                else return static_cast<ComponentPool<T>*>(pools[typeId])->get(entity);
            }

            void remove_entity(Entity entity)
            { for (auto& remover : component_removers) remover(entity); }

            template <typename T>
            void remove(Entity entity)
            {
                type_index typeId = typeid(T);
                if (pools.find(typeId) != pools.end())
                    static_cast<ComponentPool<T>*>(pools[typeId])->remove(entity);
            }

            template<typename... Components>
            void remove_from_entity(px::Entity entity)
                { (ECS.remove<Components>(entity), ...); }

            template <typename T>
            const vector<Entity>& each()
            {
                type_index typeId = typeid(T);
                return static_cast<ComponentPool<T>*>(pools[typeId])->actives;
            }

            void clear()
            {
                for (auto& [typeId, pool_ptr] : pools) { pool_deleters[typeId](pool_ptr); }

                pools.clear();
                pool_deleters.clear();
                component_removers.clear();
                previous_entity_id = -1;
            }

            int size() { return previous_entity_id + 1; }
    };
}