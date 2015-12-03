#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11

#ifdef USE_OPENGL
namespace murkyFramework {}//namespace murkyFramework
namespace GfxDevice
{
	class ShaderWrapper
	{
	public:
		GLuint value;
	};
}
#endif