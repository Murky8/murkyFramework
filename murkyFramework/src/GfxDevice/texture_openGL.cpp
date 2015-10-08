//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/version.hpp>
#include <murkyFramework/include/GfxDevice/version_gfxDevice.hpp>
#ifdef USE_OPENGL

#include <murkyFramework/include/GfxDevice/texture.hpp>
#include <murkyFramework/src/GfxDevice/public/gfxDevice.hpp>

#include <vector>
#include <regex>
#include <glew/include/GL/glew.h> 
#include <glew/include/GL/wglew.h>

#include <external/lodepng.h>
#include <murkyFramework/include/common.hpp>
#include <murkyFramework/include/debugUtils.hpp>
#include <murkyFramework/include/stringHelpers.hpp>

namespace GfxDevice
{
    // forward declarations
    void onGfxDeviceErrorTriggerBreakpoint();
    
    // forward declarations internal
    bool loadTextureDataFromFile(std::vector<u8> &textureRawOut, const std::wstring &dirName,
        const std::wstring &fileName, const std::wstring &extensionName,
        u32 &widthOut, u32 &heightOut);

    TextureWrapper   createTextureObject(u8 * in_imageData, u32 width, u32 height)    
    {
		TextureWrapper   texture;
        glGenTextures(1, &texture.value);
        glBindTexture(GL_TEXTURE_2D, texture.value);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, in_imageData);
        onGfxDeviceErrorTriggerBreakpoint();

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D,0);

        return texture;
    }


}
#endif // USE_OPENGL
