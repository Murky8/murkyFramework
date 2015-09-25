//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/version.hpp>
#include <murkyFramework/include/GfxDevice/version_gfxDevice.hpp>
#ifdef USE_DIRECT3D12

#include <windows.h>
#include <external/d3d12/d3dx12.h>
#include <d3d12.h>
#include <dxgi1_4.h>
//#include "d3dx12.h"


#include <murkyFramework/include/common.hpp>
#include <murkyFramework/include/debugUtils.hpp>


#include <d2d1_3.h>
#include <dwrite.h>
#include <d3d11on12.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>

#include <string>
#include <wrl.h>
#include <combaseapi.h>
#include <murkyFramework/include/appFramework.hpp>

//using namespace DirectX;
using namespace Microsoft::WRL;

namespace GfxDevice
{        
    // forward declarations    
    extern  HDC			hDC;
    extern  HGLRC		hRC;
    extern  HWND		hWnd;
    extern  HINSTANCE	g_hInst;

	// Pipeline objects.
	D3D12_VIEWPORT m_viewport;
	D3D12_RECT m_scissorRect;
	ComPtr<IDXGISwapChain3> m_swapChain;
	ComPtr<ID3D12Device> m_device;
	static const UINT FrameCount = 2;
	ComPtr<ID3D12Resource> m_renderTargets[FrameCount];
	ComPtr<ID3D12CommandAllocator> m_commandAllocator;
	ComPtr<ID3D12CommandQueue> m_commandQueue;
	ComPtr<ID3D12RootSignature> m_rootSignature;
	ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
	ComPtr<ID3D12PipelineState> m_pipelineState;
	ComPtr<ID3D12GraphicsCommandList> m_commandList;
	UINT m_rtvDescriptorSize;
	// Pipeline objects.

	// App resources.
	ComPtr<ID3D12Resource> m_vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;

	// Synchronization objects.
	UINT m_frameIndex;
	HANDLE m_fenceEvent;
	ComPtr<ID3D12Fence> m_fence;
	UINT64 m_fenceValue;

	inline void ThrowIfFailed(HRESULT hr)
	{
		if (FAILED(hr))
		{
			throw;
		}
	}

	void GetHardwareAdapter(_In_ IDXGIFactory4* pFactory, _Outptr_result_maybenull_ IDXGIAdapter1** ppAdapter)
	{
		IDXGIAdapter1* pAdapter = nullptr;
		*ppAdapter = nullptr;

		for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != pFactory->EnumAdapters1(adapterIndex, &pAdapter); ++adapterIndex)
		{
			DXGI_ADAPTER_DESC1 desc;
			pAdapter->GetDesc1(&desc);

			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			{
				// Don't select the Basic Render Driver adapter.
				// If you want a software adapter, pass in "/warp" on the command line.
				continue;
			}

			// Check to see if the adapter supports Direct3D 12, but don't create the
			// actual device yet.
			if (SUCCEEDED(D3D12CreateDevice(pAdapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
			{
				break;
			}
		}

		*ppAdapter = pAdapter;
	}

	_Check_return_
    bool initialise_device24(HDC &hDC, HGLRC &hRC, HWND &hWnd)
    {
        //-------------------------------------------------------------------------------------- 
        // Create Direct3D device and swap chain 
        
            GfxDevice::hDC = hDC;//hDC = GetDC(hWnd); // Get the device context for our window
            GfxDevice::hRC = hRC;
            GfxDevice::hWnd = hWnd;

#ifdef _DEBUG
			// Enable the D3D12 debug layer.
			{
				ComPtr<ID3D12Debug> debugController;
				if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
				{
					debugController->EnableDebugLayer();
				}
			}
#endif
			ComPtr<IDXGIFactory4> factory;			
			ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&factory)));

			if (/*m_useWarpDevice*/ true)
			{
				ComPtr<IDXGIAdapter> warpAdapter;
				ThrowIfFailed(factory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter)));

				ThrowIfFailed(D3D12CreateDevice(
					warpAdapter.Get(),
					D3D_FEATURE_LEVEL_11_0,
					IID_PPV_ARGS(&m_device)
					));
			}
			else
			{
				ComPtr<IDXGIAdapter1> hardwareAdapter;
				GetHardwareAdapter(factory.Get(), &hardwareAdapter);

				ThrowIfFailed(D3D12CreateDevice(
					hardwareAdapter.Get(),
					D3D_FEATURE_LEVEL_11_0,
					IID_PPV_ARGS(&m_device)
					));
			}

			// Describe and create the command queue.
			D3D12_COMMAND_QUEUE_DESC queueDesc = {};
			queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
			queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

			ThrowIfFailed(m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue)));

			// Describe and create the swap chain.
			DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
			swapChainDesc.BufferCount = FrameCount;
			swapChainDesc.BufferDesc.Width = /*m_width*/ Gapp->screenResX;
			swapChainDesc.BufferDesc.Height = /*m_height*/ Gapp->screenResY;
			swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			swapChainDesc.OutputWindow = /*m_hwnd*/ hWnd;
			swapChainDesc.SampleDesc.Count = 1;
			swapChainDesc.Windowed = TRUE;

			ComPtr<IDXGISwapChain> swapChain;
			ThrowIfFailed(factory->CreateSwapChain(
				m_commandQueue.Get(),		// Swap chain needs the queue so that it can force a flush on it.
				&swapChainDesc,
				&swapChain
				));

			ThrowIfFailed(swapChain.As(&m_swapChain));

			// This sample does not support fullscreen transitions.
			ThrowIfFailed(factory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER));

			m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();

			// Create descriptor heaps.
			{
				// Describe and create a render target view (RTV) descriptor heap.
				D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
				rtvHeapDesc.NumDescriptors = FrameCount;
				rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
				rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
				ThrowIfFailed(m_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap)));

				m_rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
			}

			// Create frame resources.
			{
				CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());

				// Create a RTV for each frame.
				for (UINT n = 0; n < FrameCount; n++)
				{
					ThrowIfFailed(m_swapChain->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n])));
					m_device->CreateRenderTargetView(m_renderTargets[n].Get(), nullptr, rtvHandle);
					rtvHandle.Offset(1, m_rtvDescriptorSize);
				}
			}

			ThrowIfFailed(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator)));
			
			debugLog << L"finished success" << L"\n";
			return true;
    }


    bool deinitialise_device24()
    {       
        return true;
    }
} // namespace GfxDevice
#endif // USE_DIRECT3D12
