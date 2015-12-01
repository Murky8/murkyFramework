//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/src/pch.hpp>

AppFramework::AppFramework()
{    
    g_appDebug = this; // warning: see g_aapDebug usage notes: for development only, remove!

    qdev::setCurrentDirectoryToAppRoot();
    debug2ResetLogFile();    
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
#ifdef ANDROID
    debugLog << L"Android\n";
#endif
    fullScreen = false;

    if(fullScreen)
    {
        HWND    desktop = GetDesktopWindow();
        RECT    screenDims;
        GetWindowRect(desktop, &screenDims);
        screenResX = screenDims.right;
        screenResY = screenDims.bottom;
    }
    else
    {
        screenResX = 800;
        screenResY = 800;
    }
    
#ifdef WINDOWS
    auto *windowsSpecific = new WindowsSpecific();
    systemSpecific = windowsSpecific;

#else
#error WINDOWS only at the moment 
#endif

    bool res = systemSpecific->createWindow(title.c_str(), screenResX, screenResY);    
    // note: wndProc starts getting called after this point. createWindow creates hWnd
    if (!res)
        triggerBreakpoint(L"createWindow failed");    
    
    GfxDeviceObj_initStruct initStruct;
    initStruct.screenWidth = screenResX;
    initStruct.screenHeight = screenResY;
    initStruct.windowsSpecific = windowsSpecific;    
    render = new RenderObj(&initStruct);

    Render::initialise(systemSpecific); // todo:depreciate

    // mouse, keyboard, etc input
    inputDevices = new InputDevices(windowsSpecific); // todo: pass less
    game = new Game();
}

AppFramework::~AppFramework()
{
    Render::deinitialise();
    //deinitialise_main();    
    delete  inputDevices;
    delete  render;
    delete  systemSpecific;
}
//void AppFramework::resume()
//{
//}
//
//void AppFramework::serialise(std::vector<char> &out)
//{    
//}
//
//void AppFramework::registerMainLoopThread( )
//{
//    mainLoopThreadAttached = true;
//    //mainLoopThread = thread;
//}
