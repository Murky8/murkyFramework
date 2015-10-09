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
#include <murkyFramework/include/collectionNamed.hpp>

namespace GfxDevice
{        
	// external forward declarations;
	extern Microsoft::WRL::ComPtr<ID3D12Device>		m_device;

	inline void ThrowIfFailed(HRESULT hr)
	{
		if (FAILED(hr))
		{
			throw;
		}
	}

	struct Vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};

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

		Vertex triangleVertices[] =
		{
			{ { 0.0f, 0.25f, 0.0f },{ 1.0f, 0.0f, 0.0f, 1.0f } },
			{ { 0.25f, -0.25f, 0.0f },{ 0.0f, 1.0f, 0.0f, 1.0f } },
			{ { -0.25f, -0.25f, 0.0f },{ 0.0f, 0.0f, 1.0f, 1.0f } }
		};

		const UINT vertexBufferSize = sizeof(triangleVertices)*2;

		// Note: using upload heaps to transfer static data like vert buffers is not 
		// recommended. Every time the GPU needs it, the upload heap will be marshalled 
		// over. Please read up on Default Heap usage. An upload heap is used here for 
		// code simplicity and because there are very few verts to actually transfer.
		// http://www.gamedev.net/topic/666986-direct3d-12-documentation-is-now-public/page-2

		hr = m_device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_vertexBuffer));
		if (FAILED(hr))
			triggerBreakpoint();

		// Copy the triangle data to the vertex buffer.
		UINT8* pVertexDataBegin;
		ThrowIfFailed(m_vertexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&pVertexDataBegin)));
		memcpy(pVertexDataBegin, triangleVertices, sizeof(triangleVertices));
		m_vertexBuffer->Unmap(0, nullptr);

		// Initialize the vertex buffer view.
		m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
		m_vertexBufferView.StrideInBytes = sizeof(Vertex);
		m_vertexBufferView.SizeInBytes = vertexBufferSize;
		//Sleep(1000);                                
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

