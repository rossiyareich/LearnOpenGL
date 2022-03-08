#include <cassert>

#include <glew/glew.h>

#include <GLFW/glfw3.h>
#include "Window.h"

namespace window
{
    Window::Window(const char* const name, const int width, const int height) : Handle{}
    {
        assert(glewInit());
        assert(glfwInit());

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        /* Create a windowed mode window and its OpenGL context */
        Handle = glfwCreateWindow(width, height, name, nullptr, nullptr);
        if (!Handle)
        {
            glfwTerminate();
            assert(false);
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(Handle);
        assert(glewInit() == GLEW_OK);
    }

    Window::~Window()
    {
        glfwTerminate();
    }

    int Window::WindowShouldClose() const
    {
        return glfwWindowShouldClose(Handle);
    }

    void Window::SetWindowShouldClose(int value) const
    {
        glfwSetWindowShouldClose(Handle, value);
    }
}
