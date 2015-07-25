//------------------------------------------------------------------------------
// 2015 J. Coelho.
#include <murkyFramework/include/version.hpp>
#include <murkyFramework/include/gfxLowLevel/version_gfxDevice.hpp>
#include <murkyFramework/include/gfxLowLevel/texture.hpp>

#include <vector>
#include <map>
#include <murkyFramework/include/types.hpp>
#include <murkyFramework/include/stringHelpers.hpp>
#include <murkyFramework/include/debugUtils.hpp>
#include <external/lodepng.h>
#include <external/boost/multi_array.hpp>
#include <memory>
#include <array>

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
        const auto subDiv = 256;

        //typedef boost::multi_array<u8, 3> array_type;
        //typedef array_type::index index;
        //array_type t(boost::extents[dim][dim][4]);

        boost::multi_array<u8, 3> t(boost::extents[subDiv][subDiv][4]);


        for (auto i = 0; i < subDiv; ++i)
            for (auto j = 0; j < subDiv; ++j)
            {                                     
                //f32 fi = (f32)i*256.f /subDiv;
                //f32 fj = (f32)j*256.f /subDiv;
                //double nulll;
                //t[j][i][0] = 255.f * modf(modf(fi*fi, &nulll) + modf(fj*fj, &nulll), &nulll);
                //t[j][i][1] =  i*i*2 + j*j*2;
                //t[j][i][2] = 0;// i*i + j*j * 2;
                t[j][i][0] = i*i + j*j;
                t[j][i][1] =  i*i*2 + j*j*2;
                t[j][i][2] =  30;
            }

        return createTextureObject((u8*)t.data(), subDiv, subDiv);
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