//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/src/private/pch.hpp>
#ifdef USE_DIRECT3D11
#define deviceObj  g_appDebug->render->gfxDevice

namespace GfxDevice
{      
    bool initialise_device(SystemSpecific * systemSpecific)
    {
        return true;
    }

    bool deinitialise_device()
    {                        
    
        return true;
    }
} // namespace GfxLowLevel
#endif // USE_DIRECT3D11
