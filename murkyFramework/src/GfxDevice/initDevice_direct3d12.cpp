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
#include <d2d1_3.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <dwrite.h>
#include <d3d11on12.h>

#include <murkyFramework/src/GfxDevice/private/d3d12/gfxDevice.h>
#include <murkyFramework/include/Render/linesShapes.hpp>
#include <murkyFramework/src/GfxDevice/private/textureHelpers.hpp>

struct Vertex
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT4 color;
};

namespace GfxDevice
{
	extern TextureWrapper createTestTextureObject();
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
		
	//_Check_return_
	bool initialise_device(HDC &in_hDC, HGLRC &in_hRC, HWND &in_hWnd)
	{
		//-------------------------------------------------------------------------------------- 
		// Create Direct3D device and swap chain 
		HRESULT hr;
		GfxDevice::hDC = in_hDC;//hDC = GetDC(hWnd); // Get the device context for our window
		GfxDevice::hRC = in_hRC;
		GfxDevice::hWnd = in_hWnd;

//#ifdef _DEBUG
		// Enable the D3D12 debug layer.
		{
			ComPtr<ID3D12Debug> debugController;
			if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
			{
				debugController->EnableDebugLayer();
			}
		}
//#endif
		ComPtr<IDXGIFactory4> factory;
		//ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&factory)));
		ThrowIfFailed(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG,IID_PPV_ARGS(&factory)));
		// CreateDXGIFactory2 and DXGI_CREATE_FACTORY_DEBUG.
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
		m_device->SetName(L"moof");
		// murky
		m_scissorRect.right = static_cast<LONG>(Gapp->screenResX);
		m_scissorRect.bottom = static_cast<LONG>(Gapp->screenResY);
		m_viewport.Width = static_cast<float>(Gapp->screenResX);
		m_viewport.Height = static_cast<float>(Gapp->screenResY);
		m_viewport.MaxDepth = 1.0f;

		// murky

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

		//// Create descriptor heaps.

		// Describe and create a shader resource view (SRV) heap for the textures.
		D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
		srvHeapDesc.NumDescriptors = 2;
		srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;	
		ThrowIfFailed(m_device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&m_srvHeap)));
        m_srvHeap->SetName(L"texture heep");

		// Describe and create a render target view (RTV) descriptor heap.
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};			
		rtvHeapDesc.NumDescriptors = FrameCount;
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		ThrowIfFailed(m_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap)));
		m_rtvHeap->SetName(L"rtv heep");

		m_rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		// Create frame resources.
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());

		// Create a RTV for each frame.
		for (UINT n = 0; n < FrameCount; n++)
		{
			ThrowIfFailed(m_swapChain->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n])));
			m_device->CreateRenderTargetView(m_renderTargets[n].Get(), nullptr, rtvHandle);
			rtvHandle.Offset(1, m_rtvDescriptorSize);
		}		

		ThrowIfFailed(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator)));

		// Create a root signature.
        // HELP0
			CD3DX12_DESCRIPTOR_RANGE ranges[1];
			ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

			CD3DX12_ROOT_PARAMETER rootParameters[1];
			rootParameters[0].InitAsDescriptorTable(1, &ranges[0], D3D12_SHADER_VISIBILITY_PIXEL);

			D3D12_STATIC_SAMPLER_DESC sampler = {};
			sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
			sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
			sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
			sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
			sampler.MipLODBias = 0;
			sampler.MaxAnisotropy = 0;
			sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
			sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
			sampler.MinLOD = 0.0f;
			sampler.MaxLOD = D3D12_FLOAT32_MAX;
			sampler.ShaderRegister = 0;
			sampler.RegisterSpace = 0;
			sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
			CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
			rootSignatureDesc.Init(_countof(rootParameters), rootParameters, 1, &sampler, 
                D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);			

			ComPtr<ID3DBlob> signature;
			ComPtr<ID3DBlob> error;
			ThrowIfFailed(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error));
			ThrowIfFailed(m_device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), 
                IID_PPV_ARGS(&m_rootSignature)));		

		// Create the pipeline state, which includes compiling and loading shaders.
		{
			Shaders::initialise();

			// Define the vertex input layout.
			D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
			{
				{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
				{ "COLOR",		0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,	0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
			};

			// Describe and create the graphics pipeline state object (PSO).
			D3D12_RASTERIZER_DESC rasterDesc{};
			rasterDesc.CullMode = D3D12_CULL_MODE_NONE;
			rasterDesc.FillMode = D3D12_FILL_MODE_SOLID;

			D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
			psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
			psoDesc.pRootSignature = m_rootSignature.Get();
			ShaderWrapper shader = shaderManager.get(L"posColTex");
			psoDesc.VS = { reinterpret_cast<UINT8*>(shader.vertexShader->GetBufferPointer()), shader.vertexShader->GetBufferSize() };
			psoDesc.PS = { reinterpret_cast<UINT8*>(shader.pixelShader->GetBufferPointer()), shader.pixelShader->GetBufferSize() };
			//psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
			psoDesc.RasterizerState = rasterDesc;
			psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
			psoDesc.DepthStencilState.DepthEnable = FALSE;
			psoDesc.DepthStencilState.StencilEnable = FALSE;
			psoDesc.SampleMask = UINT_MAX;
			psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
			psoDesc.NumRenderTargets = 1;
			psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
			psoDesc.SampleDesc.Count = 1;
			ThrowIfFailed(m_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState)));
		}

		// Create the command list.
		ThrowIfFailed(m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator.Get(), m_pipelineState.Get(), IID_PPV_ARGS(&g_commandList)));

        ComPtr<ID3D12Resource> textureUploadHeap;
        ComPtr<ID3D12Resource> textureUploadHeap2;
		CD3DX12_CPU_DESCRIPTOR_HANDLE srvHandle(m_srvHeap->GetCPUDescriptorHandleForHeapStart());	
		m_srvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		//textureUploadHeap.SetName
		{
			u32 TextureWidth = 512;
			u32 TextureHeight = 512;
			u32 TexturePixelSize = 4;

			{ // upload 1st texture

			// Describe  Texture2D.
			D3D12_RESOURCE_DESC textureDesc = {};
			textureDesc.MipLevels = 1;
			textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			textureDesc.Width = TextureWidth;
			textureDesc.Height = TextureHeight;			
			textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
			textureDesc.DepthOrArraySize = 1;
			textureDesc.SampleDesc.Count = 1;
			textureDesc.SampleDesc.Quality = 0;
			textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

				ThrowIfFailed(m_device->CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
					D3D12_HEAP_FLAG_NONE,
					&textureDesc,
					D3D12_RESOURCE_STATE_COPY_DEST,
					nullptr,
					IID_PPV_ARGS(&m_texture)));

				const UINT64 uploadBufferSize = GetRequiredIntermediateSize(m_texture.Get(), 0, 1);

				// Create the GPU upload buffer.
				ThrowIfFailed(m_device->CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
					D3D12_HEAP_FLAG_NONE,
					&CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize),
					D3D12_RESOURCE_STATE_GENERIC_READ,
					nullptr,
					IID_PPV_ARGS(&textureUploadHeap)));

				// Copy data to the intermediate upload heap and then schedule a copy 
				// from the upload heap to the Texture2D.
				u32 width;
				u32 height;
				std::vector<u8> myTexture;
				//loadTextureDataFromFile(myTexture, L"data", L"font 4c", L"png",
//					width, height);
                {
                    loadTextureDataFromFile(myTexture, L"data", L"t0 4c", L"png",
                        width, height);
                    GfxDevice::TextureWrapper newt;
                    newt.iTexture = 0;
                    GfxDevice::textureManager.add(L"t0 4c", newt);
                }
				D3D12_SUBRESOURCE_DATA textureData = {};
				//textureData.pData = &texture[0];
				textureData.pData = myTexture.data();
				textureData.RowPitch = TextureWidth * TexturePixelSize;
				textureData.SlicePitch = textureData.RowPitch * TextureHeight;

				UpdateSubresources(g_commandList.Get(), m_texture.Get(), textureUploadHeap.Get(), 0, 0, 1,
                    &textureData);
				g_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_texture.Get(),
                    D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));

				// Describe and create a SRV for the texture.
				D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
				srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
				srvDesc.Format = textureDesc.Format;
				srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
				srvDesc.Texture2D.MipLevels = 1;				

				m_device->CreateShaderResourceView(m_texture.Get(), &srvDesc, srvHandle);	
				srvHandle.Offset(m_srvDescriptorSize);
			} // upload 1st texture		

            { // upload 2nd texture

              // Describe  Texture2D.
                D3D12_RESOURCE_DESC textureDesc = {};
                textureDesc.MipLevels = 1;
                textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
                textureDesc.Width = TextureWidth;
                textureDesc.Height = TextureHeight;
                textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
                textureDesc.DepthOrArraySize = 1;
                textureDesc.SampleDesc.Count = 1;
                textureDesc.SampleDesc.Quality = 0;
                textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

                ThrowIfFailed(m_device->CreateCommittedResource(
                    &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
                    D3D12_HEAP_FLAG_NONE,
                    &textureDesc,
                    D3D12_RESOURCE_STATE_COPY_DEST,
                    nullptr,
                    IID_PPV_ARGS(&m_texture2)));

                const UINT64 uploadBufferSize = GetRequiredIntermediateSize(m_texture2.Get(), 0, 1);

                // Create the GPU upload buffer.
                ThrowIfFailed(m_device->CreateCommittedResource(
                    &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
                    D3D12_HEAP_FLAG_NONE,
                    &CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize),
                    D3D12_RESOURCE_STATE_GENERIC_READ,
                    nullptr,
                    IID_PPV_ARGS(&textureUploadHeap2)));

                // Copy data to the intermediate upload heap and then schedule a copy 
                // from the upload heap to the Texture2D.
                u32 width;
                u32 height;
                std::vector<u8> myTexture;

                {                
                loadTextureDataFromFile(myTexture, L"data", L"font 4c", L"png",
                					width, height);
                                
                    GfxDevice::TextureWrapper newt;
                    newt.iTexture = 1;
                    GfxDevice::textureManager.add(L"font 4c", newt);
                }
                D3D12_SUBRESOURCE_DATA textureData = {};
                //textureData.pData = &texture[0];
                textureData.pData = myTexture.data();
                textureData.RowPitch = TextureWidth * TexturePixelSize;
                textureData.SlicePitch = textureData.RowPitch * TextureHeight;

                UpdateSubresources(g_commandList.Get(), m_texture2.Get(), textureUploadHeap2.Get(), 0, 0, 1,
                    &textureData);
                g_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_texture2.Get(),
                    D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));

                // Describe and create a SRV for the texture.
                D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
                srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
                srvDesc.Format = textureDesc.Format;
                srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
                srvDesc.Texture2D.MipLevels = 1;            

                m_device->CreateShaderResourceView(m_texture2.Get(), &srvDesc, srvHandle);
                srvHandle.Offset(m_srvDescriptorSize);
            } // upload 2nd texture		
		}

        // Command lists are created in the recording state.		
		ThrowIfFailed(g_commandList->Close());

		ID3D12CommandList* ppCommandLists[] = { g_commandList.Get() };
		m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
	
		// Create synchronization objects and wait until assets have been uploaded to the GPU.
		{
			ThrowIfFailed(m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));
			m_fenceValue = 1;

			// Create an event handle to use for frame synchronization.
			m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
			if (m_fenceEvent == nullptr)
			{
				ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
			}

			// Wait for the command list to execute; we are reusing the same command 
			// list in our main loop but for now, we just want to wait for setup to 
			// complete before continuing.
			WaitForPreviousFrame();
		}	
		debugLog << L"finished success" << L"\n";
		return true;	
	}	

	bool deinitialise_device()
	{
		return true;
	}
} // namespace GfxDevice
#endif 
