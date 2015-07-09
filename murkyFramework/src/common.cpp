//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/version.hpp>

#include <windows.h>
#include <common.hpp>
#include <debugUtils.hpp>

void triggerBreakpoint()
{
    triggerBreakpoint(L"Error\n");    
}

void triggerBreakpoint(const std::wstring &text)
{
    debugLog << text;
    DebugBreak();
}