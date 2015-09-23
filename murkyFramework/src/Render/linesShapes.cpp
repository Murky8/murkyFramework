#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.

#include <murkyFramework/include/GfxDevice/gfxPrimativeTypes.hpp>
#include <murkyFramework/include/GfxDevice/vertexBuffer.hpp>
#include <murkyFramework/include/vectorMatrix.hpp>
#include <vector>

namespace GfxDevice
{
}

namespace Render
{    
	
	void	drawLine(Line_pct l, std::vector<Line_pct> &lines)
	{
		lines.push_back(l);		
	}

	void	drawLine(Vert_pct v0, Vert_pct v1, std::vector<Line_pct> &lines)
	{
		drawLine(Line_pct{v0, v1}, lines);
	}

	void	drawLine(vec v0, vec v1, std::vector<Line_pct> &lines)
	{
		drawLine(
			Vert_pct{v0, {1.f, 1.f, 1.f}, {0.f, 0.f}},
			Vert_pct{v1, {1.f, 1.f, 1.f}, {0.f, 0.f}}, lines);		
	}
		
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