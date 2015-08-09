//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/version.hpp>
#include <murkyFramework/include/gfxLowLevel/version_gfxDevice.hpp>

#include <windows.h>
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
#include <murkyFramework/include/state.hpp>
#include <murkyFramework/include/gfxLowLevel/gfxPrimativeTypes.hpp>
#include <murkyFramework/include/gfxLowLevel/gfxLowLevel.hpp>
#include <murkyFramework/include/gfxLowLevel/texture.hpp>
#include <murkyFramework/include/gfxLowLevel/shaders.hpp>
#include <murkyFramework/include/gfxLowLevel/vertexBuffer.hpp>
#include <murkyFramework/include/gfxHighLevel/render.hpp>
#include <murkyFramework/include/gfxHighLevel/textRender.hpp>
#define GLM_FORCE_RADIANS
#include <external/glm/glm.hpp>
#include "glm/gtc/matrix_transform.inl"
#include "../../include/gfxHighLevel/linesShapes.hpp"
#include "../../include/gfxHighLevel/projectionMat.hpp"

namespace GfxLowLevel
{
    // forward declarations
    TextureId   createTextureObjectFromFile(const std::wstring &dirName,
    const       std::wstring &fileName, const std::wstring &extensionName);
    TextureId   createTestTextureObject();
    bool        initialise_device(HDC &hDC, HGLRC &hRC, HWND &hWnd);    
    bool        deinitialise_device();    
    void        initilise_textureSystem();
    void        deinitilise_textureSystem();
}

extern std::vector<Vert_pct> gdeb_verts;
extern std::vector<Triangle_pct> gdeb_tris;

namespace RenderHi
{        
    // data    
    GfxLowLevel::VertexBufferDynamic    *vertexBufferTemp;  // for testing
    GfxLowLevel::VertexBufferDynamic    *defaultLineVB;          
    TextRender                          *textRenderer;
    mat4	                            projectionMatrix;
    GfxLowLevel::TextureManager         *textureManager;
    // forward declarations
    std::vector<Line_pct>               defaultLines;
    
    
    void initialise(HDC &hDC, HGLRC &hRC, HWND &hWnd)
    {
        debugLog << L"RenderHi::initialise" << "\n";
        GfxLowLevel::initialise_device(hDC, hRC, hWnd);
        GfxLowLevel::Shaders::initialise();
        GfxLowLevel::initilise_textureSystem();
        textureManager = new GfxLowLevel::TextureManager();
        
        GfxLowLevel::TextureId newt = GfxLowLevel::createTextureObjectFromFile(
            L"data", L"font", L"png");

        GfxLowLevel::TextureId newt2 = GfxLowLevel::createTestTextureObject();

        GfxLowLevel::TextureId newt3 = GfxLowLevel::createTextureObjectFromFile(
            L"data", L"t0", L"png");

        vertexBufferTemp = new GfxLowLevel::VertexBufferDynamic(
            GfxLowLevel::VertexType::posColTex,
            GfxLowLevel::PrimativeType::triangle,
            GfxLowLevel::Shaders::posColText,
            //textureManager->getTextureByName(L"gtex"),
            std::move(newt2),
            1024 );

        defaultLineVB = new GfxLowLevel::VertexBufferDynamic(
            GfxLowLevel::VertexType::posColTex,
            GfxLowLevel::PrimativeType::line,
            GfxLowLevel::Shaders::posColText,
            std::move(newt3),            
            16*1024);

        //textRenderer = new TextRender(textureManager->getTextureByName(L"font"));
        textRenderer = new TextRender(std::move(newt));

        Gapp.gfxInitialised = true;
    }

    void deinitialise()
    {        
        debugLog << L"RenderHi::deinitialise" << "\n";
        GfxLowLevel::deinitilise_textureSystem();
        GfxLowLevel::Shaders::deinitialise();
        delete textRenderer;
        delete textureManager; // will delete all textures?;
        delete vertexBufferTemp;

        GfxLowLevel::deinitialise_device();
    }

    mat4 makeProjectionMatrix_perspective( )
    {        
            mat4 m(zero);
            //m[0][0]=
            triggerBreakpoint();// todo: finish
            return m;
    }

    void drawAll(State &state)
    {                
        GfxLowLevel::drawBegin();
        //glm::
        defaultLines.clear();
        debugLogScreen << state.cursor << L"\n";
        //projectionMatrix = mat4(unit);
        
        projectionMatrix = makeProjectionMatrix_ortho(
            0.f, 1.f, 1.f, 0.f, -1.f, 1.f);
        GfxLowLevel::setUniform_projectionMatrix(&projectionMatrix);
        textRenderer->drawText(debugLogScreen);        

        //projectionMatrix = makeProjectionMatrix_ortho(
          //  -200.f, 200.f, -200.f, 200.f, -200.f, 200.f);
        
        projectionMatrix = makeProjectionMatrix(1.8f, 5.f, 500.f, 1.f);

        if (0)
        if (1)
        {
            projectionMatrix.v[3][0] = state.cursor.x;
            projectionMatrix.v[3][1] = state.cursor.y;
            projectionMatrix.v[3][2] = state.cursor.z;
        }
        else
        {
            projectionMatrix.v[0][3] = state.cursor.x;
            projectionMatrix.v[1][3] = state.cursor.y;
            projectionMatrix.v[2][3] = state.cursor.z;
        }

        if (0)
        {
            glm::mat4 Perspective = glm::perspective(1.8f, 1.f, 5.f, 500.f);
            glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(
                state.cursor.x, state.cursor.y, state.cursor.z));

            glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, -1.0f));
            glm::mat4 proj = Perspective*scale*trans;

            for (int j = 0; j < 4; j++)
                for (int i = 0; i < 4; i++)
                    projectionMatrix.v[j][i] = proj[j][i];
        }

        GfxLowLevel::setUniform_projectionMatrix(&projectionMatrix);


        // draw stuff here
        
        if (0)
        {
#define rn (((float)rand() / (float)RAND_MAX))
            std::vector<Triangle_pct> tris;
            srand(0);

            for (int i = 0; i < 10; i++)
            {
                Triangle_pct tri
                {
                    Vert_pct(vec3(rn, rn, 0.9f), vec3(1.0f, 1.0f, 1.0f), vec2(0.0f, 1.0f)),
                    Vert_pct(vec3(rn, rn, 0.9f), vec3(1.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f)),
                    Vert_pct(vec3(rn, rn, 0.9f), vec3(1.0f, 1.0f, 1.0f), vec2(1.0f, 0.0f))
                };
                tris.push_back(tri);
            }
            vertexBufferTemp->draw(tris.data(), tris.size());
        }
        if (0)
        {
            std::vector<Line_pct> lines;
            for (int i = 0; i < 10; i++)
            {
                Line_pct line
                {
                    Vert_pct(vec3(rn, rn, 0.9f), vec3(1.0f, 1.0f, 1.0f), vec2(0.0f, 1.0f)),
                    Vert_pct(vec3(rn, rn, 0.9f), vec3(1.0f, 1.0f, 1.0f), vec2(0.0f, 1.0f)),
                };
                lines.push_back(line);
            }
            defaultLineVB->draw(lines.data(), lines.size());
        }        
     
        if (1)
        {
            for (Vert_pct &v : gdeb_verts)
            {
                drawCrosshair(vec3(v.pos.x, v.pos.y, -v.pos.z), vec3(1, 1, 1), 1.f);
            }
            defaultLineVB->draw(defaultLines.data(), defaultLines.size());
        }

        //defaultLineVB->draw(gdeb_tris.data(), gdeb_tris.size());

        GfxLowLevel::drawEnd();
    }

    void addQuad_pct(std::vector<Triangle_pct> &tris, const Vert_pct v[4])
    {
        tris.push_back({ v[0], v[1], v[2] });
        tris.push_back({ v[1], v[3], v[2] });
    }
}
