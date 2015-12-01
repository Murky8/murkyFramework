#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11

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