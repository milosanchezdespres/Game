#pragma once

#include <GLFW/glfw3.h>

#include "singleton.h"

#define SCREEN display::instance()

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

            double lastTime;
            double currentTime;

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
                lastTime = currentTime;
                currentTime = glfwGetTime();

                return window && !glfwWindowShouldClose(window);
            }

            void begin_render(float r = 0, float g = 0, float b = 0, float a = 1)
            {
                glClearColor(r, g, b, a);
                glClear(GL_COLOR_BUFFER_BIT);

                glEnable(GL_TEXTURE_2D);
            }

            void end_render()
            {   
                //glDisable(GL_TEXTURE_2D);

                glfwSwapBuffers(window);
                glfwPollEvents();
            }

            void exit()
            {
                if (window) glfwDestroyWindow(window);

                window = nullptr;

                glfwTerminate();
            }

        private:
            void init_GL(bool center_window)
            {
                if(center_window) center();

                glfwMakeContextCurrent(window);

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
