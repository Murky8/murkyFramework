#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
#include <murkyFramework/include/GfxDevice/version_gfxDevice.hpp>

#include <vector>
#include <murkyFramework/include/types.hpp>
#include <murkyFramework/include/state.hpp>
#include <murkyFramework/include/GfxDevice/gfxLowLevel.hpp>
#include <murkyFramework/include/Render/render.hpp>
#include <murkyFramework/include/Render/textRender.hpp>

namespace Render
{
    extern TextRender *textRenderer;
       
    void initialise();
    void deinitialise();
    void drawAll(State &state);
    void addQuad_pct();
}

