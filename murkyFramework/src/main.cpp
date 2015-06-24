//------------------------------------------------------------------------------
// 2014 J. Coelho.
// Platform: Windows
// Intel C++ 15.0. Requires C++0x14

#include <windows.h>
#include <iostream>
#include <vector>
#include <thread>

#include <glew/include/GL/glew.h> 
#include <glew/include/GL/wglew.h>

#include <murkyFramework/include/appFramework.hpp>
#include <murkyFramework/include/types.hpp>
#include <murkyFramework/include/debugUtils.hpp>
#include <murkyFramework/include/loadSaveFile.hpp>
#include <murkyFramework/include/system.hpp>
#include <murkyFramework/include/gfxLowLevel/gfxPrimativeTypes.hpp>
#include <murkyFramework/include/gfxHighLevel/render.hpp>

/*
namespace
{
    HDC			hDC;		// Private GDI Device Context
    HGLRC		hRC;		// Permanent Rendering Context
    HWND		hWnd;	    // Holds Our Window Handle
    HINSTANCE	hInstance;	// Holds The Instance Of The Application
    u64         frameStartTime  = 0;
    bool        wndProcCalled = false;
}

// Forward declarations
void    mainLoop_thread_gfx();
bool    createWindow(LPCWSTR title, int width, int height);


//--------------------------------------------------------------------------
// App start point
// Note vis to this file only

int WINAPI WinMain_old(HINSTANCE	hInstance,			// Instance
    HINSTANCE	hPrevInstance,		// Previous Instance
    LPSTR		lpCmdLine,			// Command Line Parameters
    int			nCmdShow)			// Window Show State
{
    MSG		msg;
    wchar_t wcstring[] = L"Murky8";  

    //appFramework.mainLoopThread = std::thread(mainLoop);

    //std::thread mainLoopThread(mainLoop);
    //appFramework.registerMainLoopThread(mainLoop());
	debugLog << L"Gapp.screenResX " << Gapp.screenResX << "\n";
    auto res = createWindow(wcstring, Gapp.screenResX, Gapp.screenResY);

    if (res == false)
    {
        debugLog << L"Error creating window\n";
        triggerBreakpoint();
    }

    debugLog << L"Start\n";
    qdev::setCurrentDirectoryToAppRoot();
        
	debugLog << L"Gapp.screenResX " << Gapp.screenResX << "\n";
    RenderHi::initialise(); //Calls Initialise device gfx
	debugLog << L"Gapp.screenResX " << Gapp.screenResX << "\n";

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
            
            mainLoop_thread_gfx();
        }
    }

    wglMakeCurrent(hDC, 0); // Remove the rendering context from our device context
    wglDeleteContext(hRC); // Delete our rendering context

    ReleaseDC(hWnd, hDC); // Release the device context from our window
    debugLog << L"Finished\n";
	return 0;
}


//DebObj operator << (DebObj obj, int i)
//{
//    outputDebugString(std::to_wstring(i).c_str());
//    return obj;
//}

void mainLoop_thread_gfx()
{
    debugLogScreen.clear();
    debugLogScreen += L"Hello\n";
    //debugLogScreen += std::to_wstring(Gapp.frameCounter);
    //debugLogScreen += L"\n";
  */  
   /* u64 time = system2::readTimeStampTicks();
    u64 lastFrameDuration = time - frameStartTime;
    frameStartTime = time;
    f32 frameTime = lastFrameDuration / (2.677e09);
    
    debugLogScreen += std::to_wstring(frameTime);
    debugLogScreen += L"\n";
*/
/*
    RenderHi::drawAll();
    SwapBuffers(hDC);
    Gapp.frameCounter++;

    ::Sleep(1000 / 60);
}

void thread_control()
{
   
}

LRESULT CALLBACK WndProc_old(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    wndProcCalled = true;
    switch (message)
    {
    case WM_SIZE: // If our window is resizing
		debugLog << L" resize " << LOWORD(lParam) << " " << HIWORD(lParam) << L"\n";
    
	 //openglContext.reshapeWindow(LOWORD(lParam), HIWORD(lParam)); // Send the new window size to our OpenGLContext
        if (Gapp.gfxInitialised)
        {
        
        Gapp.screenResX = LOWORD(lParam);
        Gapp.screenResY = HIWORD(lParam);
        if (Gapp.screenResX < 10 || Gapp.screenResX>1900)
            triggerBreakpoint();

        if (Gapp.screenResY < 10 || Gapp.screenResY>1000)
            triggerBreakpoint();

        GfxLowLevel::onGfxDeviceErrorTriggerBreakpoint();
        }
        break;        

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

bool create40Context()
{        
    hDC = GetDC(hWnd); // Get the device context for our window

    PIXELFORMATDESCRIPTOR pfd; // Create a new PIXELFORMATDESCRIPTOR (PFD)
    memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR)); // Clear our  PFD
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR); // Set the size of the PFD to the size of the class
    pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW; // Enable double buffering, opengl support and drawing to a window
    pfd.iPixelType = PFD_TYPE_RGBA; // Set our application to use RGBA pixels
    pfd.cColorBits = 32; // Give us 32 bits of color information (the higher, the more colors)
    pfd.cDepthBits = 32; // Give us 32 bits of depth information (the higher, the more depth levels)
    pfd.iLayerType = PFD_MAIN_PLANE; // Set the layer of the PFD

    int nPixelFormat = ChoosePixelFormat(hDC, &pfd); // Check if our PFD is valid and get a pixel format back
    if (nPixelFormat == 0) // If it fails
        return false;

    bool bResult = (bool)SetPixelFormat(hDC, nPixelFormat, &pfd); // Try and set the pixel format based on our PFD
    if (!bResult) // If it fails
        return false;

    HGLRC tempOpenGLContext = wglCreateContext(hDC); // Create an OpenGL 2.1 context for our device context
    wglMakeCurrent(hDC, tempOpenGLContext); // Make the OpenGL 2.1 context current and active

    glewExperimental = GL_TRUE;
    GLenum error = glewInit(); // Enable GLEW
    if (error != GLEW_OK) // If GLEW fails
        return false;

    int attributes[] = 
    {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 4, // Set the MAJOR version of OpenGL to 4
        WGL_CONTEXT_MINOR_VERSION_ARB, 0, // Set the MINOR version of OpenGL to 0
        WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB, // Set our OpenGL context to be forward compatible
        0
    };

    if (wglewIsSupported("WGL_ARB_create_context") == 1)
    { // If the OpenGL 3.x context creation extension is available
        hRC = wglCreateContextAttribsARB(hDC, NULL, attributes); // Create and OpenGL 3.x context based on the given attributes
        wglMakeCurrent(NULL, NULL); // Remove the temporary context from being active
        wglDeleteContext(tempOpenGLContext); // Delete the temporary OpenGL 2.1 context
        wglMakeCurrent(hDC, hRC); // Make our OpenGL 3.0 context current
    }
    else
    {
        hRC = tempOpenGLContext; // If we didn't have support for OpenGL 3.x and up, use the OpenGL 2.1 context
        triggerBreakpoint();
    }

    int glVersion[2] = { -1, -1 }; // Set some default values for the version
    glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]); // Get back the OpenGL MAJOR version we are using
    glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]); // Get back the OpenGL MAJOR version we are using

    std::cout << "Using OpenGL: " << glVersion[0] << "." << glVersion[1] << std::endl; // Output which version of OpenGL we are using

    return true; // We have successfully created a context, return true
}
bool createWindow(LPCWSTR title, int width, int height)
{
    WNDCLASS windowClass;    
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
        return false;
    }

    hWnd = CreateWindowEx(dwExStyle, title, title, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, width, height, NULL, NULL, hInstance, NULL);

    bool res = create40Context(); // Create our OpenGL context on the given window we just created
    if (!res)
    {        
        return false;
    }

    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);

    return true;
}

*/