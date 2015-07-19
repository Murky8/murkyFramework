//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/gfxLowLevel/version_gfxDevice.hpp>

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

// forward declarations
namespace RenderHi
{        
    void initialise(HDC &hDC, HGLRC &hRC, HWND &hWnd);
}

void mainLoop();
void masterInitialise();
void deinitialise1();
bool createWindow(LPCWSTR title, int width, int height);

// vriables
namespace
{
    HDC			hDC;		// Private GDI Device Context
    HGLRC		hRC;		// Permanent Rendering Context
    HWND		hWnd;	    // Holds Our Window Handle
    HINSTANCE	hInstance;	// Holds The Instance Of The Application
    u64         frameStartTime = 0;
    bool        wndProcCalled = false;
}


void skool();
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE	hPrevInstance, LPSTR lpCmdLine,int nCmdShow)			// Window Show State
{
    MSG		msg;
    //skool();

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


    RenderHi::initialise(hDC, hRC, hWnd);
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
    DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_BORDER | WS_MINIMIZEBOX | WS_VISIBLE;
    DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;

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

    RECT rect;
    rect.left = 0;
    rect.top = 0;
    rect.right = width;
    rect.bottom = height;
    AdjustWindowRectEx(&rect, dwStyle, 0, dwExStyle);

    hWnd = CreateWindowEx(
        dwExStyle, 
        title, 
        title, 
        dwStyle,
        CW_USEDEFAULT, 
        CW_USEDEFAULT,
        rect.right-rect.left,
        rect.bottom -rect.top,
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
