#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
#define TEX2
#define SHA2
#define DD3

namespace murkyFramework {
namespace GfxDevice 
{
    void onGfxDeviceErrorTriggerBreakpoint();
    bool initialise_device(SystemSpecific * systemSpecific);    
    void drawBegin();
    void drawEnd();
}
}//namespace murkyFramework