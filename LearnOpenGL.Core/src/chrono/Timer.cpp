#include "Timer.h"

namespace chrono
{
    void Timer::Start()
    {
        m_StartTime = high_resolution_clock::now();
        m_Elapsed = duration<double>(0);
        m_ElapsedF = duration<float>(0);
    }

    double Timer::GetElapsed()
    {
        SetElapsed(high_resolution_clock::now() - m_StartTime);
        return m_Elapsed.count();
    }

    float Timer::GetElapsedF()
    {
        SetElapsed(high_resolution_clock::now() - m_StartTime);
        return static_cast<float>(m_Elapsed.count());
    }

    void Timer::SetElapsed(duration<double> val)
    {
        m_Elapsed = val;
        m_ElapsedF = val;
    }
}
