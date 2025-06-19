#pragma once

#include "../core/core.h"

namespace px
{
    struct Display
    {
        private:
            static inline const char* _title = nullptr;
            static inline int _width = 0;
            static inline int _height = 0;

            static inline std::deque<float> fps_values;
            static constexpr size_t fps_buffer_size = 10;

            static inline std::unordered_map<int, bool> key_press_state;
            static inline std::unordered_map<int, bool> mouse_press_state;

            static inline GLFWwindow* window = nullptr;

            static inline double lastTime = 0.0;
            static inline double currentTime = 0.0;
            static inline float frameDelta = 0.f;
            static inline float avg_fps = 0.f;

            static void init_GL(bool center_window)
            {
                if(center_window) center();

                glfwMakeContextCurrent(window);
                glfwSwapInterval(1);

                init_inputs();

                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                glOrtho(0, width(), height(), 0, -1, 1);
                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();

                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

                glEnable(GL_TEXTURE_2D);

                lastTime = glfwGetTime();
                currentTime = lastTime;
                fps_values.clear();
            }

            static void init_inputs()
            {
                glfwSetKeyCallback(window, [](GLFWwindow* w, int key, int, int action, int) {
                    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
                        glfwSetWindowShouldClose(w, true);
                });

                glfwSetMouseButtonCallback(window, [](GLFWwindow*, int, int, int) { });
                glfwSetCursorPosCallback(window, [](GLFWwindow*, double, double) { });
                glfwSetScrollCallback(window, [](GLFWwindow*, double, double) { });
            }

            static void center()
            {
                GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
                const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
                glfwGetWindowSize(window, &_width, &_height);
                int xpos = (mode->width - _width) / 2;
                int ypos = (mode->height - _height) / 2;
                glfwSetWindowPos(window, xpos, ypos);
            }

        public:
            static const char* title() { return _title; }
            static int width() { return _width; }
            static int height() { return _height; }
            static GLFWwindow* get_window() { return window; }

            static void init(const char* new_title, int new_width, int new_height, bool center_window = true, bool resizable = false, bool borderless = true)
            {
                _title = new_title;
                _width = new_width;
                _height = new_height;

                if (!glfwInit()) { std::cerr << "Failed to init GLFW\n"; return; }

                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
                glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
                glfwWindowHint(GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE);
                glfwWindowHint(GLFW_DECORATED, borderless ? GLFW_FALSE : GLFW_TRUE);

                window = glfwCreateWindow(_width, _height, _title, nullptr, nullptr);

                if (!window)
                {
                    std::cerr << "Failed to create GLFW window\n";
                    glfwTerminate();
                    return;
                }

                init_GL(center_window);

                glewExperimental = GL_TRUE;
                if (glewInit() != GLEW_OK)
                    std::cerr << "Failed to initialize GLEW\n";
            }

            static void init(const char* new_title)
            {
                _title = new_title;

                if (!glfwInit()) { std::cerr << "Failed to init GLFW\n"; return; }

                GLFWmonitor* monitor = glfwGetPrimaryMonitor();
                const GLFWvidmode* mode = glfwGetVideoMode(monitor);

                _width = mode->width;
                _height = mode->height;

                glfwWindowHint(GLFW_RED_BITS, mode->redBits);
                glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
                glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
                glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

                window = glfwCreateWindow(_width, _height, _title, monitor, nullptr);

                if (!window)
                {
                    std::cerr << "Failed to create GLFW window\n";
                    glfwTerminate();
                    return;
                }

                init_GL(false);

                glewExperimental = GL_TRUE;
                if (glewInit() != GLEW_OK)
                    std::cerr << "Failed to initialize GLEW\n";
            }

            static bool active()
            {
                if (!window) return false;

                currentTime = glfwGetTime();
                frameDelta = static_cast<float>(currentTime - lastTime);
                lastTime = currentTime;

                if (frameDelta > 0.f)
                {
                    fps_values.push_back(1.0f / frameDelta);
                    if (fps_values.size() > fps_buffer_size) fps_values.pop_front();
                    avg_fps = std::accumulate(fps_values.begin(), fps_values.end(), 0.f) / fps_values.size();
                }

                for(auto& [key, pressed] : key_press_state)
                {
                    if (glfwGetKey(window, key) == GLFW_RELEASE)
                        pressed = false;
                }

                for(auto& [btn, pressed] : mouse_press_state)
                {
                    if (glfwGetMouseButton(window, btn) == GLFW_RELEASE)
                        pressed = false;
                }

                return !glfwWindowShouldClose(window);
            }

            static bool is_key_hold(int key) { return glfwGetKey(window, key) == GLFW_PRESS; }

            static bool is_key_pressed(int key)
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

            static bool is_mouse_pressed(int button) { return glfwGetMouseButton(window, button) == GLFW_PRESS; }

            static bool is_mouse_clicked(int button)
            {
                if (glfwGetMouseButton(window, button) == GLFW_PRESS)
                {
                    if (!mouse_press_state[button])
                    {
                        mouse_press_state[button] = true;
                        return true;
                    }
                    return false;
                }
                else
                {
                    mouse_press_state[button] = false;
                    return false;
                }
            }

            static bool is_mouse_held(int button)
            {
                return glfwGetMouseButton(window, button) == GLFW_PRESS;
            }

            static void mouse(double& x, double& y) { glfwGetCursorPos(window, &x, &y); }

            static void begin_render(float r = 0.f, float g = 0.f, float b = 0.f, float a = 1.f)
            {
                glClearColor(r, g, b, a);
                glClear(GL_COLOR_BUFFER_BIT);
            }

            static void end_render()
            {
                glfwSwapBuffers(window);
                glfwPollEvents();
            }

            static int exit()
            {
                if (window)
                    glfwDestroyWindow(window);
                window = nullptr;

                glfwTerminate();

                return 0;
            }
            
            static const float& delta;
            static const float& fps;
    };

    inline const float& Display::delta = Display::frameDelta;
    inline const float& Display::fps = Display::avg_fps;
}

#define HOLD(key) px::Display::is_key_hold(GLFW_KEY_##key)
#define PRESS(key) px::Display::is_key_pressed(GLFW_KEY_##key)
#define CLICK(button) px::Display::is_mouse_clicked(GLFW_MOUSE_BUTTON_##button)
#define HCLICK(button) px::Display::is_mouse_held(GLFW_MOUSE_BUTTON_##button)

#define screen Display
