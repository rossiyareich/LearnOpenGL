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
#define assertRet(ptr) __assertRet((ptr), __LINE__, __FILE__, __FUNCTION__)
}
