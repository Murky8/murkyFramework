//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/src/private/pch.hpp>

RenderObj::RenderObj()
{
    gfxDevice = new GfxDeviceObj();
}

RenderObj::~RenderObj()
{
    delete gfxDevice;
}

