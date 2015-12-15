//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/src/pch.hpp>

namespace murkyFramework {

    // forward declarations
    void mainLoop_threadMain(class murkyFramework::AppFramework  *const app);

    namespace systemSpecific {
        LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
        {
            //http://www.cplusplus.com/forum/windows/39141/

            //AppFramework  * app = (AppFramework*)::GetWindowLongPtr(hWnd, GWLP_USERDATA);
            AppFramework  *const app = reinterpret_cast<AppFramework *const>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));

            if (app != nullptr) // note: WndProc can receive messages while initializing ie before app exsists
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
                        app->exitWholeApp = true;
                    }
                }

                //todo: should only pass/process input messages here
                app->inputDevices->processWindowsMessages(hWnd, message, wParam, lParam);
            }
            else
            {
            }
            return DefWindowProc(hWnd, message, wParam, lParam);
        }

        WindowsSpecific::WindowsSpecific() :
            appStartTime(readTimeSeconds())
        {

        }

        f64 WindowsSpecific::getPerformanceCounterFrequency()
        {
            LARGE_INTEGER  d;
            QueryPerformanceFrequency(&d);
            return static_cast<f64>(d.QuadPart);
        }

        f64 WindowsSpecific::readTimeSeconds()
        {
            static bool firstTime{ true };
            if (firstTime == true)
            {
                performanceCounterFrequency = getPerformanceCounterFrequency();
                firstTime = false;
            }
            LARGE_INTEGER n, d;

            QueryPerformanceCounter(&n);
            return static_cast<f64>(n.QuadPart) / performanceCounterFrequency;
        }

        f64 WindowsSpecific::readTimeSecondsSinceAppStart()
        {
            return (readTimeSeconds() - appStartTime);
        }

        void WindowsSpecific::windowsLoopIteration()
        {
        }

        bool WindowsSpecific::createWindow(std::wstring title, int width, int height)
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
            windowClass.lpszClassName = title.c_str();

            if (RegisterClassEx(&windowClass) == 0)
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
                title.c_str(),
                title.c_str(),
                WS_OVERLAPPEDWINDOW,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                rect.right - rect.left,
                rect.bottom - rect.top,
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

            SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)nullptr); // this will eventually hold appFramework* but 
            //  appFramework* hasn't been created yet'

            hDC = GetDC(hWnd); // Get the device context for our window

            ShowWindow(hWnd, 1);
            UpdateWindow(hWnd);
            return true;
        }

        /*struct WindowsSpecific_main2_parameters
        {
            bool exitWholeApp;
        };
*/
        void main2(class murkyFramework::AppFramework *const app)
        {
            while (app->exitWholeApp == false)
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
                    mainLoop_threadMain(app);
                    main_noGfx_parameters paras;
                    paras.app = app;
                    //paras.deltaTime = ?
                    app->main_noGfx(&paras);
                }
            }
        }

        WindowsSpecific::~WindowsSpecific()
        {
        }

    }//namespace systemSpecific
}//namespace murkyFramework


#if 0
/*
using namespace std::chrono;
#include <stdint.h>

//  Windows
#ifdef _WIN32

#include <intrin.h>

u64 ticksCPUSinceAppStart()
{
return __rdtsc();
}

//  Linux/GCC
#else

uint64_t rdtsc(){
unsigned int lo,hi;
__asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
return ((uint64_t)hi << 32) | lo;
}

#endif
*/
#endif