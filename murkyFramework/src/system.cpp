//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/version.hpp>

#include <windows.h>
#include <murkyFramework/include/common.hpp>
#include <murkyFramework/include/system.hpp>

namespace system2
{
    static const u64 appStartTime = (u64)readTimeSeconds();

    f64 readTimeSeconds()
    {
        LARGE_INTEGER n, d;

        QueryPerformanceCounter(&n);
        QueryPerformanceFrequency(&d);

        return static_cast<f64>(n.QuadPart) / static_cast<f64>(d.QuadPart);
    }

    //f64 readTimeSecondsSinceAppStart()
    //{
        //return (readTimeStampTicks() - appStartTime) / 2.677e09;  //NOTE!!!!ATTN;
    //}

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