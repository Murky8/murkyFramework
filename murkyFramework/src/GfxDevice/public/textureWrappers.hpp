#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/GfxDevice/version_gfxDevice.hpp>



// would preffer to use opaque pointers to hide devce specific stuff
#ifdef USE_OPENGL
#include <murkyFramework/src/GfxDevice/private/openGL/textureWrapper.hpp>
#endif 

#ifdef USE_DIRECT3D11
#include <murkyFramework/src/GfxDevice/private/d3d11/textureWrapper.hpp>
#endif 

#ifdef USE_DIRECT3D12
#include <murkyFramework/src/GfxDevice/private/d3d12/textureWrapper.hpp>
#endif 



