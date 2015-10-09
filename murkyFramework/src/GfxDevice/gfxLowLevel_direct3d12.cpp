//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/version.hpp>
#include <murkyFramework/include/GfxDevice/version_gfxDevice.hpp>
#ifdef USE_DIRECT3D12

#include <windows.h>
#include <combaseapi.h>
#include <wrl.h>

#include <d3d12.h>
#include <external/d3d12/d3dx12.h>
#include <dxgi1_4.h>
#include <DirectXMath.h>

#include <murkyFramework/include/vectorMatrix.hpp>
#include <murkyFramework/include/GfxDevice/gfxLowLevel.hpp>
#include <murkyFramework/src/GfxDevice/public/gfxDevice.hpp>
#include <murkyFramework/src/GfxDevice/private/d3d12/gfxDevice.h>

using namespace DirectX;
using namespace Microsoft::WRL;

/*
Notes:

descriptor heap: 
Where each descriptor fully describes an object to the GPU.
Descriptors are the primary unit of binding for a single resource in D3D12.

Create a command allocator.Creating and Recording Command Lists and Bundles.
Bundles are designed to be used and re-used any number of times. Command lists, on the other hand,
are typically executed only a single time.

A graphics root signature defines what resources are bound to the graphics pipeline.

resource heap

PSO
A pipeline state object maintains the state of all currently set shaders as well as certain fixed 
function state objects (such as the input assembler, tesselator, rasterizer and output merger).

pipeline state is attached to a command list via a pipeline state object (PSO).

You must reset the command list allocator and the command list itself before you can reuse them.
*/

namespace GfxDevice
{       

    HDC     hDC;   // a copy from main.cpp
    HGLRC   hRC;
    HWND    hWnd;
	
	// Pipeline objects.
	D3D12_VIEWPORT				m_viewport;
	D3D12_RECT					m_scissorRect;
	ComPtr<IDXGISwapChain3>		m_swapChain;
	ComPtr<ID3D12Device>		m_device;
	const UINT					FrameCount = 2;
	ComPtr<ID3D12Resource>		m_renderTargets[FrameCount];
	ComPtr<ID3D12CommandAllocator>	m_commandAllocator;
	ComPtr<ID3D12CommandQueue>	m_commandQueue;
	ComPtr<ID3D12RootSignature>	m_rootSignature;
	ComPtr<ID3D12DescriptorHeap>	m_rtvHeap;
	ComPtr<ID3D12PipelineState>	m_pipelineState;
	ComPtr<ID3D12GraphicsCommandList>	m_commandList;
	UINT m_rtvDescriptorSize = 0;
	// Pipeline objects.

	// App resources.
	//ComPtr<ID3D12Resource> m_vertexBuffer;
	//D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;

	// Synchronization objects.
	UINT m_frameIndex = 0;
	HANDLE m_fenceEvent;
	ComPtr<ID3D12Fence> m_fence;
	UINT64 m_fenceValue;
	
    // data
    mat4 projectionMatrix(unit);    

	void WaitForPreviousFrame()
	{
		// WAITING FOR THE FRAME TO COMPLETE BEFORE CONTINUING IS NOT BEST PRACTICE.
		// This is code implemented as such for simplicity. More advanced samples 
		// illustrate how to use fences for efficient resource usage.

		// Signal and increment the fence value.
		const UINT64 fence = m_fenceValue;
		ThrowIfFailed(m_commandQueue->Signal(m_fence.Get(), fence));
		m_fenceValue++;

		// Wait until the previous frame is finished.
		if (m_fence->GetCompletedValue() < fence)
		{
			ThrowIfFailed(m_fence->SetEventOnCompletion(fence, m_fenceEvent));
			WaitForSingleObject(m_fenceEvent, INFINITE);
		}

		m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();
	}

	struct Vertex
	{
		//DirectX::XMFLOAT3 position;
		vec3	pos;
		vec4	color;
	};

	void drawBegin()
    {     
		// Command list allocators can only be reset when the associated 
		//		// command lists have finished execution on the GPU; apps should use 
		//		// fences to determine GPU execution progress.
				ThrowIfFailed(m_commandAllocator->Reset());
		//
		HRESULT hr;
				{// update vb
					//https://msdn.microsoft.com/en-us/library/windows/desktop/ff476457(v=vs.85).aspx
					Vertex triangleVertices[] =
					{
						{ { 0.0f, 0.0f, 0.0f },{ 1.0f, 0.0f, 0.0f, 1.0f } },
						{ { 0.0f, 0.5f, 0.0f },{ 0.0f, 1.0f, 0.0f, 1.0f } },
						{ { 0.5f, 0.0f, 0.0f },{ 0.0f, 0.0f, 1.0f, 1.0f } },

						{ { 0.5f, 0.5f, 0.0f },{ 1.0f, 0.0f, 0.0f, 1.0f } },
						{ { 0.5f, 0.0f, 0.0f },{ 0.0f, 1.0f, 0.0f, 1.0f } },
						{ { 0.0f, 0.5f, 0.0f },{ 0.0f, 0.0f, 1.0f, 1.0f } }
					};

					const UINT vertexBufferSize = sizeof(triangleVertices);
					VertexBufferWrapper vb = vertexBufferManager.get(std::wstring(L"tris"));					
					UINT8* pVertexDataBegin;
					hr = vb.m_vertexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&pVertexDataBegin));
					ThrowIfFailed(hr);
					memcpy(pVertexDataBegin, triangleVertices, sizeof(triangleVertices));
					vb.m_vertexBuffer->Unmap(0, nullptr);
				}// update vb

		//		// However, when ExecuteCommandList() is called on a particular command 
		//		// list, that command list can then be reset at any time and must be before 
		//		// re-recording.
				ThrowIfFailed(m_commandList->Reset(m_commandAllocator.Get(), m_pipelineState.Get()));
		//
		//		// Set necessary state.
				m_commandList->SetGraphicsRootSignature(m_rootSignature.Get());
				m_commandList->RSSetViewports(1, &m_viewport);
				m_commandList->RSSetScissorRects(1, &m_scissorRect);
		//
		//		// Indicate that the back buffer will be used as a render target.
				m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(),
					D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));
		//
				CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), m_frameIndex, m_rtvDescriptorSize);
				m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);
		//
		//		// Record commands.
				static auto nonk = 0.f;
				nonk += 0.01f;
				if (nonk > 1.f)nonk = 0.f;

				const float clearColor[] = { nonk, 0.2f, 0.4f, 1.0f };
				m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
				m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
				VertexBufferWrapper vb = vertexBufferManager.get(std::wstring(L"tris"));
				m_commandList->IASetVertexBuffers(0, 1, &(vb.m_vertexBufferView) );
				m_commandList->DrawInstanced(3, 2, 0, 0);
		//
		//		// Indicate that the back buffer will now be used to present.
				m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
		//
				ThrowIfFailed(m_commandList->Close());
		
		//	void D3D12HelloTriangle::PopulateCommandList()
		//void D3D12HelloTriangle::OnRender()
		

			ID3D12CommandList* ppCommandLists[] = { m_commandList.Get() };
			m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

		//	// Present the frame.
			ThrowIfFailed(m_swapChain->Present(1, 0));

			WaitForPreviousFrame();

    }

    void drawEnd()
    {     
    }        
}
#endif 
    