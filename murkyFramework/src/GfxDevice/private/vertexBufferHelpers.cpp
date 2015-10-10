//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11

#include "murkyFramework/include/common.hpp"
#include <murkyFramework/include/GfxDevice/gfxPrimativeTypes.hpp>

namespace GfxDevice
{
	u32 getVertexSizeInBytes(VertexType vertexType)
	{		
		switch (vertexType)
		{
		case VertexType::posCol:
			return sizeof(Vert_pc);			
			break;
		case VertexType::posColTex:
			return sizeof(Vert_pct);			
			break;
		default:// catch usage of unimplemented						
			triggerBreakpoint();
		}
		return 0;
	}

	void getPrimativeInfo(PrimativeType primativeType, u32 * const nVerticiesPerPrimative)
	{
		switch (primativeType)
		{
		case PrimativeType::triangle:
			*nVerticiesPerPrimative = 3;			
			break;
		case PrimativeType::line:
			*nVerticiesPerPrimative = 2;			
			break;
		default:// Catch usage of unimplemented			            
			triggerBreakpoint();
		}
	}
}