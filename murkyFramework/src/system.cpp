#include <murkyFramework/include/system.hpp>
#include <windows.h>
#include <common.hpp>

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


    // Windows
    // QueryPerformanceCounter
    // QueryPerfomanceFrequency
}