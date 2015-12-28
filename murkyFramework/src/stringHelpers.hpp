#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
namespace murkyFramework {
std::wstring s2ws(const std::string& str);

std::wstring pChar2wstring(char const * const pChar);

std::string ws2s(const std::wstring& wstr);

std::wstring &operator << (std::wstring &lhs, const std::wstring &rhs);

std::wstring &operator << (std::wstring &st, int i);

std::wstring &operator << (std::wstring &st, float f);

std::wstring &operator << (std::wstring &lhs, vec2 v);

std::wstring &operator << (std::wstring &lhs, vec3 v);

std::wstring &operator << (std::wstring &lhs, vec4 v);

std::wstring &operator << (std::wstring &lhs, mat3 m);

std::wstring &operator << (std::wstring &lhs, mat4 m);



}//namespace murkyFramework



