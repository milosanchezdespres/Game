#pragma once

namespace components
{
    enum class StateValue
    {
        idle,
        walk,
        debug
    };

    struct State
    {
        StateValue value;
    };
}