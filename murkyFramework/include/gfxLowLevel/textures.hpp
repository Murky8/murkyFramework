//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: windows/opengl4 at the moment.
#pragma once

#include <vector>

#include <murkyFramework/include/types.hpp>
#include <map>

namespace GfxLowLevel
{
    class TextureRef    //todo rename ref part
    {
    public:
        // Constructors
        TextureRef(){}
        explicit TextureRef(const std::wstring &fileName);     
        TextureRef(u8 *in_data, u32 width, u32 height);
        
        // Destructors
        ~TextureRef();

        // Methods
        u32 getHandle() const;
        
        // Data
        u32 handle;
        //const std::wstring name;
    private:
        // Constructors
        //TextureRef(const TextureRef &rhs) = delete;

        // Methods
        void insertImageData( u8 *in_imageData, u32 width, u32 height);

        // Data
    };
    
    class TextureManager
    {
    public:

        //TextureRef  &getNewTexture(const std::wstring &dirName, const std::wstring &fileName);
        //TextureRef  &getTextureByName(std::wstring &name);
        TextureRef &TextureManager::getTexture(const std::wstring &name);           
        void        setGfxDeviceState_currentTexture(const TextureRef &texture);
        void        loadNewTexture(const std::wstring &dirName, const std::wstring &fileName);
        void        deleteAllTextures();

        //std::vector<TextureRef> textures;
        std::map< std::wstring, GfxLowLevel::TextureRef> textures;

    private:

    };
}
