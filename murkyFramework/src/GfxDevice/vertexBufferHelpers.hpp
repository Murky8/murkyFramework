#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11

namespace GfxDevice
{
	u32		getVertexSizeInBytes(VertexType vertexType);	
	void	getPrimativeInfo(PrimativeType primativeType, u32 * const nVerticiesPerPrimative);
}