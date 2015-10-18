//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/version.hpp>
#include <murkyFramework/include/GfxDevice/version_gfxDevice.hpp>
#ifdef USE_DIRECT3D11

#include <windows.h>
#include <d3d11_1.h>
#include <murkyFramework/include/GfxDevice/texture.hpp>
#include <murkyFramework/include/common.hpp>
#include <murkyFramework/src/GfxDevice/public/gfxDevice.hpp>

  
//http://gamedev.stackexchange.com/questions/14507/loading-a-texture2d-array-in-directx11
//https://msdn.microsoft.com/en-us/library/windows/desktop/ff476904(v=vs.85).aspx

namespace GfxDevice
{    
    // forward declarations
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
    extern      ID3D11VertexShader*     g_pVertexShader;
    extern      ID3D11PixelShader*      g_pPixelShader;
    extern      ID3D11InputLayout*      g_pVertexLayout;
    extern      ID3D11Buffer*           g_pVertexBuffer;  
    extern      ID3D11SamplerState          *g_pSamplerLinear;    

    // functions
    void initilise_textureSystem()
    {
        // Create the sample state
        // this has to be done after pixel shader. todo: CHECK
        HRESULT res;
        g_pSamplerLinear = nullptr;
        D3D11_SAMPLER_DESC sampDesc;
        ZeroMemory(&sampDesc, sizeof(sampDesc));
        sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        sampDesc.MinLOD = 0;
        sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

        res = g_pd3dDevice->CreateSamplerState(&sampDesc, &g_pSamplerLinear);

        if (FAILED(res))
            triggerBreakpoint();
        // Create the sample state
    }

    void deinitilise_textureSystem()
    {
        g_pSamplerLinear->Release();
    }   
        
    TextureWrapper   createTextureObject(u8 * in_imageData, u32 width, u32 height)    
    {        
        HRESULT hr;
        D3D11_TEXTURE2D_DESC desc;
        auto format = DXGI_FORMAT_R8G8B8A8_UNORM;
        auto bytesPerPixel = 4;

        ZeroMemory(&desc, sizeof(desc));
        desc.Width = width;
        desc.Height = height;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = format;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA subRes;
        subRes.pSysMem = (void*)in_imageData;
        subRes.SysMemPitch = bytesPerPixel*width;
        subRes.SysMemSlicePitch = subRes.SysMemPitch * height;

        ID3D11Texture2D* texture = nullptr;
        hr = g_pd3dDevice->CreateTexture2D(&desc, &subRes, &texture);

        if (FAILED(hr))
            triggerBreakpoint();

        D3D11_SHADER_RESOURCE_VIEW_DESC resviewDesc;
        memset(&resviewDesc, 0, sizeof(resviewDesc));
        resviewDesc.Format = format;
        resviewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        resviewDesc.Texture2D.MipLevels = 1;           


		TextureWrapper newTexture;
        hr = g_pd3dDevice->CreateShaderResourceView(texture,
            &resviewDesc, &newTexture.deviceTexture);
        
        if (FAILED(hr))
            triggerBreakpoint();

        return newTexture;
    }    
}
#endif // USE_DIRECT3D11
