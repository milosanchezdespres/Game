#pragma once

#include "singleton.h"

#include <iostream>
#include <vector>
#include <unordered_map>
#include <typeindex>

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
        
        public:
            vector<T> components;
            vector<bool> alive;
            
            ComponentPool()
            {
                components.reserve(component_pool_max_capacity);
                alive.reserve(component_pool_max_capacity);
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

                components[id] = T{};
                alive[id] = true;
            }

            T* get(Entity id)
            {
                if (id < components.size() && alive[id])  return &components[id];
                return nullptr;
            }

            void remove(Entity id) { if (id < size) { alive[id] = false; } }
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