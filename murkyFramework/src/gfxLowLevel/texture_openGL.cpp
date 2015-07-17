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
    
    struct HandleDeviceTexture
    {
        u32 deviceTexture;        
    };

    // NEW just loads the data
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


    // NEW 
    TextureId::TextureId(u8 *imageRawData, u32 width, u32 height)
    {
        //this->pHandle = new struct HandleDeviceTexture()
    }

    // Constructors

    //--------------------------------------------------------------------------
    // NEW any texture(file or memory) creation calls this
    TextureId::TextureId(const std::wstring &dirName, const std::wstring &fileName,
        const std::wstring &extensionName)
    {
        std::vector<u8> textureRaw;
        u32 width, height;

        bool res = loadTexture(textureRaw, dirName, fileName, extensionName, width, height);
        if (res == false)
            triggerBreakpoint();

        pHandle = new struct HandleDeviceTexture();
        glGenTextures(1, &pHandle->deviceTexture);
        glBindTexture(GL_TEXTURE_2D, pHandle->deviceTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureRaw.data());
        onGfxDeviceErrorTriggerBreakpoint();

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        //TextureId createTextureFromRaw(const void  *pData, u32 width, u32 height);;

    }

    TextureId::~TextureId()
    {      
        if (pHandle != nullptr)
        {
            glDeleteTextures(1, &pHandle->deviceTexture);
            delete pHandle;
            pHandle = nullptr;
        }
    }


    // Load texture from file
    TextureId::TextureId(const std::wstring &fileName)
    {     
        pHandle = new HandleDeviceTexture();
        std::vector<u8> image; //the raw pixels
        image.reserve(256*256*4);
        u32 width, height;

        auto error = lodepng::decode(image, width, height, ws2s(fileName).c_str());
        if (error != 0)            
            triggerBreakpoint();

        this->insertImageData(image.data(), width, height);        
    }            

    
    // Methods
 /*   TextureId createTextureFromRaw(const void  *pData, u32 width, u32 height)
    {
        TextureId tid;
        tid.pHandle = new HandleDeviceTexture;

        glGenTextures(1, &tid.pHandle->deviceTexture);
        glBindTexture(GL_TEXTURE_2D, tid.pHandle->deviceTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pData);
        onGfxDeviceErrorTriggerBreakpoint();

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        
        return tid;
    }*/


    // Called by constructor only
    void TextureId::insertImageData(u8 * in_imageData, u32 width, u32 height)
    {

        glGenTextures(1, &pHandle->deviceTexture);
        glBindTexture(GL_TEXTURE_2D, pHandle->deviceTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, in_imageData);
        onGfxDeviceErrorTriggerBreakpoint();

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }

    void TextureManager::loadNewTexture(const std::wstring &dirName, const std::wstring &fileName)
    {
        
        std::wstring fullPath = dirName + fileName;        

        TextureId newTexture(fullPath);

        //std::string str2 = str.substr (12,12);
        std::wregex regexExpr(L"png");
        if (regex_search(fileName, regexExpr))
        {
            std::wstring name( fileName.substr(0, fileName.size() - 4) );// todo: do properly

            textures.insert(std::pair<std::wstring, TextureId>(name, std::move(newTexture)));
            //this->textures[name] = newTexture;
        }
        else
        {
            triggerBreakpoint();
        }


        
        // strip extension of name
        //std::wstring nameNaked = fileName

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
        for each (auto &it in this->textures)
        {     
            glDeleteTextures(1, &(it.second.pHandle->deviceTexture));
            delete &(it.second.pHandle->deviceTexture);
        }

    }


}
#endif // USE_OPENGL
