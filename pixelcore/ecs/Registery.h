#pragma once

#include <typeindex>
#include <memory>

#include "../singleton.h"
#include "ComponentPool.h"

#define REG Registery::instance()

namespace px
{
    struct Registery : public Singleton<Registery>
    {
        friend class Singleton<Registery>;

        EntityID next_entity_id = 0;
        std::unordered_map<std::type_index, std::unique_ptr<IComponentPool>> pools;

        private:
            Registery() = default;

        public:
            EntityID create_entity() { return next_entity_id++; }

            template<typename T>
            void add_component(EntityID ID)
            {
                type_index type = type_index(typeid(T));

                auto it = pools.find(type);
                if(it == pools.end()) pools[type] = std::make_unique<ComponentPool<T>>();

                pools[type]->add(ID);
            }

            template<typename T>
            T* get_component(EntityID ID)
            {
                auto it = pools.find(typeid(T));
                if(it == pools.end()) return nullptr;

                auto pool = static_cast<ComponentPool<T>*>(it->second.get());
                return pool->component(ID);
            }

            template<typename T>
            T* remove_component(EntityID ID)
            {
                auto it = pools.find(typeid(T));
                if(it == pools.end()) return nullptr;

                auto pool = static_cast<ComponentPool<T>*>(it->second.get());
                pool->remove(ID);
            }
    };
};