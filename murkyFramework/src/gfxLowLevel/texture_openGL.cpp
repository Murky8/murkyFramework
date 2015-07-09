// 2015 J. Coelho.
//------------------------------------------------------------------------------
// Platform: windows/opengl4
#include <version.hpp>
#ifdef USE_OPENGL


#include <gfxLowLevel/texture.hpp>

#include <vector>
#include <regex>
#include <glew/include/GL/glew.h> 
#include <glew/include/GL/wglew.h>

#include <lodepng.h>
#include <common.hpp>
#include <debugUtils.hpp>
#include <stringHelpers.hpp>
namespace GfxLowLevel
{
    // forward declarations
    void onGfxDeviceErrorTriggerBreakpoint();
    
    // Constructors
    // Load texture from file
    TextureId::TextureId(const std::wstring &fileName)
    {     
        std::vector<u8> image; //the raw pixels
        image.reserve(256*256*4);
        u32 width, height;

        auto error = lodepng::decode(image, width, height, ws2s(fileName).c_str());
        if (error != 0)            
            triggerBreakpoint();

        this->insertImageData(image.data(), width, height);        
    }    
        
    // Methods
    u32 TextureId::getHandle() const
    {
        return handle;
    }
    
    // Called by constructor only
    void TextureId::insertImageData(u8 * in_imageData, u32 width, u32 height)
    {
        glGenTextures(1, &handle);
        glBindTexture(GL_TEXTURE_2D, handle);
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
       
    TextureManager::~TextureManager()
    {        
        for each (auto &it in this->textures)
        {     
            glDeleteTextures(1, &( it.second.handle ));
        }

    }


}
#endif // USE_OPENGL
