#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
struct GfxDeviceObj_initStruct
{
    u32 screenWidth;
    u32 screenHeight;
    WindowsSpecific* windowsSpecific;
};

class GfxDeviceObj
{
public:
    GfxDeviceObj() = delete;

    GfxDeviceObj(GfxDeviceObj_initStruct *const initStruct);
    ~GfxDeviceObj();

    void drawBegin();
    void drawEnd();
    void WaitForPreviousFrame();

    D3D12_VIEWPORT				m_viewport;
    D3D12_RECT					m_scissorRect;
    ComPtr<IDXGISwapChain3>		m_swapChain;
    ComPtr<ID3D12Device>		m_device;
    enum { FrameCount = 2 };
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

    ComPtr<ID3D12Resource> m_constantBufferGS;  // projmat
    UINT8* m_pConstantBufferGSData;  // projmat
};
