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

inline std::tuple<float, float, float> HTMLToRGBFloat(const uint32_t htmlColor)
{
    return {
        static_cast<float>((htmlColor & 0xFF0000) >> 16) / 255.0f,
        static_cast<float>(((htmlColor & 0xFF00) >> 8)) / 255.0f,
        static_cast<float>((htmlColor & 0xFF)) / 255.0f
    };
}

/*! \brief Convert HSV to RGB color space

  Converts a given set of HSV values `h', `s', `v' into RGB
  coordinates. The output RGB values are in the range [0, 1], and
  the input HSV values are in the ranges h = [0, 360], and s, v =
  [0, 1], respectively.

  \param fR Red component, used as output, range: [0, 1]
  \param fG Green component, used as output, range: [0, 1]
  \param fB Blue component, used as output, range: [0, 1]
  \param fH Hue component, used as input, range: [0, 360]
  \param fS Hue component, used as input, range: [0, 1]
  \param fV Hue component, used as input, range: [0, 1]

*/
void HSVtoRGB(float& fR, float& fG, float& fB, float fH, float fS, float fV)
{
    float fC = fV * fS; // Chroma
    float fHPrime = fmod(fH / 60.0, 6);
    float fX = fC * (1 - fabs(fmod(fHPrime, 2) - 1));
    float fM = fV - fC;

    if (0 <= fHPrime && fHPrime < 1)
    {
        fR = fC;
        fG = fX;
        fB = 0;
    }
    else if (1 <= fHPrime && fHPrime < 2)
    {
        fR = fX;
        fG = fC;
        fB = 0;
    }
    else if (2 <= fHPrime && fHPrime < 3)
    {
        fR = 0;
        fG = fC;
        fB = fX;
    }
    else if (3 <= fHPrime && fHPrime < 4)
    {
        fR = 0;
        fG = fX;
        fB = fC;
    }
    else if (4 <= fHPrime && fHPrime < 5)
    {
        fR = fX;
        fG = 0;
        fB = fC;
    }
    else if (5 <= fHPrime && fHPrime < 6)
    {
        fR = fC;
        fG = 0;
        fB = fX;
    }
    else
    {
        fR = 0;
        fG = 0;
        fB = 0;
    }

    fR += fM;
    fG += fM;
    fB += fM;
}
