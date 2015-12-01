//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/src/private/pch.hpp>

namespace GfxDevice
{

    TextureWrapper   createTextureObject(u8 * in_imageData, u32 width, u32 height)
    {
        TextureWrapper   texture;
        glGenTextures(1, &texture.value);
        glBindTexture(GL_TEXTURE_2D, texture.value);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, in_imageData);
        onGfxDeviceErrorTriggerBreakpoint();

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

        return texture;
    }
}
