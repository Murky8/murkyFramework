#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/GfxDevice/version_gfxDevice.hpp>
#include <murkyFramework/include/GfxDevice/gfxPrimativeTypes.hpp>
#include <glew/include/GL/glew.h>
#include <murkyFramework/src/GfxDevice/public/gfxDevice.hpp>


#ifdef USE_OPENGL

namespace GfxDevice
{
	class VertexBufferWrapper
	{
	public:
		// constructors				
		VertexBufferWrapper(
			VertexType vertexType, PrimativeType primativeType,
			ShaderWrapper shaderId, TextureWrapper texture,
			u32 nVerts);		
		// methods		
		void	draw(void *data, u32 nPrimatives);

		s32 getCapacityBytes() const;
		// data
		//const u32			capacity?;
		const VertexType	vertexType;
		const PrimativeType	primativeType;
		const ShaderWrapper	shaderId;
		const TextureWrapper	texture;
		GLuint					vao;
		GLuint					vbo;
		u32   capacity;
	private:		
	};
}
#endif