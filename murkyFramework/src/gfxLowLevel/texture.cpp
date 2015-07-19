//------------------------------------------------------------------------------
// 2015 J. Coelho.
#include <murkyFramework/include/gfxLowLevel/version_gfxDevice.hpp>
#include <murkyFramework/include/gfxLowLevel/texture.hpp>

#include <vector>
#include <map>
#include <murkyFramework/include/types.hpp>
#include <murkyFramework/include/stringHelpers.hpp>
#include <murkyFramework/include/debugUtils.hpp>
#include <external/lodepng.h>

namespace GfxLowLevel
{
    // forward declarations
    TextureId   createTextureObject(u8 * in_imageData, u32 width, u32 height);

    // forward declarations internal
    bool loadTextureDataFromFile(std::vector<u8> &textureRawOut, const std::wstring &dirName,
        const std::wstring &fileName, const std::wstring &extensionName,
        u32 &widthOut, u32 &heightOut);

    // functions
    void TextureManager::insert(const std::wstring &name, TextureId texID)
    {
        textures.insert(std::pair<std::wstring, TextureId>(name, std::move(texID)));
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

    TextureId   createTextureObjectFromFile(const std::wstring &dirName,
        const std::wstring &fileName, const std::wstring &extensionName)
    {
        std::vector<u8> textureRaw;
        u32 width, height;

        loadTextureDataFromFile(textureRaw, dirName, fileName, extensionName,
            width, height);

        TextureId   TextureId = createTextureObject(textureRaw.data(), width, height);
        return TextureId;
    }

    TextureId   createTestTextureObject()
    {
        u8  t[256][256][4];
        for (auto i = 0; i < 256; ++i)
            for (auto j = 0; j < 256; ++j)
            {
                t[j][i][0] = i*i + j*j;
                t[j][i][1] = t[j][i][0];
                t[j][i][2] = t[j][i][3] = 0;
            }

        return createTextureObject((u8*)t, 256, 256);
    }

    // loads trexture from file
    bool loadTextureDataFromFile(std::vector<u8> &textureRawOut, const std::wstring &dirName,
        const std::wstring &fileName, const std::wstring &extensionName,
        u32 &widthOut, u32 &heightOut)
    {
        std::wstring fullPath = dirName + L"/" + fileName + L"." + extensionName;

        if (extensionName != L"png")
            return false;

        auto error = lodepng::decode(textureRawOut, widthOut, heightOut, ws2s(fullPath).c_str());
        if (error != 0)
            return false;

        return true;
    }
}