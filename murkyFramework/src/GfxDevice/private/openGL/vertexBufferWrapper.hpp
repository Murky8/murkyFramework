#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/GfxDevice/version_gfxDevice.hpp>
#include <murkyFramework/include/GfxDevice/gfxPrimativeTypes.hpp>
#include <glew/include/GL/glew.h>

#ifdef USE_OPENGL

namespace GfxDevice
{
	class VertexBufferWrapper
	{
	public:
		// Constructors		
		// if not using a texture, pass '0'
		// capacity measures in 		
		VertexBufferWrapper(
			VertexType vertexType, PrimativeType primativeType,
			ShaderWrapper shaderId, TextureWrapper texture,
			u32 nVerts);
		~VertexBufferWrapper();
		// Methods		
		void	draw(void *data, u32 nPrimatives);

		s32 getCapacityBytes() const;
		// Data
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