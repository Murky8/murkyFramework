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
		void add(const std::wstring &name, T rhs);
		T get(const std::wstring &name);		
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
	void CollectionNamed<T>::add(const std::wstring &name, T rhs)
	{
		data.insert(std::pair<std::wstring, T>(name, rhs));
	}

	template <typename T>
	T CollectionNamed<T>::get(const std::wstring &name)
	{
		auto it = data.find(name);
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
	/*template <typename T>
	class CollectionNamedUniquePtrs
	{
	public:
		CollectionNamedUniquePtrs();
		~CollectionNamedUniquePtrs();

		void add(const std::wstring &name, const T& rhs);
		
		T &get(const std::wstring &name) const;
	private:
		std::map<std::wstring, std::uniT> data;
	};*/
}
