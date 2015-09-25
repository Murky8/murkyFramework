//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/version.hpp>
#include <murkyFramework/include/GfxDevice/version_gfxDevice.hpp>
#ifdef USE_DIRECT3D12

#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include <murkyFramework/include/GfxDevice/texture.hpp>
#include <vector>
#include <regex>
#include <murkyFramework/include/common.hpp>
#include <murkyFramework/include/debugUtils.hpp>  

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
