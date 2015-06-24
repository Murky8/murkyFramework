//------------------------------------------------------------------------------
// 2014 J. Coelho
// Platform: Windows
#include <murkyFramework/include/debugUtils.hpp>

#include <windows.h>
#include <common.hpp>
#include <codecvt>
#include <stringHelpers.hpp>
#include <murkyFramework/include/inputDevices.hpp>

//------------------------------------------------------------------------------
// Output to IDE Output

// Data
DebObj debugLog;

void    outputDebugString(const std::wstring str)
{
//#ifdef LOGGING_ENABLED  
    ::OutputDebugString(str.c_str());
//#endif
}

DebObj operator << (DebObj obj, const std::wstring str)
{
    outputDebugString(str.c_str());
    return obj;
}

DebObj operator << (DebObj obj, char *const str)
{
    std::string s4(str, strlen(str));
    std::wstring s5 = s2ws(s4);

    outputDebugString(s5.c_str());
    return obj;
}

DebObj operator << (DebObj obj, int i)
{                
    outputDebugString( std::to_wstring(i).c_str() );
    return obj;
}

// Output to IDE Output
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Output to screen

// Data
std::wstring debugLogScreen;
// Output to screen
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Keyboard
bool debugIsKeyPressed(InputDevices::KeyCode keyCode)
{
#ifdef _WINDOWS
    if (::GetAsyncKeyState(static_cast<int>(keyCode)) != 0)
        return true;
    else
        return false;
#else
breakcompilation
#endif
}
// Keyboard
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Misc
// Misc :TODO move to own file

//std::wstring operator << (std::wstring &wstr, int i)
//{
//    wstr += std::to_wstring(i);
//    return wstr;
//}
// Misc
//------------------------------------------------------------------------------




