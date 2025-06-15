#pragma once

#include "singleton.h"

#define SCREEN display::instance()

namespace px
{
    struct display : public Singleton<display>
    {
        friend class Singleton<display>;

    private:
        const char* _title;
        int _width, _height;

        display()
            : _title(nullptr)
            , _width(0)
            , _height(0)
            , title(_title)
            , width(_width)
            , height(_height)
        {}

    public:
        const char* const& title;
        const int& width;
        const int& height;

        void init(const char* new_title, int new_width, int new_height)
        {
            _title = new_title;
            _width = new_width;
            _height = new_height;
        }
    };
}
