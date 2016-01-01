//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/src/pch.hpp>

namespace murkyFramework {
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
    
    f32     randFloat(f32 min, f32 max)
    {
        return min + (max - min)*randFloat_0_to_1();
    }

    s32     randInt(s32 min, s32 max)
    {
        return(std::floor( randFloat(min, max)+0.5f ));
    }
}

}//namespace murkyFramework
