//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/version.hpp>

#include <windows.h>
#include <vector>

#include <murkyFramework/include/debugUtils.hpp>
#include <murkyFramework/include/vectorMatrix.hpp>
#include <murkyFramework/include/appFramework.hpp>
#include <murkyFramework/include/state.hpp>
#include <murkyFramework/include/GfxDevice/gfxPrimativeTypes.hpp>
#include <murkyFramework/include/GfxDevice/gfxLowLevel.hpp>
#include <murkyFramework/include/GfxDevice/texture.hpp>
#include <murkyFramework/include/GfxDevice/shaders.hpp>
#include <murkyFramework/include/GfxDevice/vertexBuffer.hpp>
#include <murkyFramework/include/Render/render.hpp>
#include <murkyFramework/include/Render/textRender.hpp>
#define GLM_FORCE_RADIANS
#include "glm/gtc/matrix_transform.inl"
#include "murkyFramework/include/Render/linesShapes.hpp"
#include "murkyFramework/include/Render/projectionMat.hpp"

namespace GfxDevice
{
    // forward declarations
    TextureId   createTextureObjectFromFile(const std::wstring &dirName,
    const       std::wstring &fileName, const std::wstring &extensionName);
    TextureId   createTestTextureObject();
	bool        initialise_device(HDC &hDC, HGLRC &hRC, HWND &hWnd);
	bool        initialise_device24(HDC &hDC, HGLRC &hRC, HWND &hWnd);
    bool        deinitialise_device();    
    void        initilise_textureSystem();
    void        deinitilise_textureSystem();
	mat4		makeProjectionMatrix_perspective(float x, float x1, float x2, float x3);
}

extern std::vector<Vert_pct> gdeb_verts;
extern std::vector<Triangle_pct> gdeb_tris;

namespace Render
{        
    // data    
    GfxDevice::VertexBufferDynamic    *vertexBufferTemp;  // for testing
    GfxDevice::VertexBufferDynamic    *defaultLineVB;          
    TextRender                          *textRenderer;
    mat4	                            projectionMatrix;
    GfxDevice::TextureManager         *textureManager;
    // forward declarations
    std::vector<Line_pct>               defaultLines;    
    
    void initialise(HDC &hDC, HGLRC &hRC, HWND &hWnd)
    {
        debugLog << L"RenderHi::initialise" << "\n";
		
		//GfxDevice::initialise_device(hDC, hRC, hWnd);
		GfxDevice::initialise_device24(hDC, hRC, hWnd);

		exit(0);


        GfxDevice::Shaders::initialise();
        GfxDevice::initilise_textureSystem();
        textureManager = new GfxDevice::TextureManager();
        
        GfxDevice::TextureId newt = GfxDevice::createTextureObjectFromFile(
            L"data", L"font", L"png");

        GfxDevice::TextureId newt2 = GfxDevice::createTestTextureObject();

        GfxDevice::TextureId newt3 = GfxDevice::createTextureObjectFromFile(
            L"data", L"t0", L"png");

        vertexBufferTemp = new GfxDevice::VertexBufferDynamic(
            GfxDevice::VertexType::posColTex,
            GfxDevice::PrimativeType::triangle,
            GfxDevice::Shaders::posColText,
            //textureManager->getTextureByName(L"gtex"),
            std::move(newt2),
            1024 );

        defaultLineVB = new GfxDevice::VertexBufferDynamic(
            GfxDevice::VertexType::posColTex,
            GfxDevice::PrimativeType::line,
            GfxDevice::Shaders::posColText,
            std::move(newt3),            
            16*1024);

        //textRenderer = new TextRender(textureManager->getTextureByName(L"font"));
        textRenderer = new TextRender(std::move(newt));

        Gapp.gfxInitialised = true;
    }

    void deinitialise()
    {        
        debugLog << L"RenderHi::deinitialise" << "\n";
        GfxDevice::deinitilise_textureSystem();
        GfxDevice::Shaders::deinitialise();
        delete textRenderer;
        delete textureManager; // will delete all textures?;
        delete vertexBufferTemp;

        GfxDevice::deinitialise_device();
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
        GfxDevice::drawBegin();        
        defaultLines.clear();
        debugLogScreen << state.cursorPos << L"\n";

		// draw onscreen stuff
        projectionMatrix = makeProjectionMatrix_ortho(
            0.f, 1.f, 1.f, 0.f, -1.f, 1.f);
        GfxDevice::setUniform_projectionMatrix(&projectionMatrix.v[0][0]);
        textRenderer->drawText(debugLogScreen);        
		// draw onscreen stuff
		
		// teapot
	
			mat4 cam = makeCameraMatrix(state.cursorPos, state.cursorOri);
			mat4 persp = Render::makeProjectionMatrix_perspective(1.74f, 0.1f, 1000.f, 1.f);
			mat4 proj = cam*persp;
			GfxDevice::setUniform_projectionMatrix(&proj.v[0][0]);
		// teapot

			
        // draw stuff here        
/*        if (0)
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
        }  */      
     
        if (1)
        {
            for (Vert_pct &v : gdeb_verts)
            {
                drawCrosshair(vec3(v.pos.x, v.pos.y, -v.pos.z), vec3(1, 1, 1), 1.f);
            }
            defaultLineVB->draw(defaultLines.data(), defaultLines.size());
        }

        //defaultLineVB->draw(gdeb_tris.data(), gdeb_tris.size());

        GfxDevice::drawEnd();
    }

    void addQuad_pct(std::vector<Triangle_pct> &tris, const Vert_pct v[4])
    {
        tris.push_back({ v[0], v[1], v[2] });
        tris.push_back({ v[1], v[3], v[2] });
    }
}
