//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/version.hpp>
#include <murkyFramework/include/GfxDevice/version_gfxDevice.hpp>
#ifdef USE_DIRECT3D11

#include <windows.h>
#include <d3d11_1.h>
#include <vector>

#include "murkyFramework/include/common.hpp"
#include <murkyFramework/include/GfxDevice/gfxLowLevel.hpp>
#include <murkyFramework/include/GfxDevice/gfxPrimativeTypes.hpp>
#include <murkyFramework/include/GfxDevice/vertexBuffer.hpp>
#include <murkyFramework/include/GfxDevice/shaders.hpp>

namespace GfxDevice
{    
    // forward declarations       
    struct HandleDeviceTexture
    {
        //ID3D11ShaderResourceView *deviceTexture;
    };

    struct handleDeviceVB
    {
        //ID3D11Buffer *deviceBuffer;
    };
    // constructor	
    VertexBufferDynamic::VertexBufferDynamic(
        VertexType vertexType, PrimativeType primativeType, 
        ShaderId shaderProgram, GfxDevice::TextureId &texture,
        u32 nVerts) :
        vertexType(vertexType), primativeType(primativeType), 
        shaderProgram(shaderProgram), texture(std::move(texture)),
        capacity(nVerts)
    {			
        u32 sizeVertex = 0;
        switch (vertexType)
        {
        case VertexType::posCol:
            sizeVertex = sizeof(Vert_pc);
            triggerBreakpoint();
            break;
        case VertexType::posColTex:
            sizeVertex = sizeof(Vert_pct);
            break;
        default:// Catch usage of unimplemented			
            sizeVertex = 0;
            triggerBreakpoint();
        }

        pHandle = new handleDeviceVB();
        
            triggerBreakpoint();
    }

    VertexBufferDynamic::~VertexBufferDynamic()
    {
        //pHandle->deviceBuffer->Release();
        //delete pHandle;
    }

    // methods
    s32 VertexBufferDynamic::getCapacityBytes() const
    {
        s32 s=-1;
        switch (this->vertexType)
        {
        case  VertexType::posColTex :  s =capacity*sizeof(Triangle_pct);
            break;
        default:
            triggerBreakpoint();
        }
    
        return s;
    }

    void	VertexBufferDynamic::draw( void *vertexData, int nPrimatives )
    {        
        if (nPrimatives >= capacity)
            triggerBreakpoint();

        u32 sizeVertex = 0;
        switch (vertexType)
        {
        case VertexType::posCol:
            sizeVertex = sizeof(Vert_pc);
            triggerBreakpoint();
            break;
        case VertexType::posColTex:
            sizeVertex = sizeof(Vert_pct);
            break;
        default:// Catch usage of unimplemented			
            sizeVertex = 0;
            triggerBreakpoint();
        }

        int nVerticiesPerPrimative = 0;        
        D3D_PRIMITIVE_TOPOLOGY primTop;
        switch (primativeType)
        {
        case PrimativeType::triangle:
            nVerticiesPerPrimative = 3;         
            primTop = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
            break;

        case PrimativeType::line:
            nVerticiesPerPrimative = 2;            
            primTop = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
            break;
        default:// Catch usage of unimplemented			            
            triggerBreakpoint();
        }        
    } 
}
#endif // USE_DIRECT3D11

