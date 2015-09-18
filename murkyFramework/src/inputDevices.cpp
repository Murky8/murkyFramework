//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/version.hpp>

#include <windows.h>
#include <winuser.h>
#include "hidusage.h"
#include <murkyFramework/include/inputDevices.hpp>
#include "../include/common.hpp"
#include "../include/debugUtils.hpp"

#ifdef _WINDOWS


//          Copyright Florian Winter 2010 - 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)




/*
namespace tk11 {
	namespace detail {

		Mouse::Mouse(HWND window_handle) {
			// Create description of raw input device
			ZeroMemory(&rid, sizeof(rid));
			rid.usUsagePage = 0x01; // HID_USAGE_PAGE_GENERIC
			rid.usUsage = 0x02; // HID_USAGE_GENERIC_MOUSE

			if (window_handle != 0) {
				// Only the given window receives raw input,
				// and even if the window does not have the input focus.
				rid.dwFlags = RIDEV_INPUTSINK;
				rid.hwndTarget = window_handle;
			}
			else {
				// The window which has the input focus receives raw input.
				rid.dwFlags = 0;
				rid.hwndTarget = 0;
			}

			// Register raw input device
			if (TRUE != RegisterRawInputDevices(&rid, 1, sizeof(rid))) {
				BOOST_THROW_EXCEPTION(Register_Raw_Input_Failed());
			}
		}

		Mouse::~Mouse() {
			// Unregister raw input device
			rid.dwFlags = RIDEV_REMOVE;
			RegisterRawInputDevices(&rid, 1, sizeof(rid));
		}

	}
}
*/

//#ifndef HID_USAGE_PAGE_GENERIC
//#define HID_USAGE_PAGE_GENERIC         ((USHORT) 0x01)
//#endif
//#ifndef HID_USAGE_GENERIC_MOUSE
//#define HID_USAGE_GENERIC_MOUSE        ((USHORT) 0x02)
//#endif
//#ifndef HID_USAGE_PAGE_KEYBOARD      
//#define HID_USAGE_PAGE_KEYBOARD       ((USHORT) 0x07)
//#endif

InputDevices::InputDevices(HWND hWnd)
{
	RAWINPUTDEVICE rid[2];
	rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;  
	rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;    
	rid[0].dwFlags = RIDEV_INPUTSINK;
	rid[0].hwndTarget = hWnd;                

	rid[1].usUsagePage = HID_USAGE_PAGE_GENERIC;     
	rid[1].usUsage = HID_USAGE_GENERIC_KEYBOARD; 
	rid[1].dwFlags = RIDEV_INPUTSINK;
	rid[1].hwndTarget = hWnd;

	//bool result = RegisterRawInputDevices(rid, 2, sizeof(RAWINPUTDEVICE));
	bool result = RegisterRawInputDevices(rid, sizeof(rid)/sizeof(RAWINPUTDEVICE), sizeof(RAWINPUTDEVICE));

	if (!result)
	{
		triggerBreakpoint();
		//EGSystemError("RegisterRawInputDevices Error: ", GetLastError());
	}	

}

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
 //   case WM_MOUSEMOVE:
	//{
	//	int newMouseX = (short)LOWORD(lParam);
	//	int newMouseY = (short)HIWORD(lParam);
	//	if (firstRun)
	//	{ // if first frame, ignore input state as it is meaningless.
	//		mouseX = newMouseX;
	//		mouseY = newMouseY;
	//	}

	//	int dx = newMouseX - mouseX;
	//	mouseDx.push_back(dx);
	//	mouseX = newMouseX;

	//	mouseDy = newMouseY - mouseY;
	//	mouseY = newMouseY;
	//	break;
	//}
	case WM_INPUT:
	{
		UINT dwSize;

		GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize,
			sizeof(RAWINPUTHEADER));
		LPBYTE lpb = new BYTE[dwSize];
		if (lpb == NULL)
		{
			break;
		}

		if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize,
			sizeof(RAWINPUTHEADER)) != dwSize)
			OutputDebugString(TEXT("GetRawInputData does not return correct size !\n"));

		RAWINPUT* raw = (RAWINPUT*)lpb;

			
		if (raw->header.dwType == RIM_TYPEKEYBOARD)
		{
			//hResult = StringCchPrintf(szTempOutput, STRSAFE_MAX_CCH, TEXT(" Kbd: make=%04x Flags:%04x Reserved:%04x ExtraInformation:%08x, msg=%04x VK=%04x \n"),
			//	raw->data.keyboard.MakeCode,
			//	raw->data.keyboard.Flags,
			//	raw->data.keyboard.Reserved,
			//	raw->Data.keyboard.ExtraInformation,
			//	raw->data.keyboard.Message,
			//	raw->data.keyboard.VKey);
			//if (FAILED(hResult))
			//{
			//	// TODO: write error handler
			//}
			//OutputDebugString(szTempOutput);
		}
		else if (raw->header.dwType == RIM_TYPEMOUSE)
		{//https://msdn.microsoft.com/en-us/library/windows/desktop/ms645578(v=vs.85).aspx

			if (raw->data.mouse.usFlags == MOUSE_MOVE_RELATIVE)
			{
				mouseDx.push_back((int)raw->data.mouse.lLastX);
				mouseDy.push_back((int)raw->data.mouse.lLastY);
			}
															
			if (raw->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN)
				mouseB0 = 1;

			if (raw->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP)
				mouseB0 = 0;
			
			if (raw->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN)
				mouseB1 = 1;

			if (raw->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP)
				mouseB1 = 0;
			//mouseB0 = ((mouseB0 << 1) + 1) & 3;

			//hResult = StringCchPrintf(szTempOutput, STRSAFE_MAX_CCH, TEXT("Mouse: usFlags=%04x ulButtons=%04x usButtonFlags=%04x usButtonData=%04x ulRawButtons=%04x lLastX=%04x lLastY=%04x ulExtraInformation=%04x\r\n"),
			//	raw->data.mouse.usFlags,
			//	raw->data.mouse.ulButtons,
			//	raw->data.mouse.usButtonFlags,
			//	raw->data.mouse.usButtonData,
			//	raw->data.mouse.ulRawButtons,
			//	raw->data.mouse.lLastX,
			//	raw->data.mouse.lLastY,
			//	raw->data.mouse.ulExtraInformation);

			//if (FAILED(hResult))
			//{
			//	// TODO: write error handler
			//}
			//OutputDebugString(szTempOutput);
		}

		delete[] lpb;
		break;
	}
    }

	if(firstRun)
		firstRun = false;
}

bool InputDevices::consumeSinglemouseDx(int &out)
{
	if (mouseDx.size() == 0)
		return false;

	out = mouseDx[0];
	mouseDx.pop_front();

	return true;
}

int InputDevices::consumeAllMouseDx()
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
