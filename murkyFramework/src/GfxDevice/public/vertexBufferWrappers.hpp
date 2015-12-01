#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11


// Tried to make this stuff (device specific) private, but quicker just to include following
// rather than complicated wrapper system. At this stage of development.

#ifdef USE_OPENGL
#include <murkyFramework/src/GfxDevice/openGL4/vertexBufferWrapper.hpp>
#endif 

#ifdef USE_DIRECT3D11
#include <murkyFramework/src/GfxDevice/d3d11/vertexBufferWrapper.hpp>
#endif 

#ifdef USE_DIRECT3D12
#include <murkyFramework/src/GfxDevice/d3d12/vertexBufferWrapper.hpp>
#endif 

