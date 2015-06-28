//------------------------------------------------------------------------------
// 2014 J. Coelho
// Platform: All
// Language: Intel C++11. 
#pragma once

#include <vectorMatrix.hpp>

//#pragma pack(push, 1)
//------------------------------------------------------------------------------
// Vertexes
class Vert_pc
{    
public:
	Vert_pc(){}
    Vert_pc(vec3, vec3);
    // Data
    vec3 pos;
    vec3 col;
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
// Lines
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

//------------------------------------------------------------------------------
// Triangles
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
