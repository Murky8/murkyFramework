#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.

#include <murkyFramework/include/gfxLowLevel/gfxPrimativeTypes.hpp>
#include <murkyFramework/include/gfxLowLevel/vertexBuffer.hpp>
#include <murkyFramework/include/vectorMatrix.hpp>
#include <vector>

namespace GfxLowLevel
{
}

namespace RenderHi
{    
    // forward declarations    
    void    drawCrosshair(vec3 pos, vec3 colour, f32 size, std::vector<Line_pct> &lines)
    {
        std::vector<Line_pct> newLines
        {
            {
                { vec3(pos - vec3(size, 0, 0)), colour, vec2(0.f, 0.f) },
                { vec3(pos + vec3(size, 0, 0)), colour, vec2(0.f, 0.f) }
            },
            {
                { vec3(pos - vec3(0, size, 0)), colour, vec2(0.f, 0.f) },
                { vec3(pos + vec3(0, size, 0)), colour, vec2(0.f, 0.f) }
            },
            {
                { vec3(pos - vec3(0, 0, size)), colour, vec2(0.f, 0.f) },
                { vec3(pos + vec3(0, 0, size)), colour, vec2(0.f, 0.f) }
            }
        };
        lines.insert(lines.end(), newLines.begin(), newLines.end());
    }
}