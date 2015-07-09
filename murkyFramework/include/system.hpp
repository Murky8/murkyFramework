#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
#include <murkyFramework/include/version.hpp>

#include <murkyFramework/include/types.hpp>

namespace system2
{
    f64 readTimeSeconds();
    f64 readTimeSecondsSinceAppStart();
    u64 ticksCPUSinceAppStart();
}
