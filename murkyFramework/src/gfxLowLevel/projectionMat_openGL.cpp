//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/version.hpp>
#include <murkyFramework/include/gfxLowLevel/version_gfxDevice.hpp>
#ifdef USE_OPENGL

#include <murkyFramework/include/gfxHighLevel/projectionMat.hpp>
#include <murkyFramework/include/gfxLowLevel/version_gfxDevice.hpp>
#include <murkyFramework/include/common.hpp>
#include <murkyFramework/include/debugUtils.hpp>

namespace GfxLowLevel
{
	//http://www.gamedev.net/page/resources/_/technical/graphics-programming-and-theory/perspective-projections-in-lh-and-rh-systems-r3598		
	mat4 makeProjectionMatrix_perspective(f32 fovYRads, f32 zNear, f32 zFar, f32 aspectRatio)
	{
		// ogl. -1 z +1

		mat4 m(zero);
		m.v[1][1] = 1.f / tan(fovYRads*0.5f);
		m.v[0][0] = m.v[1][1] * aspectRatio;

		m.v[2][2] = -(zFar + zNear) / (zFar - zNear);
		m.v[2][3] = 1.f;
		m.v[3][2] = (2.f * zFar * zNear) / (zFar - zNear);

		return m;
	}
}
#endif //USE_OPENGL