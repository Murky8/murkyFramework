#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
// contains all of the data to enable reconstruction of the complete state from 
// a saved state

#include <murkyFramework/include/version.hpp>
#include <murkyFramework/include/vectorMatrix.hpp>

struct State
{
    vec4 cursor{ 0, 0, 0 };
};