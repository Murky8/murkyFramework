#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
namespace murkyFramework {}//namespace murkyFramework
namespace Render
{    
    // forward declarations
    extern std::vector<Line_pc> defaultLines;

	void	drawLine(Line_pc l,
		std::vector<Line_pc> &lines = defaultLines);
	void	drawLine(Vert_pc l0, Vert_pc l1,
		std::vector<Line_pc> &lines = defaultLines);
    void	drawCrosshair(vec3 pos, vec3 colour, f32 size, 
        std::vector<Line_pc> &lines = defaultLines);
    
}