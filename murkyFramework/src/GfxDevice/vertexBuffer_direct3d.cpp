//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/src/private/pch.hpp>
#include <murkyFramework/include/version.hpp>
#include <murkyFramework/include/GfxDevice/version_gfxDevice.hpp>
#ifdef USE_DIRECT3D11
#include <d3d11_1.h>

#include <murkyFramework/src/GfxDevice/public/gfxDevice.hpp>
#include <murkyFramework/src/GfxDevice/private/vertexBufferHelpers.hpp>
#include <murkyFramework/include/GfxDevice/gfxLowLevel.hpp>
#include <murkyFramework/include/GfxDevice/vertexBuffer.hpp>
#include <murkyFramework/include/GfxDevice/shaders.hpp>

namespace GfxDevice
{    
    // forward declarations
    extern ID3D11Device         *g_pd3dDevice;
    extern ID3D11Buffer         *g_pVertexBuffer;
    extern ID3D11DeviceContext  *g_pImmediateContext;
    extern ID3D11InputLayout    *g_pVertexLayout_posColTex;
    extern ID3D11InputLayout    *g_pVertexLayout_posCol;    
    extern  ID3D11SamplerState  *g_pSamplerLinear;  
    extern  ID3D11Buffer        *g_pCBChangesEveryFrame;
          
    // constructor	    
	VertexBufferWrapper::VertexBufferWrapper(
		VertexType vertexType, PrimativeType primativeType,
		ShaderWrapper shaderId, TextureWrapper texture,
		u32 nVerts) :
		vertexType(vertexType), primativeType(primativeType),
		shaderId(shaderId), texture(texture),
		capacity(nVerts)
    {			
        u32 sizeVertex = getVertexSizeInBytes(vertexType);
        
        D3D11_BUFFER_DESC bd;
        ZeroMemory(&bd, sizeof(bd));
        bd.Usage = D3D11_USAGE_DYNAMIC;
        bd.ByteWidth = sizeVertex * nVerts;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        bd.MiscFlags = 0;
        
        HRESULT hr = g_pd3dDevice->CreateBuffer(&bd, NULL, &deviceBuffer);
        if (FAILED(hr))
            triggerBreakpoint();
    }
    // methods
    s32 VertexBufferWrapper::getCapacityBytes() const
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

    void	VertexBufferWrapper::draw( void *vertexData, u32 nPrimatives )
    {        
        if (nPrimatives >= capacity)
            triggerBreakpoint();

        u32 sizeVertex = getVertexSizeInBytes(vertexType);        
        switch (vertexType)
        {
        case VertexType::posCol:            
            g_pImmediateContext->IASetInputLayout(g_pVertexLayout_posCol);            
            break;
        case VertexType::posColTex:            
            g_pImmediateContext->IASetInputLayout(g_pVertexLayout_posColTex);
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


        D3D11_MAPPED_SUBRESOURCE subResource;
        g_pImmediateContext->Map(deviceBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &subResource);
        //memcpy(ms.pData, verts, sizeof(verts));
        memcpy(subResource.pData, vertexData, nPrimatives*nVerticiesPerPrimative*sizeVertex);
        g_pImmediateContext->Unmap(deviceBuffer, NULL);

        UINT stride = sizeVertex;
        UINT offset = 0;
        g_pImmediateContext->IASetVertexBuffers(0, 1, &deviceBuffer, &stride, &offset);
        g_pImmediateContext->IASetPrimitiveTopology(primTop);

        // fill vb

        //g_pVertexBuffer
        // vb
        g_pImmediateContext->VSSetShader(shaderId.pVertexShader, nullptr, 0);
        g_pImmediateContext->PSSetShader(shaderId.pPixelShader, nullptr, 0);
        g_pImmediateContext->PSSetShaderResources(
            0,
            1,
            (ID3D11ShaderResourceView * const *)&(texture.deviceTexture));
        g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pCBChangesEveryFrame);
        g_pImmediateContext->PSSetSamplers(0, 1, &g_pSamplerLinear);

        g_pImmediateContext->Draw(nPrimatives * nVerticiesPerPrimative, 0);
    } 
}
#endif // USE_DIRECT3D11

