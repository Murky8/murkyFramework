//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/gfxLowLevel/version_gfxDevice.hpp>
#ifdef USE_DIRECT3D11

#include <vector>

#include <murkyFramework/include/debugUtils.hpp>

#include <murkyFramework/include/common.hpp>


namespace GfxLowLevel
{
	//http://www.gamedev.net/page/resources/_/technical/graphics-programming-and-theory/perspective-projections-in-lh-and-rh-systems-r3598		
	mat4 makeProjectionMatrix_perspective(f32 fovYRads, f32 zNear, f32 zFar, f32 aspectRatio)
	{
		// d3d 0 z +1
		mat4 m(zero);
		m.v[1][1] = 1.f / tan(fovYRads*0.5f);
		m.v[0][0] = m.v[1][1] * aspectRatio;

		m.v[2][2] = zFar / (zFar - zNear);
		m.v[2][3] = 1.f;
		m.v[3][2] = -(zFar*zNear) / (zFar - zNear);

		return m;
	}
}
#endif //USE_DIRECT3D11
