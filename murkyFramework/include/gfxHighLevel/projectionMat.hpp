#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
#include <murkyFramework/include/gfxLowLevel/version_gfxDevice.hpp>

#include <murkyFramework/include/types.hpp>
#include <murkyFramework/include/vectorMatrix.hpp>

namespace RenderHi
{
    extern void setProjMatOrtho(f32 right, f32 left, f32 top, f32 bottom, mat4 &matOut);
}
