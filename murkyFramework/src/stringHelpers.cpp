#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/version.hpp>
#include <murkyFramework/include/stringHelpers.hpp>
#include <string>


std::wstring s2ws(const std::string& str)
{
    typedef std::codecvt_utf8<wchar_t> convert_typeX;
    std::wstring_convert<convert_typeX, wchar_t> converterX;

    return converterX.from_bytes(str);
}

std::string ws2s(const std::wstring& wstr)
{
    typedef std::codecvt_utf8<wchar_t> convert_typeX;
    std::wstring_convert<convert_typeX, wchar_t> converterX;

    return converterX.to_bytes(wstr);
}

std::wstring &operator << (std::wstring &lhs, const std::wstring &rhs)
{
    return lhs.append(rhs);
}

std::wstring &operator << (std::wstring &st, int i)
{
    st.append(std::to_wstring(i).c_str());
    st.append(L" ");
    
    return st;
}

std::wstring &operator << (std::wstring &st, float f)
{
    st.append(std::to_wstring(f).c_str());
    st.append(L" ");

    return st;;
}