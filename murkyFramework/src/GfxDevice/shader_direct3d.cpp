//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/src/private/pch.hpp>
#ifdef USE_DIRECT3D11
#define deviceObj  g_appDebug->render->gfxDevice

namespace GfxDevice
{
    using namespace DirectX;     	

    void setUniform_projectionMatrix(const float *pMat)
    {   
        deviceObj->g_pImmediateContext->UpdateSubresource(deviceObj->g_pCBChangesEveryFrame, 0, nullptr, pMat, 0, 0);
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
        
        debugLog << L"GfxLowLevel::Shaders::initialise\n";

        {
        ID3D11VertexShader	*pVertexShader;
        ID3D11PixelShader	*pPixelShader;

        // compile the vertex shader
        ID3DBlob* pVSBlob = nullptr;        
        hr = CompileShaderFromFile(L"src/GfxDevice/private/d3d11/shaders/posColTex.vs", "mainvs", "vs_4_0", &pVSBlob);
        if (FAILED(hr))
        {            
            triggerBreakpoint(L"The vs file cannot be compiled.\n");
        }

        // Create the vertex shader
        hr = deviceObj->g_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &pVertexShader);
        if (FAILED(hr))
        {
            pVSBlob->Release();
            triggerBreakpoint(L"coulndnt create vs.\n");
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
        deviceObj->g_pVertexLayout_posColTex = NULL;
        hr = deviceObj->g_pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
            pVSBlob->GetBufferSize(), &deviceObj->g_pVertexLayout_posColTex);
        pVSBlob->Release();
        if (FAILED(hr))
            triggerBreakpoint();
        // Define the input layout

        // Compile the pixel shader
        ID3DBlob* pPSBlob = nullptr;
        hr = CompileShaderFromFile(L"src/GfxDevice/private/d3d11/shaders/posColTex.ps", "mainps", "ps_4_0", &pPSBlob);
        if (FAILED(hr))
        {            
            triggerBreakpoint(L"The ps file cannot be compiled.");
        }

        // Create the pixel shader
        hr = deviceObj->g_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &pPixelShader);
        pPSBlob->Release();
        if (FAILED(hr))
            triggerBreakpoint();
        
        shaderManager.add(L"posColTex", ShaderWrapper{ pVertexShader , pPixelShader });
    }
        {
            ID3D11VertexShader	*pVertexShader;
            ID3D11PixelShader	*pPixelShader;

            // compile the vertex shader
            ID3DBlob* pVSBlob = nullptr;
            hr = CompileShaderFromFile(L"src/GfxDevice/private/d3d11/shaders/posCol.vs", "mainvs", "vs_4_0", &pVSBlob);
            if (FAILED(hr))
            {
                triggerBreakpoint(L"The vs file cannot be compiled.\n");
            }

            // Create the vertex shader
            hr = deviceObj->g_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &pVertexShader);
            if (FAILED(hr))
            {
                pVSBlob->Release();
                triggerBreakpoint(L"coulndnt create vs.\n");
            }

            // Define the input layout
            D3D11_INPUT_ELEMENT_DESC layout[] =
            {
                { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "COLOR",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }                
            };
            UINT numElements = ARRAYSIZE(layout);

            // Create the input layout
            deviceObj->g_pVertexLayout_posCol = NULL;
            hr = deviceObj->g_pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
                pVSBlob->GetBufferSize(), &deviceObj->g_pVertexLayout_posCol);
            pVSBlob->Release();
            if (FAILED(hr))
                triggerBreakpoint();
            // Define the input layout

            // Compile the pixel shader
            ID3DBlob* pPSBlob = nullptr;
            hr = CompileShaderFromFile(L"src/GfxDevice/private/d3d11/shaders/posCol.ps", "mainps", "ps_4_0", &pPSBlob);
            if (FAILED(hr))
            {
                triggerBreakpoint(L"The ps file cannot be compiled.");
            }

            // Create the pixel shader
            hr = deviceObj->g_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &pPixelShader);
            pPSBlob->Release();
            if (FAILED(hr))
                triggerBreakpoint();

            shaderManager.add(L"posCol", ShaderWrapper{ pVertexShader , pPixelShader });
        }
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
            hr = deviceObj->g_pd3dDevice->CreateBuffer(&bd, NULL, &deviceObj->g_pVertexBuffer);
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
            hr = deviceObj->g_pd3dDevice->CreateBuffer(&bd, nullptr, &deviceObj->g_pCBChangesEveryFrame);
            if (FAILED(hr))
                triggerBreakpoint();
        }        
    }

    void	Shaders::deinitialise()
    {     		
        //g_pVertexShader->Release();
        //g_pPixelShader->Release();
    }
}
#endif // USE_DIRECT3D11

