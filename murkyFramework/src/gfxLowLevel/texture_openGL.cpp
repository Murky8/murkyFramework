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
    // Constructors
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

            textures.insert(std::pair<std::wstring, TextureId>(name, newTexture));
            //this->textures[name] = newTexture;
        }
        else
        {
            triggerBreakpoint();
        }
        
        // strip extension of name
        //std::wstring nameNaked = fileName

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
