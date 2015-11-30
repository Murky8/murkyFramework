//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/src/private/pch.hpp>

namespace GfxDevice
{
    // forward declarations
    TextureWrapper		createTextureObjectFromFile(const std::wstring &dirName,
    const				std::wstring &fileName, const std::wstring &extensionName);
    TextureWrapper		createTestTextureObject();    
    bool                deinitialise_device();    
    void                initilise_textureSystem();
    void                deinitilise_textureSystem();
    mat4		        makeProjectionMatrix_perspective(float x, float x1, float x2, float x3);	        
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
    std::vector<Line_pc>               defaultLines;    
    
    bool initialise(SystemSpecific* systemSpecific)
    {		
        debugLog << L"RenderHi::initialise" << "\n";				

#ifdef USE_DIRECT3D11
        GfxDevice::initialise_device(systemSpecific);
#endif USE_DIRECT3D11

#ifdef USE_DIRECT3D12
        GfxDevice::initialise_device(systemSpecific);
#endif USE_DIRECT3D12


#ifdef USE_DIRECT3D12
        
        GfxDevice::vertexBufferManager.add(L"tris",
            GfxDevice::VertexBufferWrapper(
                GfxDevice::VertexType::posColTex,
                GfxDevice::PrimativeType::triangle,
                //GfxDevice::shaderManager.get(std::wstring(L"posColTex")),
                //newt, 1024));
                GfxDevice::ShaderWrapper(),
                GfxDevice::textureManager.get(L"t0 4c"), 6));
        
        GfxDevice::vertexBufferManager.add(L"lines",
            GfxDevice::VertexBufferWrapper(
                GfxDevice::VertexType::posCol,
                GfxDevice::PrimativeType::line,
                GfxDevice::shaderManager.get(L"posCol"),
                GfxDevice::TextureWrapper(), 16 * 1024));

        textRenderer = new TextRender(GfxDevice::textureManager.get(L"font 4c"));

#else

        GfxDevice::Shaders::initialise();        
        
        GfxDevice::TextureWrapper newt = GfxDevice::createTextureObjectFromFile(
            L"data", L"font", L"png");
        GfxDevice::textureManager.add(L"font 4c", newt);
        
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
            GfxDevice::VertexType::posCol,
            GfxDevice::PrimativeType::line,
            GfxDevice::shaderManager.get(L"posCol"),
            newt2, 16*1024));
        
        textRenderer = new TextRender(newt);
        
#endif          
        return true;
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

    void drawAll()
    {
        g_appDebug->render->gfxDevice->drawBegin();

        mat4 cam = makeCameraMatrix(g_appDebug->game->cursorPos, g_appDebug->game->cursorOri);
        mat4 persp = Render::makeProjectionMatrix_perspective(1.74f, 0.1f, 1000.f, 1.f);
        mat4 proj = cam*persp;

        g_appDebug->render->gfxDevice->setUniform_projectionMatrix(&proj.v[0][0]);

        defaultLines.clear();
        // draw onscreen stuff
        if (murkyFramework::done == false)
            debugLogScreen << L"Loading teapot!!!\n";
        debugLogScreen << g_appDebug->game->cursorPos << L"\n";

        projectionMatrix = makeProjectionMatrix_ortho(
            0.f, 1.f, 1.f, 0.f, -1.f, 1.f);

        g_appDebug->render->gfxDevice->setUniform_projectionMatrix(&projectionMatrix.v[0][0]);

        textRenderer->drawText(debugLogScreen);
        // draw onscreen stuff        

        if (murkyFramework::done == true)
        {
        // teapot
        mat4 cam = makeCameraMatrix(g_appDebug->game->cursorPos, g_appDebug->game->cursorOri);
        mat4 persp = Render::makeProjectionMatrix_perspective(1.74f, 0.1f, 1000.f, 1.f);
        mat4 proj = cam*persp;

        g_appDebug->render->gfxDevice->setUniform_projectionMatrix(&proj.v[0][0]);

            if (true)
            {
                for (Triangle_pct &t : gdeb_tris)
                {
                    drawCrosshair(vec3(t.v[0].pos.x, t.v[0].pos.y, -t.v[0].pos.z), vec3(1, 0, 0), 1.f);
                }
                GfxDevice::vertexBufferManager.get(L"lines").draw(defaultLines.data(), defaultLines.size());
            }
        }
        // teapot
        
        g_appDebug->render->gfxDevice->drawEnd();        

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