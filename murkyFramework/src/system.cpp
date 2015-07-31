//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/version.hpp>
#include <murkyFramework/include/gfxLowLevel/version_gfxDevice.hpp>

#include <windows.h>
#include <murkyFramework/include/common.hpp>
#include <murkyFramework/include/system.hpp>

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