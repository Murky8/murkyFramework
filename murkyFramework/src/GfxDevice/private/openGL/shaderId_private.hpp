#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/GfxDevice/version_gfxDevice.hpp>
#ifdef USE_OPENGL
#include <external/glew/include/GL/glew.h> 
#include <murkyFramework/src/GfxDevice/public/shaderId.hpp>
namespace GfxDevice
{
	class ShaderId_private3
	{
	public:
		GLuint value;
	};
}
#endif