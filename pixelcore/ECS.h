#pragma once

#include "singleton.h"

#include <iostream>
#include <vector>

#include <unordered_map>
#include <typeindex>

#include <algorithm>

#define ECS ecs::instance()

namespace px
{
//--------------------------------------
    using std::cout, std::endl;
    using std::vector, std::unordered_map;
    using std::type_index;

    using Entity = size_t;

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
            unordered_map<type_index, void*> pools;

        public:
            //....
    };
}