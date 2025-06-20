#pragma once

#include "../../pixelcore/pixelcore.h"

struct Button
{
    int id;
    const char* text;
    tx::view texture;
    float x, y;
};

struct ButtonHolder
{
    std::vector<Button> buttons;

    Button& add()
    {
        int index = buttons.size();
        buttons.push_back(Button());
        buttons[index].id = index;
        return button(index);
    }

    Button& button(int index) { return buttons[index]; }
};