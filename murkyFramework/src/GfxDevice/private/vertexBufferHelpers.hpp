#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include "murkyFramework/include/common.hpp"
#include <murkyFramework/include/GfxDevice/gfxPrimativeTypes.hpp>

namespace GfxDevice
{
	u32		getVertexSizeInBytes(VertexType vertexType);	
	void	getPrimativeInfo(PrimativeType primativeType, u32 * const nVerticiesPerPrimative);
}