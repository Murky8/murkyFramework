//------------------------------------------------------------------------------
// 2015 J. Coelho
// Platform: Windows 32/64
#include <murkyFramework/src/private/pch.hpp>
#include <murkyFramework/include/version.hpp>
#include <murkyFramework/include/GfxDevice/version_gfxDevice.hpp>

// todo: move libs to solution folder
#ifdef USE_DIRECT3D11
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")
#endif // USE_DIRECT3D11

#ifdef USE_OPENGL
#pragma comment(lib,"OpenGL32.lib")
#include <external/glew/src/glew.c>   // note! This is a cpp file, not hpp.
#endif // USE_OPENGL
