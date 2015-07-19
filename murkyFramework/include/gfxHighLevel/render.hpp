#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
#include <murkyFramework/include/gfxLowLevel/version_gfxDevice.hpp>

#include <vector>
#include <murkyFramework/include/types.hpp>
#include <murkyFramework/include/gfxLowLevel/gfxLowLevel.hpp>
#include <murkyFramework/include/gfxHighLevel/render.hpp>
#include <murkyFramework/include/gfxHighLevel/textRender.hpp>

namespace RenderHi
{
    extern TextRender *textRenderer;
       
    void initialise();
    void deinitialise();
    void drawAll();
    void addQuad_pct();
}

