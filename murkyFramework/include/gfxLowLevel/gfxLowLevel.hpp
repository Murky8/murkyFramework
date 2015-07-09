#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
#include <murkyFramework/include/version.hpp>
//
//#include <vector>
//
//#include <appFramework.hpp>
//#include <types.hpp>  
//#include <gfxLowLevel/gfxPrimativeTypes.hpp>
//#include <gfxLowLevel/vertexBuffer.hpp>
//#include <gfxLowLevel/shaders.hpp>
//#include <gfxLowLevel/texture.hpp>

namespace GfxLowLevel 
{
    void onGfxDeviceErrorTriggerBreakpoint();
    void initialise();
    void deinitialise();
    void drawBegin();
    void drawEnd();
}