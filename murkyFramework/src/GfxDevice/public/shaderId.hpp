#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/GfxDevice/version_gfxDevice.hpp>

#ifdef USE_OPENGL
#include <murkyFramework/src/GfxDevice/private/openGL/shaderId_private.hpp>
#endif 

#ifdef USE_DIRECT3D11
#include <murkyFramework/src/GfxDevice/direct3d11/openGL/shaderId_private.hpp>
#endif 

#ifdef USE_DIRECT3D12
#include <murkyFramework/src/GfxDevice/private/direct3d12/shaderId_private.hpp>
#endif 

