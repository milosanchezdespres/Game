#pragma once

struct FPSDisplay
{
    std::string fps_str;
    const char* text;

    //for such a small component no need to use a system
    void refresh(float value)
    {
        fps_str = std::to_string(static_cast<int>(value));
        text = fps_str.c_str();
    }
};
