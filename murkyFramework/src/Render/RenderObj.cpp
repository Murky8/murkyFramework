//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/src/pch.hpp>

#define deviceObj  g_appDebug->render->gfxDevice

using namespace Render;
using namespace GfxDevice;

namespace GfxDevice
{
    // forward declarations
    TextureWrapper		createTextureObjectFromFile(const std::wstring &dirName,
        const				std::wstring &fileName, const std::wstring &extensionName);
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

#ifdef USE_DIRECT3D12

    deviceObj->vertexBufferManager.add(L"tris",
        GfxDevice::VertexBufferWrapper(
            GfxDevice::VertexType::posColTex,
            GfxDevice::PrimativeType::triangle,
            //GfxDevice::shaderManager.get(std::wstring(L"posColTex")),
            //newt, 1024));
            GfxDevice::ShaderWrapper(),
            deviceObj->textureManager.get(L"t0 4c"), 6));

    deviceObj->vertexBufferManager.add(L"lines",
        GfxDevice::VertexBufferWrapper(
            GfxDevice::VertexType::posCol,
            GfxDevice::PrimativeType::line,
            deviceObj->shaderManager.get(L"posCol"),
            GfxDevice::TextureWrapper(), 16 * 1024));

    textRenderer = new TextRender(deviceObj->textureManager.get(L"font 4c"));

#else

    GfxDevice::Shaders::initialise();

    GfxDevice::TextureWrapper newt = GfxDevice::createTextureObjectFromFile(
        L"data", L"font", L"png");
    deviceObj->textureManager.add(L"font 4c", newt);

    GfxDevice::TextureWrapper newt2 = GfxDevice::createTestTextureObject();
    deviceObj->textureManager.add(L"test", newt2);

    deviceObj->vertexBufferManager.add(L"tris",
        GfxDevice::VertexBufferWrapper(
            GfxDevice::VertexType::posColTex,
            GfxDevice::PrimativeType::triangle,
            deviceObj->shaderManager.get(L"posColTex"),
            newt, 1024));

    deviceObj->vertexBufferManager.add(L"lines",
        GfxDevice::VertexBufferWrapper(
            GfxDevice::VertexType::posCol,
            GfxDevice::PrimativeType::line,
            deviceObj->shaderManager.get(L"posCol"),
            newt2, 16 * 1024));

    textRenderer = new TextRender(newt);

#endif  
}

RenderObj::~RenderObj()
{
    debugLog << L"RenderHi::deinitialise" << "\n";
    GfxDevice::Shaders::deinitialise();
    delete textRenderer;
    delete gfxDevice;
}

