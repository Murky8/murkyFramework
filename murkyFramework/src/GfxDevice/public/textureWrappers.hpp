#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11

// would preffer to use opaque pointers to hide devce specific stuff
#ifdef USE_OPENGL
#include <murkyFramework/src/GfxDevice/openGL4/textureWrapper.hpp>
#endif 

#ifdef USE_DIRECT3D11
#include <murkyFramework/src/GfxDevice/d3d11/textureWrapper.hpp>
#endif 

#ifdef USE_DIRECT3D12
#include <murkyFramework/src/GfxDevice/d3d12/textureWrapper.hpp>
#endif 



