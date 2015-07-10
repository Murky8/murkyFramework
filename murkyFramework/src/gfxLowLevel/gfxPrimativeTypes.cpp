//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/version.hpp>

#include <murkyFramework/include/gfxLowLevel/gfxPrimativeTypes.hpp>

Vert_pc::Vert_pc(vec3 p, vec3 c) : pos(p), col(c) 
{
}

Vert_pct::Vert_pct(vec3 p, vec3 c, vec2 t) : pos(p), col(c), textCoords(t)
{
}

Line_pc::Line_pc(Vert_pc v0, Vert_pc v1) //: v{v0, v1}
{
	v[0] = v0;
	v[1] = v1;
}

Triangle_pc::Triangle_pc(Vert_pc v0, Vert_pc v1, Vert_pc v2) //: v{ v0, v1, v2 }
{
	v[0] = v0;
	v[1] = v1;
	v[2] = v2;
}

Triangle_pct::Triangle_pct(Vert_pct v0, Vert_pct v1, Vert_pct v2)// : v{ v0, v1, v2 }
{
	v[0] = v0;
	v[1] = v1;
	v[2] = v2;
}


