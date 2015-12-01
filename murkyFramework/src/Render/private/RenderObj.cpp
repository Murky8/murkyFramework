//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/src/pch.hpp>

RenderObj::RenderObj(GfxDeviceObj_initStruct  *const initStruct)
{
    g_appDebug->render = this; // warning: see g_aapDebug usage notes: for development only, remove!
    gfxDevice = new GfxDeviceObj(initStruct);
}

RenderObj::~RenderObj()
{
    delete gfxDevice;
}

