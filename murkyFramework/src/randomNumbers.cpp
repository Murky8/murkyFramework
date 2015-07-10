//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/version.hpp>

#include <cstdlib>
#include <murkyFramework/include/common.hpp>

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

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable: 4172 ) //returning address of local variable or temporary
#endif

#ifdef __INTEL_COMPILER
#pragma warning( push )
#pragma warning( disable: 1251 ) //returning address of local variable or temporary
#endif
    
    float *const rand3Floats_m1_to_1()
    { 
        triggerBreakpoint();// test
        float fl[3];
        
        for_0_to(i, 2)
        {
            fl[i] = randFloat_m1_to_1();
        }
        return  fl;
    }

#ifdef __INTEL_COMPILER
#pragma warning( pop )
#endif

#ifdef _MSC_VER
#pragma warning( pop )
#endif

}

