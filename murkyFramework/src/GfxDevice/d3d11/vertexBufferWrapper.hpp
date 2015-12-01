#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/GfxDevice/version_gfxDevice.hpp>
#ifdef USE_DIRECT3D11
#include <murkyFramework/include/GfxDevice/gfxPrimativeTypes.hpp>
#include <murkyFramework/src/GfxDevice/public/gfxDevice.hpp>
#include <d3d11_1.h>

namespace GfxDevice
{
	class VertexBufferWrapper
	{
	public:
		// constructor				
		VertexBufferWrapper(
			VertexType vertexType, PrimativeType primativeType,
			ShaderWrapper shaderId, TextureWrapper texture,
			u32 nVerts);

		// methods		
		void	draw(void *data, u32 nPrimatives);

		s32 getCapacityBytes() const;
		// data
		//const u32			capacity?;
		const VertexType		vertexType;
		const PrimativeType		primativeType;
		const ShaderWrapper		shaderId;
		const TextureWrapper	texture;		
		ID3D11Buffer			*deviceBuffer;
		u32   capacity;
	private:		
	};
}
#endif