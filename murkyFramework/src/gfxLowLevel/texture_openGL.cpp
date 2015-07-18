//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/version.hpp>
#ifdef USE_OPENGL

#include <murkyFramework/include/gfxLowLevel/texture.hpp>

#include <vector>
#include <regex>
#include <glew/include/GL/glew.h> 
#include <glew/include/GL/wglew.h>

#include <external/lodepng.h>
#include <murkyFramework/include/common.hpp>
#include <murkyFramework/include/debugUtils.hpp>
#include <murkyFramework/include/stringHelpers.hpp>

namespace GfxLowLevel
{
    // forward declarations
    void onGfxDeviceErrorTriggerBreakpoint();
    
    void initilise_textureSystem()
    {
    }
    void deinitilise_textureSystem()
    {
    }

    // device specific handle to texture
    struct HandleDeviceTexture
    {
        u32 deviceTexture;        
    };

    // called by constructor only  
    void TextureId::insertImageData(u8 * in_imageData, u32 width, u32 height)
    {
        pHandle = new struct HandleDeviceTexture();
        glGenTextures(1, &pHandle->deviceTexture);
        glBindTexture(GL_TEXTURE_2D, pHandle->deviceTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, in_imageData);
        onGfxDeviceErrorTriggerBreakpoint();

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }

    // loads trexture from file
    bool loadTexture(std::vector<u8> &textureRawOut, const std::wstring &dirName,
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
        
    // constructor
    TextureId::TextureId(const std::wstring &dirName, const std::wstring &fileName,
        const std::wstring &extensionName)
    {
        std::vector<u8> textureRaw;
        u32 width, height;

        bool res = loadTexture(textureRaw, dirName, fileName, extensionName, width, height);
        if (res == false)
            triggerBreakpoint();

        this->insertImageData((u8*)textureRaw.data(), width, height);
    }

    // constructor. create texture from raw
    TextureId::TextureId(u8 *rawData, u32 width, u32 height)   
    {
        this->insertImageData(rawData, width, height);
    }

    // deconstructor
    TextureId::~TextureId()
    {      
        if (pHandle != nullptr)
        {
            glDeleteTextures(1, &pHandle->deviceTexture);
            delete pHandle;
            pHandle = nullptr;
        }
    }

    // todo: repeated in dx version. move to common file
    void TextureManager::insert(const std::wstring &name, TextureId texID)
    {
        textures.insert(std::pair<std::wstring, TextureId>(name, std::move(texID)));
    }

    // todo: repeated in dx version. move to common file
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
    TextureManager::TextureManager()  
    {
    }

    TextureManager::~TextureManager()
    {       

        /*for each (auto &it in this->textures)
        {     
            glDeleteTextures(1, &(it.second.pHandle->deviceTexture));
            delete &(it.second.pHandle->deviceTexture);
        }
*/
    }


}
#endif // USE_OPENGL
