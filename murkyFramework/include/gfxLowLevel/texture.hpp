//------------------------------------------------------------------------------
// 2015 J. Coelho.

#pragma once

#include <vector>

#include <murkyFramework/include/types.hpp>
#include <map>
#include <debugUtils.hpp>

namespace GfxLowLevel
{
    //extern int idCt;

    //template<typename T>
    //class CachedRefByName()
    //{
    //}

    class TextureId    //todo rename ref part
    {
    public:
        // data
        u32 handle; 
       
        // parameterised constructor
        TextureId(){}
        TextureId(const std::wstring &fileName); // todo: depreciate
                
        // Methods
        u32 getHandle() const;
        
    private:
        // constructors
        TextureId(u8 *in_data, u32 width, u32 height);// todo: depreciate
        //TextureRef(const TextureRef &rhs) = delete;

        // methods
        void insertImageData( u8 *in_imageData, u32 width, u32 height);// todo: depreciate
        
    };

    

    class TextureManager
    {
    public:
        TextureId &TextureManager::getTextureByName(const std::wstring &name);               
        void        loadNewTexture(const std::wstring &dirName, const std::wstring &fileName);
        //void        deleteAllTextures();
        //std::vector<TextureRef> textures;
        ~TextureManager();
        std::map< std::wstring, GfxLowLevel::TextureId> textures;

    private:

    };
}
