//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/version.hpp>
#include <murkyFramework/include/gfxLowLevel/version_gfxDevice.hpp>

#include <cmath>
#include <murkyFramework/include/vectorMatrix.hpp>

TypeUnit unit;
TypeZero zero;
const vec4 vec4::right(1.f, 0.f, 0.f);
const vec4 vec4::up(0.f, 1.f, 0.f);
const vec4 vec4::forward(0.f, 0.f, 1.f);

//------------------------------------------------------------------------------
#pragma region vec2
// constructors
vec2::vec2(float inx, float iny) : x(inx), y(iny)
{   
}

vec2::vec2(TypeZero dummy) : x(0.f), y(0.f)
{    
}

vec2::vec2(TypeUnit dummy) : x(1.f), y(1.f)
{    
}
// constructors ^

vec2 operator +(const vec2 &a, const vec2 &b)
{
    return{ a.x + b.x, a.y + b.y };
}

vec2 operator +=(vec2 &a, const vec2 &b)
{
    return(a = a + b);
}

vec2 operator -(const vec2 &a, const vec2 &b)
{
    return{ a.x - b.x, a.y - b.y };
}

vec2 operator -=(vec2 &a, const vec2 &b)
{
    return(a = a - b);
}

vec2 operator *(const vec2 &a, const float &s)
{
    return{ vec2(a.x * s, a.y * s) };
}

vec2 operator*=(vec2 &a, const float &m)
{
    return (a = a*m);
}

vec2 operator/(const vec2 &a, const float &d)
{
    return{ vec2(a.x / d, a.y / d) };
}

vec2 operator/=(vec2 &a, const float &d)
{
    return (a = a / d);
}
#pragma endregion vec2

//------------------------------------------------------------------------------
#pragma region vec3

// constructors
vec3::vec3(float x, float y, float z)
    : x(x), y(y), z(z)
{
}

vec3::vec3(float a)
    : x(a), y(a), z(a)
{}

vec3::vec3(vec2 v)
    : x(v.x), y(v.y), z(0.f)
{}

vec3::vec3(TypeZero dummy) : x(0.f), y(0.f), z(0.f)
{}

vec3::vec3(TypeUnit dummy) : x(1.f), y(1.f), z(1.f)
{}
// constructors ^

vec3 operator +(const vec3 &a, const vec3 &b)
{
    return{
        a.x + b.x,
        a.y + b.y,
        a.z + b.z };
}

vec3 operator +=(vec3 &a, const vec3 &b)
{
    return(a = a + b);
}

vec3 operator -(const vec3 &a, const vec3 &b)
{
    return{
        a.x - b.x,
        a.y - b.y,
        a.z - b.z };
}

vec3 operator -=(vec3 &a, const vec3 &b)
{
    return(a = a - b);
}

vec3 operator *(const vec3 &a, const float &s)
{
    return{
        a.x * s,
        a.y * s,
        a.z * s };
}

vec3 operator*=(vec3 &a, const float &m)
{
    return (a = a*m);
}

vec3 operator/(const vec3 &a, const float &d)
{
    return{
        a.x / d,
        a.y / d,
        a.z / d };
}

vec3 operator/=(vec3 &a, const float &d)
{
    return (a = a / d);
}

#pragma endregion vec3

//------------------------------------------------------------------------------
#pragma region vec4
// vec4

// constructors
vec4::vec4(float in_x, float in_y, float in_z, float in_w)
    : x(in_x), y(in_y), z(in_z), w(in_w)
{}

vec4::vec4(float a)
: x(a), y(a), z(a), w(a)
{}

vec4::vec4(TypeZero dummy) : x(0.f), y(0.f), z(0.f), w(0.f)
{}

vec4::vec4(TypeUnit dummy) : x(1.f), y(1.f), z(1.f), w(1.f)
{}
// constructors ^

vec4 operator +(const vec4 &a, const vec4 &b)
{
    return{
        a.x + b.x,
        a.y + b.y,
        a.z + b.z,
        a.w + b.w };
}

vec4 operator +=(vec4 &a, const vec4 &b)
{
    return(a = a + b);
}

vec4 operator -(const vec4 &a, const vec4 &b)
{
    return{
        a.x - b.x,
        a.y - b.y,
        a.z - b.z,
        a.w - b.w };
}

vec4 operator -=(vec4 &a, const vec4 &b)
{
    return(a = a - b);
}

vec4 operator *(const vec4 &a, const float &s)
{
    return{
        a.x * s,
        a.y * s,
        a.z * s,
        a.w * s };
}

vec4 operator*=(vec4 &a, const float &m)
{
    return (a = a*m);
}

vec4 operator/(const vec4 &a, const float &d)
{
    return{
        a.x / d,
        a.y / d,
        a.z / d,
        a.w / d };
}

vec4 operator/=(vec4 &a, const float &d)
{
    return (a = a / d);
}

vec4 cross(const vec4 &a, const vec4 &b)
{
    return{
        a.z*b.y - a.y*b.z,
        a.x*b.z - a.z*b.x,
        a.y*b.x - a.x*b.y,
        0.0f };
}

float dot(const vec4 &a, const vec4 &b)
{
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

float vec4::length() const
{
    return(sqrtf(x*x + y*y + z*z));
}

vec4 vec4::unitDir() const
{
    float	len;
    vec4    v(*this);

    len = length();
    //if( len	== 0.0f )
    //  v	= Vec4(0.f,0.f,0.f, 1.f);
    //else
    v /= len;

    return v;
}
#pragma endregion vec4

//------------------------------------------------------------------------------
#pragma region mat3

mat3::mat3(TypeUnit dummy)
{
    for (auto j = 0; j < nDimJ; ++j)
        for (auto i = 0; i < nDimI; ++i)
            v[j][i] = (i == j) ? 1.f : 0.f;
}

mat3::mat3(TypeZero dummy)
{
    for (auto j = 0; j < nDimJ; ++j)
        for (auto i = 0; i < nDimI; ++i)
            v[j][i] = 0.f;
}

mat3::mat3(f32 m[nDimJ][nDimI])
{
    for (auto j = 0; j < nDimJ; ++j)
        for (auto i = 0; i < nDimI; ++i)
            v[j][i] = m[j][i];
}
#pragma endregion mat3

//------------------------------------------------------------------------------
#pragma region mat4
mat4::mat4(const mat3 &rhs)
{
    for (auto j = 0; j < rhs.nDimJ; ++j)
        for (auto i = 0; i < rhs.nDimI; ++i)
            v[j][i] = rhs.v[j][i];
}

mat4::mat4(f32 m[nDimJ][nDimI])
{       
    for (auto j = 0; j < nDimJ; ++j)
        for (auto i = 0; i < nDimI; ++i)
            v[j][i] = m[j][i];
}

mat4::mat4(TypeUnit dummy)
{   
    for (auto j = 0; j < nDimJ; ++j)
        for (auto i = 0; i < nDimI; ++i)
            v[j][i] = (i == j) ? 1.f : 0.f;                
}

mat4::mat4(TypeZero dummy)
{   
    for (auto j = 0; j < nDimJ; ++j)
        for (auto i = 0; i < nDimI; ++i)
            v[j][i] = 0.f;    
}

mat4 mat4::transpose() const 
{
    mat4 r;
    for (auto j = 0; j < nDimJ; ++j)
        for (auto i = 0; i < nDimI; ++i)
            r.v[j][i] = v[i][j];
    return r;
}
#pragma endregion mat4




