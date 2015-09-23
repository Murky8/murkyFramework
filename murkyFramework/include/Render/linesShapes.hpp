//------------------------------------------------------------------------------
// 2015 J. Coelho.

#include <murkyFramework/include/GfxDevice/gfxPrimativeTypes.hpp>
#include <murkyFramework/include/GfxDevice/vertexBuffer.hpp>
#include <murkyFramework/include/vectorMatrix.hpp>

namespace GfxDevice
{
}

namespace Render
{    
    // forward declarations
    extern std::vector<Line_pct> defaultLines;

	void	drawLine(Line_pct l,
		std::vector<Line_pct> &lines = defaultLines);
	void	drawLine(Vert_pct l0, Vert_pct l1,
		std::vector<Line_pct> &lines = defaultLines);
    void	drawCrosshair(vec3 pos, vec3 colour, f32 size, 
        std::vector<Line_pct> &lines = defaultLines);
    
}