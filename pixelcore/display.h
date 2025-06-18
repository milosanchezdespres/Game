#pragma once

#include <GL/glew.h> 

#include <iostream>
#include <deque>
#include <numeric>

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

            GLFWwindow* window;

            std::deque<float> fps_values;
            static constexpr size_t fps_buffer_size = 10;

            Display()
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

            double lastTime;
            double currentTime;
            float frameDelta;
            float avg_fps;

            void init(const char* new_title, int new_width, int new_height, bool center_window = true, bool resizable = false)
            {
                _title = new_title;
                _width = new_width;
                _height = new_height;

                if (!glfwInit()) cerr << "Failed to init GLFW\n";
                
                if(!resizable) glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

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

            bool active()
            {
                currentTime = glfwGetTime();
                frameDelta = static_cast<float>(currentTime - lastTime);
                lastTime = currentTime;

                fps_values.push_back(static_cast<int>(1.0f / frameDelta));
                if (fps_values.size() > fps_buffer_size) fps_values.pop_front();
                avg_fps = std::accumulate(fps_values.begin(), fps_values.end(), 0.0f) / fps_values.size();

                return window && !glfwWindowShouldClose(window);
            }

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