// 2015 J. Coelho.
//------------------------------------------------------------------------------
// Platform: windows/opengl4
#pragma once

#include <gfxLowLevel/textures.hpp>

#include <vector>

#include <glew/include/GL/glew.h> 
#include <glew/include/GL/wglew.h>

#include <lodepng.h>
#include <common.hpp>
#include <stringHelpers.hpp>

namespace GfxLowLevel
{
    // Forward Declarations
    void onGfxDeviceErrorTriggerBreakpoint();
    
    // Constructors
    // Load texture from file
    TextureRef::TextureRef(const std::wstring &fileName)
    {
        std::vector<u8> image; //the raw pixels
        image.reserve(256*256*4);
        u32 width, height;

        auto error = lodepng::decode(image, width, height, ws2s(fileName).c_str());
        if (error != 0)            
            triggerBreakpoint();

        this->insertImageData(image.data(), width, height);        
    }

    TextureRef::TextureRef(u8 *in_data, u32 width, u32 height)
    {
        triggerBreakpoint();//test
        this->insertImageData(in_data, width, height);
    }  

    // Destructors
    TextureRef::~TextureRef()
    {
        triggerBreakpoint();
        glDeleteTextures(1, &handle);
    }
        
    // Methods
    u32 TextureRef::getHandle() const
    {
        return handle;
    }
    
    // Called by constructor only
    void TextureRef::insertImageData(u8 * in_imageData, u32 width, u32 height)
    {
        glGenTextures(1, &handle);
        glBindTexture(GL_TEXTURE_2D, handle);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, in_imageData);
        onGfxDeviceErrorTriggerBreakpoint();

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
}
