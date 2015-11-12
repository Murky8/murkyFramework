//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/src/private/pch.hpp>
#include <murkyFramework/include/version.hpp>
#include <murkyFramework/include/GfxDevice/version_gfxDevice.hpp>
#ifdef USE_DIRECT3D12

#include <d3d12.h>
#include <external/d3d12/d3dx12.h>
#include <dxgi1_4.h>
#include <DirectXMath.h>

#include <murkyFramework/src/GfxDevice/private/d3d12/gfxDevice.h>
#include <murkyFramework/src/GfxDevice/public/gfxDevice.hpp>
#include <murkyFramework/include/GfxDevice/gfxLowLevel.hpp>

using namespace DirectX;
using namespace Microsoft::WRL;

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
	ComPtr<ID3D12CommandQueue>		m_commandQueue;
	ComPtr<ID3D12RootSignature>		m_rootSignature;
	ComPtr<ID3D12DescriptorHeap>	m_rtvHeap;
	ComPtr<ID3D12DescriptorHeap>	m_srvHeap;

	ComPtr<ID3D12PipelineState>		m_pipelineState;
	ComPtr<ID3D12GraphicsCommandList>	g_commandList;
    UINT m_rtvDescriptorSize = 0;
    UINT m_srvDescriptorSize = 0;
	// Pipeline objects.

	// App resources.	
	ComPtr<ID3D12Resource> m_texture;
	ComPtr<ID3D12Resource> m_texture2;
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

	void drawBegin()
	{
		// Command list allocators can only be reset when the associated 
				// command lists have finished execution on the GPU; apps should use 
				// fences to determine GPU execution progress.
		ThrowIfFailed(m_commandAllocator->Reset());

		HRESULT hr;

		// However, when ExecuteCommandList() is called on a particular command 
		// list, that command list can then be reset at any time and must be before 
		// re-recording.
		ThrowIfFailed(g_commandList->Reset(m_commandAllocator.Get(), m_pipelineState.Get()));

		// Set necessary state.
		g_commandList->SetGraphicsRootSignature(m_rootSignature.Get());


		ID3D12DescriptorHeap* ppHeaps[] = { m_srvHeap.Get() };
		g_commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
                
        // set current texture
        CD3DX12_GPU_DESCRIPTOR_HANDLE srvGPUHandle(m_srvHeap->GetGPUDescriptorHandleForHeapStart());
        srvGPUHandle.Offset(1, m_srvDescriptorSize);
        g_commandList->SetGraphicsRootDescriptorTable(0, srvGPUHandle);

		g_commandList->RSSetViewports(1, &m_viewport);
		g_commandList->RSSetScissorRects(1, &m_scissorRect);

		// Indicate that the back buffer will be used as a render target.
		g_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(),
			D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), m_frameIndex, m_rtvDescriptorSize);
		g_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

		static auto nonk = 0.f;
		nonk += 0.01f;
		if (nonk > 1.f)nonk = 0.f;


		const float clearColor[] = { nonk, 0.2f, 0.4f, 1.0f };
		g_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

		if(0)
		{// update vb
			 //https://msdn.microsoft.com/en-us/library/windows/desktop/ff476457(v=vs.85).aspx
			Vert_pct triangleVertices[] =
			{
				{ { 0.0f, 0.0f, 0.0f },{ 1.0f, 0.0f, 0.0f},{0.f, 0.f} },
				{ { 0.0f, 0.5f, 0.0f },{ 0.0f, 1.0f, 0.0f},{0.f, 1.f} },
				{ { 0.5f, 0.0f, 0.0f },{ 0.0f, 0.0f, 1.0f},{1.f, 0.f} },

				{ { 0.5f, 0.5f, 0.0f },{ 1.0f, 0.0f, 0.0f},{ 1.f, 1.f } },
				{ { 0.5f, 0.0f, 0.0f },{ 0.0f, 1.0f, 0.0f},{ 1.f, 0.f } },
				{ { 0.0f, 0.5f, 0.0f },{ 0.0f, 0.0f, 1.0f},{ 0.f, 1.f } }
			};

			const UINT vertexBufferSize = sizeof(triangleVertices);
			VertexBufferWrapper vb = vertexBufferManager.get(L"tris");
			UINT8* pVertexDataBegin;
			hr = vb.m_vertexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&pVertexDataBegin));
			ThrowIfFailed(hr);
			memcpy(pVertexDataBegin, triangleVertices, sizeof(triangleVertices));
			vb.m_vertexBuffer->Unmap(0, nullptr);
		}// update vb
	}

    void drawEnd()
    {   				
		if (0)
		{
			g_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			VertexBufferWrapper vb = vertexBufferManager.get(L"tris");
			g_commandList->IASetVertexBuffers(0, 1, &(vb.m_vertexBufferView));
			g_commandList->DrawInstanced(6, 1, 0, 0);
		}

		//		// Indicate that the back buffer will now be used to present.
		g_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
		
		// finished writing to command list
		ThrowIfFailed(g_commandList->Close());
		ID3D12CommandList* ppCommandLists[] = { g_commandList.Get() };
		m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
		// Present the frame.
		ThrowIfFailed(m_swapChain->Present(1, 0));
		WaitForPreviousFrame();
    }        
}
#endif 
    