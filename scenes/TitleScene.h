#pragma once

#include "../pixelcore/GameBoard.h"

#include "../components/TitleButton.h"
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
        UI.add<TitleButtonHolder>();
        UI.add<FPSDisplay>();

        UI.component<TitleButtonHolder>()->init("button1", texture("button1")->width(), texture("button1")->height(), 2, 10);

        UI.component<TitleButtonHolder>()->add("new");
        UI.component<TitleButtonHolder>()->add("debug");
        UI.component<TitleButtonHolder>()->add("quit");
    }

    void update() override
    {
        auto holder = UI.component<TitleButtonHolder>();

        UI.component<FPSDisplay>()->refresh(FPS);

        if (CLICK(LEFT) && holder->selected_button == 0) GOTO("DebugScene");
        if (CLICK(LEFT) && holder->selected_button == 1) GOTO("DebugScene");
        if (CLICK(LEFT) && holder->selected_button == 2) EXIT;

        holder->free();
    }
 
    void render() override
    {
        auto holder = UI.component<TitleButtonHolder>();
        auto fps = UI.component<FPSDisplay>();

        int btn_width = texture("button1")->width();
        int btn_height = texture("button1")->height() / 2;

        int btn_vertical_margin = 5;

        int btn_count = holder->buttons.size();
        
        float offsety = 0;
        float offset_increment_value = holder->scale * (btn_height + btn_vertical_margin);

        int centerx = (SCREEN().width / 2) - (holder->scale * (btn_width / 2));
        int centery = (SCREEN().height / 2) - ((btn_count * offset_increment_value) / 2);

        int btnx = 0, btny = 0;
        int paddingx = 0, paddingy = 0;

        int hovered_index = -1;
        int index = 0;

        for(TitleButton btn : holder->buttons)
        {
            btnx = centerx;
            btny = centery + offsety;

            paddingx = (holder->texture_width / 2) - (holder->scale * holder->padding);
            paddingy = (holder->texture_height / 2) - (paddingx / 4);

            int is_hovered = CURSOR.x >= btnx && CURSOR.x <= btnx + (holder->scale * btn_width) && CURSOR.y >= btny && CURSOR.y <= btny + (holder->scale * btn_height);

            if(is_hovered)
                hovered_index = index;

            holder->set_status(index, is_hovered);

            BLIT(texture(holder->texture_alias), btnx, btny, holder->scale, btn.surface);
            FONT->write(btn.text, btnx + paddingx, btny + paddingy, holder->scale + 1);

            offsety += offset_increment_value;

            index++;
        }

        holder->selected_button = hovered_index;

        FONT->write(fps->text, 20, 20, holder->scale + 1);
    }

    void exit() override
    {
        //...
    }
};
