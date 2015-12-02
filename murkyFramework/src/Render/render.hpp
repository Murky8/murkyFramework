#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.

namespace Render
{
    extern class TextRender *textRenderer;
       
    bool initialise(SystemSpecific* systemSpecific);
    void deinitialise();
    void drawAll();
    void addQuad_pct();
}

