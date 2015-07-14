//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/version.hpp>

#include <vector>
#include <codecvt>

#include <murkyFramework/include/types.hpp>
#include <murkyFramework/include/debugUtils.hpp>
#include <murkyFramework/include/loadSaveFile.hpp>
#include <murkyFramework/include/randomNumbers.hpp>
#include <murkyFramework/include/system.hpp>
#include <murkyFramework/include/fileUtils.hpp>
#include <murkyFramework/include/vectorMatrix.hpp>
#include <murkyFramework/include/appFramework.hpp>
#include <murkyFramework/include/gfxLowLevel/gfxPrimativeTypes.hpp>
#include <murkyFramework/include/gfxLowLevel/gfxLowLevel.hpp>
#include <murkyFramework/include/gfxLowLevel/texture.hpp>
#include <murkyFramework/include/gfxLowLevel/shaders.hpp>
#include <murkyFramework/include/gfxLowLevel/vertexBuffer.hpp>
#include <murkyFramework/include/gfxHighLevel/render.hpp>
#include <murkyFramework/include/gfxHighLevel/textRender.hpp>
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
    TextRender      *textRenderer;
    mat4	        projectionMatrix;
    GfxLowLevel::VertexBufferDynamic *vertexBufferTemp;
    GfxLowLevel::TextureManager *textureManager;
    
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
        textureManager->loadNewTexture(L"data/", L"t0.dds");
#endif // USE_DIRECT3D

        vertexBufferTemp = new GfxLowLevel::VertexBufferDynamic(
            GfxLowLevel::VertexType::posColTex,
            GfxLowLevel::PrimativeType::triangle,
            GfxLowLevel::Shaders::posColText,
            textureManager->getTextureByName(L"t0"),
            1024 );

//#ifdef USE_OPENGL             
        textRenderer = new TextRender(textureManager->getTextureByName(L"font"));  
//#endif 
        Gapp.gfxInitialised = true;
    }

    void deinitialise()
    {        
        debugLog << L"RenderHi::deinitialise" << "\n";

        GfxLowLevel::Shaders::deinitialise();
        delete textRenderer;
        delete textureManager; // will delete all textures;
        delete vertexBufferTemp;

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
        GfxLowLevel::drawBegin();
        //glm::
        projectionMatrix = mat4(Unit::UNIT);
        projectionMatrix = makeProjectionMatrix_ortho(
            0.f, 1.f, 1.f, -0.f, -1.f, 1.f);

        GfxLowLevel::setUniform_projectionMatrix(&projectionMatrix);

        // draw stuff here
        std::wstring tex;
        tex += L"0hellome!\n";
        tex += std::to_wstring(Gapp.frameCounter);
        tex += L"\n";
        tex += L"2denboofme!\n";
        textRenderer->drawText(tex);
              
        #define rn (((float)rand() / (float)RAND_MAX))
        std::vector<Triangle_pct> tris;
        srand(0);
        
        for (int i = 0; i < 10; i++)
        {
        Triangle_pct tri
        {
        Vert_pct( vec3(rn, rn, 0.9f), vec3(1.0f, 1.0f, 1.0f), vec2(0.0f, 1.0f) ) ,
        Vert_pct( vec3(rn, rn, 0.9f), vec3(1.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f) ) ,
        Vert_pct( vec3(rn, rn, 0.9f), vec3(1.0f, 1.0f, 1.0f), vec2( 1.0f, 0.0f) )
        };
        tris.push_back(tri);
        }
        vertexBufferTemp->draw(tris.data(), tris.size());
        
     
        GfxLowLevel::drawEnd();
    }

    void addQuad_pct(std::vector<Triangle_pct> &tris, const Vert_pct v[4])
    {
        tris.push_back({ v[0], v[1], v[2] });
        tris.push_back({ v[1], v[3], v[2] });
    }
}
