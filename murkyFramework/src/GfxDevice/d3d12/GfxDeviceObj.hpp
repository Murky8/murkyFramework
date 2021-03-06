#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11. openGL4
namespace murkyFramework {
    namespace GfxDevice {
    
enum RootParameters : u32
{
    RootParameterTexture = 0,
    RootParameterConstantBuf,
    RootParametersCount
};

    using namespace DirectX;
    using namespace Microsoft::WRL;
    
    //__declspec(align(256))
    struct MatrixContainer
    {
        mat4 matrix;
        u8 pad[256 - sizeof(mat4)];
    };

    struct ConstantBufferGS
    {
        static const int nMatricies = 2;
        MatrixContainer matricies[nMatricies];
        
        // Constant buffers are 256-byte aligned in GPU memory. Padding is added
        // for convenience when computing the struct's size.
    };

    inline void ThrowIfFailed(HRESULT hr)
    {
        if (FAILED(hr))
        {
            throw;
        }
    }
//}
    struct GfxDeviceObj_initStruct
    {
        u32 screenWidth;
        u32 screenHeight;
        systemSpecific::WindowsSpecific* windowsSpecific;
    };

    class GfxDeviceObj 
    {
    public:

        std::vector<mat4> projectionMatricies{ mat4(unit),mat4(unit) , mat4(unit) };

        ConstantBufferGS constantBufferGS = {};
        void setUniform_projectionMatrix(const float *pMat, int slot=0);
        int currentSlot = 0;
        void setCurrentSlot(int slot);

        murkyFramework::CollectionNamed<GfxDevice::ShaderWrapper>		shaderManager;
        murkyFramework::CollectionNamed<GfxDevice::TextureWrapper>		textureManager;
        murkyFramework::CollectionNamed<GfxDevice::VertexBufferWrapper>	vertexBufferManager;

        void loadTexturesInDir(std::wstring directoryName);
        void loadShadersInDir(std::wstring directoryName);


        GfxDeviceObj() = delete;

        GfxDeviceObj(GfxDeviceObj_initStruct *const initStruct);
        void initialise();
        ~GfxDeviceObj();

        void drawBegin();
        void drawEnd();
        void waitForGPUFinish();
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
        ComPtr<ID3D12DescriptorHeap>    m_dsvHeap;
        ComPtr<ID3D12Resource>          m_depthStencil;
        ComPtr<ID3D12PipelineState>		m_pipelineState_pct;
        ComPtr<ID3D12PipelineState>		m_pipelineState_pc;
        ComPtr<ID3D12GraphicsCommandList>	g_commandList;
        UINT m_rtvDescriptorSize = 0;
        UINT m_srvDescriptorSize = 0;
        // Pipeline objects.

        // App resources.	
        ComPtr<ID3D12Resource> m_texture[10];
        
        // Synchronization objects.
        UINT m_frameIndex = 0;
        HANDLE m_fenceEvent;
        ComPtr<ID3D12Fence> m_fence;
        UINT64 m_fenceValue;

        ComPtr<ID3D12Resource> m_constantBufferGS;  // projmat
        //UINT8* m_pConstantBufferGSData;  // projmat
    };
    }//namespace GfxDevice
}//namespace murkyFramework