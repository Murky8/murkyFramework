//------------------------------------------------------------------------------
// 2015 J. Coelho.

#include <vector>
#include <codecvt>

#include <types.hpp>
#include <debugUtils.hpp>
#include <loadSaveFile.hpp>
#include <randomNumbers.hpp>
#include <system.hpp>
#include <fileUtils.hpp>
#include <vectorMatrix.hpp>
#include <murkyFramework/include/appFramework.hpp>
#include <gfxLowLevel/gfxPrimativeTypes.hpp>
#include <gfxLowLevel/gfxLowLevel.hpp>
#include <gfxLowLevel/texture.hpp>
#include <gfxLowLevel/shaders.hpp>
#include <gfxLowLevel/vertexBuffer.hpp>
#include <gfxHighLevel/render.hpp>
#include <gfxHighLevel/textRender.hpp>
#include <external/glm/glm.hpp>

namespace GfxLowLevel
{
    bool deinitialise_device();
}

namespace RenderHi
{    
//------------------------------------------------------------------------------
// forward declarations

    // data
    TextRender *textRenderer;
    mat4	projectionMatrix;
    GfxLowLevel::VertexBufferDynamic *vertexBufferTemp;
    GfxLowLevel::TextureManager *textureManager;
    //GfxLowLevel::ShaderManager *shaderManager;
    // functions
    /*
    void glFrustumf(float near, float far){
        float aspectRatio = .5;
        float DEG2RAD = 3.14159f / 180.0f;
        float fov = 90 * DEG2RAD;
        float h = cosf(0.5f*fov) / sinf(0.5f*fov);
        float w = h * aspectRatio;
        float a = -(near + far) / (near - far);
        float b = -((2 * far*near) / (far - near));

        float proj[16] = {
            w, 0, 0, 0,
            0, h, 0, 0,
            0, 0, a, 1,
            0, 0, b, 0
        };
*/

    mat4 makeProjectionMatrix_ortho(f32 left, f32 right, f32 bottom, f32 top, f32 zNear = -1.f, f32 zFar = 1.f)
    {
        mat4 m(Unit::UNIT);
        m.v[0][0] = 2.f / (right - left);
        m.v[1][1] = 2.f / (top - bottom);
        m.v[2][2] = -2.f / (zFar - zNear);        
        m.v[3][0] = -(right + left) / (right - left);
        m.v[3][1] = -(top + bottom) / (top - bottom);
        m.v[3][2] = -(zFar + zNear) / (zFar - zNear);


        m.v[2][2] *= -1.f;  // left hand coord system fix (z=-z)
        m.v[3][2] *= -1.f;  // left hand coord system fix (z=-z)
                        
        return m;
    }
    
    void initialise()
    {
        debugLog << L"RenderHi::initialise" << "\n";
        GfxLowLevel::Shaders::initialise();
        textureManager = new GfxLowLevel::TextureManager();
#ifdef USE_OPENGL
        textureManager->loadNewTexture(L"data/", L"font.png");
        textureManager->loadNewTexture(L"data/", L"t0.png");
#endif 

#ifdef USE_DIRECT3D
        textureManager->loadNewTexture(L"data/", L"font.dds");
        textureManager->loadNewTexture(L"data/", L"seafloor.dds");

        vertexBufferTemp = new GfxLowLevel::VertexBufferDynamic(
            GfxLowLevel::VertexType::posColTex,
            GfxLowLevel::PrimativeType::triangle,
            GfxLowLevel::Shaders::posColText,
            textureManager->getTextureByName(L"font")
            );
#endif // USE_DIRECT3D

#ifdef USE_OPENGL


      //shaderManager = new GfxLowLevel::ShaderManager();  

        textRenderer = new TextRender(textureManager->getTextureByName(L"font"));

        vertexBufferTemp = new GfxLowLevel::VertexBufferDynamic(
            GfxLowLevel::VertexType::posColTex,
            GfxLowLevel::PrimativeType::triangle,
            GfxLowLevel::Shaders::posColText,
            textureManager->getTextureByName(L"font")
            );

#endif 
        Gapp.gfxInitialised = true;
    }

    void deinitialise()
    {
        //textures.clear();
        /*for each (auto &i in textures)
        {
            delete i;
        }
*/

        debugLog << L"RenderHi::deinitialise" << "\n";

        GfxLowLevel::Shaders::deinitialise();
        delete textureManager; // will delete all textures;
        GfxLowLevel::deinitialise_device();
    }

    mat4 makeProjectionMatrix_perspective( )
    {        
            mat4 m(Zero::ZERO);
            //m[0][0]=
            triggerBreakpoint();// todo: finish
            return m;
    }

    void drawAll()
    {
        GfxLowLevel::onGfxDeviceErrorTriggerBreakpoint();
        
        GfxLowLevel::drawBegin();
#ifdef USE_OPENGL
        //glm::
        projectionMatrix = mat4(Unit::UNIT);
        projectionMatrix = makeProjectionMatrix_ortho(
            0.f, 1.f, 1.f, -0.f, -1.f, 1.f);

        GfxLowLevel::setUniform_projectionMatrix(&projectionMatrix);
        GfxLowLevel::onGfxDeviceErrorTriggerBreakpoint();

        // draw stuff here
        std::wstring tex;
        tex += L"0hellome!\n";
        tex += std::to_wstring(Gapp.frameCounter);
        tex += L"\n";
        tex += L"2denboofme!\n";
        textRenderer->drawText(tex);

        GfxLowLevel::onGfxDeviceErrorTriggerBreakpoint();

        
        #define rn (((float)rand() / (float)RAND_MAX))
        std::vector<Triangle_pct> tris;
        srand(0);
        
        for (int i = 0; i < 10; i++)
        {
        Triangle_pct tri
        {
        Vert_pct( vec3(rn, rn, 0.5f), vec3(0.8f, 0.0f, 0.0f), vec2(0.0f, 0.7f) ) ,
        Vert_pct( vec3(rn, rn, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f) ) ,
        Vert_pct( vec3(rn, rn, 0.5f), vec3(0.33f, 0.34f, 0.35f), vec2( 0.36f, 0.37f) )
        };
        tris.push_back(tri);
        }
        vertexBufferTemp->draw(tris.data(), tris.size());
        
        GfxLowLevel::onGfxDeviceErrorTriggerBreakpoint();
#endif
        GfxLowLevel::drawEnd();
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
