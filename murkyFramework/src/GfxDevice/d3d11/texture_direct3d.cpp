//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/src/pch.hpp>
#ifdef USE_DIRECT3D11
  
//http://gamedev.stackexchange.com/questions/14507/loading-a-texture2d-array-in-directx11
//https://msdn.microsoft.com/en-us/library/windows/desktop/ff476904(v=vs.85).aspx
#define deviceObj  g_appDebug->render->gfxDevice

namespace GfxDevice
{    
    // functions
    void initilise_textureSystem()
    {
        // Create the sample state
        // this has to be done after pixel shader. todo: CHECK
        HRESULT res;
        deviceObj->g_pSamplerLinear = nullptr;
        D3D11_SAMPLER_DESC sampDesc;
        ZeroMemory(&sampDesc, sizeof(sampDesc));
        sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        sampDesc.MinLOD = 0;
        sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

        res = deviceObj->g_pd3dDevice->CreateSamplerState(&sampDesc, &deviceObj->g_pSamplerLinear);

        if (FAILED(res))
            triggerBreakpoint();
        // Create the sample state
    }

    void deinitilise_textureSystem()
    {
        deviceObj->g_pSamplerLinear->Release();
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
        hr = deviceObj->g_pd3dDevice->CreateTexture2D(&desc, &subRes, &texture);

        if (FAILED(hr))
            triggerBreakpoint();

        D3D11_SHADER_RESOURCE_VIEW_DESC resviewDesc;
        memset(&resviewDesc, 0, sizeof(resviewDesc));
        resviewDesc.Format = format;
        resviewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        resviewDesc.Texture2D.MipLevels = 1;           


		TextureWrapper newTexture;
        hr = deviceObj->g_pd3dDevice->CreateShaderResourceView(texture,
            &resviewDesc, &newTexture.deviceTexture);
        
        if (FAILED(hr))
            triggerBreakpoint();

        return newTexture;
    }    
}
#endif // USE_DIRECT3D11
