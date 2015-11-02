#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/GfxDevice/version_gfxDevice.hpp>
#ifdef USE_DIRECT3D12
#include <windows.h>
#include <combaseapi.h>
#include <wrl.h>
#include <d3d12.h>
#include <external/d3d12/d3dx12.h>
#include <dxgi1_4.h>
#include <d2d1_3.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <dwrite.h>
#include <d3d11on12.h>
#include <murkyFramework/include/common.hpp>
using namespace DirectX;
using namespace Microsoft::WRL;

namespace GfxDevice
{
	class TextureWrapper
	{
	public:
        u32 iTexture;
        ComPtr<ID3D12Resource> deviceTexture;
	};
}
#endif