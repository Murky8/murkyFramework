#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
namespace murkyFramework {
namespace GfxDevice
{
    // forward declarations
    TextureWrapper		createTextureObjectFromFile(const std::wstring &dirName,
        const				std::wstring &fileName, const std::wstring &extensionName);
    TextureWrapper		createTestTextureObject();
    void                initilise_textureSystem();
    void                deinitilise_textureSystem();
}
}//namespace murkyFramework