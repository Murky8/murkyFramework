//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11. d3d12
#include <murkyFramework/src/pch.hpp>
namespace murkyFramework {
    namespace GfxDevice {

        void GfxDeviceObj::setUniform_projectionMatrix(const float *pMat)
        {
            *((mat4*)&this->projectionMat) = *(mat4*)pMat;
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

        GfxDeviceObj::GfxDeviceObj(GfxDeviceObj_initStruct *const initStruct)
        {
            g_appDebug->render->gfxDevice = this; // warning: see g_aapDebug usage notes: for development only, remove!

            // Create Direct3D device and swap chain 
            HRESULT hr;

            systemSpecific::WindowsSpecific * ws = initStruct->windowsSpecific;

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
            ThrowIfFailed(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&factory)));
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
            RECT    screenDims{ 0 };
            GetWindowRect(ws->gethWnd(), &screenDims);

            //auto screenWidth = screenDims.right - screenDims.left;
            //auto screenHeight = screenDims.top - screenDims.bottom;

            m_scissorRect.left = m_scissorRect.top = 0.f;
            m_scissorRect.right = static_cast<LONG>(screenDims.right);
            m_scissorRect.bottom = static_cast<LONG>(screenDims.bottom);

            m_viewport.TopLeftX = m_viewport.TopLeftY = 0.f;
            m_viewport.Width = static_cast<float>(screenDims.right);
            m_viewport.Height = static_cast<float>(screenDims.bottom);
            m_viewport.MinDepth = 0.0f;
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
            swapChainDesc.BufferDesc.Width = screenDims.right;
            swapChainDesc.BufferDesc.Height = screenDims.bottom;
            swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
            swapChainDesc.OutputWindow = /*m_hwnd*/ ws->gethWnd();
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
            ThrowIfFailed(factory->MakeWindowAssociation(ws->gethWnd(), DXGI_MWA_NO_ALT_ENTER));

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

            // create constatnt buffer stuff
            const UINT constantBufferGSSize = sizeof(ConstantBufferGS) * FrameCount;

            ThrowIfFailed(m_device->CreateCommittedResource(
                &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
                D3D12_HEAP_FLAG_NONE,
                &CD3DX12_RESOURCE_DESC::Buffer(constantBufferGSSize),
                D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr,
                IID_PPV_ARGS(&m_constantBufferGS)
                ));

            ThrowIfFailed(m_constantBufferGS->Map(0, nullptr, reinterpret_cast<void**>(&m_pConstantBufferGSData)));
            ZeroMemory(m_pConstantBufferGSData, constantBufferGSSize);
            // create constatnt buffer stuff

            // Create a root signature.
            // HELP0
            CD3DX12_DESCRIPTOR_RANGE ranges[1];
            ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

            CD3DX12_ROOT_PARAMETER rootParameters[RootParametersCount];
            rootParameters[RootParameterTexture].InitAsDescriptorTable(1, &ranges[0], D3D12_SHADER_VISIBILITY_PIXEL);
            rootParameters[RootParameterConstantBuf].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);

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

            GfxDevice::Shaders::initialise();// error!!! using uninitialised object? (shadermanager)?

            // Create the pipeline state, which includes compiling and loading shaders.m_pipelineState_pct
            {

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
                GfxDevice::ShaderWrapper shader = shaderManager.get(L"posColTex");
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
                ThrowIfFailed(m_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState_pct)));
            }

            // Create the pipeline state, which includes compiling and loading shaders.m_pipelineState_pc
            {

                // Define the vertex input layout.
                D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
                {
                    { "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
                    { "COLOR",		0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
                };

                // Describe and create the graphics pipeline state object (PSO).
                D3D12_RASTERIZER_DESC rasterDesc{};
                rasterDesc.CullMode = D3D12_CULL_MODE_NONE;
                rasterDesc.FillMode = D3D12_FILL_MODE_SOLID;

                D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
                psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
                psoDesc.pRootSignature = m_rootSignature.Get();
                GfxDevice::ShaderWrapper shader = shaderManager.get(L"posCol");
                psoDesc.VS = { reinterpret_cast<UINT8*>(shader.vertexShader->GetBufferPointer()), shader.vertexShader->GetBufferSize() };
                psoDesc.PS = { reinterpret_cast<UINT8*>(shader.pixelShader->GetBufferPointer()), shader.pixelShader->GetBufferSize() };
                //psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
                psoDesc.RasterizerState = rasterDesc;
                psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
                psoDesc.DepthStencilState.DepthEnable = FALSE;
                psoDesc.DepthStencilState.StencilEnable = FALSE;
                psoDesc.SampleMask = UINT_MAX;
                psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
                psoDesc.NumRenderTargets = 1;
                psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
                psoDesc.SampleDesc.Count = 1;
                ThrowIfFailed(m_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState_pc)));
            }

            // Create the command list.
            ThrowIfFailed(m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator.Get(), m_pipelineState_pct.Get(), IID_PPV_ARGS(&g_commandList)));

            CD3DX12_CPU_DESCRIPTOR_HANDLE srvHandle(m_srvHeap->GetCPUDescriptorHandleForHeapStart());
            m_srvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

            ComPtr<ID3D12Resource> textureUploadHeap;   // kkep in scope until command list executed!
            ComPtr<ID3D12Resource> textureUploadHeap2;

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
                //WaitForPreviousFrame();
                // WAITING FOR THE FRAME TO COMPLETE BEFORE CONTINUING IS NOT BEST PRACTICE.
                // This is code implemented as such for simplicity. More advanced samples 
                // illustrate how to use fences for efficient resource usage.

                // Signal and increment the fence value.
                const UINT64 fence = m_fenceValue;
                ThrowIfFailed(m_commandQueue->Signal(m_fence.Get(), fence));
                m_fenceValue++;

                // Wait until the previous frame is finished.
                const UINT64 v = m_fence->GetCompletedValue();
                if (v < fence)
                {
                    ThrowIfFailed(m_fence->SetEventOnCompletion(fence, m_fenceEvent));
                    WaitForSingleObject(m_fenceEvent, INFINITE);
                }

                m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();
            }
            loadTexturesInDir(L"data");
            debugLog << L"finished success" << L"\n";
        }

        void GfxDeviceObj::initialise()
        {
        }

        GfxDeviceObj::~GfxDeviceObj()
        {

        }

        void GfxDeviceObj::drawBegin()
        {
            // Command list allocators can only be reset when the associated 
            // command lists have finished execution on the GPU; apps should use 
            // fences to determine GPU execution progress.
            HRESULT hr;

            ThrowIfFailed(m_commandAllocator->Reset());
            // However, when ExecuteCommandList() is called on a particular command 
            // list, that command list can then be reset at any time and must be before 
            // re-recording.
            ThrowIfFailed(g_commandList->Reset(m_commandAllocator.Get(), m_pipelineState_pct.Get()));

            // Set necessary state.
            g_commandList->SetGraphicsRootSignature(m_rootSignature.Get());

            {
                ConstantBufferGS constantBufferGS = {};

                *((mat4*)&constantBufferGS.worldViewProjection) = this->projectionMat;

                UINT8* destination = m_pConstantBufferGSData + sizeof(ConstantBufferGS) * m_frameIndex;
                memcpy(destination, &constantBufferGS, sizeof(ConstantBufferGS));
                g_commandList->SetGraphicsRootConstantBufferView(RootParameterConstantBuf, m_constantBufferGS->GetGPUVirtualAddress() + m_frameIndex * sizeof(ConstantBufferGS));
            }

            ID3D12DescriptorHeap* ppHeaps[] = { m_srvHeap.Get() };
            g_commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

            // set current texture// now done in vbdraw
            //CD3DX12_GPU_DESCRIPTOR_HANDLE srvGPUHandle(m_srvHeap->GetGPUDescriptorHandleForHeapStart());
            //srvGPUHandle.Offset(1, m_srvDescriptorSize);     //note offset!!!
            //g_commandList->SetGraphicsRootDescriptorTable(RootParameterTexture, srvGPUHandle);

            g_commandList->RSSetViewports(1, &m_viewport);
            g_commandList->RSSetScissorRects(1, &m_scissorRect);

            // Indicate that the back buffer will be used as a render target.
            g_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(),
                D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

            CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), m_frameIndex, m_rtvDescriptorSize);
            g_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

            //static auto nonk = 0.f;
            //nonk += 0.01f;
            //if (nonk > 1.f)nonk = 0.f;    
            //const float clearColor[] = { nonk, 0.2f, 0.4f, 1.0f };

            const float clearColor[] = { 0.f, 0.f, 0.f, 1.0f };

            g_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
        }

        void GfxDeviceObj::drawEnd()
        {
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

        void GfxDeviceObj::waitForGPUFinish()
        {
            //reset the fence signal
            m_fence.Get()->Signal(0);
            //set the event to be fired once the signal value is 1
            m_fence->SetEventOnCompletion(1, m_fenceEvent);

            //after the command list has executed, tell the GPU to signal the fence
            m_commandQueue->Signal(m_fence.Get(), 1);

            //wait for the event to be fired by the fence
            WaitForSingleObject(m_fenceEvent, INFINITE);
        }

        void GfxDeviceObj::WaitForPreviousFrame()
        {
            // WAITING FOR THE FRAME TO COMPLETE BEFORE CONTINUING IS NOT BEST PRACTICE.
            // This is code implemented as such for simplicity. More advanced samples 
            // illustrate how to use fences for efficient resource usage.

            // Signal and increment the fence value.
            const UINT64 fence = m_fenceValue;
            ThrowIfFailed(m_commandQueue->Signal(m_fence.Get(), fence));
            m_fenceValue++;

            // Wait until the previous frame is finished.
            const UINT64 v = m_fence->GetCompletedValue();
            if (v < fence)
            {
                ThrowIfFailed(m_fence->SetEventOnCompletion(fence, m_fenceEvent));
                WaitForSingleObject(m_fenceEvent, INFINITE);
            }

            m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();
        }

        void GfxDeviceObj::loadTexturesInDir(std::wstring directoryName)
        {
            FileDirectoryWalker fileWalker(directoryName, L"\\.png$");

            while (fileWalker.findNext())
            {
                debugLog << L"RenderObj::loadTexturesInDir loaded " << fileWalker.findData.cFileName << "\n";
                FilePathSplit pathBits(std::wstring(fileWalker.findData.cFileName));

                GfxDevice::TextureWrapper newt = GfxDevice::createTextureObjectFromFile(
                    directoryName, pathBits.fileName, pathBits.extensionName);

                textureManager.add(pathBits.fileName, newt);
            }
        }

        void GfxDeviceObj::loadShadersInDir(std::wstring directoryName)
        {
#ifdef _DEBUG
            // Enable better shader debugging with the graphics debugging tools.
            UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
            UINT compileFlags = 0;
#endif

            FileDirectoryWalker fileWalker(directoryName, L"\\.hlsl$");

            while (fileWalker.findNext())
            {

                debugLog << L"gfxDevice::loadShadersInDir loaded " << fileWalker.findData.cFileName << "\n";
                FilePathSplit pathBits(std::wstring(fileWalker.findData.cFileName));

                {
                    //std::wstring vsPath = directoryName + L"/" + pathBits.fileName + L".vsh";
                    HRESULT hr;
                    GfxDevice::ShaderWrapper newShader;

                    ID3DBlob* pErrorBlob = nullptr;
                    //std::wstring fileName = { L"src/GfxDevice/d3d12/shaders/posColTex.hlsl" };
                    std::wstring fileName = directoryName + L"/" + pathBits.fileName + L".hlsl";
                    hr = D3DCompileFromFile(fileName.c_str(), nullptr, nullptr, "VSMain", "vs_5_0", compileFlags, 0, &newShader.vertexShader, &pErrorBlob);
                    if (FAILED(hr))
                    {
                        if (pErrorBlob)
                        {
                            OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
                            pErrorBlob->Release();
                        }
                        triggerBreakpoint();
                    }

                    pErrorBlob = nullptr;
                    hr = D3DCompileFromFile(fileName.c_str(), nullptr, nullptr, "PSMain", "ps_5_0", compileFlags, 0, &newShader.pixelShader, &pErrorBlob);
                    if (FAILED(hr))
                    {
                        if (pErrorBlob)
                        {
                            OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
                            pErrorBlob->Release();
                        }
                        triggerBreakpoint();
                    }
                    if (pErrorBlob) pErrorBlob->Release();
                    shaderManager.add(pathBits.fileName, newShader);
                }
            }
        }
        /*
        void GfxDeviceObj::loadShadersInDir(std::wstring directoryName)
        {
            FileDirectoryWalker fileWalker(directoryName, L"\\.png$");

            while (fileWalker.findNext())
            {
                debugLog << L"RenderObj::loadTexturesInDir loaded " << fileWalker.findData.cFileName << "\n";
                FilePathSplit pathBits(std::wstring(fileWalker.findData.cFileName));

                GfxDevice::TextureWrapper newt = GfxDevice::createTextureObjectFromFile(
                    directoryName, pathBits.fileName, pathBits.extensionName);

                textureManager.add(pathBits.fileName, newt);
            }
        }
        */

        /*
        void WaitForCommandQueueFence()
        {
            //reset the fence signal
            mFence->Signal(0);
            //set the event to be fired once the signal value is 1
            mFence->SetEventOnCompletion(1, mHandle);

            //after the command list has executed, tell the GPU to signal the fence
            mCommandQueue->Signal(mFence.Get(), 1);

            //wait for the event to be fired by the fence
            WaitForSingleObject(mHandle, INFINITE);
        }


        // this is the function that cleans up Direct3D and COM
        void CleanD3D(void)
        {
            //close the event handle so that mFence can actually release()
            CloseHandle(mHandle);
        }

        */

    }//namespace GfxDevice
}//namespace murkyFramework