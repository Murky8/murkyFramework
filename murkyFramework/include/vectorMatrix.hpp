#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11

#include <murkyFramework/include/version.hpp>

#include <vector>
#include <murkyFramework/include/common.hpp>	

enum class Unit{ UNIT }; // arrrgghh!
enum class Zero{ ZERO }; // see above

//------------------------------------------------------------------------------
class vec2 
{
public:
    static const auto nDim = 2;	
    // Constructors                
    vec2(){}
    vec2(float x, float y);
    // Destructors
    // Methods
    friend std::wostream &operator<<(std::wostream &st, const vec2 &v);
    // Data
    
    union
    {
        struct 
        {
            float	x, y;
        };
        float	s[nDim];
    };
    
private:
    //vec2() = delete;
};

//------------------------------------------------------------------------------
// used for packed structures. E.g. openGL buffers.
class vec3
{
public:
    static const auto nDim = 3;
    // Constructors                
    vec3(){}
    vec3(float x, float y, float z);
    explicit	vec3(float a);
    explicit	vec3(vec2 v);
    // Destructors
    // Methods
    friend std::wostream &operator<<(std::wostream &st, const vec3 &v);
    // Data
    union
    {
        struct
        {
            float	x, y, z;
        };
        float	s[nDim];
    };
            
private:
    //vec3() = delete;
};

//------------------------------------------------------------------------------
// Main vector class
class vec4
{
public:
    static const auto nDim = 4;
    // Constructors            
    vec4(){}	//remove when c++11 compat
    vec4(float x, float y, float z, float w = 1.f);
    vec4(float a);
    vec4(vec2);
    vec4(vec3);
    // Destructors
    // Methods
    vec4        unitDir() const;
    float       length() const;
    void        split(vec4 &dir, float &len) const;    
    friend std::wostream &operator<<(std::wostream &st, const vec4 &v);
    // Data
    union
    {
        struct
        {
            float	x, y, z, w;
        };
        float	s[nDim];
    };
            
private:
    //vec4() = delete;
};

//------------------------------------------------------------------------------
class mat3
{
public:
    static const auto nDimI = 3;
    static const auto nDimJ = 3;
    // Constructors
    mat3(){}
    mat3(f32 m_[nDimJ][nDimI] );
    mat3(Unit);
    mat3(Zero);
    mat3(const std::vector<u8> &inData);
    // Destructors
    // Methods
        vec4    get_r() const;
        vec4    get_u() const;
        vec4    get_f() const;
        void    set_r(vec4);
        void    set_u(vec4);
        void    set_f(vec4);     
        mat3    inverse() const;
        mat3    transpose() const;

        void serialize(std::vector<u8> &outData) const;
        friend std::wostream &operator<<(std::wostream &st, const mat3 &v);
    // Data
        f32 v[nDimJ][nDimI];
private:
    // Constructors
        //mat3() = delete;    
};

//------------------------------------------------------------------------------
class mat4
{
public:
    static const auto nDimI = 4;
    static const auto nDimJ = 4;
    // Constructors
    mat4(){}
    mat4(f32 m[nDimJ][nDimI]);
    mat4(Unit);
    mat4(Zero);
    mat4(const std::vector<u8> &inData);
    // Destructors
    // Methods
    vec4    get_r() const;
    vec4    get_u() const;
    vec4    get_f() const;
    void    set_r(vec4);
    void    set_u(vec4);
    void    set_f(vec4);
    mat4    inverse() const;
    mat4    transpose() const;

    void    serialize(std::vector<u8> &outData) const;
    friend  std::wostream &operator<<(std::wostream &st, const mat4 &v);
    // Data
    union
    {
        f32 v[nDimJ][nDimI];
        //f32 m[nDimJ*nDimI];
    };

private:
    // Constructors
    //mat4() = delete;
    // Destructors
    // Methods
    // Data        
};

class Quaternion
{
public:
    vec4 q;
};

//--------------------------------------------------------------------------
// Operators 

// vec4
vec4 operator +(const vec4 &a, const vec4 &b);
vec4 operator +(const vec4 &a, const vec4 &b);

vec4 operator +=(vec4 &a, const vec4 &b);

vec4 operator -(const vec4 &a, const vec4 &b);
vec4 operator -=(vec4 &a, const vec4 &b);

vec4 operator *(const vec4 &a, const float &m);
vec4 operator *=(vec4 &a, const vec4 &m);

vec4 operator /(const vec4 &a, const float &d);
vec4 operator /=(vec4 &a, const vec4 &d);

vec4 operator *(const mat3 &m, const vec4 &va);
vec4 operator *(const vec4 &va, const mat3 &m);

vec4  cross(const vec4 &a, const vec4 &b);
float dot(const vec4 &a, const vec4 &b);

// vec3
vec3 operator +=(vec3 &a, const vec3 &b);

// vec2
vec2 operator +=(vec2 &a, const vec2 &b);


mat3 operator *(const mat3 &m, const float &s);
mat3 operator *=(mat3 &m, const float &s);
mat3 operator *(const mat3 &m0, const mat3 &m1);
mat4 operator *(const mat4 &m0, const mat4 &m1);

