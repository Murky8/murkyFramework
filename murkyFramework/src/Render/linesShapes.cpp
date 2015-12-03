//------------------------------------------------------------------------------
// 2015 J. Coelho.
#include <murkyFramework/src/pch.hpp>

namespace murkyFramework {
namespace Render
{    
	void	drawLine(Line_pc l, std::vector<Line_pc> &lines)
	{
		lines.push_back(l);		
	}

	void	drawLine(Vert_pc v0, Vert_pc v1, std::vector<Line_pc> &lines)
	{
		drawLine(Line_pc{v0, v1}, lines);
	}

	void	drawLine(vec v0, vec v1, std::vector<Line_pc> &lines)
	{
		drawLine(
			Vert_pc{v0, {1.f, 1.f, 1.f}},
			Vert_pc{v1, {1.f, 1.f, 1.f}}, lines);		
	}
		
    void    drawCrosshair(vec3 pos, vec3 colour, f32 size, std::vector<Line_pc> &lines)
    {
        std::vector<Line_pc> newLines
        {
            {
                { vec3(pos - vec3(size, 0.f, 0.f)), colour },
                { vec3(pos + vec3(size, 0.f, 0.f)), colour }
            },
            {
                { vec3(pos - vec3(0.f, size, 0.f)), colour },
                { vec3(pos + vec3(0.f, size, 0.f)), colour }
            },
            {
                { vec3(pos - vec3(0.f, 0.f, size)), colour },
                { vec3(pos + vec3(0.f, 0.f, size)), colour }
            }
        };
        lines.insert(lines.end(), newLines.begin(), newLines.end());
    }
}
}//namespace murkyFramework