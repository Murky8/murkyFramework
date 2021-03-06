#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
namespace murkyFramework {

#if 0
    rrr0
    uuu0
    fff0
    ttt1
#endif
  
class TypeUnit
{
    const u8 dummyVal= 0;
};
extern TypeUnit unit;

class TypeZero
{
public:
    const u8 dummyVal= 0;
};
extern TypeZero zero;



//------------------------------------------------------------------------------
class vec2 
{
public:
    static const auto nDim = 2;	
    // constructors                
    vec2(){}
    vec2(float x, float y);
    vec2(TypeZero dummy);
    vec2(TypeUnit dummy);
        
    friend std::wostream &operator<<(std::wostream &st, const vec2 &v);
    // data    
    union
    {
        struct 
        {
            float x, y;
        };
        float	s[nDim];
    };    
private:    
};

//------------------------------------------------------------------------------
// used for packed structures. E.g. openGL buffers.
class vec3
{
public:
    static const auto nDim = 3;
    // constructors                
    vec3(){}
    vec3(float x, float y, float z);
	vec3(float const* const pFloat);
	vec3(float a);
    vec3(vec2 v);
    vec3(class vec4 v);
    vec3(TypeZero dummy);
    vec3(TypeUnit dummy);
        
    friend std::wostream &operator<<(std::wostream &st, const vec3 &v);
    // data
    union
    {
        struct
        {
            float x, y, z;
        };
        float s[nDim];
    };            
private:    
};


//------------------------------------------------------------------------------
// Main vector class
class vec4
{
public:
    static const auto nDim = 4;
    // constructors            
    vec4(){}	//remove when c++11 compat
    vec4(float x, float y, float z, float w =0.f);
    //vec4(float a);    
	vec4(float const * const pFloat);
    vec4(vec3);    
    vec4(TypeZero dummy);
    vec4(TypeUnit dummy);

    // methods
    vec4    unitDir() const;
    float   length() const;
    bool    split_3c(vec4 &dir, float &len) const;    	
    friend  std::wostream &operator<<(std::wostream &st, const vec4 &v);

    // data
    static const vec4 right;
    static const vec4 up;
    static const vec4 forward;
    union
    {
        struct
        {
            float x, y, z, w;
        };
        float s[nDim];
    };            
private:    
};


typedef vec4 vec;

//------------------------------------------------------------------------------
class mat3
{
public:
    static const auto nDimI = 3;
    static const auto nDimJ = 3;    
    // constructors
    mat3(){}
    mat3(f32 m_[nDimJ][nDimI] );
    mat3(TypeUnit);
    mat3(TypeZero);	    
    explicit mat3(const class mat4& rhs);
    mat3(const std::vector<u8> &inData);    
    // methods
        vec3    get_r() const;
        vec3    get_u() const;
        vec3    get_f() const;
		void	set_v(vec3 in, int rowIndex);
		void    set_r(vec4);
        void    set_u(vec4);
        void    set_f(vec4);     
        mat3    inverse() const;
        mat3    transpose() const;

        void serialize(std::vector<u8> &outData) const;
        friend std::wostream &operator<<(std::wostream &st, const mat3 &v);
    // data
        f32 v[nDimJ][nDimI];
private:    
};


//------------------------------------------------------------------------------
#pragma region mat43
class mat43
{
    // w component not used
public:
    static const auto nDimI = 4;
    static const auto nDimJ = 3;    

    mat43() {};
    mat43(TypeUnit);
    mat43(TypeZero);
    mat43(const mat3& rhs);

    mat43 transposed() const ;
    
    union
    {
        f32  v[nDimJ][nDimI];
        vec4 row[nDimJ];
        struct
        {
            vec4 r;
            vec4 u;
            vec4 f;
        };
    };
};

mat43 operator *(const mat43 &ml, const mat43 &mr);
vec4 operator *(const vec4 &v, const mat43 &m);

#pragma endregion

//------------------------------------------------------------------------------
class mat4
{
public:
    static const auto nDimI = 4;
    static const auto nDimJ = 4;
    // constructors
    mat4(){}
    explicit mat4(const mat3 &rhs);
    mat4(const mat43 &m, const vec4 &v);
    mat4(f32 m[nDimJ][nDimI]);
    mat4(TypeUnit);
    mat4(TypeZero);
    mat4(float const * const inData);
    // methods
    vec4    get_r() const;
    vec4    get_u() const;
    vec4    get_f() const;
	vec4    get_t() const;  
    mat3    get_ruf() const;
	void	set_v(vec4 in, int index);
	void    set_r(vec4);
    void    set_u(vec4);
	void    set_f(vec4);
	void    set_t(vec4);
    void    set_ruf(const mat3 &rhs);	
    
    
    mat4    transposed() const;
    mat4    transposedOri() const;
    void    splitToTransOri(mat4 &out_trans, mat4 &out_ori) const;

    void    serialize(std::vector<u8> &outData) const;
    friend  std::wostream &operator<<(std::wostream &st, const mat4 &v);
    // data
    union
    {
        f32 v[nDimJ][nDimI];
        struct
        {
            vec4 r;
            vec4 u;
            vec4 f;          
        };
        struct
        {
            mat43 ruf;
            vec4 trans;
        };
        //f32 m[nDimJ*nDimI];
    };

private:    
};

class Quaternion
{
public:
    vec4 q;
};

//--------------------------------------------------------------------------
// operators 

// vec2
vec2 operator +(const vec2 &a, const vec2 &b);
vec2 operator +=(vec2 &a, const vec2 &b);

vec2 operator -(const vec2 &a, const vec2 &b);
vec2 operator -=(vec2 &a, const vec2 &b);

vec2 operator *(const vec2 &a, const float &s);
vec2 operator *=(vec2 &va, const vec2 &vb);

vec2 operator /(const vec2 &a, const float &d);
vec2 operator /=(vec2 &a, const vec2 &d);

// vec3
vec3 operator +(const vec3 &a, const vec3 &b);
vec3 operator +=(vec3 &a, const vec3 &b);

vec3 operator -(const vec3 &a, const vec3 &b);
vec3 operator -=(vec3 &a, const vec3 &b);

vec3 operator *(const vec3 &a, const float &s);
vec3 operator *=(vec3 &va, const vec3 &vb);

vec3 operator /(const vec3 &a, const float &d);
vec3 operator /=(vec3 &a, const vec3 &d);

// vec4
vec4 operator +(const vec4 &a, const vec4 &b);
vec4 operator +=(vec4 &a, const vec4 &b);

vec4 operator -(const vec4 &a, const vec4 &b);
vec4 operator -=(vec4 &a, const vec4 &b);

vec4 operator *(const vec4 &a, const float &m);
vec4 operator *(const float &m, const vec4 &a);
vec4 operator *=(vec4 &a, const vec4 &m);
vec4 operator *=(vec4 &a, const float &m);
                                         
vec4 operator /(const vec4 &a, const float &d);
vec4 operator /=(vec4 &a, const vec4 &d);

vec4  cross(const vec4 &a, const vec4 &b);
float dot(const vec4 &a, const vec4 &b);

// mat3
vec3 operator *(const mat3 &m, const vec3 &va);
vec3 operator *(const vec3 &v, const mat3 &m);

mat3 operator *(const mat3 &m, const float &s);
mat3 operator *=(mat3 &m, const float &s);
mat3 operator *(const mat3 &ma, const mat3 &mb);

// mat4
vec4 operator *(const mat4 &m, const vec4 &v);
vec4 operator *(const vec4 &v, const mat4 &m);
mat4 operator *(const mat4 &m, const float &s);
mat4 operator *=(mat4 &m, const float &s);
mat4 operator *=(mat4 &ml, const mat4 &mr);
mat4 operator *(const mat4 &ma, const mat4 &mb);

struct veci_u32
{
    u32 ix, iy, iz, iw;
};

struct veci_u16
{
    u16 ix, iy, iz, iw;
};


}//namespace murkyFramework
