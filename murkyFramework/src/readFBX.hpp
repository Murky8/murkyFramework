#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11

namespace murkyFramework
{
	void loadFBX_tris(const std::wstring &filePathName, std::vector<Triangle_pct> &tris);	
	void serializeTris(const std::wstring &filePathName, std::vector<Triangle_pct> &tris);
	void deserializeTris(const std::wstring &filePathName, std::vector<Triangle_pct> &tris);

    void loadFBX(const std::wstring &filePathName, std::vector<Vert_pct> &vertices, std::vector<u16> &indices);
    void serializeTris(const std::wstring &filePathName, std::vector<Vert_pct> &vertices, std::vector<u16> &indices);
    void deserializeTris(const std::wstring &filePathName, std::vector<Vert_pct> &vertices, std::vector<u16> &indices);

    void compileFBX(FilePathSplit pathSplit);	
}

