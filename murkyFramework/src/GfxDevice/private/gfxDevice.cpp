#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/src/private/pch.hpp>
#include <murkyFramework/include/GfxDevice/version_gfxDevice.hpp>
#include <murkyFramework/include/collectionNamed.hpp>

#include <murkyFramework/src/GfxDevice/public/gfxDevice.hpp>

namespace GfxDevice
{
	// GfxDevice internal data/state
	murkyFramework::CollectionNamed<ShaderWrapper>			shaderManager;
	murkyFramework::CollectionNamed<TextureWrapper>			textureManager;
	murkyFramework::CollectionNamed<VertexBufferWrapper>	vertexBufferManager;
}

//#ifdef USE_OPENGL
//
//#endif 
//
//#ifdef USE_DIRECT3D11
//
//#endif 
//
//#ifdef USE_DIRECT3D12
//
//#endif 

