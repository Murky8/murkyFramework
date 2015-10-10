//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/version.hpp>

#include <windows.h>
#include <vector>
#include <murkyFramework/include/GfxDevice/version_gfxDevice.hpp>
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
#include <glm/gtc/matrix_transform.inl>
#include <murkyFramework/include/Render/linesShapes.hpp>
#include <murkyFramework/include/Render/projectionMat.hpp>
#include <murkyFramework/include/readFBX.hpp>
#include <murkyFramework/include/collectionNamed.hpp>
#include <murkyFramework/src/GfxDevice/public/gfxDevice.hpp>

namespace GfxDevice
{
	// forward declarations
    TextureWrapper   createTextureObjectFromFile(const std::wstring &dirName,
    const       std::wstring &fileName, const std::wstring &extensionName);
    TextureWrapper   createTestTextureObject();
	bool        initialise_device(HDC &hDC, HGLRC &hRC, HWND &hWnd);	
    bool        deinitialise_device();    
    void        initilise_textureSystem();
    void        deinitilise_textureSystem();
	mat4		makeProjectionMatrix_perspective(float x, float x1, float x2, float x3);	
		
}
extern std::vector<Triangle_pct> gdeb_tris;

namespace Render
{        
	using namespace murkyFramework;
    // data    
    //GfxDevice::VertexBufferWrapper    *vertexBufferTemp;  // for testing
    //GfxDevice::VertexBufferWrapper    *defaultLineVB;          
    TextRender                          *textRenderer;
    mat4	                            projectionMatrix;    
    // forward declarations
    std::vector<Line_pct>               defaultLines;    
    
    void initialise(HDC &hDC, HGLRC &hRC, HWND &hWnd)
    {		
		debugLog << L"RenderHi::initialise" << "\n";				
		GfxDevice::initialise_device(hDC, hRC, hWnd);

#ifdef USE_DIRECT3D12
		

		GfxDevice::vertexBufferManager.add(L"tris",
			GfxDevice::VertexBufferWrapper(
				GfxDevice::VertexType::posColTex,
				GfxDevice::PrimativeType::triangle,
				//GfxDevice::shaderManager.get(std::wstring(L"posColTex")),
				//newt, 1024));
				GfxDevice::ShaderWrapper(),
				GfxDevice::TextureWrapper(), 6));

		GfxDevice::shaderManager.add(L"posColTex", GfxDevice::ShaderWrapper()); // dummy object

		textRenderer = new TextRender(GfxDevice::TextureWrapper());

		Gapp->gfxInitialised = true;
		return;
		//exit(0);
#endif

        GfxDevice::Shaders::initialise();        
        
        GfxDevice::TextureWrapper newt = GfxDevice::createTextureObjectFromFile(
            L"data", L"font", L"png");
		GfxDevice::textureManager.add(L"font", newt);
        
		GfxDevice::TextureWrapper newt2 = GfxDevice::createTestTextureObject();
		GfxDevice::textureManager.add(L"test", newt2);
        
		GfxDevice::vertexBufferManager.add(L"tris", 
			GfxDevice::VertexBufferWrapper(
            GfxDevice::VertexType::posColTex,
            GfxDevice::PrimativeType::triangle,
	        GfxDevice::shaderManager.get( L"posColTex"),            
            newt, 1024 ));

		GfxDevice::vertexBufferManager.add(L"lines",
			GfxDevice::VertexBufferWrapper(
            GfxDevice::VertexType::posColTex,
            GfxDevice::PrimativeType::line,
	        GfxDevice::shaderManager.get(L"posColTex"),
            newt2, 16*1024));

        //textRenderer = new TextRender(textureManager->getTextureByName(L"font"));
        textRenderer = new TextRender(newt);

        Gapp->gfxInitialised = true;
    }

    void deinitialise()
    {        
        debugLog << L"RenderHi::deinitialise" << "\n";        
        GfxDevice::Shaders::deinitialise();
        delete textRenderer;          

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
#ifdef USE_DIRECT3D12
		GfxDevice::drawBegin();
		static Vert_pct triangleVertices[] =
		{
			{ { 0.0f, 0.0f, 0.0f },{ 1.0f, 0.0f, 0.0f },{ 0.f, 0.f } },
			{ { 0.0f, 0.5f, 0.0f },{ 0.0f, 1.0f, 0.0f },{ 0.f, 1.f } },
			{ { 0.5f, 0.0f, 0.0f },{ 0.0f, 0.0f, 1.0f },{ 1.f, 0.f } },

			{ { 0.5f, 0.5f, 0.0f },{ 1.0f, 0.0f, 0.0f },{ 1.f, 1.f } },
			{ { 0.5f, 0.0f, 0.0f },{ 0.0f, 1.0f, 0.0f },{ 1.f, 0.f } },
			{ { 0.0f, 0.5f, 0.0f },{ 0.0f, 0.0f, 1.0f },{ 0.f, 1.f } }
		};

		GfxDevice::vertexBufferManager.get(L"tris")
			.draw(reinterpret_cast<void*>(triangleVertices), 2);

		debugLogScreen << state.cursorPos << L"\n";
		textRenderer->drawText(debugLogScreen);

		GfxDevice::drawEnd();
#else
		GfxDevice::drawBegin();
		defaultLines.clear();
		// draw onscreen stuff
		if (murkyFramework::done == false)
			debugLogScreen << L"Loading teapot!!!\n";
		debugLogScreen << state.cursorPos << L"\n";


		projectionMatrix = makeProjectionMatrix_ortho(
			0.f, 1.f, 1.f, 0.f, -1.f, 1.f);
		GfxDevice::setUniform_projectionMatrix(&projectionMatrix.v[0][0]);
		textRenderer->drawText(debugLogScreen);
		// draw onscreen stuff

		if (murkyFramework::done == true)
		{
		// teapot
		mat4 cam = makeCameraMatrix(state.cursorPos, state.cursorOri);
		mat4 persp = Render::makeProjectionMatrix_perspective(1.74f, 0.1f, 1000.f, 1.f);
		mat4 proj = cam*persp;
		GfxDevice::setUniform_projectionMatrix(&proj.v[0][0]);

			if (1)
			{
				for (Triangle_pct &t : gdeb_tris)
				{
					drawCrosshair(vec3(t.v[0].pos.x, t.v[0].pos.y, -t.v[0].pos.z), vec3(1, 1, 1), 1.f);
				}
				GfxDevice::vertexBufferManager.get(L"lines").draw(defaultLines.data(), defaultLines.size());
			}
		}
		// teapot
        //defaultLineVB->draw(gdeb_tris.data(), gdeb_tris.size());
        GfxDevice::drawEnd();
#endif
    }

    void addQuad_pct(std::vector<Triangle_pct> &tris, const Vert_pct v[4])
    {
        tris.push_back({ v[0], v[1], v[2] });
        tris.push_back({ v[1], v[3], v[2] });
    }
}

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