#pragma once

#include <cstdint>
#include <unordered_map>

namespace px
{
    using EntityID = uint32_t;

    struct IComponentPool
    {
        IComponentPool() = default;
        ~IComponentPool() = default;
        virtual void add(EntityID ID) {}
        virtual void remove(EntityID ID) {}
    };
    
    template<typename T>
    struct ComponentPool : public IComponentPool
    {
        std::unordered_map<EntityID, T> components;

        ComponentPool() : IComponentPool() {}

        void add(EntityID ID) override
        {
            auto it = components.find(ID);
            if(it == components.end())
                components[ID] = T();
        }

        T* component(EntityID ID)
        { 
            auto it = components.find(ID);
            return (it != components.end()) ? &it->second : nullptr; 
        }

        void remove(EntityID ID) override { components.erase(ID); }
    };
};