//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/version.hpp>
#ifdef USE_DIRECT3D

#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include "DDSTextureLoader.h"

#include <murkyFramework/include/gfxLowLevel/texture.hpp>

#include <vector>
#include <regex>


#include <external/lodepng.h>
#include <murkyFramework/include/common.hpp>
#include <murkyFramework/include/debugUtils.hpp>
#include <murkyFramework/include/stringHelpers.hpp>

        //// http://stackoverflow.com/questions/24958213/load-texture-in-directx-11-1
        //// http://stackoverflow.com/questions/14802205/creating-texture-programmatically-directx

namespace GfxLowLevel
{
    //--------------------------------------------------------------------------------------
    // forward declarations
    using namespace DirectX;
    void        onGfxDeviceErrorTriggerBreakpoint();
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

    extern ID3D11SamplerState          *g_pSamplerLinear;
    // Constructors
    // Load texture from file
    TextureId::TextureId(const std::wstring &fileName)
    {     
        //triggerBreakpoint();
    }    
        
    // Methods
    u32 TextureId::getHandle() const
    {
        return handle;
    }
    
    //// Called by constructor only
    //void TextureId::insertImageData(u8 * in_imageData, u32 width, u32 height)
    //{
    //    triggerBreakpoint();
    //}

    void TextureManager::loadNewTexture(const std::wstring &dirName, const std::wstring &fileName)
    {
        std::wstring fullPath = dirName + fileName;
        
        //std::string str2 = str.substr (12,12);
        std::wregex regexExpr(L"dds");
        std::wstring name;
        if (regex_search(fileName, regexExpr))
        {
            name = fileName.substr(0, fileName.size() - 4); // todo: do properly
        }
        else
        {
            triggerBreakpoint();
        }
        HRESULT res;
        //std::wstring fullPath = dirName + fileName;        
        //
        //std::vector<u8> image; //the raw pixels

        //image.reserve(256 * 256 * 4);
        //u32 width, height;
        //auto error = lodepng::decode(image, width, height, ws2s(fullPath).c_str());

        //if (error != 0)
        //    triggerBreakpoint();

        // d3d
        ID3D11ShaderResourceView    *pTextureRV = nullptr;
        g_pSamplerLinear = nullptr;

        res = CreateDDSTextureFromFile(g_pd3dDevice, fullPath.c_str(), nullptr, &pTextureRV);
        if (FAILED(res))    
            triggerBreakpoint();

        // Create the sample state
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

        TextureId textureId;
        textureId.handle = (u32)pTextureRV;
        
        textures.insert(std::pair<std::wstring, TextureId>(name, textureId));
        
        //D3D11_TEXTURE2D_DESC desc;
        //ZeroMemory(&desc, sizeof(desc));
        //desc.Width = width;
        //desc.Height = height;
        //desc.MipLevels = desc.ArraySize = 1;
        //desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        //desc.SampleDesc.Count = 1;
        //desc.Usage = D3D11_USAGE_DYNAMIC;
        //desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        //desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        //desc.MiscFlags = 0;


        //ID3D11Texture2D *pTexture = NULL;
        //res = g_pd3dDevice->CreateTexture2D(&desc, NULL, &pTexture);
        //if (res != S_OK)
        //    triggerBreakpoint();

        //D3D11_MAPPED_SUBRESOURCE mappedResource;
        //g_pImmediateContext->Map(m_texture, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
        //memcpy(mappedResource.pData, buffer, rowspan*height);
        //g_pImmediateContext->Unmap(m_texture, 0);


        //res = g_pImmediateContext->Map(m_instanceBuffer.Get(), 0, D3D11_MAP_WRITE, 0, &inst)
          //  if (res != S_OK)
            //    triggerBreakpoint();
        // d3d


        // opengl TextureId newTexture(fullPath);


        /*
        std::wregex regexExpr(L"png");
        if (regex_search(fileName, regexExpr))
        {
            std::wstring name( fileName.substr(0, fileName.size() - 4) );

            textures.insert(std::pair<std::wstring, TextureId>(name, newTexture));   
        }
        else
        {
            triggerBreakpoint();
        }  
        */
    }

    TextureId &TextureManager::getTextureByName(const std::wstring &name)
    {       
        auto it = textures.find(name);
        if (it != textures.end())
        {
            return it->second;
        }
        else
        {
            triggerBreakpoint();
            return it->second;
        }
    }

    TextureManager::~TextureManager()
    {
        for (auto &it : textures)
        {
            if (it.second.handle)
            {
                ((ID3D11ShaderResourceView*)(it.second.handle))->Release();
            }
        }
    }

}
#endif // USE_DIRECT3D
