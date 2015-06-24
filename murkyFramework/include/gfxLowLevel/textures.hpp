//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: windows/opengl4 at the moment.
#pragma once

#include <vector>

#include <murkyFramework/include/types.hpp>

namespace GfxLowLevel
{
    class TextureRef    //todo rename ref part
    {
    public:
        // Constructors
        explicit TextureRef(const std::wstring &fileName);     
        TextureRef(u8 *in_data, u32 width, u32 height);
        
        // Destructors
        ~TextureRef();

        // Methods
        u32 getHandle() const;
        
        // Data
        //const std::wstring name;
    private:
        // Constructors
        TextureRef() = delete;

        // Methods
        void insertImageData( u8 *in_imageData, u32 width, u32 height);

        // Data
        u32 handle;
    };
}

// Constructors
// Destructors
// Methods
// Data