//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11

#include <murkyFramework/src/pch.hpp>

namespace murkyFramework {

void triggerBreakpoint()
{
    triggerBreakpoint(L"Error\n");    
}

void triggerBreakpoint(const std::wstring &text)
{
    debugLog << text;
    DebugBreak();
}
}//namespace murkyFramework