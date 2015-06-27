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
#include <fileUtils.hpp>
#include <vectorMatrix.hpp>
#include <murkyFramework/include/appFramework.hpp>
#include <gfxLowLevel/gfxPrimativeTypes.hpp>
#include <gfxLowLevel/gfxLowLevel.hpp>
#include <gfxLowLevel/textures.hpp>
#include <gfxLowLevel/shaders.hpp>
#include <gfxLowLevel/vertexBuffer.hpp>
#include <gfxHighLevel/render.hpp>
#include <gfxHighLevel/textRender.hpp>

namespace RenderHi
{
	// resources for this app	
	TextRender *textRenderer;
	
    void initialise()
    {
		debugLog << L"RenderHi::initialise" << "\n";
		GfxLowLevel::Shaders::initialise();
		textRenderer = new TextRender();
		textRenderer->text = L"h";
		
        Gapp.gfxInitialised = true;
    }

	void drawAll()
	{
		//GfxLowLevel::drawBegin();
		GfxLowLevel::onGfxDeviceErrorTriggerBreakpoint();

		textRenderer->drawText();

		GfxLowLevel::onGfxDeviceErrorTriggerBreakpoint();

		//GfxLowLevel::drawEnd();
	}

	void addQuad_pct(std::vector<Triangle_pct> &tris, const Vert_pct v[4])
	{
		tris.push_back({ v[0], v[1], v[2] });
		tris.push_back({ v[1], v[3], v[2] });
	}

	// class Lines_pcSoftBuffer

    // Constructor
    //
    //GfxLowLevel::TextureRef             *font_texture;    // todo temp        
    //RenderHi::Triangles_pctSoftBuffer   *font_softBuf;    // todo temp    

    //Lines_pcSoftBuffer::Lines_pcSoftBuffer(u32 capacity) :
    //vertexBuffer(
    //capacity * 2,
    //GfxLowLevel::VertexType::posCol,
    //GfxLowLevel::BufferAccessType::dynamic)
    //{
    //}

    //void Lines_pcSoftBuffer::addLine(Line_pc line)
    //{
    //    if (lines.size() >= vertexBuffer.capacity*2)
    //        triggerBreakpoint();

    //    lines.push_back(line);
    //}

    //void Lines_pcSoftBuffer::drawAll()
    //{
    //    //GfxLowLevel::drawLines_pc(vertexBuffer, lines);
    //    //lines.clear();
    //}

    //------------------------------------------------------------------------------
    // Triangles_pcSoftBuffer

    //// Constructors
    //Triangles_pcSoftBuffer::Triangles_pcSoftBuffer(u32 capacity) :
    //    vertexBuffer(
    //    capacity * 3,
    //    GfxLowLevel::VertexType::posCol,
    //    GfxLowLevel::BufferAccessType::dynamic)
    //{
    //}

    //void Triangles_pcSoftBuffer::addTriangle(Triangle_pc triangle)
    //{
    //    if (triangles.size() >= vertexBuffer.capacity * 3)
    //        triggerBreakpoint();
    //    triangles.push_back(triangle);
    //}

    //void Triangles_pcSoftBuffer::drawAll()
    //{
    //    GfxLowLevel::drawTriangles_pc(vertexBuffer, triangles);
    //    triangles.clear();
    //}

    //------------------------------------------------------------------------------
    // Triangles_pctSoftBuffer
    //Triangles_pctSoftBuffer::Triangles_pctSoftBuffer(u32 capacity) :
    //    vertexBuffer(
    //    capacity * 3,
    //    GfxLowLevel::VertexType::posColTex,
    //    GfxLowLevel::BufferAccessType::dynamic)
    //{
    //}

    //void Triangles_pctSoftBuffer::addTriangle(Triangle_pct triangle)
    //{
    //    if (triangles.size() >= vertexBuffer.capacity * 3)
    //        triggerBreakpoint();
    //    triangles.push_back(triangle);
    //}

    //void Triangles_pctSoftBuffer::drawAll()
    //{
    //    GfxLowLevel::drawTriangles_pct(vertexBuffer, triangles, *font_texture);
    //    triangles.clear();
    //}

//        GfxLowLevel::initialise();

  //      GfxLowLevel::loadAllShaders();             
 
       /* FileDirectoryWalker directoryWalker(L"data/*.png", L"");
        std::wstring str;
        
        while(directoryWalker.findNext(str))
        {            
            OutputDebugString(str.c_str());
            OutputDebugString(L"\n");
        }*/

        //::textRef = new GfxLowLevel::TextureRef(L"data/t0.png");        
        //::linesSoftBuffer = new Lines_pcSoftBuffer(256);
        //::trianglesSoftBuffer = new Triangles_pcSoftBuffer(1);
      
        //::vbtris_pct = new GfxLowLevel::VertexBufferRef_Depreciate(
        //    100, GfxLowLevel::VertexType::posColTex, GfxLowLevel::BufferAccessType::dynamic);

        //// Font
        //font_texture = new GfxLowLevel::TextureRef(L"data/font.png");
        //font_softBuf = new RenderHi::Triangles_pctSoftBuffer(2 * 1024);
        // Font


        
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


}
