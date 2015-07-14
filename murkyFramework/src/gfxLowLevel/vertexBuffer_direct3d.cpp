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
    // forward declarations
    extern ID3D11Device         *g_pd3dDevice;
    extern ID3D11Buffer         *g_pVertexBuffer;
    extern ID3D11DeviceContext  *g_pImmediateContext;
    extern ID3D11InputLayout    *g_pVertexLayout;
    extern  ID3D11VertexShader  *g_pVertexShader;
    extern  ID3D11PixelShader   *g_pPixelShader;
    extern  ID3D11SamplerState  *g_pSamplerLinear;  
    extern  ID3D11Buffer        *g_pCBChangesEveryFrame;
        
    struct HandleDeviceTexture
    {
        ID3D11ShaderResourceView *deviceTexture;
    };

    struct handleDeviceVB
    {
        ID3D11Buffer *deviceBuffer;
    };
    // constructor	
    VertexBufferDynamic::VertexBufferDynamic(
        VertexType vertexType, PrimativeType primativeType, 
        ShaderId shaderProgram, GfxLowLevel::TextureId &texture,
        u32 nVerts) :
        vertexType(vertexType), primativeType(primativeType), 
        shaderProgram(shaderProgram), texture(texture),
        capacity(nVerts)
    {			
        pHandle = new handleDeviceVB();
        D3D11_BUFFER_DESC bd;
        ZeroMemory(&bd, sizeof(bd));
        bd.Usage = D3D11_USAGE_DYNAMIC;
        bd.ByteWidth = sizeof(Vert_pct) * nVerts;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        bd.MiscFlags = 0;
        
        HRESULT hr = g_pd3dDevice->CreateBuffer(&bd, NULL, &pHandle->deviceBuffer);
        if (FAILED(hr))
            triggerBreakpoint();
    }

    VertexBufferDynamic::~VertexBufferDynamic()
    {
        pHandle->deviceBuffer->Release();
        delete pHandle;
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
        g_pImmediateContext->IASetInputLayout(g_pVertexLayout);

        D3D11_MAPPED_SUBRESOURCE subResource;
        g_pImmediateContext->Map(pHandle->deviceBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &subResource);
        //memcpy(ms.pData, verts, sizeof(verts));
        memcpy(subResource.pData, vertexData, nPrimatives*sizeof(Triangle_pct));
        g_pImmediateContext->Unmap(pHandle->deviceBuffer, NULL);

        UINT stride = sizeof(Vert_pct);
        UINT offset = 0;
        g_pImmediateContext->IASetVertexBuffers(0, 1, &pHandle->deviceBuffer, &stride, &offset);
        g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        // fill vb

        //g_pVertexBuffer
        // vb
        g_pImmediateContext->VSSetShader(g_pVertexShader, nullptr, 0);
        g_pImmediateContext->PSSetShader(g_pPixelShader, nullptr, 0);
        g_pImmediateContext->PSSetShaderResources(
            0,
            1,
            (ID3D11ShaderResourceView * const *)&(texture.pHandle->deviceTexture));
        g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pCBChangesEveryFrame);
        g_pImmediateContext->PSSetSamplers(0, 1, &g_pSamplerLinear);

        g_pImmediateContext->Draw(nPrimatives * 3, 0);
    } 
}
#endif // USE_DIRECT3D

