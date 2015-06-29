//------------------------------------------------------------------------------
// 2015 J. Coelho
// Platforms: Intel C++11, MS C++11(2013), Win32

#include <version.hpp>
#include <vectorMatrix.hpp>

#include <cmath>


//------------------------------------------------------------------------------
// vec4

vec4::vec4(float in_x, float in_y, float in_z, float in_w)
    : x(in_x), y(in_y), z(in_z), w(in_w)
{}

vec4::vec4(float a)
: x(a), y(a), z(a), w(a)
{}

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

//------------------------------------------------------------------------------
// vec3
vec3::vec3(float x, float y, float z)
: x(x), y(y), z(z) 
{
    //debugLog << L"vec3 " << x << y << z << "\n";
}

vec3::vec3(float a)
: x(a), y(a), z(a) 
{}

vec3::vec3(vec2 v) 
:x(v.x), y(v.y), z(0.f)
{}

//------------------------------------------------------------------------------
// mat4
mat4::mat4(f32 m[nDimJ][nDimI])
{       
    for (auto j = 0; j < nDimJ; ++j)
        for (auto i = 0; i < nDimI; ++i)
            v[j][i] = m[j][i];
}

mat4::mat4(Unit)
{   
    for (auto j = 0; j < nDimJ; ++j)
        for (auto i = 0; i < nDimI; ++i)
            v[j][i] = (i == j) ? 1.f : 0.f;                
}

mat4::mat4(Zero)
{   
    for (auto j = 0; j < nDimJ; ++j)
        for (auto i = 0; i < nDimI; ++i)
            v[j][i] = 0.f;    
}

vec3 operator +(const vec3 &a, const vec3 &b)
{    
    return{
        a.x + b.x,
        a.y + b.y,
        a.z + b.z };
}

vec3 operator -(const vec3 &a, const vec3 &b)
{
    return{
        a.x - b.x,
        a.y - b.y,
        a.z - b.z };    
}

vec3 operator +=(vec3 &a, const vec3 &b)
{
    return(a = a + b);
}

vec3 operator -=(vec3 &a, const vec3 &b)
{
    return(a = a - b);
}


//------------------------------------------------------------------------------
// vec2

vec2::vec2(float inx, float iny) : x(inx), y(iny)
{
    //debugLog << L"vec2 " << x << y << "\n";
}

vec2 operator +(const vec2 &a, const vec2 &b)
{
    return{
        a.x + b.x,
        a.y + b.y };
}

vec2 operator +=(vec2 &a, const vec2 &b)
{
    return(a = a + b);
}

vec2 operator -(const vec2 &a, const vec2 &b)
{
    return{
        a.x - b.x,
        a.y - b.y };
}

vec2 operator -=(vec2 &a, const vec2 &b)
{
    return(a = a - b);
}



