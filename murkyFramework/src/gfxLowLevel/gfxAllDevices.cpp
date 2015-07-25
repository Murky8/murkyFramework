//------------------------------------------------------------------------------
// 2015 J. Coelho
// Platform: Windows 32/64
#include <murkyFramework/include/version.hpp>
#include <murkyFramework/include/gfxLowLevel/version_gfxDevice.hpp>

// todo: move libs to solution folder

#ifdef USE_DIRECT3D
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")
#endif // USE_DIRECT3D

#ifdef USE_OPENGL
#pragma comment(lib,"OpenGL32.lib")
#include <external/glew/src/glew.c>   // note! This is a cpp file, not hpp.
#endif // USE_OPENGL
