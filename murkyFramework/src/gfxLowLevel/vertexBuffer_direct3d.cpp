//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/version.hpp>
#ifdef USE_DIRECT3D

#include <windows.h>
#include <d3d11_1.h>
#include <vector>

#include "murkyFramework/include/common.hpp"
#include <murkyFramework/include/gfxLowLevel/gfxLowLevel.hpp>
#include <murkyFramework/include/gfxLowLevel/gfxPrimativeTypes.hpp>
#include <murkyFramework/include/gfxLowLevel/vertexBuffer.hpp>
#include <murkyFramework/include/gfxLowLevel/shaders.hpp>

namespace GfxLowLevel
{
    //------------------------------------------------------------------------------
    // forward declarations
    extern ID3D11Device *g_pd3dDevice;
    extern ID3D11Buffer *g_pVertexBuffer;
    void    onGfxDeviceErrorTriggerBreakpoint();
    
    // constructor	
    VertexBufferDynamic::VertexBufferDynamic(
        VertexType vertexType, PrimativeType primativeType, 
        ShaderId shaderProgram, GfxLowLevel::TextureId &texture,
        u32 nVerts) :
        vertexType(vertexType), primativeType(primativeType), 
        shaderProgram(shaderProgram), texture(texture),
        capacity(nVerts)
    {				        
        D3D11_BUFFER_DESC bd;
        ZeroMemory(&bd, sizeof(bd));
        bd.Usage = D3D11_USAGE_DYNAMIC;
        bd.ByteWidth = sizeof(Vert_pct) * nVerts;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        bd.MiscFlags = 0;
        
        HRESULT hr = g_pd3dDevice->CreateBuffer(&bd, NULL, &g_pVertexBuffer);
        if (FAILED(hr))
            triggerBreakpoint();
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