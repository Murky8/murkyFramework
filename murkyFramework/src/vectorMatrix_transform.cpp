//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
// for projection matrix see 'gfxHigh/blhah'
#include <murkyFramework/include/version.hpp>

#include <cmath>
#include <murkyFramework/include/vectorMatrix.hpp>
#include <murkyFramework/include/vectorMatrix_rotation.hpp>

//------------------------------------------------------------------------------
#pragma region quaternion
// constructors
// constructors ^
#pragma endregion quaternion

// given unit dir (x, y, z) and rotation angle (w); create matrix achieving this rotation
// rodriguez formula
mat3 makeRotationMatrix4c(vec rv)
{
	float x = rv.x;
	float y = rv.y;
	float z = rv.z;
	float w = rv.w;

	float cwxy, cwxz, cwyz;
	float swx, swy, swz;
	float cwxx, cwyy, cwzz;

	f32 xx = x*x;
	f32 yy = y*y;
	f32 zz = z*z;

	f32 xy = x*y;
	f32 yz = y*z;
	f32 xz = z*x;

	f32 sw = sinf(w);
	swx = sw*x;
	swy = sw*y;
	swz = sw*z;

	f32 cw = cosf(w);
	cwxy = cw*xy;
	cwxz = cw*xz;
	cwyz = cw*yz;

	cwxx = cw*xx;
	cwyy = cw*yy;
	cwzz = cw*zz;

	mat3 m;
	m.v[0][0] = xx + cw - cwxx;
	m.v[0][1] = xy + -cwxy - swz;
	m.v[0][2] = xz + -cwxz + swy;

	m.v[1][0] = xy + -cwxy + swz;
	m.v[1][1] = yy + cw - cwyy;
	m.v[1][2] = yz + -cwyz - swx;

	m.v[2][0] = xz + -cwxz - swy;
	m.v[2][1] = yz + -cwyz + swx;
	m.v[2][2] = zz + cw - cwzz;

	return m;
}

mat3 makeRotationMatrix3c(vec rv)
{
	vec4 dir;
	float len;

	rv.split(dir, len);
	return makeRotationMatrix4c(vec4(dir.x, dir.y, dir.z, len));
}









