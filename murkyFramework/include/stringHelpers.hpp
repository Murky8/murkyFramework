#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
#include <murkyFramework/include/gfxLowLevel/version_gfxDevice.hpp>

#include <string>
#include <codecvt>

std::wstring s2ws(const std::string& str);

std::string ws2s(const std::wstring& wstr);

std::wstring &operator << (std::wstring &lhs, const std::wstring &rhs);

std::wstring &operator << (std::wstring &st, int i);

std::wstring &operator << (std::wstring &st, float f);



