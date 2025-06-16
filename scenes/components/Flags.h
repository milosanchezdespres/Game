#pragma once

//bunch of flags that can change systems' behavior toward one component

namespace components
{
    enum class EntityType
    {
        player,
        npc,
        none
    };

    struct Flags
    {
        EntityType type;
        bool enabled;
        //...
    };
}