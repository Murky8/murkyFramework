#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
#include <murkyFramework/include/gfxLowLevel/version_gfxDevice.hpp>

namespace GfxLowLevel 
{
    void onGfxDeviceErrorTriggerBreakpoint();
    void initialise();
    void deinitialise();
    void drawBegin();
    void drawEnd();
}