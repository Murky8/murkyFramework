#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11

namespace murkyFramework
{

	/*class FBX_loader()
	{
	public:		
	}
*/
	void loadFBX_tris(const std::wstring &filePathName, std::vector<Triangle_pct> &tris);	
    void loadFBX(const std::wstring &filePathName, std::vector<Vert_pct> &vertices, std::vector<u16> &indices);

	void serializeTris(const std::wstring &filePathName, std::vector<Triangle_pct> &tris);
	void deserializeTris(const std::wstring &filePathName, std::vector<Triangle_pct> &tris);

	extern bool done; //todo: temp!!!
}

