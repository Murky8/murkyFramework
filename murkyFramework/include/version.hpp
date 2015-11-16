#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho
// Platform: Windows 32/64

// windows
#if WIN32 || _WIN64
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#define WINDOWS
#if _WIN64
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif

// android
//#ifdef ANDROID

//// Check GCC
//#if __GNUC__
//#if __x86_64__ || __ppc64__
//#define ENVIRONMENT64
//#else
//#define ENVIRONMENT32
//#endif
//#endif
//#ifdef _MSC_VER
//#pragma warning( push )
//#pragma warning( disable: 4172 ) //returning address of local variable or temporary
//#endif
//
//#ifdef __INTEL_COMPILER
//#pragma warning( push )
//#pragma warning( disable: 1251 ) //returning address of local variable or temporary
//#endif
//
//
//#ifdef __INTEL_COMPILER
//#pragma warning( pop )
//#endif
//
//#ifdef _MSC_VER
//#pragma warning( pop )
//#endif