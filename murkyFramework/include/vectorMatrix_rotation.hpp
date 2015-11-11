#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11

//#include <vector>
#include <murkyFramework/include/vectorMatrix.hpp>
#include <murkyFramework/include/common.hpp>	

// given unit dir (x, y, z) and rotation angle (w); create matrix achieving this rotation
// 3 components. 


mat3 makeRotationMatrix4c(vec rv);
mat3 makeRotationMatrix3c(vec rv);
