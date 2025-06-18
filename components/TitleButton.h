#pragma once

struct TitleButton
{
    const char* text;
    Surface surface;
};

struct TitleButtonHolder
{
    std::vector<TitleButton> buttons;

    const char* texture_alias;

    int padding;
    int texture_width, texture_height;
    int scale;

    void init(const char* _texture_alias, int _texture_width, int _texture_height, int _scale = 1, int _padding = 5)
    {
        texture_alias = _texture_alias;

        texture_width = _texture_width;
        texture_height = _texture_height;

        scale = _scale;
        padding = _padding;
    }

    void add(const char* str, int status = 0)
    {
        buttons.push_back(TitleButton());
        buttons[buttons.size() - 1].text = str;
        buttons[buttons.size() - 1].surface = {0, (float)(status * (texture_height / 2)), texture_width, texture_height / 2 };
    }

    TitleButton get(int index) { return buttons[index]; }

    void set_status(int index, int status)
        { buttons[buttons.size() - 1].surface = {0, (float)(status * (texture_height / 2)), texture_width, texture_height / 2 }; }
};