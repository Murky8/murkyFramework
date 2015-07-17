#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
#include <murkyFramework/include/version.hpp>

#include <map>
#include <vector>
#include <murkyFramework/include/types.hpp>
#include <murkyFramework/include/debugUtils.hpp>
#include <murkyFramework/include/common.hpp>

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

        struct HandleDeviceTexture *pHandle = nullptr;
        
        //default constructor
        TextureId() = delete;
        
        // parameterised constructor
        TextureId(
            const std::wstring &dirName,
            const std::wstring &fileName,
            const std::wstring &extensionName
            );

        // copy constructor
        TextureId(const TextureId &rhs) = delete;
                
        // copy assign
        TextureId& operator=(const TextureId& rhs) = delete;
        
        // c++11 move constructor
        TextureId(TextureId&& rhs)
        {
            this->pHandle = rhs.pHandle;
            rhs.pHandle = nullptr;
        }
        
        // c++11 move assigment operator
        TextureId &operator=(TextureId&& rhs) = delete;
        /*{
            if (this->pHandle != nullptr)
            {
                this->pHandle = rhs.pHandle;                
                rhs.pHandle = nullptr;
            }
        }
        */
        TextureId(const std::wstring &fileName); // todo: depr..

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

        void TextureManager::insert(const std::wstring &name, TextureId texID);

        TextureId &TextureManager::getTextureByName(const std::wstring &name);               
        void        loadNewTexture(const std::wstring &dirName, const std::wstring &fileName);        
        std::map< std::wstring, GfxLowLevel::TextureId> textures;
    private:
    };
}
