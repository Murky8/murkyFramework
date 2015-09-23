//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/gfxLowLevel/version_gfxDevice.hpp>

#include <murkyFramework/include/gfxHighLevel/camera.hpp>
#include <murkyFramework/include/common.hpp>
#include <murkyFramework/include/debugUtils.hpp>

Camera::Camera(mat3 &ori, vec4 pos, f32 fov) : ori(ori), pos(pos), fov(fov)
{    
}

void Camera::getProjectionMat(f32 nearZ, f32 farZ, mat4 &out) const
{
    /*
    glm::mat4 projMatrix = glm::perspective(
        2.0f,   // The horizontal Field of View, in degrees : the amount of "zoom". Think "camera lens". Usually between 90° (extra wide) and 30° (quite zoomed in)
        1.0f,   // Aspect Ratio. Depends on the size of your window. Notice that 4/3 == 800/600 == 1280/960, sounds familiar ?
        0.1f,  // Near clipping plane. Keep as big as possible, or you'll get precision issues.
        2.0f);  // Far clipping plane. Keep as little as possible.

    projMatrix[2][0] *= -1.0f;
    projMatrix[2][1] *= -1.0f;
    projMatrix[2][2] *= -1.0f;
    projMatrix[2][3] *= -1.0f;
    */
    /*
    const float n = frustNearDist;
    const float f = frustFarDist;

    // This is transposed from normal mat representation
    //xs, 0,   0,     0,       x    x
    //0,  1.f, 0,     0,       y    y
    //0,  0,   v22,   1,       z    z
    //0,  0,   v32,   0};      1    w 

    Mat4    trans(1.f);
    trans.v[3][0] = -pos.x;
    trans.v[3][1] = -pos.y;
    trans.v[3][2] = -pos.z;

    const float v22 = (f + n) / (f - n);
    const float v32 = -2.f*f*n / (f - n);

    float xs = 1080.f / 1920.f;

    Mat4 rot(ori.transposed());
    rot.v[3][3] = 1.0f;

    projMat.v[0][0] = xs;     projMat.v[0][1] = 0.f;    projMat.v[0][2] = 0.f;    projMat.v[0][3] = 0.f;
    projMat.v[1][0] = 0.f;    projMat.v[1][1] = 1.f;    projMat.v[1][2] = 0.f;    projMat.v[1][3] = 0.f;
    projMat.v[2][0] = 0.f;    projMat.v[2][1] = 0.f;    projMat.v[2][2] = v22;    projMat.v[2][3] = 1.f;
    projMat.v[3][0] = 0.f;    projMat.v[3][1] = 0.f;    projMat.v[3][2] = v32;    projMat.v[3][3] = 0.f;


    projMat = trans*rot*projMat;
    */
}