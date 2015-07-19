#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
#include <murkyFramework/include/gfxLowLevel/version_gfxDevice.hpp>
#include <murkyFramework/include/vectorMatrix.hpp>

namespace GfxLowLevel
{
    enum class VertexType{ posCol, posColTex };
    enum class PrimativeType{ triangle, line, point };
}

//#pragma pack(push, 1)
//------------------------------------------------------------------------------
// vertexes
class Vert_pc
{    
public:
    Vert_pc(){}
    Vert_pc(vec3, vec3);
    // Data
    vec3 pos; // 12 bytes	
    vec3 col; // 12 bytes	
private:
    //Vert_pc() = delete;   
};

class Vert_pct
{    
public:
    Vert_pct(){}
    Vert_pct(vec3, vec3, vec2);
    // Data
    vec3 pos; // 12 bytes	
    vec3 col; // 12 bytes	
    vec2 textCoords; // 8 bytes
private:
    //Vert_pct() = delete;
};

//------------------------------------------------------------------------------
// lines
class Line_pc
{
public:
    Line_pc(){}
    Line_pc(Vert_pc, Vert_pc);    
    // Data
    Vert_pc v[2];
private:
//    Line_pc() = delete;
};

class Line_pct
{
public:
    Line_pct(){}
    Line_pct(Vert_pct, Vert_pct);
    // Data
    Vert_pct v[2];
private:
    //    Line_pct() = delete;
};

//------------------------------------------------------------------------------
// triangles
class Triangle_pc
{
public:
    Vert_pc v[3];
    Triangle_pc(){}
    Triangle_pc(Vert_pc, Vert_pc, Vert_pc);    
    
private:
  //  Triangle_pc() = delete;
};

class Triangle_pct
{    
public:
    Vert_pct v[3];
    Triangle_pct(){}
    Triangle_pct(Vert_pct, Vert_pct, Vert_pct);
private:
    //Triangle_pct() = delete;
};

//#pragma pack(pop)
