//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/version.hpp>

#include <windows.h>
#include <murkyFramework/include/inputDevices.hpp>

#ifdef _WINDOWS
bool InputDevices::keyStatus(InputDevices::KeyCode iKey)
{
    if (GetAsyncKeyState(static_cast<int>(iKey)) != 0)
        return true;
    else
        return false;
};

void InputDevices::processWindowsMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {    
    case WM_MOUSEMOVE:
        int newMouseX = (short)LOWORD(lParam);
        int newMouseY = (short)HIWORD(lParam);

        int dx = newMouseX - mouseX;
        mouseDx.push_back(dx);

        mouseX = newMouseX;

        mouseDy = newMouseY - mouseY;
        mouseY = newMouseY;
        break;
    }
}

int InputDevices::consume_mouseDx()
{
    int acc = 0;
    while (mouseDx.size() != 0)
    {
        acc+= mouseDx[0];
        mouseDx.pop_front();        
    }
    return acc;
}

#endif
