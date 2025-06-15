#pragma once

#include "../pixelcore/ECS.h"
#include "../pixelcore/GameMode.h"
using namespace px;

#include <iostream>

struct versusCPU : public GameMode
{
    versusCPU() : GameMode() {}
    ~versusCPU(){}

    void enter() override
    {
        cout << "enter" << endl;
        //...
    }

    void render() override
    {
        cout << "render" << endl;
        //...
    }

    void exit() override
    {
        cout << "exit" << endl;
        //...
    }
};