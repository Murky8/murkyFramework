//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/version.hpp>
namespace murkyFramework {
template< typename T >
void clamp(T &v, T min, T max)
{
    if (v < min)
        v = min;
    if (v > max)
        v = max;
}
}//namespace murkyFramework