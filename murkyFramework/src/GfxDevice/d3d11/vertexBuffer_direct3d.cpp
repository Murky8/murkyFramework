//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/src/pch.hpp>
#ifdef USE_DIRECT3D11
#define deviceObj  g_appDebug->render->gfxDevice

namespace GfxDevice
{    
         
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
        
        HRESULT hr = deviceObj->g_pd3dDevice->CreateBuffer(&bd, NULL, &deviceBuffer);
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
            deviceObj->g_pImmediateContext->IASetInputLayout(deviceObj->g_pVertexLayout_posCol);
            break;
        case VertexType::posColTex:            
            deviceObj->g_pImmediateContext->IASetInputLayout(deviceObj->g_pVertexLayout_posColTex);
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
        deviceObj->g_pImmediateContext->Map(deviceBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &subResource);
        //memcpy(ms.pData, verts, sizeof(verts));
        memcpy(subResource.pData, vertexData, nPrimatives*nVerticiesPerPrimative*sizeVertex);
        deviceObj->g_pImmediateContext->Unmap(deviceBuffer, NULL);

        UINT stride = sizeVertex;
        UINT offset = 0;
        deviceObj->g_pImmediateContext->IASetVertexBuffers(0, 1, &deviceBuffer, &stride, &offset);
        deviceObj->g_pImmediateContext->IASetPrimitiveTopology(primTop);

        // fill vb

        //g_pVertexBuffer
        // vb
        deviceObj->g_pImmediateContext->VSSetShader(shaderId.pVertexShader, nullptr, 0);
        deviceObj->g_pImmediateContext->PSSetShader(shaderId.pPixelShader, nullptr, 0);
        deviceObj->g_pImmediateContext->PSSetShaderResources(
            0,
            1,
            (ID3D11ShaderResourceView * const *)&(texture.deviceTexture));
        deviceObj->g_pImmediateContext->VSSetConstantBuffers(0, 1, &deviceObj->g_pCBChangesEveryFrame);
        deviceObj->g_pImmediateContext->PSSetSamplers(0, 1, &deviceObj->g_pSamplerLinear);

        deviceObj->g_pImmediateContext->Draw(nPrimatives * nVerticiesPerPrimative, 0);
    } 
}
#endif // USE_DIRECT3D11

