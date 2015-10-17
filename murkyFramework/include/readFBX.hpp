#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <string>
#include <vector>
#include <murkyFramework/include/GfxDevice/gfxPrimativeTypes.hpp>

namespace murkyFramework
{

	/*class FBX_loader()
	{
	public:		
	}
*/
	void loadFBX_tris(const std::wstring &dirName, const std::wstring &fileName, const std::wstring &extensionName,
		std::vector<Triangle_pct> &out_tris);

	void serializeTris(const std::wstring &dirName, const std::wstring &fileName, const std::wstring &extensionName,
		std::vector<Triangle_pct> &tris);

	void deserializeTris(const std::wstring &dirName, const std::wstring &fileName, const std::wstring &extensionName,
		std::vector<Triangle_pct> &tris);

	extern bool done; //todo: temp!!!
}

