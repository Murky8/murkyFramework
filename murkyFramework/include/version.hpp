#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho
// Platform: Windows 32/64

//#define USE_OPENGL
#define USE_DIRECT3D

// Check windows
#if WIN32 || _WIN64
#if _WIN64
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif

//// Check GCC
//#if __GNUC__
//#if __x86_64__ || __ppc64__
//#define ENVIRONMENT64
//#else
//#define ENVIRONMENT32
//#endif
//#endif



