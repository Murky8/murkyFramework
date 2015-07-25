//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/version.hpp>

#include <windows.h>
#include <murkyFramework/include/inputDevices.hpp>
   
InputDevices *pInputDevices;

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
    int newMouseX = (short)LOWORD(lParam);
    int newMouseY = (short)HIWORD(lParam);
    
    mouseDx = mouseX - newMouseX;
    mouseX  = newMouseX;

    mouseDy = mouseY - newMouseY;
    mouseY = newMouseY;

}
#endif
