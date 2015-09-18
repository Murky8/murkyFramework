//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/version.hpp>
#include <murkyFramework/include/gfxLowLevel/version_gfxDevice.hpp>

#include <murkyFramework/include/gfxHighLevel/projectionMat.hpp>
#include <murkyFramework/include/gfxLowLevel/version_gfxDevice.hpp>
#include <murkyFramework/include/common.hpp>
#include <murkyFramework/include/debugUtils.hpp>

namespace RenderHi
{
//    void setProjMatOrtho(f32 right, f32 left, f32 top, f32 bottom, mat4 &matOut)
//    {
//        /*matOut.v[0][0] = 2 / (right - left);
//        matOut.v[1][1] = 2 / (top - bottom);
//        matOut.v[2][2] = -T(2) / (zFar - zNear);
//        matOut.v[3][0] = -(right + left) / (right - left);
//        matOut.v[3][1] = -(top + bottom) / (top - bottom);
//        matOut.v[3][2] = -(zFar + zNear) / (zFar - zNear);
//*/
//        // ATTN!!! FIXME!!!
//        auto    zFar    = 1.0f;
//        auto    zNear   = -1.0f;
//        
//        matOut = mat4(unit);
//
//        matOut.v[0][0] = 2.f / (right - left);
//        matOut.v[1][1] = 2.f / (top - bottom);
//        matOut.v[2][2] = 2.f / (zFar - zNear); 
//        //matOut.v[2][2]
//        matOut.v[3][0] = -(right + left) / (right - left);
//        matOut.v[3][1] = -(top + bottom) / (top - bottom);
//        matOut.v[3][2] = -(zFar + zNear) / (zFar - zNear);
//    }
	


    // coord origin is lower -left
    mat4 makeProjectionMatrix_ortho(f32 left, f32 right, f32 bottom, f32 top, f32 zNear, f32 zFar)
    {
        mat4 m(unit);
        m.v[0][0] = 2.f / (right - left);
        m.v[1][1] = 2.f / (top - bottom);
        m.v[2][2] = -2.f / (zFar - zNear);
        m.v[3][0] = -(right + left) / (right - left);
        m.v[3][1] = -(top + bottom) / (top - bottom);
        m.v[3][2] = -(zFar + zNear) / (zFar - zNear);


        m.v[2][2] *= -1.f;  // left hand coord system fix (z=-z)
        m.v[3][2] *= -1.f;  // left hand coord system fix (z=-z)

        return m;
    }

    //  0  1  2   3
    //0 [w][ ][   ][  ]
    //1 [ ][h][   ][  ]
    //2 [ ][ ][?za][-1]
    //3 [ ][ ][?zb][  ]

    mat4 makeProjectionMatrix(f32 fovRads, f32 zNear, f32 zFar, f32 aspectRatio)
    {
        f32 tanHalfFovy = tan(fovRads / 2.f);
        
        mat4 m(zero);
        
        m.v[0][0] = 1.f / (aspectRatio * tanHalfFovy);
        m.v[1][1] = 1.f / tanHalfFovy;
        m.v[2][2] = -(zFar + zNear) / (zFar - zNear);
        m.v[2][3] = -1.f;
        m.v[3][2] = (-2.f * zFar * zNear) / (zFar - zNear);
        //m.v[2][2] *= -1.f;  // left hand coord system fix (z=-z)
        //m.v[3][2] *= -1.f;  // left hand coord system fix (z=-z)
    
        return m;
    }

    //  0  1  2   3
    //0 [1][ ][ ][x]
    //1 [ ][1][ ][y]
    //2 [ ][ ][1][z]
    //3 [ ][ ][ ][1]
    mat4 makeTranslationMatrix(vec4 trans)
    {        
        mat4 m(unit);

        m.v[0][3] = trans.x;
        m.v[1][3] = trans.y;
        m.v[2][3] = trans.z;
        
        return m;
    }

	   /*static void my_PerspectiveFOV(double fov, double aspect, double near, double far, double* mret) {
        double D2R = M_PI / 180.0;
        double yScale = 1.0 / tan(D2R * fov / 2);
        double xScale = yScale / aspect;
        double nearmfar = near - far;
        double m[] = {
            xScale, 0, 0, 0,
            0, yScale, 0, 0,
            0, 0, (far + near) / nearmfar, -1,
            0, 0, 2 * far*near / nearmfar, 0
        };*/
    /*template <typename T>
    GLM_FUNC_QUALIFIER detail::tmat4x4<T, defaultp> perspective
        (
        T const & fovy,
        T const & aspect,
        T const & zNear,
        T const & zFar
        )
    {
        assert(aspect != static_cast<T>(0));
        assert(zFar != zNear);

#ifdef GLM_FORCE_RADIANS
        T const rad = fovy;
#else
#		pragma message("GLM: perspective function taking degrees as a parameter is deprecated. #define GLM_FORCE_RADIANS before including GLM headers to remove this message.")
        T const rad = glm::radians(fovy);
#endif

        T tanHalfFovy = tan(rad / static_cast<T>(2));

        detail::tmat4x4<T, defaultp> Result(static_cast<T>(0));
        Result[0][0] = static_cast<T>(1) / (aspect * tanHalfFovy);
        Result[1][1] = static_cast<T>(1) / (tanHalfFovy);
        Result[2][2] = -(zFar + zNear) / (zFar - zNear);
        Result[2][3] = -static_cast<T>(1);
        Result[3][2] = -(static_cast<T>(2) * zFar * zNear) / (zFar - zNear);
        return Result;
    }*/
    /*
    void BuildPerspProjMat(float *m, float fov, float aspect,
    float znear, float zfar)
    {
    float ymax = znear * tan(fov * PI_OVER_360);
    float ymin = -ymax;
    float xmax = ymax * aspect;
    float xmin = ymin * aspect;

    float width = xymax - xmin;
    float height = xymax - ymin;

    float depth = zfar - znear;
    float q = -(zfar + znear) / depth;
    float qn = -2 * (zfar * znear) / depth;

    float w = 2 * znear / width;
    w = w / aspect;
    float h = 2 * znear / height;

    m[0]  = w;
    m[1]  = 0
    m[2]  = 0;
    m[3]  = 0;

    m[4]  = 0;
    m[5]  = h;
    m[6]  = 0;
    m[7]  = 0;

    m[8]  = 0;
    m[9]  = 0;
    m[10] = q;
    m[11] = -1;

    m[12] = 0;
    m[13] = 0;
    m[14] = qn;
    m[15] = 0;
    }
    */
    
    

    ////--------------------------------------------------------------------------
    //// Create rotation matrix from a vector
    //// Rotating around vector by |vector| rads
    //// clockwise along axis
    //Mat3 createRotationMat(const Vec4 &v)
    //{
    //    float len;
    //    Vec4  unit;
    //    Mat3  m;
    //    len = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);

    //    if (len == 0)
    //    {   // unit matrix, no rotation
    //        m.setUnit();
    //    }
    //    else
    //    {
    //        unit.x = v.x / len;
    //        unit.y = v.y / len;
    //        unit.z = v.z / len;
    //        m = rodriguez(Vec4(unit.x, unit.y, unit.z, len));
    //    }
    //    return m;
    //}
  /*  mat4 makeLookAtMatrix(mat3 ori, vec3 trans)
    {
        mat4 m(unit);
        return m;
    }*/
/*
    template <typename T>
    GLM_FUNC_QUALIFIER detail::tmat4x4<T, defaultp> frustum
        (
        T const & left,
        T const & right,
        T const & bottom,
        T const & top,
        T const & nearVal,
        T const & farVal
        )
    {
        detail::tmat4x4<T, defaultp> Result(0);
        Result[0][0] = (static_cast<T>(2) * nearVal) / (right - left);
        Result[1][1] = (static_cast<T>(2) * nearVal) / (top - bottom);
        Result[2][0] = (right + left) / (right - left);
        Result[2][1] = (top + bottom) / (top - bottom);
        Result[2][2] = -(farVal + nearVal) / (farVal - nearVal);
        Result[2][3] = static_cast<T>(-1);
        Result[3][2] = -(static_cast<T>(2) * farVal * nearVal) / (farVal - nearVal);
        return Result;
    }*/
/*
    template <typename T>
    GLM_FUNC_QUALIFIER detail::tmat4x4<T, defaultp> perspective
        (
        T const & fovy,
        T const & aspect,
        T const & zNear,
        T const & zFar
        )
    {
        assert(aspect != static_cast<T>(0));
        assert(zFar != zNear);

#ifdef GLM_FORCE_RADIANS
        T const rad = fovy;
#else
#		pragma message("GLM: perspective function taking degrees as a parameter is deprecated. #define GLM_FORCE_RADIANS before including GLM headers to remove this message.")
        T const rad = glm::radians(fovy);
#endif

        T tanHalfFovy = tan(rad / static_cast<T>(2));

        detail::tmat4x4<T, defaultp> Result(static_cast<T>(0));
        Result[0][0] = static_cast<T>(1) / (aspect * tanHalfFovy);
        Result[1][1] = static_cast<T>(1) / (tanHalfFovy);
        Result[2][2] = -(zFar + zNear) / (zFar - zNear);
        Result[2][3] = -static_cast<T>(1);
        Result[3][2] = -(static_cast<T>(2) * zFar * zNear) / (zFar - zNear);
        return Result;
    }
*/
}
