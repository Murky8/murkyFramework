//------------------------------------------------------------------------------
// 2015 J. Coelho.
// C++11. Windows. /*Android.*/
// pragma once
#include <vector>

#include <version.hpp>
#include <common.hpp>
#include <debugUtils.hpp>


class SerializationStream
{
public:
    // Constructors
    SerializationStream();
    // Destructors
    // Methods
    template<typename T>
    void add(T data);
    // Data
    std::vector<u8> data;
private:
    // Constructors
    // Destructors
    // Methods
    // Data
};

