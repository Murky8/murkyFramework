//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/GfxDevice/version_gfxDevice.hpp>
#ifdef USE_DIRECT3D11

#include <windows.h>
#include <d3d11_1.h>
#include <DirectXColors.h>

#include <vector>

#include <murkyFramework/include/vectorMatrix.hpp>
#include <murkyFramework/include/GfxDevice/gfxLowLevel.hpp>
#include <murkyFramework/include/GfxDevice/gfxPrimativeTypes.hpp>
#include <murkyFramework/include/GfxDevice/texture.hpp>

// forward declarations
namespace Render  // todo: delete
{            
}

namespace GfxDevice
{       
    
    HDC     hDC;   // a copy from main.cpp
    HGLRC   hRC;
    HWND    hWnd;

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
    
    ID3D11InputLayout       *g_pVertexLayout = nullptr;
    ID3D11Buffer            *g_pVertexBuffer = nullptr;
    ID3D11Debug             *d3dDebug = nullptr;
    ID3D11SamplerState      *g_pSamplerLinear = nullptr;
    ID3D11RasterizerState   *g_pRasterState = nullptr;
    ID3D11Buffer*            g_pCBChangesEveryFrame = nullptr;

    // data
    mat4 projectionMatrix(unit);    
	        
#define rn (((float)rand() / (float)RAND_MAX))
    void drawBegin()
    {     
        g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, DirectX::Colors::MidnightBlue);   
        g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

                   
    }
    
    void drawEnd()
    {

        g_pSwapChain->Present(0, 0);        
    }        
}
#endif // USE_DIRECT3D11
    