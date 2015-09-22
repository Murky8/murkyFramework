#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
#include <murkyFramework/include/gfxLowLevel/version_gfxDevice.hpp>

#include <murkyFramework/include/types.hpp>
#include <murkyFramework/include/vectorMatrix.hpp>

namespace RenderHi
{    
	mat4 makeCameraMatrix(vec pos, mat3 ori);
	mat4 makeProjectionMatrix_perspective1(f32 fovYRads, f32 zNear, f32 zFar, f32 aspectRatio);
	mat4 makeProjectionMatrix_perspective2(f32 fovYRads, f32 zNear, f32 zFar, f32 aspectRatio);
	mat4 makeProjectionMatrix_ortho(f32 left, f32 right, f32 bottom, f32 top, f32 zNear = -1.f, f32 zFar = 1.f);    
}
