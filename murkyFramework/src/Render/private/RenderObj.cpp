//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/src/private/pch.hpp>

RenderObj::RenderObj(GfxDeviceObj_initStruct  *const initStruct)
{
    gfxDevice = new GfxDeviceObj(initStruct);
}

RenderObj::~RenderObj()
{
    delete gfxDevice;
}

