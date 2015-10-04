#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/GfxDevice/version_gfxDevice.hpp>
#ifdef USE_DIRECT3D12

#include <external/glew/include/GL/glew.h> 
#include <murkyFramework/src/GfxDevice/public/shaderId.hpp>
#include <murkyFramework/src/GfxDevice/private/d3d12/shaderId_private.hpp>
namespace GfxDevice
{

	ShaderId2::ShaderId2(class ShaderId_private* in_pShaderId_private)
		: pShaderId_private(new ShaderId_private)
	{
		*pShaderId_private = *in_pShaderId_private;
	}

	ShaderId2::~ShaderId2()
	{
		delete pShaderId_private;
	}
}
#endif
