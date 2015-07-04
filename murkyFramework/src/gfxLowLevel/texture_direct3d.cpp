// 2015 J. Coelho.
//------------------------------------------------------------------------------
// Platform: windows/opengl4
#include <murkyFramework/include/version.hpp>
#ifdef USE_DIRECT3D

#include <murkyFramework/include/gfxLowLevel/textures.hpp>

#include <vector>
#include <regex>

#include <murkyFramework/include/lodepng.h>
#include <murkyFramework/include/common.hpp>
#include <murkyFramework/include/debugUtils.hpp>
#include <murkyFramework/include/stringHelpers.hpp>

namespace GfxLowLevel
{
    // forward declarations
    void onGfxDeviceErrorTriggerBreakpoint();
    
    // Constructors
    // Load texture from file
    TextureId::TextureId(const std::wstring &fileName)
    {     
        triggerBreakpoint();
    }    
        
    // Methods
    u32 TextureId::getHandle() const
    {
        return handle;
    }
    
    // Called by constructor only
    void TextureId::insertImageData(u8 * in_imageData, u32 width, u32 height)
    {
        triggerBreakpoint();
    }

    void TextureManager::loadNewTexture(const std::wstring &dirName, const std::wstring &fileName)
    {
        
        std::wstring fullPath = dirName + fileName;        

        TextureId newTexture(fullPath);

        //std::string str2 = str.substr (12,12);
        std::wregex regexExpr(L"png");
        if (regex_search(fileName, regexExpr))
        {
            std::wstring name( fileName.substr(0, fileName.size() - 4) );

            textures.insert(std::pair<std::wstring, TextureId>(name, newTexture));
            //this->textures[name] = newTexture;
        }
        else
        {
            triggerBreakpoint();
        }        
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

    void TextureManager::setGfxDeviceState_currentTexture( const TextureId &texture )
    {
        
        triggerBreakpoint();
    }

    void TextureManager::deleteAllTextures()
    {
        
        triggerBreakpoint();

        /*for each (auto &it in this->textures)
        {
            debugLog << L"gfxDevice: released texture \n";            
        }
        */
    }
}
#endif // USE_DIRECT3D
