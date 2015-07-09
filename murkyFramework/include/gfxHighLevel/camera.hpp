#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
#include <murkyFramework/include/version.hpp>

#include <murkyFramework/include/types.hpp>
#include <murkyFramework/include/vectorMatrix.hpp>

class Camera
{
public:
    // Constructors
    Camera(mat3 &ori, vec4 pos, f32 fov);
    // Destructors
    // Methods
    void getProjectionMat(f32 nearZ, f32 farZ, mat4 &out ) const;
    // Data
    mat3    ori;
    vec4    pos;
    f32     fov;
private:
    // Constructors
    Camera() = delete;
    // Destructors
    // Methods
    // Data
};
    
