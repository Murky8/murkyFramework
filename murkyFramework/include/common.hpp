#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
#include <murkyFramework/include/version.hpp>

#include <string>
#include <murkyFramework/include/types.hpp>

void triggerBreakpoint();
void triggerBreakpoint(const std::wstring &text);


// loops from 0 to and including _last
#define for_0_to( _ind, _last )  for(  int (_ind)=0 ; (_ind)<=(_last) ; ++(_ind) )