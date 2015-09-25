//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/version.hpp>
#include <murkyFramework/include/GfxDevice/version_gfxDevice.hpp>
#include <murkyFramework/include/GfxDevice/gfxPrimativeTypes.hpp>

#ifdef USE_DIRECT3D12
#include <vector>
#include <windows.h>


#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>

#include <murkyFramework/include/debugUtils.hpp>
#include <murkyFramework/include/GfxDevice/gfxLowLevel.hpp>
#include <murkyFramework/include/GfxDevice/shaders.hpp>
#include <murkyFramework/include/loadSaveFile.hpp>

namespace GfxDevice
{
    using namespace DirectX;
    extern     void GfxDevice::onGfxDeviceErrorTriggerBreakpoint();
    
    namespace Shaders
    {     
        ShaderId posColText;
    }

    struct HandleDeviceShader
    {        
    };

    void setUniform_projectionMatrix(const float *pMat)
    {   
     // g_pImmediateContext->UpdateSubresource(g_pCBChangesEveryFrame, 0, nullptr, pMat, 0, 0);
    }
//
//    HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
//    {
//        HRESULT hr = S_OK;
//
//        DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
//#ifdef _DEBUG
//        // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
//        // Setting this flag improves the shader debugging experience, but still allows 
//        // the shaders to be optimized and to run exactly the way they will run in 
//        // the release configuration of this program.
//        dwShaderFlags |= D3DCOMPILE_DEBUG;
//
//        // Disable optimizations to further improve shader debugging
//        dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
//#endif
//
//        ID3DBlob* pErrorBlob = nullptr;
//        hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
//            dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
//        if (FAILED(hr))
//        {
//            if (pErrorBlob)
//            {
//                OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
//                pErrorBlob->Release();
//            }
//            triggerBreakpoint();
//            return hr;
//        }
//        if (pErrorBlob) pErrorBlob->Release();
//
//        return S_OK;
//    }
//
    void	Shaders::initialise()    
    {   debugLog << L"GfxLowLevel::Shaders::initialise" << "\n";                
    }

    void	Shaders::deinitialise()
    {                       
    }
}
#endif // USE_DIRECT3D12
