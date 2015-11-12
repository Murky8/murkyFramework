#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/version.hpp>

namespace GfxDevice
{
	bool loadTextureDataFromFile(std::vector<u8> &textureRawOut, const std::wstring &dirName,
		const std::wstring &fileName, const std::wstring &extensionName,
		u32 &widthOut, u32 &heightOut);
}
