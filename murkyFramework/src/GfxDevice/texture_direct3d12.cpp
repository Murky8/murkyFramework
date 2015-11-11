//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/src/private/pch.hpp>
#include <murkyFramework/include/GfxDevice/version_gfxDevice.hpp>
#ifdef USE_DIRECT3D12

#include <murkyFramework/include/GfxDevice/texture.hpp>
#include <vector>

#include <murkyFramework/src/GfxDevice/public/textureWrappers.hpp>

static u32 nextTextureID = { 0 };
namespace GfxDevice
{    

    // forward declarations
    
    // functions
	bool loadTextureDataFromFile(std::vector<u8> &textureRawOut, const std::wstring &dirName,
		const std::wstring &fileName, const std::wstring &extensionName,
		u32 &widthOut, u32 &heightOut);

	TextureWrapper   createTextureObject(u8 * in_imageData, u32 width, uw32 height)
	{
		TextureWrapper   texture;
        texture.iTexture = nextTextureID;
        ++nextTextureID;

		return texture;
	}

    void initilise_textureSystem()
    {    
    }

    void deinitilise_textureSystem()
    {     
    }
}
#endif // USE_DIRECT3D12
