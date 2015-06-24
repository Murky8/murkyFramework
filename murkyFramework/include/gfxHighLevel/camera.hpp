//------------------------------------------------------------------------------
// 2015 J. Coelho.
// C++11. Windows. /*Android.*/
// pragma once

#include <vectorMatrix.hpp>
#include <version.hpp>
#include <types.hpp>
#include <debugUtils.hpp>

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
    
