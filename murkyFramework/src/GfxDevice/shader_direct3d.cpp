//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/version.hpp>
#include <murkyFramework/include/GfxDevice/version_gfxDevice.hpp>
#include <murkyFramework/include/GfxDevice/gfxPrimativeTypes.hpp>

#ifdef USE_DIRECT3D11

#include <vector>
#include <windows.h>

#include <d3d11_1.h>
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
    // Forward declarations    
    extern  D3D_DRIVER_TYPE         g_driverType;
    extern  D3D_FEATURE_LEVEL       g_featureLevel;
    extern  HINSTANCE               g_hInst;
    extern  HWND                    g_hWnd;
    extern  ID3D11Device*           g_pd3dDevice;
    extern  ID3D11Device1*          g_pd3dDevice1;
    extern  ID3D11DeviceContext*    g_pImmediateContext;
    extern  ID3D11DeviceContext1*   g_pImmediateContext1;
    extern  ID3D11RenderTargetView* g_pRenderTargetView;
    extern  IDXGISwapChain*         g_pSwapChain;
    extern  IDXGISwapChain1*        g_pSwapChain1;

    extern  ID3D11VertexShader*     g_pVertexShader;
    extern  ID3D11PixelShader*      g_pPixelShader;
    extern  ID3D11InputLayout*      g_pVertexLayout;
    extern  ID3D11Buffer*           g_pVertexBuffer;
    extern  ID3D11Buffer            *g_pCBChangesEveryFrame;
    extern  ID3D11SamplerState       *g_pSamplerLinear;
    extern     void GfxDevice::onGfxDeviceErrorTriggerBreakpoint();
    
    namespace Shaders
    {     
        ShaderId posColText;
    }

    struct HandleDeviceShader
    {
        ID3D11VertexShader     *pVertexShader;
        ID3D11PixelShader      *pPixelShader;
    };

    void setUniform_projectionMatrix(const float *pMat)
    {   
        g_pImmediateContext->UpdateSubresource(g_pCBChangesEveryFrame, 0, nullptr, pMat, 0, 0);
    }

    HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
    {
        HRESULT hr = S_OK;

        DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
        // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
        // Setting this flag improves the shader debugging experience, but still allows 
        // the shaders to be optimized and to run exactly the way they will run in 
        // the release configuration of this program.
        dwShaderFlags |= D3DCOMPILE_DEBUG;

        // Disable optimizations to further improve shader debugging
        dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

        ID3DBlob* pErrorBlob = nullptr;
        hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
            dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
        if (FAILED(hr))
        {
            if (pErrorBlob)
            {
                OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
                pErrorBlob->Release();
            }
            triggerBreakpoint();
            return hr;
        }
        if (pErrorBlob) pErrorBlob->Release();

        return S_OK;
    }

    void	Shaders::initialise()    
    {
        HRESULT hr = S_OK;
        GfxDevice::onGfxDeviceErrorTriggerBreakpoint();
        debugLog << L"GfxLowLevel::Shaders::initialise" << "\n";        

        // Compile the vertex shader
        ID3DBlob* pVSBlob = nullptr;
        hr = CompileShaderFromFile(L"src/shaders/posColTex.vs", "mainvs", "vs_4_0", &pVSBlob);
        if (FAILED(hr))
        {
            MessageBox(nullptr,
                L"The FX file cannot be compiled.", L"Error", MB_OK);
            triggerBreakpoint();
        }

        // Create the vertex shader
        hr = g_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &g_pVertexShader);
        if (FAILED(hr))
        {
            pVSBlob->Release();
            triggerBreakpoint();
        }

        // Define the input layout
        D3D11_INPUT_ELEMENT_DESC layout[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
        };
        UINT numElements = ARRAYSIZE(layout);

        // Create the input layout
        g_pVertexLayout = NULL;
        hr = g_pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
            pVSBlob->GetBufferSize(), &g_pVertexLayout);
        pVSBlob->Release();
        if (FAILED(hr))
            triggerBreakpoint();
             

        // Compile the pixel shader
        ID3DBlob* pPSBlob = nullptr;
        hr = CompileShaderFromFile(L"src/shaders/posColTex.ps", "mainps", "ps_4_0", &pPSBlob);
        if (FAILED(hr))
        {
            MessageBox(nullptr,
                L"The FX file cannot be compiled.", L"Error", MB_OK);
            triggerBreakpoint();
        }

        // Create the pixel shader
        hr = g_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &g_pPixelShader);
        pPSBlob->Release();
        if (FAILED(hr))
            triggerBreakpoint();

        Shaders::posColText.handle = (u32)g_pVertexShader;
        Shaders::posColText.handle2 = (u32)g_pPixelShader;

        // murky VB
        /*    Vert_pct vertices[] =
            {
            { vec3(0.0f, 0.0f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.f, 1.f) },
            { vec3(0.0f, 1.0f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(0.f, 0.f) },
            { vec3(1.0f, 0.0f, 0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(1.f, 1.f) },
            { vec3(0.0f, 1.0f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.f, 0.f) },
            { vec3(1.0f, 1.0f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(1.f, 0.f) },
            { vec3(1.0f, 0.0f, 0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(1.f, 1.f) }
            };*/
        {
            D3D11_BUFFER_DESC bd;
            ZeroMemory(&bd, sizeof(bd));
            bd.Usage = D3D11_USAGE_DYNAMIC;
            bd.ByteWidth = sizeof(Triangle_pct) * 10;
            bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            bd.MiscFlags = 0;
            //D3D11_SUBRESOURCE_DATA InitData;
            //ZeroMemory(&InitData, sizeof(InitData));
            //InitData.pSysMem = vertices;
            hr = g_pd3dDevice->CreateBuffer(&bd, NULL, &g_pVertexBuffer);
            if (FAILED(hr))
                triggerBreakpoint();
        }        // Set vertex buffer

        {// shader constants
            D3D11_BUFFER_DESC bd;
            ZeroMemory(&bd, sizeof(bd));
            bd.Usage = D3D11_USAGE_DEFAULT;
            bd.ByteWidth = sizeof(mat4);
            bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
            bd.CPUAccessFlags = 0;
            hr = g_pd3dDevice->CreateBuffer(&bd, nullptr, &g_pCBChangesEveryFrame);
            if (FAILED(hr))
                triggerBreakpoint();
        }
        
    }

    void	Shaders::deinitialise()
    {               
        g_pVertexShader->Release();
        g_pPixelShader->Release();
    }
}
#endif // USE_DIRECT3D11

