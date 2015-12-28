//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/src/pch.hpp>

namespace murkyFramework {
    // forward declartions
    void compileResources();
    AppFramework::AppFramework(AppFramework_initStruct &appInit)
    {
        murkyFramework::g_appDebug = this; // warning: see g_aapDebug usage notes: for development only, remove!
        
        qdev::setCurrentDirectoryToAppRoot(); // // set to solution directory:'dev c++'
        
        debug2ResetLogFile();
        wchar_t wcstring[] = L"Murky8\n";

#ifdef USE_OPENGL
        debugLog << L"Using openGL\n";    title += L"OpenGL 4  ";
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
        
#ifdef WINDOWS
        if (fullScreen)
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

        auto *windowsSpecific = new systemSpecific::WindowsSpecific();
        system = windowsSpecific;
        //note: windows msg loop inactive until following is set. needed to pass info between 

#else
#error WINDOWS only at the moment 
#endif

        bool res = system->createWindow(title.c_str(), screenResX, screenResY);
        SetWindowLongPtr(windowsSpecific->gethWnd(), GWLP_USERDATA, (LONG_PTR)this);
        // note: wndProc starts getting called after this point. createWindow creates hWnd
        if (!res)
            triggerBreakpoint(L"createWindow failed");

        GfxDevice::GfxDeviceObj_initStruct initStruct;
        initStruct.screenWidth = screenResX;
        initStruct.screenHeight = screenResY;
        initStruct.windowsSpecific = windowsSpecific;
        render = new Render::RenderObj(&initStruct);

        // mouse, keyboard, etc input
        inputDevices = new InputDevices(windowsSpecific); // todo: pass less

        // the game
        game = new Game();

        // sound
        audio = new AudioObj();

        // resources 
        render->loadResources();  //todo: thread
        
        main_noGfx = appInit.main_noGfx;
        main_gfx = appInit.main_gfx;
    }
   
    void AppFramework::run()
    {
#ifdef WINDOWS
        systemSpecific::WindowsSpecific* windowsSpecific{
        dynamic_cast<murkyFramework::systemSpecific::WindowsSpecific*>(system) };
        systemSpecific::main2(this);
#endif
    }

    AppFramework::~AppFramework()
    {
        delete  inputDevices;
        delete  render;
        delete  system;
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


}//namespace murkyFramework