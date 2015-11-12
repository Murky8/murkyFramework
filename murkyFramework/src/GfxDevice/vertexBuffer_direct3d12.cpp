//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/src/private/pch.hpp>
#include <murkyFramework/include/version.hpp>
#include <murkyFramework/include/GfxDevice/version_gfxDevice.hpp>
#ifdef USE_DIRECT3D12

#include <d3d11_1.h>
#include <d3d12.h>
#include <external/d3d12/d3dx12.h>
#include <murkyFramework/src/GfxDevice/public/gfxDevice.hpp>
#include <murkyFramework/src/GfxDevice/private/d3d12/gfxDevice.h>
#include <murkyFramework/src/GfxDevice/private/vertexBufferHelpers.hpp>

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
		HRESULT hr;
        u32 sizeVertex = getVertexSizeInBytes(vertexType);
				
		int sizeBytes = capacity*sizeof(Vert_pct);
		hr = m_device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(sizeBytes),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_vertexBuffer));
		if (FAILED(hr))
			triggerBreakpoint();

		// Initialize the vertex buffer view.
		m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
		m_vertexBufferView.StrideInBytes = sizeof(Vert_pct);
		m_vertexBufferView.SizeInBytes = sizeBytes;
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

	void	VertexBufferWrapper::draw(void *vertexData, u32 nPrimatives)
	{
		if (nPrimatives >= capacity)
			triggerBreakpoint();

		u32 sizeVertex = getVertexSizeInBytes(vertexType);		
		u32 nVerticiesPerPrimative;
		getPrimativeInfo(primativeType, &nVerticiesPerPrimative);

		D3D_PRIMITIVE_TOPOLOGY primTop;
		switch (primativeType)
		{
		case PrimativeType::triangle:		
			primTop = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;	break;
		case PrimativeType::line:			
			primTop = D3D_PRIMITIVE_TOPOLOGY_LINELIST;		break;
		default:// Catch usage of unimplemented			            
			triggerBreakpoint();
		}
		HRESULT hr;
		//const UINT vertexBufferSize = nPrimatives*sizeof(Vert_pct);		
		static UINT8* pVertexDataBegin;
		hr = m_vertexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&pVertexDataBegin));
		ThrowIfFailed(hr);
		memcpy(pVertexDataBegin, vertexData, nPrimatives*nVerticiesPerPrimative*sizeVertex);
		m_vertexBuffer->Unmap(0, nullptr);

        // set current texture
        CD3DX12_GPU_DESCRIPTOR_HANDLE srvGPUHandle(m_srvHeap->GetGPUDescriptorHandleForHeapStart());
        srvGPUHandle.Offset(this->texture.iTexture, m_srvDescriptorSize);
        g_commandList->SetGraphicsRootDescriptorTable(0, srvGPUHandle);

		g_commandList->IASetPrimitiveTopology(primTop);
		g_commandList->IASetVertexBuffers(0, 1, &(m_vertexBufferView));
		g_commandList->DrawInstanced(3*nPrimatives, 1, 0, 0);	
	}
}
#endif // USE_DIRECT3D11

