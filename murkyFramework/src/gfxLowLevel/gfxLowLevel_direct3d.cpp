//------------------------------------------------------------------------------
// 2015 J. Coelho.
// OpenGL default state: http://www.glprogramming.com/red/appendixb.html

#include <murkyFramework/include/version.hpp>
#ifdef USE_DIRECT3D

#include <gfxLowLevel/gfxLowLevel.hpp>
#include <windows.h>
#include <d3d11_1.h>
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

    
    // Data
    mat4 projectionMatrix(Unit::UNIT);    
        
    //void serializeState();1

    void onGfxDeviceErrorTriggerBreakpoint()
    {
    
            triggerBreakpoint();
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

        /*glViewport(0, 0, Gapp.screenResX, Gapp.screenResY);
        glClearColor(0.4f, 0.6f, 0.9f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);*/
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
    