//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/src/private/pch.hpp>
#ifdef USE_DIRECT3D12

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

