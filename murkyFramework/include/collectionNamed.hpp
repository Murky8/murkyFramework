#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11

#include <string>
#include <map>
#include <memory>
#include "common.hpp"

namespace murkyFramework
{
	template <typename T>
	class CollectionNamed
	{
	public:
		CollectionNamed();
		~CollectionNamed();
		void add(wchar_t *name, T rhs);
		T get(wchar_t *name);
	private:
		std::map<std::wstring,T> data;
	};

	template <typename T>
	CollectionNamed<T>::CollectionNamed()
	{
	}

	template <typename T>
	CollectionNamed<T>::~CollectionNamed()
	{
		//triggerBreakpoint();
	}

	template <typename T>
	void CollectionNamed<T>::add(wchar_t *name, T rhs)
	{
		data.insert(std::pair<std::wstring, T>(std::wstring(name), rhs));
	}

	template <typename T>
	T CollectionNamed<T>::get(wchar_t *name)
	{
		auto it = data.find(std::wstring(name));
		if (it != data.end())
		{
			return it->second;
		}
		else
		{
			triggerBreakpoint(L"could not find name in collection\n");
			return it->second; // stop compiler warning			
		}
	}
}
