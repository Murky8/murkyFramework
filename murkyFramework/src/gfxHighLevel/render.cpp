//------------------------------------------------------------------------------
// 2014 J. Coelho.

#include <vector>
#include <codecvt>
//#include <GL/glew.h> 
//#nclude <GL/wglew.h>

#include <debugUtils.hpp>
#include <loadSaveFile.hpp>
#include <randomNumbers.hpp>
#include <system.hpp>
#include <gfxLowLevel/gfxLowLevel.hpp>
#include <gfxLowLevel/textures.hpp>
#include <gfxHighLevel/render.hpp>
#include <vectorMatrix.hpp>
#include <gfxLowLevel/gfxPrimativeTypes.hpp>
#include <gfxHighLevel/textRender.hpp>
#include <murkyFramework/include/appFramework.hpp>
#include <fileUtils.hpp>

// Private data. Depreciate
namespace
{
    RenderHi::Lines_pcSoftBuffer *linesSoftBuffer;          // todo temp
    RenderHi::Triangles_pcSoftBuffer *trianglesSoftBuffer;  // todo temp    
    GfxLowLevel::VertexBufferRef *vbtris_pct;               //todo temp 
    GfxLowLevel::TextureRef   *textRef;                     //todo temp    

    // Font
}

namespace GfxLowLevel
{// From ?
    extern void loadAllShaders(); // From shaders.cpp
    extern GfxLowLevel::TextureRef   *textRef;
    extern GfxLowLevel::TextureRef   *textFontRef;
}

namespace RenderHi
{// class Lines_pcSoftBuffer
    // Constructor
    
    GfxLowLevel::TextureRef             *font_texture;    // todo temp        
    RenderHi::Triangles_pctSoftBuffer   *font_softBuf;    // todo temp    

    Lines_pcSoftBuffer::Lines_pcSoftBuffer(u32 capacity) :
    vertexBuffer(
    capacity * 2,
    GfxLowLevel::VertexType::posCol,
    GfxLowLevel::BufferAccessType::dynamic)
    {
    }

    void Lines_pcSoftBuffer::addLine(Line_pc line)
    {
        if (lines.size() >= vertexBuffer.capacity*2)
            triggerBreakpoint();

        lines.push_back(line);
    }

    void Lines_pcSoftBuffer::drawAll()
    {
        //GfxLowLevel::drawLines_pc(vertexBuffer, lines);
        //lines.clear();
    }

    //------------------------------------------------------------------------------
    // Triangles_pcSoftBuffer

    // Constructors
    Triangles_pcSoftBuffer::Triangles_pcSoftBuffer(u32 capacity) :
        vertexBuffer(
        capacity * 3,
        GfxLowLevel::VertexType::posCol,
        GfxLowLevel::BufferAccessType::dynamic)
    {
    }

    void Triangles_pcSoftBuffer::addTriangle(Triangle_pc triangle)
    {
        if (triangles.size() >= vertexBuffer.capacity * 3)
            triggerBreakpoint();
        triangles.push_back(triangle);
    }

    void Triangles_pcSoftBuffer::drawAll()
    {
        GfxLowLevel::drawTriangles_pc(vertexBuffer, triangles);
        triangles.clear();
    }

    //------------------------------------------------------------------------------
    // Triangles_pctSoftBuffer
    Triangles_pctSoftBuffer::Triangles_pctSoftBuffer(u32 capacity) :
        vertexBuffer(
        capacity * 3,
        GfxLowLevel::VertexType::posColTex,
        GfxLowLevel::BufferAccessType::dynamic)
    {
    }

    void Triangles_pctSoftBuffer::addTriangle(Triangle_pct triangle)
    {
        if (triangles.size() >= vertexBuffer.capacity * 3)
            triggerBreakpoint();
        triangles.push_back(triangle);
    }

    void Triangles_pctSoftBuffer::drawAll()
    {
        GfxLowLevel::drawTriangles_pct(vertexBuffer, triangles, *font_texture);
        triangles.clear();
    }

#include <iostream>
//#include <boost/filesystem.hpp>
 //using namespace boost::filesystem;

    void initialise()
    {
		debugLog << L"Gapp.screenResX " << Gapp.screenResX << "\n";

        GfxLowLevel::initialise();

        GfxLowLevel::loadAllShaders();             
 
       /* FileDirectoryWalker directoryWalker(L"data/*.png", L"");
        std::wstring str;
        
        while(directoryWalker.findNext(str))
        {            
            OutputDebugString(str.c_str());
            OutputDebugString(L"\n");
        }*/

        ::textRef = new GfxLowLevel::TextureRef(L"data/t0.png");        
        ::linesSoftBuffer = new Lines_pcSoftBuffer(256);
        ::trianglesSoftBuffer = new Triangles_pcSoftBuffer(1);
      
        ::vbtris_pct = new GfxLowLevel::VertexBufferRef(
            100, GfxLowLevel::VertexType::posColTex, GfxLowLevel::BufferAccessType::dynamic);

        // Font
        font_texture = new GfxLowLevel::TextureRef(L"data/font.png");
        font_softBuf = new RenderHi::Triangles_pctSoftBuffer(2 * 1024);
        // Font

        Gapp.gfxInitialised = true;
    }

    void addQuad_pct( std::vector<Triangle_pct> &tris, const Vert_pct v[4])
    {
        tris.push_back( {v[0], v[1], v[2] } );
        tris.push_back( {v[1], v[3], v[2] } );        
    }

    void drawAll()
    {
        GfxLowLevel::drawBegin();
        
        // test
        
        /*for_0_to(i, 20)
        {
            ::linesSoftBuffer->addLine({
                Vert_pc(vec3(qmaths::randFloat_0_to_1(), qmaths::randFloat_0_to_1(), 0.0f), vec3(1.0f, 1.0f, 0.0f)),
                Vert_pc(vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 1.0f))
            });
        }        
        linesSoftBuffer->drawAll(); 
        */
        /*
        ::trianglesSoftBuffer->addTriangle ({
                { { 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } },
                { { 0.0f, 0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
                { { 0.5f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } }
        });

        ::trianglesSoftBuffer->drawAll();
        */
        //test


        // draw texture
        //std::vector<Triangle_pct> tris;
        //Triangle_pct tri
        //{
        //    { { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } },
        //    { { 0.0f, 0.5f, 1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } },
        //    { { 0.5f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } },
        //};
        //tris.push_back(tri0);

        //Triangle_pct tri1
        //{
        //    { { 0.0f, 0.5f, 1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } },
        //    { { 0.5f, 0.5f, 1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } },
        //    { { 0.5f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } },
        //};
        //tris.push_back(tri1);


        //GfxLowLevel::drawTriangles_pct( *::vbtris_pct, tris, *::textRef);
        //// draw texture

        
        // text
        /*if( Gapp.inputDevices.keyStatus(InputDevices::KeyCode::c)==true)
        {
            debugLogScreen += L"Samy has no tits";
        }
*/
        //drawText(debugLogScreen, *textRef);
        // text

        GfxLowLevel::drawEnd();
    }

}
