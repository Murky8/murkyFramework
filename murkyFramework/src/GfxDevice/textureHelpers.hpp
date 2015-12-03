#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
namespace murkyFramework {}//namespace murkyFramework
namespace GfxDevice
{
	bool loadTextureDataFromFile(std::vector<u8> &textureRawOut, const std::wstring &dirName,
		const std::wstring &fileName, const std::wstring &extensionName,
		u32 &widthOut, u32 &heightOut);
}
