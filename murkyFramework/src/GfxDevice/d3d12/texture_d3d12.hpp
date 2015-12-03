#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
namespace murkyFramework {}//namespace murkyFramework
namespace GfxDevice
{  
    class TextureId    //todo rename ref part
    {
    public:       
        // data
        struct HandleDeviceTexture *pHandle;
                        
        // constructor
        TextureId(HandleDeviceTexture*);
              
        // destructor
        ~TextureId();      
        
        TextureId() = delete;                                   // default constructor
        TextureId(const TextureId &rhs) = delete;               // copy constructor                      
        TextureId &operator=(const TextureId& rhs) = delete;    // copy assign
        TextureId &operator=(TextureId&& rhs) = delete;         // c++11 move assigment operator
        
        // move constructor
        TextureId(TextureId&& rhs)
        {
            this->pHandle = rhs.pHandle;
            rhs.pHandle = nullptr;
        }
        
    private:                
        // called by a constructor only 
        void insertImageData( u8 *in_imageData, u32 width, u32 height);        
    };       

    class TextureManager
    {
    public:  
        void TextureManager::insert(const std::wstring &name, TextureId texID);
        TextureId &TextureManager::getTextureByName(const std::wstring &name);               
        
    private:
        std::map< std::wstring, GfxDevice::TextureId> textures;
    };
}
