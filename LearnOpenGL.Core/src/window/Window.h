#pragma once
#include <GLFW/glfw3.h>

namespace window
{
    class Window
    {
    public:
        GLFWwindow* Handle;

        Window(const char*, int width, int height);
        ~Window();

        int WindowShouldClose() const;
        void SetWindowShouldClose(int value) const;
    };
}
