#pragma once

#include <chrono>

using namespace std::chrono;

namespace chrono
{
    class Timer
    {
    private:
        time_point<steady_clock> m_StartTime;
        duration<double> m_Elapsed;
        duration<float> m_ElapsedF;
    public:
        void Start();
        double GetElapsed();
        float GetElapsedF();
    private:
        void SetElapsed(duration<double> val);
    };
}
