#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/src/GfxDevice/public/shaderId.hpp>
#include <d3d11_1.h>

namespace GfxDevice
{
	class ShaderId_private3
	{
	public:
		ID3D11VertexShader     *pVertexShader;
		ID3D11PixelShader      *pPixelShader;
	};
}