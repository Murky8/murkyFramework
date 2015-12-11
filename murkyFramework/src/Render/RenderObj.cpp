//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/src/pch.hpp>

#define deviceObj  g_appDebug->render->gfxDevice

namespace murkyFramework {
        namespace GfxDevice
        {
            // forward declarations
            TextureWrapper		createTextureObjectFromFile(const std::wstring &dirName,
                const               std::wstring &fileName, const std::wstring &extensionName);
            TextureWrapper		createTestTextureObject();
            void                initilise_textureSystem();
            void                deinitilise_textureSystem();
        }
        extern std::vector<Triangle_pct> gdeb_tris;// todo: remove
    namespace Render {

        std::vector<Line_pc> defaultLines;
        RenderObj::RenderObj(GfxDevice::GfxDeviceObj_initStruct  *const initStruct)
        {
            g_appDebug->render = this; // warning: see g_aapDebug usage notes: for development only, remove!
            gfxDevice = new GfxDevice::GfxDeviceObj(initStruct);

            debugLog << L"RenderHi::initialise" << "\n";

            deviceObj->vertexBufferManager.add(L"tris",
                GfxDevice::VertexBufferWrapper(
                    GfxDevice::VertexType::posColTex,
                    GfxDevice::PrimativeType::triangle,
                    deviceObj->shaderManager.get(L"posColTex"),
                    deviceObj->textureManager.get(L"font 4c"), 1024));

            deviceObj->vertexBufferManager.add(L"lines",
                GfxDevice::VertexBufferWrapper(
                    GfxDevice::VertexType::posCol,
                    GfxDevice::PrimativeType::line,
                    deviceObj->shaderManager.get(L"posCol"),
                    deviceObj->textureManager.get(L"font 4c"), 16 * 1024));

            textRenderer = new TextRender(deviceObj->textureManager.get(L"font 4c"));
        }

        RenderObj::~RenderObj()
        {
            debugLog << L"RenderHi::deinitialise" << "\n";
            GfxDevice::Shaders::deinitialise();
            delete textRenderer;
            delete gfxDevice;
        }

        // methods

        void RenderObj::drawAll()
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

            mat4  projectionMatrix;
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
                        drawCrosshair(vec3(t.v[0].pos.x, t.v[0].pos.y, -t.v[0].pos.z), vec3(1, 0, 0), 1.f, defaultLines);

                    }
                    deviceObj->vertexBufferManager.get(L"lines").draw(
                        defaultLines.data(), 
                        defaultLines.size());
                }
            }
            // teapot

            g_appDebug->render->gfxDevice->drawEnd();
        }     
    }//namespace Render
}//namespace murkyFramework

