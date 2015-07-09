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
#include <murkyFramework/include/gfxLowLevel/gfxPrimativeTypes.hpp>
#include <murkyFramework/include/debugUtils.hpp>
#include <murkyFramework/include/system.hpp>
#include <murkyFramework/include/vectorMatrix.hpp>

#include <murkyFramework/include/appFramework.hpp>

    namespace RenderHi
    {        
        extern GfxLowLevel::TextureManager *textureManager;
    }

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

    extern      ID3D11SamplerState*     g_pSamplerLinear;
    
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

    //void D3DGraphics::RenderVerts(Vertex* verts, UINT count, D3D11_PRIMITIVE_TOPOLOGY type)
    //{
    //    D3D11_MAPPED_SUBRESOURCE ms;
    //    // map the buffer   
    //    m_pDeviceContext->Map(m_pVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
    //    // copy the data to it
    //    memcpy(ms.pData, verts, sizeof(verts));
    //    // unmap it
    //    m_pDeviceContext->Unmap(m_pVertexBuffer, NULL);

    //    // select which vertex buffer to display
    //    UINT stride = sizeof(Vertex);
    //    UINT offset = 0;
    //    m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

    //    // select which primtive type we are using
    //    m_pDeviceContext->IASetPrimitiveTopology(type);

    //    // draw the vertex buffer to the back buffer
    //    m_pDeviceContext->Draw(count, 0);
    //}


    /*
    context->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
    memcpy(resource.pData, &totalVertices[0], sizeof(totalVertices));
    context->Unmap(vertexBuffer, 0);*/

#define rn (((float)rand() / (float)RAND_MAX))
    void drawBegin()
    {     
        g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, DirectX::Colors::MidnightBlue);   

        // vb
        // fill vb
        std::vector<Triangle_pct> tris;
        //srand(1);

        for (int i = 0; i < 10; i++)
        {
            Triangle_pct tri
            {
                Vert_pct(vec3(rn, rn, 0.5f), vec3(0.8f, 0.0f, 0.0f), vec2(0.0f, 0.7f)),
                Vert_pct(vec3(rn, rn, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f)),
                Vert_pct(vec3(rn, rn, 0.5f), vec3(0.33f, 0.34f, 0.35f), vec2(0.36f, 0.37f))
            };
            tris.push_back(tri);
        }
  
        D3D11_MAPPED_SUBRESOURCE subResource;
        g_pImmediateContext->Map(g_pVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &subResource);
        //memcpy(ms.pData, verts, sizeof(verts));
        memcpy(subResource.pData, tris.data(), tris.size()*sizeof(Triangle_pct));
        g_pImmediateContext->Unmap(g_pVertexBuffer, NULL);

        UINT stride = sizeof(Vert_pct);
        UINT offset = 0;
        g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);    
        g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        // fill vb

        //g_pVertexBuffer
        // vb
        g_pImmediateContext->VSSetShader(g_pVertexShader, nullptr, 0);
        g_pImmediateContext->PSSetShader(g_pPixelShader, nullptr, 0);
        g_pImmediateContext->PSSetShaderResources(
            0,
            1,
            ( ID3D11ShaderResourceView * const *)&RenderHi::textureManager->getTextureByName(L"font").handle );
        g_pImmediateContext->PSSetSamplers(0, 1, &g_pSamplerLinear);

        g_pImmediateContext->Draw(10*3, 0);
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
    