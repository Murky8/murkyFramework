//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/src/private/pch.hpp>
#include <murkyFramework/include/version.hpp>
#include <murkyFramework/include/GfxDevice/version_gfxDevice.hpp>

namespace GfxDevice
{
    // forward declarations

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

    u32 getVertexSize(VertexType vertexType)
    { // todo: better way to do this?
        switch (vertexType)
        {
        case VertexType::posCol:
            return sizeof(Vert_pc);
            break;
        case VertexType::posColTex:
            return sizeof(Vert_pct);
            break;
        default:// Catch usage of unimplemented			            
            triggerBreakpoint();
        }
        return -1;
    }
}