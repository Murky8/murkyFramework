//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/version.hpp>
#include <murkyFramework/include/GfxDevice/version_gfxDevice.hpp>

#include <murkyFramework/include/debugUtils.hpp>
#include <windows.h>
#include <codecvt>
#include <murkyFramework/include/common.hpp>
#include <murkyFramework/include/stringHelpers.hpp>
#include <murkyFramework/include/inputDevices.hpp>
#include <murkyFramework/include/vectorMatrix.hpp>
#include <fstream>
#include <iostream>

std::wstring debugScreenText;
const std::wstring logFileNameWithPath = { L"../../logFile.html" };

//void debug2LogToFile(const std::wstring &textToAdd, const std::wstring filePathWithName)
//{
//    std::ofstream outfile(logFileNameWithPath);
//}
//using namespace std;
void debug2ResetLogFile()
{
	remove(ws2s(logFileNameWithPath).c_str());
}

void debug2LogToFile(const std::wstring &textToAdd )
{
    std::fstream ofile;
    ofile.open(logFileNameWithPath, std::fstream::app);

    ofile << ws2s(textToAdd) << "\n";
    ofile.close();
}

// Data
#pragma region old code
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

DebObj operator << (DebObj obj, wchar_t *const str)
{    
    std::wstring s = std::wstring(str);

    outputDebugString(s.c_str());
    return obj;
}

DebObj operator << (DebObj obj, int i)
{                
    outputDebugString(std::to_wstring(i).c_str());
    outputDebugString(L" ");
    return obj;
}

DebObj operator << (DebObj obj, float f)
{
    outputDebugString(std::to_wstring(f).c_str());
    outputDebugString(L" ");
    return obj;
}

DebObj operator << (DebObj obj, double f)
{
    outputDebugString(std::to_wstring(f).c_str());
    outputDebugString(L" ");
    return obj;
}

DebObj operator << (DebObj obj, vec2 v)
{
    for (int i = 0; i < vec2::nDim; ++i)
    {
        outputDebugString(std::to_wstring(v.s[i]).c_str());
        outputDebugString(L" ");
    }

    return obj;
}

DebObj operator << (DebObj obj, vec3 v)
{
    for (int i = 0; i < vec3::nDim; ++i)
    {
        outputDebugString(std::to_wstring(v.s[i]).c_str());
        outputDebugString(L" ");
    }

    return obj;
}

DebObj operator << (DebObj obj, vec4 v)
{
    for (int i = 0; i < vec4::nDim; ++i)
    {
        outputDebugString(std::to_wstring(v.s[i]).c_str());
        outputDebugString(L" ");
    }

    return obj;
}

DebObj operator << (DebObj obj, mat4 m)
{
    for (int j = 0; j < 4; ++j)
    {
        for (int i = 0; i < 4; ++i)
        {
            outputDebugString(std::to_wstring(m.v[j][i]).c_str());
            outputDebugString(L" ");
        }
        outputDebugString(L"\n");
    }    
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
#pragma endregion 
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




