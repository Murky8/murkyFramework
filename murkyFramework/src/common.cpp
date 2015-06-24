//------------------------------------------------------------------------------
// 2014 J. Coelho.
// Platform: All
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