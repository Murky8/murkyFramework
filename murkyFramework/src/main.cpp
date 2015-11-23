//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/src/private/pch.hpp>

// external forward declarations
namespace Render
{        
    void initialise(const SystemSpecific &);
}

// forward declarations
void mainLoop_threadMain(AppFramework  * app);
bool createWindow(LPCWSTR title, int width, int height);

// private data/state
namespace
{
    //HDC		hDC;		// opengl only? SwapBuffers(GfxDevice::hDC)
    //HGLRC		hRC;		// opengl only?
    //HINSTANCE	hInstance;	// Holds The Instance Of The Application
    //HWND		hWnd;	    // Holds Our Window Handle
    u64         frameStartTime = 0;    
}

// only global for debugging/development purposes.
AppFramework *g_appDebug = nullptr;
bool g_appInitialised = false;

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
}

void deinitialise_main()
{
    Render::deinitialise();
}

//------------------------------------------------------------------------------
// 
int main()
{    
    //skool();    
    AppFramework *app = new AppFramework;
    g_appDebug = app;
       
    compileResources();// move this!	            
    
    SetWindowLongPtr( //note: windows msg loop inactive until following is set.
        (dynamic_cast<WindowsSpecific*>(app->systemSpecific))->gethWnd(), 
        GWLP_USERDATA, 
        (LONG_PTR)app); 

    while (app->exitWholeApp==false)
    {
    MSG		msg;
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {	// Is There A Message Waiting?        
            if (msg.message == WM_QUIT)
            {				// Have We Received A Quit Message?
                app->exitWholeApp = TRUE;							// If So done=TRUE
            }
            else
            {
                TranslateMessage(&msg);				// Translate The Message
                DispatchMessage(&msg);				// Dispatch The Message
            }
        }
        else
        {										// If There Are No Messages        
            //if (app->gfxInitialised == false)
              //  triggerBreakpoint();

            mainLoop_threadMain(app);
        }
    }

    delete app;    
    debugLog << L"Finished\n";
}

//bool createWindow(LPCWSTR title, int width, int height)
//{
//    hInstance = GetModuleHandle(nullptr);
//    if (hInstance == NULL)
//        triggerBreakpoint();
//
//    WNDCLASSEX windowClass = { 0 };
//    windowClass.cbSize = sizeof(WNDCLASSEX);
//    windowClass.style = CS_HREDRAW | CS_VREDRAW;
//    windowClass.lpfnWndProc = WndProc;
//    windowClass.hInstance = hInstance;
//    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
//    windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
//    windowClass.lpszClassName = title;
//    
//    if (RegisterClassEx(&windowClass)==0)
//    {
//        auto errorCode = GetLastError();
//        triggerBreakpoint(L"Init device failed\n");
//        return false;
//    }
//
//    RECT rect;
//    rect.left = 0;
//    rect.top = 0;
//    rect.right = width;
//    rect.bottom = height;
//    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
//
//    hWnd = CreateWindowEx(
//        NULL, 
//        title, 
//        title, 
//        WS_OVERLAPPEDWINDOW,
//        CW_USEDEFAULT, 
//        CW_USEDEFAULT,
//        rect.right-rect.left,
//        rect.bottom -rect.top,
//        NULL, 
//        NULL, 
//        hInstance, 
//        NULL
//        );
//
//    if (hWnd == NULL)
//    {
//        //https://msdn.microsoft.com/en-us/library/windows/desktop/ms681381(v=vs.85).aspx
//        auto errorCode = GetLastError();
//        triggerBreakpoint();
//        return false;
//    }
//
//    hDC = GetDC(hWnd); // Get the device context for our window
//     
//    ShowWindow(hWnd, 10);
//    UpdateWindow(hWnd);
//    return true;
//}
