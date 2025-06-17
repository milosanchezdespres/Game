#pragma once

#include <iostream>
#include <cstdint>
#include <unordered_map>
#include <typeindex>
#include <memory>

#include "singleton.h"

#define MAKE_ENTITY {ECS().create_entity()}

namespace px
{
    using EntityID = size_t;

    struct IComponentPool
    {
        IComponentPool() = default;
        virtual ~IComponentPool() = default;

        virtual void add(EntityID ID) {}
        virtual void remove(EntityID ID) {}
    };

    template<typename T> struct ComponentPool : public IComponentPool
    {
        std::unordered_map<EntityID, T> components;

        ComponentPool() : IComponentPool() {}

        ~ComponentPool()
        {
            for(auto& [entity, component] : components)
                remove(entity);
        }

        void add(EntityID ID) override
        {
            auto it = components.find(ID);
            if(it == components.end()) components[ID] = T();
        }

        T* get(EntityID ID)
        {
            auto it = components.find(ID);
            if(it == components.end()) return nullptr;
            
            return &components[ID];
        }

        void remove(EntityID ID) override
        {
            auto it = components.find(ID);
            if(it != components.end()) components.erase(ID);
        }
    };

    struct ecs : public Singleton<ecs>
    {
        friend class Singleton<ecs>;
        
        private: EntityID entity_next_ID = 0;

        public:
            std::unordered_map<std::type_index, std::unique_ptr<IComponentPool>> pools;

            EntityID create_entity() { return entity_next_ID++; }

            template<typename T> void add(EntityID ID)
            {
                std::type_index index(typeid(T));
                auto it = pools.find(index);

                if(it == pools.end()) pools[index] = std::make_unique<ComponentPool<T>>();
                pools[index]->add(ID);
            }

            template<typename T> T* get(EntityID entity)
            {
                std::type_index index(typeid(T));
                auto it = pools.find(index);

                if(it == pools.end()) return nullptr;
                else return static_cast<ComponentPool<T>*>(it->second.get())->get(entity);
            }

            template<typename T> void remove(EntityID entity)
            {
                std::type_index index(typeid(T));
                auto it = pools.find(index);

                if(it != pools.end()) static_cast<ComponentPool<T>*>(it->second.get())->remove(entity);
            }

        private:
            ecs() = default;
    };

    struct Entity
    {
        EntityID ID;

        template<typename T> void add() { ecs::instance().add<T>(ID); }
        template<typename T> T* component() { return ecs::instance().get<T>(ID); }
        template<typename T> void remove() { ecs::instance().remove<T>(ID); }
    };
}

DEFINE_SINGLETON_ACCESSOR(px::ecs, ECS);