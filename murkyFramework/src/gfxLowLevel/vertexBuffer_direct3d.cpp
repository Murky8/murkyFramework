//------------------------------------------------------------------------------
// 2014 J. Coelho.
//
// Wrapper is agnostic
// Platform: OpenGL 4.x version
// TODO remove buffer access type? static/dynamic buffers have completely different usage?
#include <murkyFramework/include/version.hpp>
#ifdef USE_DIRECT3D

#include <vector>

#include <murkyFramework/include/gfxLowLevel/gfxPrimativeTypes.hpp>
#include <murkyFramework/include/gfxLowLevel/vertexBuffer.hpp>
#include <murkyFramework/include/gfxLowLevel/shaders.hpp>

//------------------------------------------------------------------------------
// forward declarations

namespace GfxLowLevel
{
    void onGfxDeviceErrorTriggerBreakpoint();

    // constructor	
    VertexBufferDynamic::VertexBufferDynamic(
        VertexType vertexType, PrimativeType primativeType, ShaderId shaderProgram, GfxLowLevel::TextureId &texture ) :
        vertexType(vertexType), primativeType(primativeType), shaderProgram(shaderProgram), texture(texture)
    {				
        triggerBreakpoint();
        onGfxDeviceErrorTriggerBreakpoint();
    }

    // methods
    void	VertexBufferDynamic::draw( void *data, int nPrimatives )
    {
        triggerBreakpoint();

    }
    /*
    VertexBufferRef_Depreciate::VertexBufferRef_Depreciate(u32 in_capacity, VertexType vertexType, BufferAccessType bufferAccessType) :
        capacity(in_capacity), vertexType(vertexType), bufferAccessType(bufferAccessType)
    {
        glGenBuffers(1, (GLuint*)&bufferHandle);
        onGfxDeviceErrorTriggerBreakpoint();

        glBindBuffer(GL_ARRAY_BUFFER, bufferHandle);
        onGfxDeviceErrorTriggerBreakpoint();
        u32 sizeVertex;
        switch (vertexType)
        {
        case VertexType::posCol :
            sizeVertex = sizeof(Vert_pc);
            break;
        case VertexType::posColTex:
            sizeVertex = sizeof(Vert_pct);
            break;
        default:// Catch usage of unimplemented
            sizeVertex = 0;
            triggerBreakpoint();
        }

        GLuint gl_bufferAccessType;
        switch (bufferAccessType)
        {
        case BufferAccessType::dynamic:
            gl_bufferAccessType = GL_DYNAMIC_DRAW;
            break;
        default:// Catch usage of unimplemented
            gl_bufferAccessType = 0;
            triggerBreakpoint();
        }

        glBufferData(GL_ARRAY_BUFFER, capacity * sizeVertex, NULL, gl_bufferAccessType);
        onGfxDeviceErrorTriggerBreakpoint();

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        onGfxDeviceErrorTriggerBreakpoint();
    }

    u32 VertexBufferRef_Depreciate::getBufferHandle() const 
    {
        return bufferHandle;
    }
    */
}
#endif // USE_DIRECT3D