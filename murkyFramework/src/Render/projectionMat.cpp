//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/src/pch.hpp>

namespace Render
{
    // see: 3d projection maths.doc/note:12
    mat4 makeCameraMatrix(vec in_pos, mat3 in_ori)
    {
        mat4 m(unit);
        m.set_ori(in_ori.transpose());

        vec t = -1.f*(in_pos*m);
        m.set_t(t);

        m.v[3][3] = 1.f;
        return m;
    }

    // coord origin is lower -left
    mat4 makeProjectionMatrix_ortho(f32 left, f32 right, f32 bottom, f32 top, f32 zNear, f32 zFar)
    {
        mat4 m(unit);
        m.v[0][0] = -2.f / (left - right);
        m.v[1][1] = -2.f / (bottom - top);

        m.v[3][0] = -(-left - right) / (left - right);
        m.v[3][1] = -(-bottom - top) / (bottom - top);
        return m;
    }

    //http://www.gamedev.net/page/resources/_/technical/graphics-programming-and-theory/perspective-projections-in-lh-and-rh-systems-r3598		
    mat4 makeProjectionMatrix_perspective(f32 fovYRads, f32 zNear, f32 zFar, f32 aspectRatio)
    {
        // d3d 0 z +1
        mat4 m(zero);
        m.v[1][1] = 1.f / tan(fovYRads*0.5f);
        m.v[0][0] = m.v[1][1] * aspectRatio;

        m.v[2][2] = zFar / (zFar - zNear);
        m.v[2][3] = 1.f;
        m.v[3][2] = -(zFar*zNear) / (zFar - zNear);

        return m;
    }
}

