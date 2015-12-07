//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/src/pch.hpp>

namespace murkyFramework {
#define deviceObj  g_appDebug->render->gfxDevice

    namespace GfxDevice
    {
        using namespace DirectX;

        void setUniform_projectionMatrix(const float *pMat)
        {

        }
     
        void	Shaders::initialise()
        {

        #ifdef DD3
            deviceObj->loadShadersInDir(L"src/GfxDevice/d3d12/shaders");
        #else
            //debugLog << L"GfxLowLevel::Shaders::initialise" << "\n";                
            HRESULT hr;
#ifdef _DEBUG
            // Enable better shader debugging with the graphics debugging tools.
            UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
            UINT compileFlags = 0;
#endif

            {// shder pct
                ShaderWrapper newShader;

                ID3DBlob* pErrorBlob = nullptr;
                std::wstring fileName = { L"src/GfxDevice/d3d12/shaders/posColTex.hlsl" };
                hr = D3DCompileFromFile(fileName.c_str(), nullptr, nullptr, "VSMain", "vs_5_0", compileFlags, 0, &newShader.vertexShader, &pErrorBlob);
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
                hr = D3DCompileFromFile(fileName.c_str(), nullptr, nullptr, "PSMain", "ps_5_0", compileFlags, 0, &newShader.pixelShader, &pErrorBlob);
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
                deviceObj->shaderManager.add(L"posColTex", newShader);
            }

            {// shder pc
                ShaderWrapper newShader;

                ID3DBlob* pErrorBlob = nullptr;
                std::wstring fileName = { L"src/GfxDevice/d3d12/shaders/posCol.hlsl" };
                hr = D3DCompileFromFile(fileName.c_str(), nullptr, nullptr, "VSMain", "vs_5_0", compileFlags, 0, &newShader.vertexShader, &pErrorBlob);
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
                hr = D3DCompileFromFile(fileName.c_str(), nullptr, nullptr, "PSMain", "ps_5_0", compileFlags, 0, &newShader.pixelShader, &pErrorBlob);
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
                deviceObj->shaderManager.add(L"posCol", newShader);
            }
        #endif
        }

        void	Shaders::deinitialise()
        {
        }
    }

}//namespace murkyFramework
