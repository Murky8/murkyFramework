#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
#include <murkyFramework/include/gfxLowLevel/version_gfxDevice.hpp>
#include <murkyFramework/include/stringHelpers.hpp>

#include <iostream>

class DebObj
{//dummy object to allow << operator to work on something
};

// data
extern DebObj debugLog;
extern std::wstring debugLogScreen;

// methods
void    outputDebugString(const std::wstring str);
DebObj operator << (DebObj obj, const std::wstring str);
DebObj operator << (DebObj obj, char *const str);
DebObj operator << (DebObj obj, int i);
DebObj operator << (DebObj obj, float f);


