#pragma once

#include <GLFW/glfw3.h>

#include <iostream>

#include "singleton.h"

#define SCREEN display::instance()
#define delta SCREEN.frameDelta
#define fps static_cast<int>(1.0f / delta) 

namespace px
{
    using std::cerr;

    struct display : public Singleton<display>
    {
        friend class Singleton<display>;

        private:
            const char* _title;
            int _width, _height;

            GLFWwindow* window;

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

            double lastTime;
            double currentTime;
            float frameDelta;

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
            }

            bool active()
            {
                currentTime = glfwGetTime();
                frameDelta = static_cast<float>(currentTime - lastTime);
                lastTime = currentTime;

                return window && !glfwWindowShouldClose(window);
            }

            void begin_render(float r = 0, float g = 0, float b = 0, float a = 1)
            {
                glClearColor(r, g, b, a);
                glClear(GL_COLOR_BUFFER_BIT);

                glEnable(GL_TEXTURE_2D);
            }

            //...

            //this is only for debugging
            void blit(float x, float y, float r, float g, float b, float a, int scale)
            {
                glColor4f(r, g, b, a);
                glPointSize(scale);
                glBegin(GL_POINTS);
                glVertex2f(x, y);
                glEnd();
            }

            void end_render()
            {   
                //glDisable(GL_TEXTURE_2D);

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

                lastTime = glfwGetTime();
                currentTime = glfwGetTime();
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
