//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/version.hpp>
#include <murkyFramework/include/GfxDevice/version_gfxDevice.hpp>
#ifdef USE_DIRECT3D12

#include <windows.h>
#include <directxcolors.h>
#include <murkyFramework/include/GfxDevice/texture.hpp>
#include <vector>
#include <regex>
#include <murkyFramework/include/common.hpp>
#include <murkyFramework/include/debugUtils.hpp>  
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
namespace GfxDevice
{    
    // forward declarations
    
    // functions
    void initilise_textureSystem()
    {    
    }

    void deinitilise_textureSystem()
    {     
    }

    // device specific handle to texture
    struct HandleDeviceTexture
    {
        //f3D11ShaderResourceView *deviceTexture;
    };    

    // TextureId constructor
    TextureId::TextureId(HandleDeviceTexture* pHandleDeviceTexture) :
        pHandle(pHandleDeviceTexture)
    {
    }

    // TextureId deconstructor
    TextureId::~TextureId()
    {
        if (pHandle != nullptr)
        {
           // pHandle->deviceTexture->Release();
            delete pHandle;
            pHandle = nullptr;
        }
    }
      
    TextureId   createTextureObject(u8 * in_imageData, u32 width, u32 height)    
    {
        TextureId   textureId( new HandleDeviceTexture );                
        return textureId;
    }
}
#endif // USE_DIRECT3D12
