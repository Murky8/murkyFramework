//------------------------------------------------------------------------------
// 2015 J. Coelho.
// C++11. Windows. /*Android.*/
// pragma once

#include <version.hpp>
#include <types.hpp>
#include <debugUtils.hpp>
#include <murkyFramework/include/vectorMatrix.hpp>

namespace RenderHi
{
    extern void setProjMatOrtho(f32 right, f32 left, f32 top, f32 bottom, mat4 &matOut);
}

//