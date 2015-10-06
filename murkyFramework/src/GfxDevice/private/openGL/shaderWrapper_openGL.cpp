#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/GfxDevice/version_gfxDevice.hpp>
#ifdef USE_OPENGL

#include <external/glew/include/GL/glew.h> 
#include <murkyFramework/src/GfxDevice/public/shaderId.hpp>
#include <murkyFramework/src/GfxDevice/private/openGL/shaderId_private.hpp>

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

	ShaderId2& ShaderId2::operator=(ShaderId2&& rhs); // move assigment
	{
		delete pShaderId_private;
		this->pShaderId_private = rhs.pShaderId_private;
		rhs.pShaderId_private = nullptr;
	}

	ShaderId2::ShaderId2(ShaderId2&& rhs);// move constructor
	{
		this->pShaderId_private = rhs.pShaderId_private;
		rhs.pShaderId_private = nullptr;
	}

}
#endif
