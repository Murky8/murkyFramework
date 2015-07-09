//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/version.hpp>
#ifdef USE_DIRECT3D

#include <windows.h>
#include <d3d11_1.h>
#include <murkyFramework/include/common.hpp>

namespace GfxLowLevel
{
    //--------------------------------------------------------------------------------------
    // Global Variables
    //--------------------------------------------------------------------------------------
    //HINSTANCE               g_hInst = nullptr;
    //HWND                    g_hWnd = nullptr;
    // Forward declarations    
    extern    HINSTANCE               g_hInst;
    extern    HWND                    g_hWnd;
    extern    D3D_DRIVER_TYPE         g_driverType;
    extern    D3D_FEATURE_LEVEL       g_featureLevel;
    extern    ID3D11Device*           g_pd3dDevice;
    extern    ID3D11Device1*          g_pd3dDevice1;
    extern    ID3D11DeviceContext*    g_pImmediateContext;
    extern    ID3D11DeviceContext1*   g_pImmediateContext1;
    extern    IDXGISwapChain*         g_pSwapChain;
    extern    IDXGISwapChain1*        g_pSwapChain1;
    extern    ID3D11RenderTargetView* g_pRenderTargetView;

    extern    ID3D11VertexShader*     g_pVertexShader;
    extern    ID3D11PixelShader*      g_pPixelShader;
    extern    ID3D11InputLayout*      g_pVertexLayout;
    extern    ID3D11Buffer*           g_pVertexBuffer;

    extern    ID3D11SamplerState       *g_pSamplerLinear;
    extern    ID3D11Debug             *d3dDebug;

    // Called from: shead.cpp/
    bool initialise_device(HDC &hDC, HGLRC &hRC, HWND &hWnd)
    {
        //-------------------------------------------------------------------------------------- 
        // Create Direct3D device and swap chain 
        //--------------------------------------------------------------------------------------         
        
            RECT rc;
            GetClientRect(hWnd, &rc);
            UINT width = rc.right - rc.left;
            UINT height = rc.bottom - rc.top;

            UINT createDeviceFlags = 0;
#ifdef _DEBUG 
            createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif 

            D3D_DRIVER_TYPE driverTypes[] =
            {
                D3D_DRIVER_TYPE_HARDWARE,
                D3D_DRIVER_TYPE_WARP,
                D3D_DRIVER_TYPE_REFERENCE,
            };
            UINT numDriverTypes = ARRAYSIZE(driverTypes);

            D3D_FEATURE_LEVEL featureLevels[] =
            {
                D3D_FEATURE_LEVEL_11_1,
                D3D_FEATURE_LEVEL_11_0,
                D3D_FEATURE_LEVEL_10_1,
                D3D_FEATURE_LEVEL_10_0,
            };
            UINT numFeatureLevels = ARRAYSIZE(featureLevels);

            HRESULT hr = S_FALSE;
            for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
            {
                g_driverType = driverTypes[driverTypeIndex];
                hr = D3D11CreateDevice(nullptr, g_driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
                    D3D11_SDK_VERSION, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext);

                if (hr == E_INVALIDARG)
                {
                    // DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it 
                    hr = D3D11CreateDevice(nullptr, g_driverType, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1,
                        D3D11_SDK_VERSION, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext);
                }

                if (SUCCEEDED(hr))
                    break;
            }

            if (FAILED(hr))
            {
                triggerBreakpoint();
                return hr;
            }

            // debug mode
            if (SUCCEEDED(g_pd3dDevice->QueryInterface(__uuidof(ID3D11Debug), (void**)&d3dDebug)))
            {
                ID3D11InfoQueue *d3dInfoQueue = nullptr;
                if (SUCCEEDED(d3dDebug->QueryInterface(__uuidof(ID3D11InfoQueue), (void**)&d3dInfoQueue)))
                {
#ifdef _DEBUG
                    d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
                    d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
#endif

                    D3D11_MESSAGE_ID hide[] =
                    {
                        D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
                        // Add more message IDs here as needed
                    };

                    D3D11_INFO_QUEUE_FILTER filter;
                    memset(&filter, 0, sizeof(filter));
                    filter.DenyList.NumIDs = _countof(hide);
                    filter.DenyList.pIDList = hide;
                    d3dInfoQueue->AddStorageFilterEntries(&filter);
                    d3dInfoQueue->Release();
                }                
            }
            // debug mode

            // Obtain DXGI factory from device (since we used nullptr for pAdapter above) 
            IDXGIFactory1* dxgiFactory = nullptr;
            {
                IDXGIDevice* dxgiDevice = nullptr;
                hr = g_pd3dDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
                if (SUCCEEDED(hr))
                {
                    IDXGIAdapter* adapter = nullptr;
                    hr = dxgiDevice->GetAdapter(&adapter);
                    if (SUCCEEDED(hr))
                    {
                        hr = adapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactory));
                        adapter->Release();
                    }
                    dxgiDevice->Release();
                }
            }
            if (FAILED(hr))
            {
                triggerBreakpoint();
                return hr;
            }

            // Create swap chain 
            IDXGIFactory2* dxgiFactory2 = nullptr;
            hr = dxgiFactory->QueryInterface(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory2));
            if (dxgiFactory2)
            {
                // DirectX 11.1 or later 
                hr = g_pd3dDevice->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&g_pd3dDevice1));
                if (SUCCEEDED(hr))
                {
                    (void)g_pImmediateContext->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&g_pImmediateContext1));
                }

                DXGI_SWAP_CHAIN_DESC1 sd;
                ZeroMemory(&sd, sizeof(sd));
                sd.Width = width;
                sd.Height = height;
                sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
                sd.SampleDesc.Count = 1;
                sd.SampleDesc.Quality = 0;
                sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
                sd.BufferCount = 1;

                hr = dxgiFactory2->CreateSwapChainForHwnd(g_pd3dDevice, hWnd, &sd, nullptr, nullptr, &g_pSwapChain1);
                if (SUCCEEDED(hr))
                {
                    hr = g_pSwapChain1->QueryInterface(__uuidof(IDXGISwapChain), reinterpret_cast<void**>(&g_pSwapChain));
                }

                dxgiFactory2->Release();
            }
            else
            {
                // DirectX 11.0 systems 
                DXGI_SWAP_CHAIN_DESC sd;
                ZeroMemory(&sd, sizeof(sd));
                sd.BufferCount = 1;
                sd.BufferDesc.Width = width;
                sd.BufferDesc.Height = height;
                sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
                sd.BufferDesc.RefreshRate.Numerator = 60;
                sd.BufferDesc.RefreshRate.Denominator = 1;
                sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
                sd.OutputWindow = hWnd;
                sd.SampleDesc.Count = 1;
                sd.SampleDesc.Quality = 0;
                sd.Windowed = TRUE;

                hr = dxgiFactory->CreateSwapChain(g_pd3dDevice, &sd, &g_pSwapChain);
            }

            // Note this tutorial doesn't handle full-screen swapchains so we block the ALT+ENTER shortcut 
            dxgiFactory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER);

            dxgiFactory->Release();

            if (FAILED(hr))
            {
                return hr;
                triggerBreakpoint();
            }

            // Create a render target view 
            ID3D11Texture2D* pBackBuffer = nullptr;
            hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
            if (FAILED(hr))
            {
                triggerBreakpoint();
                return hr;
            }

            hr = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_pRenderTargetView);
            pBackBuffer->Release();
            if (FAILED(hr))
            {
                triggerBreakpoint();
                return hr;
            }

            g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, nullptr);

            // Setup the viewport 
            D3D11_VIEWPORT vp;
            vp.Width = (FLOAT)width;
            vp.Height = (FLOAT)height;
            vp.MinDepth = 0.0f;
            vp.MaxDepth = 1.0f;
            vp.TopLeftX = 0;
            vp.TopLeftY = 0;
            g_pImmediateContext->RSSetViewports(1, &vp);


            if( hr == S_OK )
                return true;
                
            triggerBreakpoint();
    }

    bool deinitialise_device()
    {
        
        /*
        if (g_pSamplerLinear) g_pSamplerLinear->Release();
        if (g_pCBNeverChanges) g_pCBNeverChanges->Release();
        if (g_pCBChangeOnResize) g_pCBChangeOnResize->Release();
        if (g_pCBChangesEveryFrame) g_pCBChangesEveryFrame->Release();
        if (g_pVertexBuffer) g_pVertexBuffer->Release();
        if (g_pIndexBuffer) g_pIndexBuffer->Release();
        if (g_pVertexLayout) g_pVertexLayout->Release();
        if (g_pVertexShader) g_pVertexShader->Release();
        if (g_pPixelShader) g_pPixelShader->Release();
        if (g_pDepthStencil) g_pDepthStencil->Release();
        if (g_pDepthStencilView) g_pDepthStencilView->Release();
        if (g_pRenderTargetView) g_pRenderTargetView->Release();
        if (g_pSwapChain1) g_pSwapChain1->Release();
        if (g_pSwapChain) g_pSwapChain->Release();
        if (g_pImmediateContext1) g_pImmediateContext1->Release();
        if (g_pImmediateContext) g_pImmediateContext->Release();
        if (g_pd3dDevice1) g_pd3dDevice1->Release();
        if (g_pd3dDevice) g_pd3dDevice->Release();
        */
        
        if (g_pImmediateContext) g_pImmediateContext->ClearState();        


      
        if (g_pVertexLayout) g_pVertexLayout->Release();
        if (g_pVertexBuffer) g_pVertexBuffer->Release();


        if (g_pRenderTargetView) g_pRenderTargetView->Release();
        if (g_pSwapChain1) g_pSwapChain1->Release();
        if (g_pSwapChain) g_pSwapChain->Release();
        if (g_pImmediateContext1) g_pImmediateContext1->Release();
        if (g_pImmediateContext) g_pImmediateContext->Release();
        if (g_pd3dDevice1) g_pd3dDevice1->Release();
        if (g_pd3dDevice) g_pd3dDevice->Release();
        
        d3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_SUMMARY | D3D11_RLDO_DETAIL);
        
        d3dDebug->Release();
        return true;
    }
} // namespace GfxLowLevel
#endif // USE_DIRECT3D
