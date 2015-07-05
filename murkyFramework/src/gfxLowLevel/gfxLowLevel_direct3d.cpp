//------------------------------------------------------------------------------
// 2015 J. Coelho.
// OpenGL default state: http://www.glprogramming.com/red/appendixb.html

#include <murkyFramework/include/version.hpp>
#ifdef USE_DIRECT3D

#include <gfxLowLevel/gfxLowLevel.hpp>
#include <windows.h>
#include <d3d11_1.h>
#include <DirectXColors.h>
#include "common.hpp"
#include <vector>

#include <murkyFramework/include/debugUtils.hpp>
#include <murkyFramework/include/system.hpp>
#include <murkyFramework/include/vectorMatrix.hpp>

#include <murkyFramework/include/appFramework.hpp>

namespace GfxLowLevel
{       
    // Forward declarations    
    extern    HINSTANCE               g_hInst;
    extern    HWND                    g_hWnd;
    extern    D3D_DRIVER_TYPE         g_driverType;
    extern    D3D_FEATURE_LEVEL       g_featureLevel ;
    extern    ID3D11Device*           g_pd3dDevice;
    extern    ID3D11Device1*          g_pd3dDevice1;
    extern    ID3D11DeviceContext*    g_pImmediateContext;
    extern    ID3D11DeviceContext1*   g_pImmediateContext1;
    extern    IDXGISwapChain*         g_pSwapChain;
    extern    IDXGISwapChain1*        g_pSwapChain1;
    extern    ID3D11RenderTargetView* g_pRenderTargetView;

    extern      ID3D11VertexShader*     g_pVertexShader;
    extern      ID3D11PixelShader*      g_pPixelShader;
    extern      ID3D11InputLayout*      g_pVertexLayout;
    extern      ID3D11Buffer*           g_pVertexBuffer;

    
    // Data
    mat4 projectionMatrix(Unit::UNIT);    
        
    //void serializeState();1

    void onGfxDeviceErrorTriggerBreakpoint()
    {
    
            //triggerBreakpoint();
    }    

    void setStateToAppDefault()
    {
        //glEnable(GL_DEPTH_TEST);
        //glDepthFunc(GL_LESS);                
        //glEnable(GL_CULL_FACE);
        //glFrontFace(GL_CW);
        //glCullFace(GL_BACK);
    }

        


    void drawBegin()
    {     
        g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, DirectX::Colors::MidnightBlue);
        
        g_pImmediateContext->VSSetShader(g_pVertexShader, nullptr, 0);
        g_pImmediateContext->PSSetShader(g_pPixelShader, nullptr, 0);
        g_pImmediateContext->Draw(3, 0);
        g_pSwapChain->Present(0, 0);

        
        onGfxDeviceErrorTriggerBreakpoint();	
    }

    /*void setShaderUniform_projMat(mat4 &projMat)
    {        
        glUniformMatrix4fv(GfxLowLevel::uniforms_projMatrix, 1, GL_FALSE, &(projMat.v[0][0]));    
    }
*/
    void drawEnd()
    {
        //glFlush();
    }    
    
}
#endif // USE_DIRECT3D
    