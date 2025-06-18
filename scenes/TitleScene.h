#pragma once

#include "../pixelcore/GameBoard.h"

#include "../components/Button.h"
#include "../components/FPSDisplay.h"

struct TitleScene : public IGameLoop
{
    Entity UI;

    TitleScene() : IGameLoop() {}

    void start() override
    {
        BGCOLOR = NCOLOR("#004b8f");

        load_texture_as("button1", "ui/button1");

        UI = MAKE_ENTITY;
        UI.add<ButtonHolder>();
        UI.add<FPSDisplay>();

        UI.component<ButtonHolder>()->init("button1", texture("button1")->width(), texture("button1")->height(), 2, 10);

        UI.component<ButtonHolder>()->add("new");
        UI.component<ButtonHolder>()->add("debug");
        UI.component<ButtonHolder>()->add("quit");
    }

    void update() override
    {
        UI.component<FPSDisplay>()->refresh(FPS);

        if (PRESS(SPACE)) GOTO("DebugScene");

        //...
    }
 
    void render() override
    {
        auto holder = UI.component<ButtonHolder>();
        auto fps = UI.component<FPSDisplay>();

        int btn_width = texture("button1")->width();
        int btn_height = texture("button1")->height() / 2;

        int btn_vertical_margin = 25;

        int btn_count = holder->buttons.size();
    
        float offsety = 0;
        float offset_increment_value = btn_height + (holder->scale * btn_vertical_margin);

        int centerx = (SCREEN().width / 2) - (holder->scale * (btn_width / 2));
        int centery = (SCREEN().height / 2) - (btn_count * (holder->scale * (btn_height / 2)));

        int btnx = 0, btny = 0;
        int paddingx = 0, paddingy = 0;

        for(Button btn : holder->buttons)
        {
            btnx = centerx;
            btny = centery + offsety;

            paddingx = (holder->texture_width / 2) - (holder->scale * holder->padding);
            paddingy = (holder->texture_height / 2) - (paddingx / 4);

            BLIT(texture(holder->texture_alias), btnx, btny, holder->scale, btn.surface);
            FONT->write(btn.text, btnx + paddingx, btny + paddingy, holder->scale + 1);

            offsety += offset_increment_value;
        }

        FONT->write(fps->text, 20, 20, holder->scale + 1);
    }

    void exit() override
    {
        //...
    }
};
