//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/src/pch.hpp>
namespace murkyFramework {
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
            return -1;
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

        u32 getNVertsPerPrimative(PrimativeType primativeType)
        { // todo: better way to do this?
            switch (primativeType)
            {
            case PrimativeType::triangle:
                return 3;
                break;

            case PrimativeType::line:
                return 2;
                break;
            default:// Catch usage of unimplemented			            
                triggerBreakpoint();
            }

            return -1;
        }

    }//namespace GfxDevice

}//namespace murkyFramework