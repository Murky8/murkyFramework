#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
#include <murkyFramework/include/GfxDevice/version_gfxDevice.hpp>

namespace GfxDevice 
{
    void onGfxDeviceErrorTriggerBreakpoint();
    void initialise();
    void deinitialise();
    void drawBegin();
    void drawEnd();
}