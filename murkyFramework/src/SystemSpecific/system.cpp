//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/src/pch.hpp>

namespace murkyFramework {

//using namespace std::literals;
using namespace std::chrono;

namespace system2
{
    f64 getPerformanceCounterFrequency()
    {
        LARGE_INTEGER  d;
        QueryPerformanceFrequency(&d);
        return static_cast<f64>(d.QuadPart);
    }

    static f64 performanceCounterFrequency;
    static const f64 appStartTime = readTimeSeconds();
    /*
    double getCurrentTime()
    {
        using Clock = std::chrono::high_resolution_clock;
        static auto startTime = Clock::now().time_since_epoch().count();

        constexpr auto num = Clock::period::num;
        constexpr auto den = Clock::period::den;

        auto t = Clock::now().time_since_epoch().count() - startTime;
        auto s = (double)t*num / den;
        return s;
    }

    void logCurrentTime()
    {
        auto s = getCurrentTime();
        std::cout << "Time" << " " << std::fixed << s << "\n";
    }
*/
    f64 readTimeSeconds()
    {
        static bool firstTime {true};
        if (firstTime == true)
        {
            performanceCounterFrequency = getPerformanceCounterFrequency();
            firstTime = false;
        }
        LARGE_INTEGER n, d;

        QueryPerformanceCounter(&n);        
        return static_cast<f64>(n.QuadPart) / performanceCounterFrequency;
    }

    f64 readTimeSecondsSinceAppStart()
    {
		return (readTimeSeconds() - appStartTime);
    }

    #include <stdint.h>

//  Windows
#ifdef _WIN32

#include <intrin.h>

u64 ticksCPUSinceAppStart()
{
    return __rdtsc();
}

//  Linux/GCC
#else

uint64_t rdtsc(){
    unsigned int lo,hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
}

#endif

    // Windows
    // QueryPerformanceCounter
    // QueryPerfomanceFrequency
}

}//namespace murkyFramework