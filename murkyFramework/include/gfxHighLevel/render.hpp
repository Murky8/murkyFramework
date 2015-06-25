//------------------------------------------------------------------------------
// 2014 J. Coelho.
// Platform: Agnostic no Opengl here
// General frame drawing higher level. 
// TODO change name of file to something more meaningful

#pragma once
#include <vector>

#include <types.hpp>
#include <gfxLowLevel/gfxLowLevel.hpp>

// Private data
namespace
{
    //mat3 
}

namespace RenderHi
{
    class Lines_pcSoftBuffer
    {
    public:
        // Constructors        
        Lines_pcSoftBuffer(u32 capacity);

        // Destructors
        // Methods
        void addLine(Line_pc line);        
        void drawAll();

        // Data
    private:                
        // Constructors        
        Lines_pcSoftBuffer()= delete;
        // Destructors
        // Methods
        // Data
        std::vector<Line_pc> lines;
        GfxLowLevel::VertexBufferRef_Depreciate vertexBuffer;   
    };

    class Triangles_pcSoftBuffer
    {
    public:
        // Constructors        
        Triangles_pcSoftBuffer(u32 capacity);

        // Destructors
        // Methods
        void addTriangle(Triangle_pc Triangle);
        void drawAll();

        // Data
    private:
        // Constructors        
        Triangles_pcSoftBuffer() = delete;
        // Destructors
        // Methods
        // Data
        std::vector<Triangle_pc> triangles;
        GfxLowLevel::VertexBufferRef_Depreciate vertexBuffer;
    };

    class Triangles_pctSoftBuffer
    {
    public:
        // Constructors        
        Triangles_pctSoftBuffer(u32 capacity);

        // Destructors
        // Methods
        void addTriangle(Triangle_pct Triangle);
        void drawAll();

        // Data
    private:
        // Constructors        
        Triangles_pctSoftBuffer() = delete;
        // Destructors
        // Methods
        // Data
        std::vector<Triangle_pct> triangles;
        GfxLowLevel::VertexBufferRef_Depreciate vertexBuffer;
    };

    //class   SoftHardVB_pctTris
    //{
    //public:
    //    // Constructors
    //    SoftHardVB_pctTris(const GfxLowLevel::TextureRef &texture, u32 capacityMax);
    //    // Destructors
    //    // Methods
    //    void pushTri(Triangle_pct tri);
    //    void draw();
    //    // Data
    //private:
    //    // Constructors
    //    SoftHardVB_pctTris() = delete;
    //    // Destructors
    //    // Methods
    //    // Data
    //    std::vector<Triangle_pct>       triangles;
    //    GfxLowLevel::VertexBufferRef    vertexBuffer;
    //    GfxLowLevel::TextureRef         texture;
    //};

    
    void initialise();
    void drawAll();
    void addQuad_pct();
}

