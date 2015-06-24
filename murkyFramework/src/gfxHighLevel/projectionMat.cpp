//------------------------------------------------------------------------------
// 2015 J. Coelho.
// C++11. Windows. /*Android.*/
// pragma once

#include <murkyFramework/include/gfxHighLevel/projectionMat.hpp>

#include <murkyFramework/include/version.hpp>
#include <murkyFramework/include/common.hpp>
#include <murkyFramework/include/debugUtils.hpp>

namespace RenderHi
{
    void setProjMatOrtho(f32 right, f32 left, f32 top, f32 bottom, mat4 &matOut)
    {
        /*matOut.v[0][0] = 2 / (right - left);
        matOut.v[1][1] = 2 / (top - bottom);
        matOut.v[2][2] = -T(2) / (zFar - zNear);
        matOut.v[3][0] = -(right + left) / (right - left);
        matOut.v[3][1] = -(top + bottom) / (top - bottom);
        matOut.v[3][2] = -(zFar + zNear) / (zFar - zNear);
*/
        // ATTN!!! FIXME!!!
        auto    zFar    = 1.0f;
        auto    zNear   = -1.0f;
        
        matOut = mat4(Unit::UNIT);

        matOut.v[0][0] = 2.f / (right - left);
        matOut.v[1][1] = 2.f / (top - bottom);
        matOut.v[2][2] = 2.f / (zFar - zNear); 
        //matOut.v[2][2]
        matOut.v[3][0] = -(right + left) / (right - left);
        matOut.v[3][1] = -(top + bottom) / (top - bottom);
        matOut.v[3][2] = -(zFar + zNear) / (zFar - zNear);
    }
}
/*
namespace ?
{
    class ?
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

//------------------------------------------------------------------------------
// Reference
//