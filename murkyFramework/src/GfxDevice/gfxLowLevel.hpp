#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
#include <murkyFramework/src/private/systemSpecific.hpp>
namespace GfxDevice 
{
    void onGfxDeviceErrorTriggerBreakpoint();
    bool initialise_device(SystemSpecific * systemSpecific);    
    void drawBegin();
    void drawEnd();
}