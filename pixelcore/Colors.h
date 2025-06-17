#pragma once

#include <array>
#include <cstdint>
#include <ostream>

#define COLOR(str) MAKE_COLOR(HEX(str))

namespace px
{
    using Color = uint32_t;

    constexpr Color MAKE_COLOR(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    {
        return (static_cast<Color>(r) << 24)
            | (static_cast<Color>(g) << 16)
            | (static_cast<Color>(b) << 8)
            | (static_cast<Color>(a));
    }

    constexpr std::array<uint8_t, 4> RGBA(Color c)
    {
        return std::array<uint8_t, 4>{
            static_cast<uint8_t>((c >> 24) & 0xFF),
            static_cast<uint8_t>((c >> 16) & 0xFF),
            static_cast<uint8_t>((c >> 8) & 0xFF),
            static_cast<uint8_t>(c & 0xFF)
        };
    }

    constexpr std::array<float, 4> NRGBA(Color c) //NORMALIZED
    {
        return std::array<float, 4>{
            static_cast<float>((c >> 24) & 0xFF) / 255.0f,
            static_cast<float>((c >> 16) & 0xFF) / 255.0f,
            static_cast<float>((c >> 8) & 0xFF) / 255.0f,
            static_cast<float>(c & 0xFF) / 255.0f
        };
    }

    constexpr std::array<uint8_t, 4> HEX(const char* s)
    {
        auto hex = [](char c) constexpr -> uint8_t
        {
            return (c >= '0' && c <= '9') ? c - '0'
                : (c >= 'a' && c <= 'f') ? c - 'a' + 10
                : (c >= 'A' && c <= 'F') ? c - 'A' + 10
                : 0;
        };

        return
        {
            static_cast<uint8_t>((hex(s[1]) << 4) | hex(s[2])),
            static_cast<uint8_t>((hex(s[3]) << 4) | hex(s[4])),
            static_cast<uint8_t>((hex(s[5]) << 4) | hex(s[6])),
            (s[7] == '\0') ? static_cast<uint8_t>(255) : static_cast<uint8_t>((hex(s[7]) << 4) | hex(s[8]))
        };
    }

    constexpr Color MAKE_COLOR(const std::array<uint8_t,4>& arr)
        { return MAKE_COLOR(arr[0], arr[1], arr[2], arr[3]); }

}

namespace std
{
    inline std::ostream& operator<<(std::ostream& os, const std::array<uint8_t, 4>& arr)
    {
        os << "("
           << static_cast<int>(arr[0]) << ", "
           << static_cast<int>(arr[1]) << ", "
           << static_cast<int>(arr[2]) << ", "
           << static_cast<int>(arr[3]) << ")";
        return os;
    }

    inline std::ostream& operator<<(std::ostream& os, const std::array<float, 4>& arr)
    {
        os << "(" << arr[0] << ", " << arr[1] << ", " << arr[2] << ", " << arr[3] << ")";
        return os;
    }
}
