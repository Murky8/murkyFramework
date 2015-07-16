#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
#include <murkyFramework/include/version.hpp>

#include <map>
#include <vector>
#include <murkyFramework/include/types.hpp>
#include <murkyFramework/include/debugUtils.hpp>

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

        struct HandleDeviceTexture *pHandle;
        // parameterised constructor
        TextureId(){}
        TextureId(const std::wstring &fileName); // todo: depreciate
        ~TextureId();
        // Methods
           
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
        TextureManager();
        ~TextureManager();

        void TextureManager::insert(const std::wstring &name, const TextureId texID);

        TextureId &TextureManager::getTextureByName(const std::wstring &name);               
        void        loadNewTexture(const std::wstring &dirName, const std::wstring &fileName);        
        std::map< std::wstring, GfxLowLevel::TextureId> textures;
    private:
    };
}
