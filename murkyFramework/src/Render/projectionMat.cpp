//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/src/pch.hpp>
namespace murkyFramework {
namespace Render
{

    mat4    makeLookAtPosOri(vec4 cameraPos, vec4 subjectPos)
    {
        mat4 res;
        res.f = (subjectPos - cameraPos).unitDir();
        res.r = cross(res.f, vec4(0.f, 1.f, 0.f)).unitDir();
        res.u = cross(res.r, res.f);
        res.trans = cameraPos;

        return res;
    }

    // see: 3d projection maths.doc/note:12
    mat4 makeCameraMatrix(mat4 in_transOri)
    {
        /*mat4 m;
        m = in_ori.transpose();

        vec t = -1.f*(in_pos*m);

        m.set_t(t);

        m.v[3][3] = 1.f;
        return m;*/
        mat4 trans;
        mat4 ori;
        in_transOri.splitToTransOri(trans, ori);
        
        ori = ori.transposed();
        ori.set_t(-1.f*(trans.get_t()*ori));        

        ori.v[3][3] = 1.f; // necessary?
        return ori;
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
}//namespace murkyFramework

