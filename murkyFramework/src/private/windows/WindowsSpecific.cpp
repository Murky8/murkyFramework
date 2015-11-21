//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/src/private/pch.hpp>


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //http://www.cplusplus.com/forum/windows/39141/

    //AppFramework  * app = (AppFramework*)::GetWindowLongPtr(hWnd, GWLP_USERDATA);
    AppFramework  * app= reinterpret_cast<AppFramework*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
    
    if (app!=nullptr) // note: WndProc can receive messages while initializing ie before app exsists
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

        app->inputDevices->processWindowsMessages(hWnd, message, wParam, lParam);
    }
    else
    {

    }

    return DefWindowProc(hWnd, message, wParam, lParam);
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


WindowsSpecific::WindowsSpecific()
{
}

WindowsSpecific::~WindowsSpecific()
{
}