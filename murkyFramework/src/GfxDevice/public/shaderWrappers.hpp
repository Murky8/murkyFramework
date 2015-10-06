#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/GfxDevice/version_gfxDevice.hpp>

// Tried to make this stuff (device specific) private, but quicker just to include following
// rather than complicated wrapper system. At this stage of development.

#ifdef USE_OPENGL
#include <murkyFramework/src/GfxDevice/private/openGL/shaderWrapper.hpp>
#endif 

#ifdef USE_DIRECT3D11
#include <murkyFramework/src/GfxDevice/private/d3d11/shaderWrapper.hpp>
#endif 

#ifdef USE_DIRECT3D12
#include <murkyFramework/src/GfxDevice/private/d3d12/shaderWrapper.hpp>
#endif 

