//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/src/pch.hpp>

namespace qmaths
{
    float randFloat_0_to_1()
    {
        return (float)rand() / (float)RAND_MAX;
    }

    float randFloat_m1_to_1()
    {
        return randFloat_0_to_1()*2.0f - 1.0f;
    }    
}

