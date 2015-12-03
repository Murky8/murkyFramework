//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/src/pch.hpp>

#define deviceObj  g_appDebug->render->gfxDevice

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

    void drawAll()
    {
        deviceObj->drawBegin();

        mat4 cam = makeCameraMatrix(g_appDebug->game->cursorPos, g_appDebug->game->cursorOri);
        mat4 persp = Render::makeProjectionMatrix_perspective(1.74f, 0.1f, 1000.f, 1.f);
        mat4 proj = cam*persp;

        deviceObj->setUniform_projectionMatrix(&proj.v[0][0]);

        defaultLines.clear();
        // draw onscreen stuff
        if (murkyFramework::done == false)
            debugLogScreen << L"Loading teapot!!!\n";
        debugLogScreen << g_appDebug->game->cursorPos << L"\n";

        projectionMatrix = makeProjectionMatrix_ortho(
            0.f, 1.f, 1.f, 0.f, -1.f, 1.f);

        deviceObj->setUniform_projectionMatrix(&projectionMatrix.v[0][0]);

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
                deviceObj->vertexBufferManager.get(L"lines").draw(defaultLines.data(), defaultLines.size());
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
