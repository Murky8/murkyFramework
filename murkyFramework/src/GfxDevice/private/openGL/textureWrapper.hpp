#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/GfxDevice/version_gfxDevice.hpp>
#ifdef USE_OPENGL
#include <external/glew/include/GL/glew.h> 

namespace GfxDevice
{
	class TextureWrapper
	{
	public:
		GLuint value;
	};
}
#endif


#if 0

class 
class TextureWrapper_container
{
    TextureWrapper2() = delete;
    TextureWrapper2(class DeviceTexture *pDevText)
    {    
        pDevText = new DeviceTexture
    }

    ~TextureWrapper2()
    {

    }

private:
    class DeviceTexture *pDevText;
};
#endif