//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/src/private/pch.hpp>
#include <murkyFramework/include/version.hpp>
#include <murkyFramework/include/GfxDevice/version_gfxDevice.hpp>
#include <murkyFramework/include/GfxDevice/gfxPrimativeTypes.hpp>

#ifdef USE_DIRECT3D12
//#include <vector>
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

#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>

#include <murkyFramework/include/debugUtils.hpp>
#include <murkyFramework/include/GfxDevice/gfxLowLevel.hpp>
#include <murkyFramework/include/GfxDevice/shaders.hpp>
#include <murkyFramework/include/loadSaveFile.hpp>
#include <murkyFramework/include/collectionNamed.hpp>
#include <murkyFramework/include/GfxDevice/d3d12/shaders_d3d12.hpp>
#include <murkyFramework/include/GfxDevice/vertexBuffer.hpp>

namespace GfxDevice
{
    using namespace DirectX;    
	
    void setUniform_projectionMatrix(const float *pMat)
    {   
     
    }
//    HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
//    {
//    }
//
    void	Shaders::initialise()    
    {
		//debugLog << L"GfxLowLevel::Shaders::initialise" << "\n";                
		HRESULT hr;
#ifdef _DEBUG
		// Enable better shader debugging with the graphics debugging tools.
		UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
		UINT compileFlags = 0;
#endif
		ShaderWrapper newShader;

		ID3DBlob* pErrorBlob = nullptr;
        
		hr = D3DCompileFromFile(L"src/GfxDevice/private/d3d12/shaders/shaders.hlsl", nullptr, nullptr, "VSMain", "vs_5_0", compileFlags, 0, &newShader.vertexShader, &pErrorBlob);
		if (FAILED(hr))
		{
			if (pErrorBlob)
			{
				OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
				pErrorBlob->Release();
			}
			triggerBreakpoint();
		}

		pErrorBlob = nullptr;
		hr = D3DCompileFromFile(L"src/GfxDevice/private/d3d12/shaders/shaders.hlsl", nullptr, nullptr, "PSMain", "ps_5_0", compileFlags, 0, &newShader.pixelShader, &pErrorBlob);
		if (FAILED(hr))
		{
			if (pErrorBlob)
			{
				OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
				pErrorBlob->Release();
			}
			triggerBreakpoint();
		}
		if (pErrorBlob) pErrorBlob->Release();
		shaderManager.add(L"posColTex", newShader);
    }

    void	Shaders::deinitialise()
    {                       
    }
}
#endif // USE_DIRECT3D12

