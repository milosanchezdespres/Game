#pragma once

#define MAKE_COLOR(r, g, b, a) (r << 24) | (g << 16) | (b << 8) | a

namespace px
{
    using Color = uint32_t;

    //...
};