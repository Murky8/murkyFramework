//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/gfxLowLevel/version_gfxDevice.hpp>
#include <murkyFramework/include/common.hpp>

#include <windows.h>
#include <murkyFramework/include/debugUtils.hpp>

void triggerBreakpoint()
{
    triggerBreakpoint(L"Error\n");    
}

void triggerBreakpoint(const std::wstring &text)
{
    debugLog << text;
    DebugBreak();
}