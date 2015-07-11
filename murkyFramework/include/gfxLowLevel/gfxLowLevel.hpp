#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
#include <murkyFramework/include/version.hpp>

namespace GfxLowLevel 
{
    void onGfxDeviceErrorTriggerBreakpoint();
    void initialise();
    void deinitialise();
    void drawBegin();
    void drawEnd();
}