//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/src/pch.hpp>

#define deviceObj  g_appDebug->render->gfxDevice

namespace murkyFramework {
        // forward declarations
        namespace GfxDevice
        {
            TextureWrapper		createTextureObjectFromFile(const std::wstring &dirName,
                const               std::wstring &fileName, const std::wstring &extensionName);
            TextureWrapper		createTestTextureObject();
        }        
    namespace Render {
                
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
                    deviceObj->textureManager.get(L"t0 4c"), 16*1024));

            deviceObj->vertexBufferManager.add(L"lines",
                GfxDevice::VertexBufferWrapper(
                    GfxDevice::VertexType::posCol,
                    GfxDevice::PrimativeType::line,
                    deviceObj->shaderManager.get(L"posCol"),
                    deviceObj->textureManager.get(L"font 4c"), 16 * 1024));

            textRenderer = new TextRender(deviceObj->textureManager.get(L"font 4c"));

            vibuffer = new GfxDevice::VertexIndexBufferWrapper(
                GfxDevice::VertexType::posColTex,
                GfxDevice::PrimativeType::triangle,
                deviceObj->shaderManager.get(L"posColTex"),
                deviceObj->textureManager.get(L"manta"), 
                16*1024, 64*1024 
                );                
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
            defaultLines.clear();

            // set up ALL proj matrixies before begin
            mat4 cam = makeCameraMatrix(cameraPosOri);
            mat4 persp = Render::makeProjectionMatrix_perspective(1.74f, 0.1f, 1000.f, 1.f);
            mat4 proj = cam*persp;

            deviceObj->setUniform_projectionMatrix(&proj.v[0][0], 0);

            mat4  projectionMatrix;
            projectionMatrix = makeProjectionMatrix_ortho(
                0.f, 1.f, 1.f, 0.f, -1.f, 1.f);

            deviceObj->setUniform_projectionMatrix(&projectionMatrix.v[0][0], 1);
            // set up ALL proj matrixies before begin

            deviceObj->drawBegin();

            // draw onscreen stuff
            
            //debugLogScreen << g_appDebug->game->cursorPosOri << L"\n";

            deviceObj->setCurrentSlot(1);
            textRenderer->drawText(debugLogScreen);
            // draw onscreen stuff ^
            deviceObj->setCurrentSlot(0);

            drawCrosshair(vec3(0, 0, 0), vec3(1, 0, 0), 1.0f, defaultLines);
                        
            g_appDebug->main_gfx();

            deviceObj->vertexBufferManager.get(L"lines").draw(defaultLines.data(), defaultLines.size());

            //Model& model = modelManager.get(L"manta");
            //vibuffer->draw(
            //    model.vertices.data(), model.vertices.size(),
            //    model.indicies.data(), model.indicies.size() / 3);
            
            g_appDebug->render->gfxDevice->drawEnd();
        }   

        void RenderObj::loadResources()
        {
        /*    Model &newModel = modelManager.getNew(L"teamod");

            loadFBX(L"murkyFramework/data/tea.FBX", newModel.vertices, newModel.indicies);
            newModel.texture = this->gfxDevice->textureManager.get(L"t0 4c");*/
        
            std::wstring dir = { L"Project1/data/models" };
            FileDirectoryWalker fileWalker(dir, L"\\.FBX$");

            while (fileWalker.findNext())
            {
                std::wstring fileName;
                fileName = dir + L"/" + (fileWalker.findData.cFileName);
                FilePathSplit pathSplit(fileName);

                std::wstring binPath = makePathString(pathSplit.directoryPath, pathSplit.fileName, L"bin");

                if (fileNeedsCompiling(pathSplit))
                {// compile to .bin
                    debugLog << L"bin is not current. compiling \n";
                    std::vector<Vert_pct> verts;
                    std::vector<u16> indices;
                    loadFBX(pathSplit.getJoinedFilePath(), verts, indices);
                    serializeTris(binPath, verts, indices);
                }
                else
                {
                    debugLog << L"bin is current \n";
                }
                Model &newModel = modelManager.getNew(pathSplit.fileName);
                deserializeTris(binPath, newModel.vertices, newModel.indicies);
                newModel.texture = this->gfxDevice->textureManager.get(L"manta");             
            }        
        }



    }//namespace Render
}//namespace murkyFramework

