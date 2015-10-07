#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/GfxDevice/version_gfxDevice.hpp>
#ifdef USE_DIRECT3D11
#include <d3d11_1.h>

namespace GfxDevice
{
	class TextureWrapper
	{
	public:
		ID3D11ShaderResourceView *deviceTexture;		
	};
}
#endif