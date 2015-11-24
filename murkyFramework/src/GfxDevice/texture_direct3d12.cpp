//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/src/private/pch.hpp>
#ifdef USE_DIRECT3D12

static u32 nextTextureID = { 0 };
namespace GfxDevice
{    
    // forward declarations
    
    // functions
	bool loadTextureDataFromFile(std::vector<u8> &textureRawOut, const std::wstring &dirName,
		const std::wstring &fileName, const std::wstring &extensionName,
		u32 &widthOut, u32 &heightOut);

//https://github.com/Microsoft/DirectX-Graphics-Samples/blob/master/Samples/D3D1211On12/src/D3D1211On12.cpp
	TextureWrapper   createTextureObject(u8 * in_imageData, u32 width, u32 height)
	{
		TextureWrapper   texture;
        texture.iTexture = nextTextureID;
        ++nextTextureID;
        ComPtr<ID3D12Resource> textureUploadHeap;   // kkep in scope until command list executed

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
