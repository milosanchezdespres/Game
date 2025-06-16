#pragma once

#include "Registery.h"

namespace px
{
    struct Entity
    {
        EntityID ID;

        template<typename T> void add() { REG.add_component<T>(ID); }
        template<typename T> T* get() { return REG.get_component<T>(ID); }
        template<typename T> T* remove() { return REG.remove_component<T>(ID); }
    };
}