#pragma once

#include <array>

#define MAKE_COLOR(r, g, b, a) (r << 24) | (g << 16) | (b << 8) | a
#define RGBA(c) std::array<uint8_t, 4> \
{ \
    static_cast<uint8_t>((c >> 24) & 0xFF), \
    static_cast<uint8_t>((c >> 16) & 0xFF), \
    static_cast<uint8_t>((c >> 8) & 0xFF),  \
    static_cast<uint8_t>((c) & 0xFF)         \
}

namespace px
{
    using Color = uint32_t;

    //...
};

namespace std
{
    std::ostream& operator<<(std::ostream& os, const std::array<uint8_t,4>& arr)
    {
        os << "("
        << static_cast<int>(arr[0]) << ", "
        << static_cast<int>(arr[1]) << ", "
        << static_cast<int>(arr[2]) << ", "
        << static_cast<int>(arr[3]) << ")";
        return os;
    }
}