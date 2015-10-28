//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/version.hpp>
#include <murkyFramework/include/GfxDevice/version_gfxDevice.hpp>

#include <windows.h>

#include <iostream>
#include <stdlib.h>
#include <wchar.h>

#include <murkyFramework/include/debugUtils.hpp>
#include <murkyFramework/include/appFramework.hpp>
#include <murkyFramework/include/types.hpp>
#include <murkyFramework/include/loadSaveFile.hpp>
#include <murkyFramework/include/inputDevices.hpp>
#include <murkyFramework/include/Render/render.hpp>
#include <murkyFramework/include/state.hpp>
#include <murkyFramework/include/readFBX.hpp>
#include <murkyFramework/include/system.hpp>
#include <murkyFramework/include/fileUtils.hpp>

// external forward declarations
namespace Render
{        
    void initialise(HDC &hDC, HGLRC &hRC, HWND &hWnd);
}

// forward declarations
void mainLoop_threadMain(InputDevices &inputDevices, State &state);
bool createWindow(LPCWSTR title, int width, int height);

// private data/state
namespace
{
    HWND		hWnd;	    // Holds Our Window Handle
    HINSTANCE	hInstance;	// Holds The Instance Of The Application

    HDC			hDC;		// opengl only? SwapBuffers(GfxDevice::hDC)
    HGLRC		hRC;		// opengl only?
    u64         frameStartTime = 0;    
}

// global data/state
AppFramework *Gapp;
std::vector<Triangle_pct> gdeb_tris;

// testing
void skool();

void compileFBX(FilePathSplit pathSplit)
{
    
    std::wstring binPath = makePathString(pathSplit.directoryPath, pathSplit.fileName, L"bin");

    bool binExsists;
    u64 fbxModTime = getFileModificationTime1601(pathSplit.getJoinedFilePath());
    u64 binModTime = getFileModificationTime1601(binPath, &binExsists);
        
    if (fbxModTime>binModTime || binExsists == false)
    {// compile to .bin
        debugLog << L"bin is not current. compiling \n";
        murkyFramework::loadFBX_tris(pathSplit.getJoinedFilePath(), gdeb_tris);
        murkyFramework::serializeTris(binPath, gdeb_tris);
    }
    else
    {
        debugLog << L"bin is current \n";
        murkyFramework::deserializeTris(binPath, gdeb_tris);
    }
    
}

void compileResources()
{
    // scan directory	
//	visitAllFilesInDirectory(L"data", pr);
//	exit(0);
    std::wregex regexp {L"FBX"};
    visitAllFilesInDirectory(L"data", compileFBX, regexp);

    murkyFramework::done = true;
}

// called from void main()
void initialise_main()
{ 	
    qdev::setCurrentDirectoryToAppRoot();
    debug2ResetLogFile();

    Gapp = new AppFramework();
    std::wstring title{ L"Murky " };
    wchar_t wcstring[] = L"Murky8\n";

#ifdef USE_OPENGL
    debugLog << L"Using openGL\n";    title += L"OpenGL 4  ";
#endif
#ifdef USE_DIRECT3D11
    debugLog << L"Using D3d11 \n";    title += L"D3d11  ";
#endif
#ifdef USE_DIRECT3D12
    debugLog << L"Using D3d12 \n";	title += L"D3d12  ";
#endif
#ifdef ENVIRONMENT32
    debugLog << L"32 bit ";    title += L"32 bit  ";
#endif
#ifdef ENVIRONMENT64
    debugLog << L"64 bit ";    title += L"64 bit  ";
#endif
#ifdef WIN32
    debugLog << L"Windows\n";
#endif    

    // create window. nothing device specific here.
    {
        if (Gapp->fullScreen)
        {
            HWND    desktop = GetDesktopWindow();
            RECT    screenDims;
            GetWindowRect(desktop, &screenDims);
            Gapp->screenResX = screenDims.right;
            Gapp->screenResY = screenDims.bottom;
        }
        else
        {
            Gapp->screenResX = 800;
            Gapp->screenResY = 800;
        }

        auto res = createWindow(title.c_str(), Gapp->screenResX, Gapp->screenResY);    
        if (!res)
            triggerBreakpoint(L"Init device failed");
    }

    Render::initialise(hDC, hRC, hWnd);

    f64 t = system2::readTimeSecondsSinceAppStart();
    
    //  move this!!!!	
    compileResources();
    Gapp->initialised = true;
}

void deinitialise_main()
{
    Render::deinitialise();
}

//------------------------------------------------------------------------------
// 
int main()
{
    
    initialise_main();
        
    // private data/state
    InputDevices *pInputDevices(new InputDevices(hWnd));
    State		state;
    
    // enable passing user data to WndProc
    SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pInputDevices);
    
    while (!Gapp->exitWholeApp)
    {
    MSG		msg;
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {	// Is There A Message Waiting?        
            if (msg.message == WM_QUIT)
            {				// Have We Received A Quit Message?
                Gapp->exitWholeApp = TRUE;							// If So done=TRUE
            }
            else
            {
                TranslateMessage(&msg);				// Translate The Message
                DispatchMessage(&msg);				// Dispatch The Message
            }
        }
        else
        {										// If There Are No Messages        
            if (Gapp->gfxInitialised == false)
                triggerBreakpoint();

            mainLoop_threadMain(*pInputDevices, state);

        }
    }

    deinitialise_main();    
    delete pInputDevices;
    debugLog << L"Finished\n";
}

//http://www.cplusplus.com/forum/windows/39141/
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{    
    switch (message)
    {

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_ESCAPE:
            Gapp->exitWholeApp = true;
        }
    }

    // note: WndProc can recieve messages while initialising
    if (Gapp->initialised)
    {    
        static InputDevices *pInputDevices = (InputDevices*)::GetWindowLongPtr(hWnd, GWLP_USERDATA);
        pInputDevices->processWindowsMessages(hWnd, message, wParam, lParam);
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

bool createWindow(LPCWSTR title, int width, int height)
{
    hInstance = GetModuleHandle(nullptr);
    if (hInstance == NULL)
        triggerBreakpoint();

    WNDCLASSEX windowClass = { 0 };
    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = WndProc;
    windowClass.hInstance = hInstance;
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    windowClass.lpszClassName = title;
    
    if (RegisterClassEx(&windowClass)==0)
    {
        auto errorCode = GetLastError();
        triggerBreakpoint(L"Init device failed\n");
        return false;
    }

    RECT rect;
    rect.left = 0;
    rect.top = 0;
    rect.right = width;
    rect.bottom = height;
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

    hWnd = CreateWindowEx(
        NULL, 
        title, 
        title, 
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 
        CW_USEDEFAULT,
        rect.right-rect.left,
        rect.bottom -rect.top,
        NULL, 
        NULL, 
        hInstance, 
        NULL
        );

    if (hWnd == NULL)
    {
        //https://msdn.microsoft.com/en-us/library/windows/desktop/ms681381(v=vs.85).aspx
        auto errorCode = GetLastError();
        triggerBreakpoint();
        return false;
    }

    hDC = GetDC(hWnd); // Get the device context for our window
     
    ShowWindow(hWnd, 10);
    UpdateWindow(hWnd);
    return true;
}
