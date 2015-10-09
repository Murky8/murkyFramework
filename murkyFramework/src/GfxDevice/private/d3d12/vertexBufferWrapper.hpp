#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/GfxDevice/version_gfxDevice.hpp>
#ifdef USE_DIRECT3D12
#include <murkyFramework/include/GfxDevice/gfxPrimativeTypes.hpp>
#include <murkyFramework/src/GfxDevice/public/gfxDevice.hpp>
#include <d3d12.h>
#include <wrl.h>


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
		// Methods		
		void	draw(void *data, u32 nPrimatives);

		s32 getCapacityBytes() const;
		// Data
		//const u32			capacity?;
		const VertexType		vertexType;
		const PrimativeType		primativeType;
		const ShaderWrapper		shaderId;
		const TextureWrapper	texture;
		Microsoft::WRL::ComPtr<ID3D12Resource> m_vertexBuffer;
		D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
		u32   capacity;
	private:		
	};
}
#endif