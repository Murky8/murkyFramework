#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11

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
        T& getNew(const std::wstring &name);
	private:
		std::map<std::wstring,T> data;
        const u32 capacity;
	};

	template <typename T>
	CollectionNamed<T>::CollectionNamed() : capacity(100)
	{
        int i = 123;
    }

	template <typename T>
	CollectionNamed<T>::~CollectionNamed()
	{}

	template <typename T>
	void CollectionNamed<T>::add(const std::wstring &name, T rhs)
	{
        if(data.size() >= capacity)
            triggerBreakpoint(L"container capacity exceeded\n");
        else
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

    template <typename T>
    T& CollectionNamed<T>::getNew(const std::wstring &name)
    {        
        T t;

        auto res = data.insert(std::pair<std::wstring, T>(name, t));
        if (res.second == false)
            triggerBreakpoint();

        return res.first->second;
    }

    template <typename T>
    class CollectionNamedIndirect
    {
    private:
        //std::map<std::wstring, *T> data;
    };


}
