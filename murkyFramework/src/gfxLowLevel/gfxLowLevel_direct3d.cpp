//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/gfxLowLevel/version_gfxDevice.hpp>
#ifdef USE_DIRECT3D

#include <windows.h>
#include <d3d11_1.h>
#include <DirectXColors.h>

#include <vector>

#include <murkyFramework/include/types.hpp>  
#include "murkyFramework/include/common.hpp"
#include <murkyFramework/include/debugUtils.hpp>
#include <murkyFramework/include/appFramework.hpp>
#include <murkyFramework/include/system.hpp>
#include <murkyFramework/include/vectorMatrix.hpp>
#include <murkyFramework/include/gfxLowLevel/gfxLowLevel.hpp>
#include <murkyFramework/include/gfxLowLevel/gfxPrimativeTypes.hpp>
#include <murkyFramework/include/gfxLowLevel/vertexBuffer.hpp>
#include <murkyFramework/include/gfxLowLevel/shaders.hpp>
#include <murkyFramework/include/gfxLowLevel/texture.hpp>

////////////////////////////////////////////////////////////////////////////
// forward declarations
namespace RenderHi  // todo: delete
{        
    extern GfxLowLevel::TextureManager *textureManager;
}

namespace GfxLowLevel
{       
struct HandleDeviceTexture
{
    ID3D11ShaderResourceView *deviceTexture;
};
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

    ID3D11VertexShader*     g_pVertexShader = nullptr;
    ID3D11PixelShader       *g_pPixelShader = nullptr;
    ID3D11InputLayout       *g_pVertexLayout = nullptr;
    ID3D11Buffer            *g_pVertexBuffer = nullptr;
    ID3D11Debug             *d3dDebug = nullptr;
    ID3D11SamplerState      *g_pSamplerLinear = nullptr;
    ID3D11RasterizerState *g_pRasterState = nullptr;
    ID3D11Buffer*            g_pCBChangesEveryFrame = nullptr;

    // Data
    mat4 projectionMatrix(Unit::UNIT);    

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

#define rn (((float)rand() / (float)RAND_MAX))
    void drawBegin()
    {     
        g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, DirectX::Colors::MidnightBlue);   
        g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);


        if (0)
        {
            // vb
            // fill vb
            std::vector<Triangle_pct> tris;
            //srand(1);

            // Set the input layout
            g_pImmediateContext->IASetInputLayout(g_pVertexLayout);
            
            for (int i = 0; i < 10; i++)
            {
                Triangle_pct tri
                {
                    Vert_pct(vec3(rn, rn, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 0.7f)),
                    Vert_pct(vec3(rn, rn, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f)),
                    Vert_pct(vec3(rn, rn, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.36f, 0.37f))
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
                (ID3D11ShaderResourceView * const *)&RenderHi::textureManager->getTextureByName(L"font").pHandle->deviceTexture);
            g_pImmediateContext->PSSetSamplers(0, 1, &g_pSamplerLinear);

            //g_pImmediateContext->Draw(10 * 3, 0);
        }

        
        onGfxDeviceErrorTriggerBreakpoint();	
    }

    /*void setShaderUniform_projMat(mat4 &projMat)
    {        
        glUniformMatrix4fv(GfxLowLevel::uniforms_projMatrix, 1, GL_FALSE, &(projMat.v[0][0]));    
    }
*/
    void drawEnd()
    {

        g_pSwapChain->Present(0, 0);        
    }        
}
#endif // USE_DIRECT3D
    