//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/collectionNamed.hpp>
#include <murkyFramework/include/stringHelpers.hpp>

//void TextureManager::insert(const std::wstring &name, TextureId texID)
//{
//	textures.insert(std::pair<std::wstring, TextureId>(name, std::move(texID)));
//}
//
//TextureId &TextureManager::getTextureByName(const std::wstring &name)
//{
//	auto it = textures.find(name);
//	if (it != textures.end())
//	{
//		return it->second;
//	}
//	else
//	{
//		triggerBreakpoint();
//		return it->second;
//	}
//}

namespace murkyFramework
{

	template <typename T>
	CollectionNamed<T>::CollectionNamed()
	{
	}

	template <typename T>
	CollectionNamed<T>::~CollectionNamed()
	{
		triggerBreakpoint();
	}

	template <typename T>
	void CollectionNamed<T>::add(const std::wstring &name, const T& rhs)
	{
		data.insert(std::pair<std::wstring, T>(name, rhs));

	}

	template <typename T>
	T &CollectionNamed<T>::get(const std::wstring &name) const
	{
		auto it = data.find(name);
		if (it != data.end())
		{
			return it->second;
		}
		else
		{
			triggerBreakpoint();
			return it->second; // stop compiler warning			
		}
	}
}