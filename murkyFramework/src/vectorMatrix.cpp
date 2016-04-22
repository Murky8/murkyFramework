//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/src/pch.hpp>
namespace murkyFramework {
// convention:
// http://www.mindcontrol.org/~hplus/graphics/matrix-layout.html
// using row-major matrix
// object's orientation matrix is:

// Rx Ry Rz  
// Ux Uy Uz
// Fx Fy Fz

// [0][0] [0][1] ...
// [1][0] [j][i] as memory increase left to right then downwards.


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

vec3::vec3(float const* const pFloat)
{
	x = pFloat[0];
	y = pFloat[1];
	z = pFloat[2];	
}

vec3::vec3(float a)
    : x(a), y(a), z(a)
{}

vec3::vec3(vec2 v)
    : x(v.x), y(v.y), z(0.f)
{}

vec3::vec3(vec4 v)
	: x(v.x), y(v.y), z(v.z)
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

#pragma region vec3p
#pragma endregion vec3p
//------------------------------------------------------------------------------
#pragma region vec4
// vec4

// constructors
vec4::vec4(float in_x, float in_y, float in_z, float in_w)
    : x(in_x), y(in_y), z(in_z), w(in_w)
{}

//vec4::vec4(float a)
//	: x(a), y(a), z(a), w(a)
//{}

vec4::vec4(float const* const pFloat) 
	:	x(pFloat[0]), y(pFloat[1]), z(pFloat[2]), w(pFloat[3])
{}

vec4::vec4(vec3 in) : x(in.x), y(in.y), z(in.z), w(0.f)
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

vec4 operator *(const vec4 &v, const float &s)
{
    return{
        v.x * s,
        v.y * s,
        v.z * s,
        v.w * s };
}

vec4 operator*(const float& s, const vec4& v)
{
	return{
		v.x * s,
		v.y * s,
		v.z * s,
		v.w * s };
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

bool vec4::split_3c(vec4 &dir, f32 &len) const
{
	len = sqrtf(dot(*this, *this));	

	if (len <= 0.00000001f) // todo: fix!!!
	{
		dir = vec4(zero);	
        len = 0.f;
		return false;
	}
	else
	{
		dir = *this / len;
		dir.w = 0.f;
		return true;
	}	
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

mat3::mat3(const mat4& rhs)
{   
    for (auto j = 0; j < nDimJ; ++j)
        for (auto i = 0; i < nDimI; ++i)
            v[j][i] = rhs.v[j][i];
}


vec3 mat3::get_r() const
{
	return vec4(v[0]);
}

vec3 mat3::get_u() const
{
	return vec4(v[1]);
}

vec3 mat3::get_f() const
{
	return vec4(v[2]);
}

void mat3::set_v(vec3 in, int rowIndex)
{
	v[rowIndex][0] = in.x;
	v[rowIndex][1] = in.y;
	v[rowIndex][2] = in.z;	
}

void mat3::set_r(vec4 in)
{
	set_v(in, 0);
}

void mat3::set_u(vec4 in)
{
	set_v(in, 1);
}

void mat3::set_f(vec4 in)
{
	set_v(in, 2);
}

mat3 operator *(const mat3 &m0, const mat3 &m1)
{
	mat3 res;
	for(int y=0;y<mat3::nDimJ;y++)
		for(int x=0;x<mat3::nDimI;x++)
		{
			res.v[y][x]  =	m0.v[y][0]*m1.v[0][x] +m0.v[y][1]*m1.v[1][x] +m0.v[y][2]*m1.v[2][x];
		}
	return res;
}

mat3    mat4::get_ruf() const
{
    mat3 out;
    for (int j = 0;j<mat3::nDimJ;j++)
        for (int i = 0;i < mat3::nDimI;i++)
        {
            out.v[j][i] = this->v[j][i];
        }
    return out;
}

void    mat4::set_ruf(const mat3 &m)
{
    for (int j = 0;j<mat3::nDimJ;j++)
        for (int i = 0;i < mat3::nDimI;i++)
        {
            this->v[j][i] = m.v[j][i];
        }
}

#pragma endregion mat3


//------------------------------------------------------------------------------
#pragma region mat43
mat43::mat43(TypeUnit dummmyVal)
{
    r = vec4(1.0f, 0.f, 0.f);
    u = vec4(0.0f, 1.f, 0.f);
    f = vec4(0.0f, 0.f, 1.f);
}

mat43::mat43(TypeZero dummmyVal)
{
    r = vec4(0.0f);
    u = vec4(0.0f);
    f = vec4(0.0f);
}
mat43   mat43::transposed() const
{
    mat43 r;
    for (auto j = 0; j < nDimJ; ++j)
        for (auto i = 0; i < nDimJ; ++i)
            r.v[j][i] = v[i][j];
    return r;
}

mat43 operator *(const mat43 &ml, const mat43 &mr)
{
    //note: check
    mat43 mrt = mr.transposed();
    mat43 res;

    for (auto j = 0; j < mat43::nDimJ; ++j)
        for (auto i = 0; i < mat43::nDimJ; ++i)
            res.v[j][i] = dot(ml.row[j], mrt.row[i]);

    //paranoid
    res.v[0][3] = 0;
    res.v[1][3] = 0;
    res.v[2][3] = 0;

    return res;
}

vec4 operator *(const vec4 &v, const mat43 &m)
{
    mat43 mt = m.transposed();
    vec4 res;
    res.x = dot(v, mt.r);
    res.y = dot(v, mt.u);
    res.z = dot(v, mt.f);
    res.w = 0.f;//paranoid
    return res;
}


#pragma endregion

//------------------------------------------------------------------------------
#pragma region mat4
mat3 mat3::transpose() const
{	
	mat3 r;
	for (auto j = 0; j < nDimJ; ++j)
		for (auto i = 0; i < nDimI; ++i)
			r.v[j][i] = v[i][j];
	return r;
}

mat4::mat4(const mat43 &m, const vec4 &v) : ruf(m), trans(v)
{
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

mat4::mat4(float const* inData)
{
	set_r(vec4(inData + 0));
	set_u(vec4(inData + 4));
	set_f(vec4(inData + 8));
	set_t(vec4(inData + 12));	
}

mat4::mat4(const mat3 &rhs)
{
    *this = mat4{ unit };

    for (auto j = 0; j < nDimJ-1; ++j)
        for (auto i = 0; i < nDimI-1; ++i)
            v[j][i] = rhs.v[j][i];
}


vec4 mat4::get_r() const
{	return vec4(v[0]);}

vec4 mat4::get_u() const
{	return vec4(v[1]);}

vec4 mat4::get_f() const
{	return vec4(v[2]);}

vec4 mat4::get_t() const
{	return vec4(v[3]);}

void mat4::set_v(vec4 in, int rowIndex)
{
	v[rowIndex][0] = in.x;
	v[rowIndex][1] = in.y;
	v[rowIndex][2] = in.z;
	v[rowIndex][3] = in.w;
}

void mat4::set_r(vec4 in)
{	set_v(in, 0);}

void mat4::set_u(vec4 in)
{	set_v(in, 1);}

void mat4::set_f(vec4 in)
{	set_v(in, 2);}

void mat4::set_t(vec4 in)
{	set_v(in, 3);}



mat4 mat4::transposed() const 
{
    mat4 r;
    for (auto j = 0; j < nDimJ; ++j)
        for (auto i = 0; i < nDimI; ++i)
            r.v[j][i] = v[i][j];
    return r;
}

mat4 mat4::transposedOri() const
{
    mat4 res{unit};
    for (auto j = 0; j < nDimJ-1; ++j)
        for (auto i = 0; i < nDimI-1; ++i)
            res.v[j][i] = v[i][j];
    return res;
}

void    mat4::splitToTransOri(mat4 &out_trans, mat4 &out_ori) const
{
    out_trans = out_ori = mat4{ unit };

    for (auto j = 0; j < nDimJ-1; ++j)
        for (auto i = 0; i < nDimI-1; ++i)
            out_ori.v[i][j] = this->v[i][j];

    out_trans.set_t(this->get_t());
}

mat4 operator *(const mat4 &m0, const mat4 &m1)
{
	mat4 res;
	for (int y = 0 ; y < mat4::nDimJ ; y++)
		for (int x = 0 ; x < mat4::nDimI ; x++)
		{
			res.v[y][x] = m0.v[y][0]*m1.v[0][x] + m0.v[y][1]*m1.v[1][x] 
				+ m0.v[y][2]*m1.v[2][x] + m0.v[y][3] * m1.v[3][x];
		}
	return res;
}

mat4 operator *=(mat4 &ml, const mat4 &mr)
{
    return ml = ml*mr;
}

vec4 operator*(const vec4& v, const mat4& m)
{
	vec	t;
	t.x = v.x*m.v[0][0] + v.y*m.v[1][0] + v.z*m.v[2][0] + v.z*m.v[3][0];
	t.y = v.x*m.v[0][1] + v.y*m.v[1][1] + v.z*m.v[2][1] + v.z*m.v[3][1];
	t.z = v.x*m.v[0][2] + v.y*m.v[1][2] + v.z*m.v[2][2] + v.z*m.v[3][2];
	t.w = v.x*m.v[0][3] + v.y*m.v[1][3] + v.z*m.v[2][3] + v.z*m.v[3][3];
	return t;
}

// v interpreted as column vector
vec4 operator*(const mat4& m, const vec4& v)
{
	vec	t;
	t.x = v.x*m.v[0][0] + v.y*m.v[0][1] + v.z*m.v[0][2] + v.z*m.v[0][3];
	t.y = v.x*m.v[1][0] + v.y*m.v[1][1] + v.z*m.v[1][2] + v.z*m.v[1][3];
	t.z = v.x*m.v[2][0] + v.y*m.v[2][1] + v.z*m.v[2][2] + v.z*m.v[2][3];
	t.w = v.x*m.v[3][0] + v.y*m.v[3][1] + v.z*m.v[3][2] + v.z*m.v[3][3];
	return t;
}

/*
inline vec	operator *(const mat &m, const vec &v)
{
	vec	t;
	t.x = v.x *m.m[0][0] + v.y *m.m[0][1] + v.z *m.m[0][2];
	t.y = v.x *m.m[1][0] + v.y *m.m[1][1] + v.z *m.m[1][2];
	t.z = v.x *m.m[2][0] + v.y *m.m[2][1] + v.z * m.m[2][2];
	return t;
}

inline vec	operator *(const vec &v, const mat &m)
{
	vec	t;
	t.x = v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0];
	t.y = v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1];
	t.z = v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2];
	return t;
}
*/
#pragma endregion mat4




}//namespace murkyFramework