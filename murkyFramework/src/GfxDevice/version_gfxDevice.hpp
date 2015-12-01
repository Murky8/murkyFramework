#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho
// Platform: Windows 32/64
// note: partially depreciated as defines are now stored in project settings.

//#define USE_OPENGL
//#define USE_DIRECT3D11
//#define USE_DIRECT3D12

#ifndef USE_OPENGL
#ifndef USE_DIRECT3D11
#ifndef USE_DIRECT3D12

//stop compilation: at least one of these must be defined
#error Gfx device version needs to be specified.

#endif
#endif
#endif

