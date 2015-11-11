#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <string>

#include <murkyFramework/include/GfxDevice/gfxPrimativeTypes.hpp>

namespace murkyFramework
{

	/*class FBX_loader()
	{
	public:		
	}
*/
	void loadFBX_tris(const std::wstring &filePathName, std::vector<Triangle_pct> &tris);	
	void serializeTris(const std::wstring &filePathName, std::vector<Triangle_pct> &tris);
	void deserializeTris(const std::wstring &filePathName, std::vector<Triangle_pct> &tris);

	extern bool done; //todo: temp!!!
}

