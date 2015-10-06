#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/GfxDevice/version_gfxDevice.hpp>
#include <murkyFramework/include/collectionNamed.hpp>
#include <murkyFramework/src/GfxDevice/public/shaderWrappers.hpp>
#include <murkyFramework/src/GfxDevice/public/textureWrappers.hpp>
#include <murkyFramework/src/GfxDevice/public/vertexBufferWrappers.hpp>

namespace GfxDevice
{
	extern murkyFramework::CollectionNamed<ShaderWrapper>		shaderManager;
	extern murkyFramework::CollectionNamed<TextureWrapper>		textureManager;
	extern murkyFramework::CollectionNamed<VertexBufferWrapper>	vertexBufferManager;
}

//
//#ifdef USE_OPENGL
//#include <murkyFramework/src/GfxDevice/private/openGL/shaderId_private.hpp>
//#endif 
//
//#ifdef USE_DIRECT3D11
//#include <murkyFramework/src/GfxDevice/private/d3d11/shaderId_private.hpp>
//#endif 
//
//#ifdef USE_DIRECT3D12
//#include <murkyFramework/src/GfxDevice/private/d3d12/shaderId_private.hpp>
//#endif 

//extern 
