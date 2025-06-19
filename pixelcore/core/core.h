#pragma once

//----------------------------------------------------------
#include "std.h"
#include "singleton.h"
#include "Colors.h"

//----------------------------------------------------------
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif

//----------------------------------------------------------
#define STB_IMAGE_IMPLEMENTATION
#include "../../build/includes/stb_image.h"

//----------------------------------------------------------
const std::string  TEXDIR = "build/debug/assets";
//...

//----------------------------------------------------------
inline void print() { std::cout << std::endl; }

template<typename T, typename... Args>
inline void print(const T& first, const Args&... rest)
{
    std::cout << first;
    if constexpr(sizeof...(rest) > 0)
    {
        std::cout << ' ';
        print(rest...);
    }
    else
    {
        std::cout << std::endl;
    }
}