#pragma once

#include <array>
#include <cstdint>
#include <cstdio>
#include <ostream>
#include <cstring>
//...

//------------------------------------------------------------
//for GLFW use // array with R, G, B, A, R, G ... layout
#define Pixels(pixel_count) array<Color, pixel_count * 4>

#define set_pixel(pixels, index, color) \
    pixels[index * 4] = unpack(color)[0];         /* R */ \
    pixels[index * 4 + 1] = unpack(color)[1];     /* G */ \
    pixels[index * 4 + 2] = unpack(color)[2];     /* B */ \
    pixels[index * 4 + 3] = unpack(color)[3];     /* A */

#define get_pixel(pixels, index) \
    pack(pixels[index * 4], pixels[index * 4 + 1], pixels[index * 4 + 2], pixels[index * 4 + 3])

//palette of 256 colors stored as a single number each
#define Palette array<Color, 256>
#define set_color(pixels, index, color) pixels[index] = color
#define get_color(pixels, index) pixels[index]

//------------------------------------------------------------
//allows you to expand in one go a color into a function
#define expand(color) unpack(color)[0], unpack(color)[1], unpack(color)[2], unpack(color)[3]

//allows you to expand in one go a color into a function with normalized values
#define nexpand(color) nunpack(color)[0], nunpack(color)[1], nunpack(color)[2], nunpack(color)[3]

//allows you to expand in one go a color into a function (but does not expand alpha)
#define expand2(color) unpack(color)[0], unpack(color)[1], unpack(color)[2]

//allows you to expand in one go a color into a function with normalized values (but does not expand alpha)
#define nexpand2(color) nunpack(color)[0], nunpack(color)[1], nunpack(color)[2]

namespace px
{
//------------------------------------------------------------
    using std::array;
    using std::uint8_t, std::uint32_t;
    using std::sprintf;
    //...

    using ub8 = uint8_t;
    using ub32 = uint32_t;

    //RGBA //packed to unsigned int 32
    using Color = ub32;

    //RGBA 255 //unpacked
    using RGBA = array<ub8, 4>;
    //RGBA normalized //unpacked
    using nRGBA = array<float, 4>;

    //array of char containing hex values
    using HEX = array<char, 9>;

//------------------------------------------------------------
/*
    pack function behavior (b. shifting)
    -------------------------------------
    Byte Index	3	2	1	0
    Color Comp	R	G	B	A
-------------------------------------------- */

    //pack rgba values to a single ub32
    constexpr Color pack(ub8 R, ub8 G, ub8 B, ub8 A = 255) { return (static_cast<ub32>(R) << 24) | (static_cast<ub32>(G) << 16) | (static_cast<ub32>(B) << 8) | static_cast<ub32>(A); }
    //pack rgba color to a single ub32
    constexpr Color pack(RGBA color) { return pack(color[0], color[1], color[2], color[3]); }
    //pack normalized color to a single ub32
    constexpr Color pack(nRGBA color)
    {
        return pack
        (
            static_cast<ub8>(color[0] * 255),
            static_cast<ub8>(color[1] * 255),
            static_cast<ub8>(color[2] * 255),
            static_cast<ub8>(color[3] * 255)
        );
    }

    //get a ub32 color from hex value
    constexpr Color pack(const char* hex)
    {
        int r = 0, g = 0, b = 0, a = 0;

        if(hex[0] == '#') hex++;

        int len = std::strlen(hex);
        if(len == 6)
        {
            sscanf(hex, "%2x%2x%2x", &r, &g, &b);
            a = 255;
        }
        else if(len == 8) sscanf(hex, "%2x%2x%2x", &r, &g, &b);
        else r = g = b = a = 0;

        return pack(r, g, b, a);
    }

    constexpr Color pack(HEX hex) { return pack(hex.data()); }

    //unpack color to rgba values
    constexpr RGBA unpack(Color color)
    {
        return 
        {
            static_cast<ub8>((color >> 24) & 0xFF),
            static_cast<ub8>((color >> 16) & 0xFF),
            static_cast<ub8>((color >>  8)  & 0xFF),
            static_cast<ub8>((color      )  & 0xFF),
        };
    }

    //unpack color to normalized values
    constexpr nRGBA nunpack(Color color)
    {
        return 
        {
            ((color >> 24) & 0xFF) / 255.0f,
            ((color >> 16) & 0xFF) / 255.0f,
            ((color >>  8) & 0xFF) / 255.0f,
            ((color      ) & 0xFF) / 255.0f
        };
    }

    //give hex string for given ub32 color
    constexpr HEX hex(Color color)
    {
        auto to_hex_char = [](unsigned v) constexpr -> char { return (v < 10) ? ('0' + v) : ('A' + (v - 10)); };

        RGBA rgba = unpack(color);

        return HEX{
            to_hex_char(rgba[0] >> 4), to_hex_char(rgba[0] & 0xF),
            to_hex_char(rgba[1] >> 4), to_hex_char(rgba[1] & 0xF),
            to_hex_char(rgba[2] >> 4), to_hex_char(rgba[2] & 0xF),
            to_hex_char(rgba[3] >> 4), to_hex_char(rgba[3] & 0xF),
            '\0'
        };
    }

    //give hex string for given rgba color
    constexpr HEX hex(RGBA color) { return hex(pack(color)); }

    //give hex string for given normalized color
    constexpr HEX hex(nRGBA color) { return hex(pack(color)); }
}

/* ---------- overloading cout << for unit tests / ! \ DEBUG ONLY / ! \ ---------- */
namespace std
{
    using namespace px;
    
    ostream& operator<<(ostream& os, const RGBA& color)
    {
        os <<       to_string(color[0])
        + ", " +    to_string(color[1])
        + ", " +    to_string(color[2])
        + ", " +    to_string(color[3]);

        return os;
    }
    
    ostream& operator<<(ostream& os, const nRGBA& color)
    {
        os <<       to_string(color[0])
        + ", " +    to_string(color[1])
        + ", " +    to_string(color[2])
        + ", " +    to_string(color[3]);
        
        return os;
    }
    
    ostream& operator<<(ostream& os, const HEX& hexstr) { os << hexstr.data(); return os;  }
}