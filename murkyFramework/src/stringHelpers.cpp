//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/src/private/pch.hpp>

std::wstring s2ws(const std::string& str)
{
    typedef std::codecvt_utf8<wchar_t> convert_typeX;
    std::wstring_convert<convert_typeX, wchar_t> converterX;

    return converterX.from_bytes(str);
}

std::wstring pChar2wstring(char const * const pChar)
{
    std::string temp(pChar, strlen(pChar));
    std::wstring wstr = s2ws(temp);
    return wstr;
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

    return st;
}

std::wstring &operator << (std::wstring &lhs, vec2 v)
{
    for (int i = 0; i < vec2::nDim; ++i)
    {
        lhs.append(std::to_wstring(v.s[i]).c_str());
        lhs.append(L" ");
    }
    return lhs;
}

std::wstring &operator << (std::wstring &lhs, vec3 v)
{
    for (int i = 0; i < vec3::nDim; ++i)
    {
        lhs.append(std::to_wstring(v.s[i]).c_str());
        lhs.append(L" ");        
    }
    return lhs;
}

std::wstring &operator << (std::wstring &lhs, vec4 v)
{
    for (int i = 0; i < vec4::nDim; ++i)
    {
        lhs.append(std::to_wstring(v.s[i]).c_str());
        lhs.append(L" ");        
    }
    return lhs;
}

std::wstring &operator << (std::wstring &lhs, mat3 m)
{
	for (int j = 0; j < mat3::nDimJ; ++j)
	{
		for (int i = 0; i < mat3::nDimI; ++i)
		{
			lhs.append(std::to_wstring(m.v[j][i]).c_str());
			lhs.append(L" ");
		}
		lhs.append(L"\n");
	}
	return lhs;
}

std::wstring &operator << (std::wstring &lhs, mat4 m)
{
	for (int j = 0; j < mat4::nDimJ; ++j)
	{
		for (int i = 0; i < mat4::nDimI; ++i)
		{
			lhs.append(std::to_wstring(m.v[j][i]).c_str());
			lhs.append(L" ");
		}
		lhs.append(L"\n");
	}
	return lhs;
}