#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
#include <murkyFramework/include/version.hpp>



template <typename T0, typename T1>
auto max(T0 p0, T1 p1) -> decltype( p0+p1 )
{
    if (p1 > p0)
        return p1;
    else
        return p0;
}

/*
class Component
{

}

class Pseudoclass
{
    std::list<Component>    components;
    void serialise();
}

*/
/*
namespace
{
    class x
    {
    public:
        // Constructors
        // Destructors
        // Methods
        // Data
    private:
        // Constructors
        // Destructors
        // Methods
        // Data
    };
}
*/