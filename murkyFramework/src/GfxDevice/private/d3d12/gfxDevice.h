#pragma once

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

namespace GfxDevice
{
using namespace DirectX;
using namespace Microsoft::WRL;

	// forward declarations    
	extern  HDC			hDC;
	extern  HGLRC		hRC;
	extern  HWND		hWnd;

// forward declarations
extern void WaitForPreviousFrame();
//// Pipeline objects.
extern D3D12_VIEWPORT m_viewport;
extern D3D12_RECT m_scissorRect;
extern ComPtr<IDXGISwapChain3> m_swapChain;
extern ComPtr<ID3D12Device> m_device;
extern const UINT FrameCount;
extern ComPtr<ID3D12Resource> m_renderTargets[]; //note: !!!
extern ComPtr<ID3D12CommandAllocator> m_commandAllocator;
extern ComPtr<ID3D12CommandQueue> m_commandQueue;
extern ComPtr<ID3D12RootSignature> m_rootSignature;
extern ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
extern ComPtr<ID3D12PipelineState> m_pipelineState;
extern ComPtr<ID3D12GraphicsCommandList> m_commandList;
extern UINT m_rtvDescriptorSize;
// Pipeline objects.

// App resources.
//extern ComPtr<ID3D12Resource> m_vertexBuffer;
//extern D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;

// Synchronization objects.
extern UINT m_frameIndex;
extern HANDLE m_fenceEvent;
extern ComPtr<ID3D12Fence> m_fence;
extern UINT64 m_fenceValue;


inline void ThrowIfFailed(HRESULT hr)
{
	if (FAILED(hr))
	{
		throw;
	}
}
}
