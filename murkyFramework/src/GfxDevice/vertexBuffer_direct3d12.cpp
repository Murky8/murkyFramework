//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/version.hpp>
#include <murkyFramework/include/GfxDevice/version_gfxDevice.hpp>
#ifdef USE_DIRECT3D12

#include <windows.h>
#include <d3d11_1.h>
#include <vector>
#include <combaseapi.h>
#include <wrl.h>

#include <d3d12.h>
#include <external/d3d12/d3dx12.h>
#include <dxgi1_4.h>
#include <d2d1_3.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <dwrite.h>
#include <d3d11on12.h>
#include "murkyFramework/include/common.hpp"
#include <murkyFramework/include/GfxDevice/gfxLowLevel.hpp>
#include <murkyFramework/include/GfxDevice/gfxPrimativeTypes.hpp>
#include <murkyFramework/include/GfxDevice/vertexBuffer.hpp>
#include <murkyFramework/include/GfxDevice/shaders.hpp>
#include <murkyFramework/src/GfxDevice/public/gfxDevice.hpp>
#include <murkyFramework/src/GfxDevice/private/d3d12/gfxDevice.h>
#include <murkyFramework/include/collectionNamed.hpp>

struct Vertex
{
	//DirectX::XMFLOAT3 position;
	vec3	pos;
	vec4	color;
};
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
        u32 sizeVertex = 0;
		/*
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
			*/

		int sizeBytes = capacity*sizeof(Vertex);
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
		m_vertexBufferView.StrideInBytes = sizeof(Vertex);
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
	/*	g_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		g_commandList->IASetVertexBuffers(0, 1, &(m_vertexBufferView));
		g_commandList->DrawInstanced(3*nPrimatives, 1, 0, 0);
	}*/

}
#endif // USE_DIRECT3D11

