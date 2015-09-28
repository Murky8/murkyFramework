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
#include <d2d1_3.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <dwrite.h>
#include <d3d11on12.h>

#include <murkyFramework/include/vectorMatrix.hpp>
#include <murkyFramework/include/GfxDevice/gfxLowLevel.hpp>
#include <murkyFramework/include/GfxDevice/gfxPrimativeTypes.hpp>
#include <murkyFramework/include/GfxDevice/vertexBuffer.hpp>
#include <murkyFramework/include/GfxDevice/shaders.hpp>
#include <murkyFramework/include/GfxDevice/texture.hpp>

// forward declarations
namespace Render  // todo: delete
{        
    extern GfxDevice::TextureManager *textureManager;
}

using namespace DirectX;
using namespace Microsoft::WRL;

namespace GfxDevice
{       
    struct HandleDeviceTexture
    {
        //ID3D11ShaderResourceView *deviceTexture;
    };

    HDC     hDC;   // a copy from main.cpp
    HGLRC   hRC;
    HWND    hWnd;
	
	// forward declarations
	extern void WaitForPreviousFrame();
	//// Pipeline objects.
	extern D3D12_VIEWPORT m_viewport;
	extern D3D12_RECT m_scissorRect;
	extern ComPtr<IDXGISwapChain3> m_swapChain;
	extern ComPtr<ID3D12Device> m_device;
	const UINT FrameCount = 2;
	extern ComPtr<ID3D12Resource> m_renderTargets[FrameCount]; //note: !!!
	extern ComPtr<ID3D12CommandAllocator> m_commandAllocator;
	extern ComPtr<ID3D12CommandQueue> m_commandQueue;
	extern ComPtr<ID3D12RootSignature> m_rootSignature;
	extern ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
	extern ComPtr<ID3D12PipelineState> m_pipelineState;
	extern ComPtr<ID3D12GraphicsCommandList> m_commandList;
	extern UINT m_rtvDescriptorSize;
	// Pipeline objects.

	// App resources.
	extern ComPtr<ID3D12Resource> m_vertexBuffer;
	extern D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;

	// Synchronization objects.
	extern UINT m_frameIndex;
	extern HANDLE m_fenceEvent;
	extern ComPtr<ID3D12Fence> m_fence;
	extern UINT64 m_fenceValue;

    // data
    mat4 projectionMatrix(unit);    

    void onGfxDeviceErrorTriggerBreakpoint()
    {            
    }    	

	inline void ThrowIfFailed(HRESULT hr)
	{
		if (FAILED(hr))
		{
			throw;
		}
	}

	void drawBegin()
    {     
		// Command list allocators can only be reset when the associated 
		//		// command lists have finished execution on the GPU; apps should use 
		//		// fences to determine GPU execution progress.
				ThrowIfFailed(m_commandAllocator->Reset());
		//
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
				m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));
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
				m_commandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
				m_commandList->DrawInstanced(3, 1, 0, 0);
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
#endif // USE_DIRECT3D11
    