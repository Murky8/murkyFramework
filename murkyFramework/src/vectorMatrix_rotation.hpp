#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
namespace murkyFramework {}//namespace murkyFramework

// given unit dir (x, y, z) and rotation angle (w); create matrix achieving this rotation
// 3 components. 


mat3 makeRotationMatrix4c(vec rv);
mat3 makeRotationMatrix3c(vec rv);
