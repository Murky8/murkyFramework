//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/src/pch.hpp>

namespace murkyFramework {
#define deviceObj  g_appDebug->render->gfxDevice

using namespace Render;
using namespace GfxDevice;

namespace GfxDevice
{
    // forward declarations
    TextureWrapper		createTextureObjectFromFile(const std::wstring &dirName,
    const               std::wstring &fileName, const std::wstring &extensionName);
    TextureWrapper		createTestTextureObject();
    void                initilise_textureSystem();
    void                deinitilise_textureSystem();    
}
extern std::vector<Triangle_pct> gdeb_tris;

RenderObj::RenderObj(GfxDeviceObj_initStruct  *const initStruct)
{
    g_appDebug->render = this; // warning: see g_aapDebug usage notes: for development only, remove!
    gfxDevice = new GfxDeviceObj(initStruct);
    debugLog << L"RenderHi::initialise" << "\n";

#ifndef USE_DIRECT3D12
    GfxDevice::Shaders::initialise();
#endif

    gfxDevice->loadTexturesInDir(L"data");

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

}//namespace murkyFramework

