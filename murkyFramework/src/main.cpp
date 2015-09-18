//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/version.hpp>
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
#include <murkyFramework/include/inputDevices.hpp>
#include <murkyFramework/include/system.hpp>
#include <murkyFramework/include/gfxLowLevel/gfxPrimativeTypes.hpp>
#include <murkyFramework/include/gfxLowLevel/gfxLowLevel.hpp>
#include <murkyFramework/include/gfxHighLevel/render.hpp>
#include <murkyFramework/include/state.hpp>
#include <murkyFramework/include/readFBX.hpp>

#include <d3d11_1.h>// temp
#include <memory>

// forward declarations
namespace RenderHi
{        
    void initialise(HDC &hDC, HGLRC &hRC, HWND &hWnd);
}
void mainLoop_threadMain(InputDevices &inputDevices, State &state);
bool createWindow(LPCWSTR title, int width, int height);

// variables
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
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE	hPrevInstance, LPSTR lpCmdLine,int nCmdShow)			// Window Show State


void initialise_main()
{ 
    std::wstring title{ L"Murky " };

    wchar_t wcstring[] = L"Murky8\n";

#ifdef USE_OPENGL
    debugLog << L"Using openGL\n";
    title += L"OpenGL 4  ";
#endif

#ifdef USE_DIRECT3D
    debugLog << L"Using D3d \n";
    title += L"D3d11  ";

#endif

#ifdef ENVIRONMENT32
    debugLog << L"32 bit ";
    title += L"32 bit  ";
#endif

#ifdef ENVIRONMENT64
    debugLog << L"64 bit ";
    title += L"64 bit  ";
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

    auto res = createWindow(title.c_str(), Gapp.screenResX, Gapp.screenResY);
        

    if (!res)
        triggerBreakpoint(L"Init device failed");


    RenderHi::initialise(hDC, hRC, hWnd);
    bool res2 = loadFBX(L"data", L"tea", L"FBX");

    Gapp.initialised = true;
}

void deinitialise_main()
{
    RenderHi::deinitialise();
}

int main()
{
    MSG		msg;
    //skool();
    
    initialise_main();
        
    InputDevices *pInputDevices(new InputDevices(hWnd));
    State state;

    
    ::SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pInputDevices);
    

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

            mainLoop_threadMain(*pInputDevices, state);

        }
    }

    deinitialise_main();

    //wglMakeCurrent(hDC, 0); // Remove the rendering context from our device context
    //wglDeleteContext(hRC); // Delete our rendering context
    //ReleaseDC(hWnd, hDC); // Release the device context from our window

    delete pInputDevices;
    debugLog << L"Finished\n";
}

//http://www.cplusplus.com/forum/windows/39141/
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

    // note: WndProc can recieve messages while initialising
    if (Gapp.initialised)
    {    
        static InputDevices *pInputDevices = (InputDevices*)::GetWindowLongPtr(hWnd, GWLP_USERDATA);
        pInputDevices->processWindowsMessages(hWnd, message, wParam, lParam);
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}
/*
LRESULT MandelbrotMgr::HandleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
// Current mouse position
int nMouseX = ( short )LOWORD( lParam );
int nMouseY = ( short )HIWORD( lParam );
D3DXVECTOR2 vPosNow = D3DXVECTOR2( static_cast< float >( nMouseX ), static_cast< float >( nMouseY ) );

static D3DXVECTOR2 vPosPrevL;
static D3DXVECTOR2 vPosPrevR;

//float fFPS = DXUTGetFPS();
//if( fFPS < 1.0f ) { fFPS = 60.0f; }
float fFPS = 60.0f;

//if( vPosNow.x > 500 ) { return FALSE; }

switch( uMsg )
{
case WM_LBUTTONDOWN:
case WM_LBUTTONDBLCLK:
vPosPrevL = vPosNow;
return TRUE;

case WM_LBUTTONUP:
return TRUE;

case WM_RBUTTONDOWN:
case WM_RBUTTONDBLCLK:
vPosPrevR = vPosNow;
return TRUE;

case WM_RBUTTONUP:
case WM_MBUTTONUP:
return TRUE;

case WM_MOUSEMOVE:
if( ( MK_LBUTTON & wParam ) )
{
const float POS_DELTA = 0.005f;
D3DXVECTOR2 vPosDiff = vPosNow - vPosPrevL;
D3DXVECTOR2 vPosDiffScaled = vPosDiff * POS_DELTA * ( 60.0f / fFPS );
vPosDiffScaled.x = -vPosDiffScaled.x;
TranslateCenter( vPosDiffScaled );
vPosPrevL = vPosNow;
}
if( ( MK_RBUTTON & wParam ) )
{
const float ZOOM_DELTA = 0.01f;
float fPosDiff = ( vPosNow.x - vPosPrevR.x ) * ZOOM_DELTA;
float fPosDiffScaled = 1.0f - fPosDiff * ( 60.0f / fFPS );
if( fPosDiffScaled < 0.0f ) {
fPosDiffScaled  = 1.0f / abs( fPosDiffScaled );
}
ZoomScale( fPosDiffScaled );
vPosPrevR = vPosNow;
}
return TRUE;
}

return FALSE;
}
*/

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
