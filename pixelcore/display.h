#pragma once

#include <GL/glew.h> 

#include <iostream>
#include <deque>
#include <numeric>
#include <unordered_map>

#include "singleton.h"
#include "Texture.h"

namespace px
{
    using std::cerr;

    struct Display : public Singleton<Display>
    {
        friend class Singleton<Display>;

    private:
        const char* _title;
        int _width, _height;

        std::deque<float> fps_values;
        static constexpr size_t fps_buffer_size = 10;

        std::unordered_map<int, bool> key_press_state;

        Display()
            : _title(nullptr)
            , _width(0)
            , _height(0)
            , title(_title)
            , width(_width)
            , height(_height)
        {}

    public:
        GLFWwindow* window;

        const char* const& title;
        const int& width;
        const int& height;

        double lastTime;
        double currentTime;
        float frameDelta;
        float avg_fps;

        void init(const char* new_title, int new_width, int new_height, bool center_window = true, bool resizable = false, bool borderless = true)
        {
            _title = new_title;
            _width = new_width;
            _height = new_height;

            if (!glfwInit()) cerr << "Failed to init GLFW\n";

            if(!resizable) glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
            if(borderless) glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

            window = glfwCreateWindow(width, height, title, nullptr, nullptr);

            if (!window)
            {
                std::cerr << "Failed to create GLFW window\n";
                glfwTerminate();
            }

            init_GL(center_window);

            glewExperimental = GL_TRUE;
            if (glewInit() != GLEW_OK)
                std::cerr << "Failed to initialize GLEW\n";
        }

        void init(const char* new_title)
        {
            _title = new_title;

            if (!glfwInit()) cerr << "Failed to init GLFW\n";

            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);

            _width = mode->width;
            _height = mode->height;

            glfwWindowHint(GLFW_RED_BITS, mode->redBits);
            glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
            glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
            glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

            window = glfwCreateWindow(width, height, title, monitor, nullptr);

            if (!window)
            {
                std::cerr << "Failed to create GLFW window\n";
                glfwTerminate();
            }

            init_GL(false);

            glewExperimental = GL_TRUE;
            if (glewInit() != GLEW_OK)
                std::cerr << "Failed to initialize GLEW\n";
        }

        bool active()
        {
            currentTime = glfwGetTime();
            frameDelta = static_cast<float>(currentTime - lastTime);
            lastTime = currentTime;

            fps_values.push_back(static_cast<int>(1.0f / frameDelta));
            if (fps_values.size() > fps_buffer_size) fps_values.pop_front();
            avg_fps = std::accumulate(fps_values.begin(), fps_values.end(), 0.0f) / fps_values.size();

            for(auto& [key, pressed] : key_press_state)
            {
                if (glfwGetKey(window, key) == GLFW_RELEASE)
                    pressed = false;
            }

            return window && !glfwWindowShouldClose(window);
        }

        bool is_key_hold(int key)
            { return glfwGetKey(window, key) == GLFW_PRESS; }

        bool is_key_pressed(int key)
        {
            if (glfwGetKey(window, key) == GLFW_PRESS)
            {
                if (!key_press_state[key])
                {
                    key_press_state[key] = true;
                    return true;
                }
                return false;
            }
            else
            {
                key_press_state[key] = false;
                return false;
            }
        }

        bool is_mouse_pressed(int button)
            { return glfwGetMouseButton(window, button) == GLFW_PRESS; }

        void mouse(double& x, double& y) { glfwGetCursorPos(window, &x, &y); }

        void begin_render(float r = 0, float g = 0, float b = 0, float a = 1)
        {
            glClearColor(r, g, b, a);
            glClear(GL_COLOR_BUFFER_BIT);
        }

        void blit(Texture* texture, float x, float y, int scale = 1, Surface surface = { 0, 0, 0, 0 })
        {
            if (surface.width == 0 || surface.height == 0)
            {
                surface.x = 0;
                surface.y = 0;
                surface.width = texture->width();
                surface.height = texture->height();
            }

            float w = surface.width;
            float h = surface.height;

            glBindTexture(GL_TEXTURE_2D, texture->ID());

            float u0 = surface.x / static_cast<float>(texture->width());
            float v0 = surface.y / static_cast<float>(texture->height());
            float u1 = (surface.x + surface.width) / static_cast<float>(texture->width());
            float v1 = (surface.y + surface.height) / static_cast<float>(texture->height());

            glBegin(GL_QUADS);
                glTexCoord2f(u0, v0); glVertex2f(x, y);
                glTexCoord2f(u1, v0); glVertex2f(x + w * scale, y);
                glTexCoord2f(u1, v1); glVertex2f(x + w * scale, y + h * scale);
                glTexCoord2f(u0, v1); glVertex2f(x, y + h * scale);
            glEnd();

            glBindTexture(GL_TEXTURE_2D, 0);
        }

        void end_render()
        {
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        int exit()
        {
            if (window) glfwDestroyWindow(window);

            window = nullptr;

            glfwTerminate();

            return 0;
        }

    private:
        void init_GL(bool center_window)
        {
            if(center_window) center();

            glfwMakeContextCurrent(window);
            glfwSwapInterval(1);

            init_inputs();

            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(0, width, height, 0, -1, 1);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glEnable(GL_TEXTURE_2D);

            lastTime = glfwGetTime();
            currentTime = glfwGetTime();
            fps_values.clear();
        }

        void init_inputs()
        {
            glfwSetKeyCallback(window, [](GLFWwindow* w, int key, int scancode, int action, int mods) {
                if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
                    glfwSetWindowShouldClose(w, true);
            });

            glfwSetMouseButtonCallback(window, [](GLFWwindow*, int button, int action, int mods) { });

            glfwSetCursorPosCallback(window, [](GLFWwindow*, double xpos, double ypos) { });

            glfwSetScrollCallback(window, [](GLFWwindow*, double xoffset, double yoffset) { });
        }

        void center()
        {
            GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
            glfwGetWindowSize(window, &_width, &_height);
            int xpos = (mode->width - width) / 2;
            int ypos = (mode->height - height) / 2;
            glfwSetWindowPos(window, xpos, ypos);
        }
    };
}

DEFINE_SINGLETON_ACCESSOR(px::Display, SCREEN);

#define DELTA SCREEN().frameDelta
#define FPS SCREEN().avg_fps

#define BLIT SCREEN().blit

#define HOLD(key) SCREEN().is_key_hold(GLFW_KEY_##key)
#define PRESS(key) SCREEN().is_key_pressed(GLFW_KEY_##key)
#define CLICK(BTN) SCREEN().is_mouse_pressed(GLFW_MOUSE_BUTTON_##BTN)

#define CURSOR []() { double x = 0, y = 0; SCREEN().mouse(x, y); struct { double x, y; } pos = {x, y}; return pos; }()
