//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/version.hpp>

#include <iostream>
#include <vector>
#include <thread>

#include <windows.h>
#include <stdlib.h>

#include <murkyFramework/include/debugUtils.hpp>
#include <murkyFramework/include/appFramework.hpp>
#include <murkyFramework/include/types.hpp>
#include <murkyFramework/include/debugUtils.hpp>
#include <murkyFramework/include/loadSaveFile.hpp>
#include <murkyFramework/include/system.hpp>
#include <murkyFramework/include/gfxLowLevel/gfxPrimativeTypes.hpp>
#include <murkyFramework/include/gfxLowLevel/gfxLowLevel.hpp>
#include <murkyFramework/include/gfxHighLevel/render.hpp>

#include <d3d11_1.h>// temp

//--------------------------------------------------------------------------------------
// forward declarations
namespace GfxLowLevel
{    
    extern  HINSTANCE               g_hInst;
    extern  HWND                    g_hWnd;
    extern  D3D_DRIVER_TYPE         g_driverType;
    extern  D3D_FEATURE_LEVEL       g_featureLevel;
    extern  ID3D11Device*           g_pd3dDevice;
    extern  ID3D11Device1*          g_pd3dDevice1;
    extern  ID3D11DeviceContext*    g_pImmediateContext;
    extern  ID3D11DeviceContext1*   g_pImmediateContext1;
    extern  IDXGISwapChain*         g_pSwapChain;
    extern  IDXGISwapChain1*        g_pSwapChain1;
    extern  ID3D11RenderTargetView* g_pRenderTargetView;

    extern  ID3D11Texture2D          *g_pDepthStencil;
    extern  ID3D11DepthStencilView   *g_pDepthStencilView;

    extern  ID3D11VertexShader*     g_pVertexShader;
    extern  ID3D11PixelShader*      g_pPixelShader;
    extern  ID3D11InputLayout*      g_pVertexLayout;
    extern  ID3D11Buffer*           g_pVertexBuffer;

    extern  ID3D11SamplerState       *g_pSamplerLinear;
    extern  ID3D11Debug             *d3dDebug;

    extern  ID3D11RasterizerState *g_pRasterState;
    extern  ID3D11Buffer*           g_pCBChangesEveryFrame;
extern  ID3D11Debug             *d3dDebug; // temp
    bool initialise_device(HDC &hDC, HGLRC &hRC, HWND &hWnd);  
}
using namespace GfxLowLevel;

void mainLoop();
void masterInitialise();
void deinitialise1();
bool createWindow(LPCWSTR title, int width, int height);

//--------------------------------------------------------------------------------------
// Variables
// todo: depreciate

namespace
{
    HDC			hDC;		// Private GDI Device Context
    HGLRC		hRC;		// Permanent Rendering Context
    HWND		hWnd;	    // Holds Our Window Handle
    HINSTANCE	hInstance;	// Holds The Instance Of The Application
    u64         frameStartTime = 0;
    bool        wndProcCalled = false;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE	hPrevInstance, LPSTR lpCmdLine,int nCmdShow)			// Window Show State
{
    MSG		msg;
    masterInitialise();

    while (!Gapp.exitWholeApp)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {	// Is There A Message Waiting?        
            if (msg.message == WM_QUIT)
            {				// Have We Received A Quit Message?
                Gapp.exitWholeApp = TRUE;							// If So done=TRUE
            }
            else
            {
                TranslateMessage(&msg);				// Translate The Message
                DispatchMessage(&msg);				// Dispatch The Message
            }
        }
        else
        {										// If There Are No Messages        
            if (Gapp.gfxInitialised == false)
                triggerBreakpoint();
            mainLoop();
        }
    }

    deinitialise1();

    //wglMakeCurrent(hDC, 0); // Remove the rendering context from our device context
    //wglDeleteContext(hRC); // Delete our rendering context

    //ReleaseDC(hWnd, hDC); // Release the device context from our window
    debugLog << L"Finished\n";
}

 void skool();

void masterInitialise()
{
    //skool();
    
    wchar_t wcstring[] = L"Murky8\n";
    
#ifdef USE_OPENGL
    debugLog << L"Using openGL\n";
#endif

#ifdef USE_DIRECT3D
    debugLog << L"Using D3d\n";
#endif

#ifdef ENVIRONMENT32
    debugLog << L"32 bit ";
#endif
    
#ifdef ENVIRONMENT64
    debugLog << L"64 bit ";
#endif

#ifdef WIN32
    debugLog << L"Windows\n";
#endif
    
/*
#ifdef __INTEL_COMPILER
    debugLog << L"Compiled with Intel " << (int)__INTEL_COMPILER << L"\n";
    #else
    #ifdef _MSC_VER
        debugLog << L"Compiled with MS C++ " << (int)_MSC_VER<< L"\n";
    #endif
#endif
    //__clang__
    */

    qdev::setCurrentDirectoryToAppRoot();

    {
        HWND    desktop = GetDesktopWindow();
        RECT    screenDims;
        GetWindowRect(desktop, &screenDims);

        if (Gapp.fullScreen)
        {
            Gapp.screenResX = screenDims.right;
            Gapp.screenResY = screenDims.bottom;
        }
        else
        {
            Gapp.screenResX = 800;
            Gapp.screenResY = 800;
        }
    }

    auto res = createWindow(wcstring, Gapp.screenResX, Gapp.screenResY);
  
    if (!res)
        triggerBreakpoint(L"Init device failed");

    GfxLowLevel::initialise_device(hDC, hRC, hWnd);
    if (0)
    {// debgregee

        //g_pImmediateContext->ClearState();
        //g_pImmediateContext->Flush();

        g_pRasterState->Release();
        g_pDepthStencil->Release();
        g_pDepthStencilView->Release();
 
        g_pRenderTargetView->Release();

        if(g_pSwapChain1) g_pSwapChain1->Release();
        g_pSwapChain->Release();

        if (g_pImmediateContext1)g_pImmediateContext1->Release();
        g_pImmediateContext->Release();

        if(g_pd3dDevice1) g_pd3dDevice1->Release();
        g_pd3dDevice->Release();
        //Sleep(1000);
        d3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_SUMMARY | D3D11_RLDO_DETAIL);
        exit(0);
    }

    RenderHi::initialise();    
}

void deinitialise1()
{
    RenderHi::deinitialise();
}
    
void mainLoop()
{
    GfxLowLevel::onGfxDeviceErrorTriggerBreakpoint();
                
    RenderHi::drawAll();

#ifdef USE_OPENGL
    SwapBuffers(hDC);
#endif

    Gapp.frameCounter++;

    ::Sleep(1000 / 60);
    GfxLowLevel::onGfxDeviceErrorTriggerBreakpoint();
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    wndProcCalled = true;
    switch (message)
    {
    
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_ESCAPE:
            Gapp.exitWholeApp = true;
        }
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

bool createWindow(LPCWSTR title, int width, int height)
{
    WNDCLASS windowClass;
    DWORD dwExStyle = WS_EX_APPWINDOW;// | WS_EX_WINDOWEDGE;

    hInstance = GetModuleHandle(NULL);

    windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    windowClass.lpfnWndProc = (WNDPROC)WndProc;
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.hInstance = hInstance;
    windowClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.hbrBackground = NULL;
    windowClass.lpszMenuName = NULL;
    windowClass.lpszClassName = title;

    if (!RegisterClass(&windowClass))
    {
        triggerBreakpoint(L"Init device failed");

        return false;
    }

    hWnd = CreateWindowEx(
        dwExStyle, 
        title, 
        title, 
        WS_BORDER, //WS_OVERLAPPEDWINDOWWS_POPUPWINDOW,
        CW_USEDEFAULT, 
        0, 
        width, 
        height, 
        NULL, 
        NULL, 
        hInstance, 
        NULL
        );
    hDC = GetDC(hWnd); // Get the device context for our window

     
    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);

    return true;
}
