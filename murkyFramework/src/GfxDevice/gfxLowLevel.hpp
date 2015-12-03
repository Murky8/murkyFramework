#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
namespace murkyFramework {}//namespace murkyFramework
namespace GfxDevice 
{
    void onGfxDeviceErrorTriggerBreakpoint();
    bool initialise_device(SystemSpecific * systemSpecific);    
    void drawBegin();
    void drawEnd();
}