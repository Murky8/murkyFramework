#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
namespace murkyFramework {
namespace Render
{    
    mat4    makeLookAtPosOri(vec4 cameraPos, vec4 subjectPos);
	mat4	makeCameraMatrix(mat4 transRot);	
	mat4	makeProjectionMatrix_ortho(f32 left, f32 right, f32 bottom, f32 top, f32 zNear = -1.f, f32 zFar = 1.f);    
	mat4	makeProjectionMatrix_perspective(float x, float x1, float x2, float x3);
}
}//namespace murkyFramework
