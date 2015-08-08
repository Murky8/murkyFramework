//------------------------------------------------------------------------------
// 2015 J. Coelho.

#include <murkyFramework/include/gfxLowLevel/gfxPrimativeTypes.hpp>
#include <murkyFramework/include/gfxLowLevel/vertexBuffer.hpp>
#include <murkyFramework/include/vectorMatrix.hpp>

namespace GfxLowLevel
{
}

namespace RenderHi
{    
    // forward declarations
    extern std::vector<Line_pct> defaultLines;

    void drawCrosshair(vec3 pos, vec3 colour, f32 size, 
        std::vector<Line_pct> &lines = defaultLines);
    
}