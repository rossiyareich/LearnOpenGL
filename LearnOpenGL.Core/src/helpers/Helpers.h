#pragma once
#include <cassert>
#include <cstdint>

#define decltypearraymember(array) decltype((array)[0])
#define carraysize(array) (sizeof((array)) / sizeof(decltypearraymember((array))))

template <typename T = int, T TAssertationFailed = -1>
T __assertRet(const T ptr, const uint32_t line, const char* const file, const char* const function)
{
    if (ptr == TAssertationFailed)
    {
        std::cout << "Assertation failed at LINE: " << line << ", FILE: " << file << ", FUNCTION: " << function <<
            std::endl;
        assert(false);
    }

    return ptr;
}

#define assertRet(ptr) __assertRet((ptr), __LINE__, __FILE__, __FUNCTION__)

#define OnKeyPressed(window, key) if(glfwGetKey((window).Handle, (key)) == GLFW_PRESS)
#define _OnKeyPressed(window, key) else OnKeyPressed(window, key)

template <typename T>
T map(T x, T in_min, T in_max, T out_min, T out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
