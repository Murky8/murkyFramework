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

    D3D_DRIVER_TYPE         g_driverType = D3D_DRIVER_TYPE_NULL;        // todo: move to gfxLowLevel_d3d...
    D3D_FEATURE_LEVEL       g_featureLevel = D3D_FEATURE_LEVEL_11_0;
    ID3D11Device*           g_pd3dDevice = nullptr;
    ID3D11Device1*          g_pd3dDevice1 = nullptr;
    ID3D11DeviceContext*    g_pImmediateContext = nullptr;
    ID3D11DeviceContext1*   g_pImmediateContext1 = nullptr;
    IDXGISwapChain*         g_pSwapChain = nullptr;
    IDXGISwapChain1*        g_pSwapChain1 = nullptr;
    ID3D11RenderTargetView* g_pRenderTargetView = nullptr;

    ID3D11Texture2D         *g_pDepthStencil = nullptr;
    ID3D11DepthStencilView  *g_pDepthStencilView = nullptr;

    ID3D11InputLayout       *g_pVertexLayout_posColTex = nullptr;
    ID3D11InputLayout       *g_pVertexLayout_posCol = nullptr;

    ID3D11Buffer            *g_pVertexBuffer = nullptr;
    ID3D11Debug             *d3dDebug = nullptr;
    ID3D11SamplerState      *g_pSamplerLinear = nullptr;
    ID3D11RasterizerState   *g_pRasterState = nullptr;
    ID3D11Buffer*            g_pCBChangesEveryFrame = nullptr;    

    void drawBegin();    
    void drawEnd();
};

