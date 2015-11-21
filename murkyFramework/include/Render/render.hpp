#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.

namespace Render
{
    extern TextRender *textRenderer;
       
    void initialise(WindowsSpecific *const windowsSpecific);
    void deinitialise();
    void drawAll(State &state);
    void addQuad_pct();
}

