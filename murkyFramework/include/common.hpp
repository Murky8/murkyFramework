//------------------------------------------------------------------------------
// 2015 J. Coelho.
// C++11
#pragma once

#include <string>
#include <murkyFramework/include/types.hpp>

void triggerBreakpoint();
void triggerBreakpoint(const std::wstring &text);



// loops from 0 to including _last
#define for_0_to( _ind, _last )  for(  int (_ind)=0 ; (_ind)<=(_last) ; ++(_ind) )