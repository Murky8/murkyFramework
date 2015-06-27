//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: any, c++11
// reference: http://antongerdelan.net/opengl/vertexbuffers.html
// notes:
// low level device vertex buffer encapsulation

#pragma once
#include <common.hpp>

namespace GfxLowLevel
{ 

    //--------------------------------------------------------------------------
    // Vertex buffer
    enum class VertexType{ posCol, posColTex };
    enum class PrimativeType{ triangle, line, point };
	
	class VertexBufferDynamic
	{
	public:
		// Constructors		
		// if not using a texture, pass '0'
		VertexBufferDynamic( VertexType vertexType, PrimativeType primativeType, u32 shaderProgram, u32 texture );

		// Methods		
		void	draw(void *data, int nPrimatives);

		// Data
		//const u32			capacity?;
		const VertexType	vertexType;		
		const PrimativeType primativeType;
		const u32			shaderProgram;
		const u32			texture;
	private:
		u32 vbo;
		u32 vao;
		VertexBufferDynamic() = delete;
	};
  
    //--------------------------------------------------------------------------
}