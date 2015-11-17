//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/src/private/pch.hpp>
#include <murkyFramework/src/private/windows/windowsSpec.hpp>
#if 0
HWND createWindow(LPCWSTR title, int width, int height)
{
    HWND hWnd;
    HINSTANCE	hInstance;	

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
        title,
        title,
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

    hDC = GetDC(hWnd); // Get the device context for our window

    ShowWindow(hWnd, 10);
    UpdateWindow(hWnd);
    return true;
}
#endif

WindowsSpec::WindowsSpec()
{
}

WindowsSpec::~WindowsSpec()
{
}