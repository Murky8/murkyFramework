#include <murkyFramework/src/pch.hpp>

namespace murkyFramework
{
    //void thread_mainGfx()
    class myGame : public Game
    {
    };
          

    void  main_noGfx()
    {           
    }


    void  main_gfx()
    {

    static auto * p = new u8[256 * 256 * 4];
    for (int i = 0;i < 256 * 256 * 4;++i)
        p[i] = i;
    g_appDebug->render->gfxDevice->uploadDynamicTexture(p);

    Vert_pct quad[4] =
    {
        { vec3{ 0.f, 0.f, 0.f }, vec3{ 1.f, 1.f, 1.f }, vec2{ 0.f, 0.f } },
        { vec3{ 0.f, 1.f, 0.f }, vec3{ 1.f, 1.f, 1.f }, vec2{ 0.f, 1.f } },
        { vec3{ 1.f, 0.f, 0.f }, vec3{ 1.f, 1.f, 1.f }, vec2{ 1.f, 0.f } },
        { vec3{ 1.f, 1.f, 0.f }, vec3{ 1.f, 1.f, 1.f }, vec2{ 1.f, 1.f } }
    };

    Vert_pct triVerts[6] =
    { quad[0], quad[1], quad[2], quad[1], quad[3], quad[2] };

    g_appDebug->render->gfxDevice->vertexBufferManager.get(L"dynamicTextureQuad")
        .draw(triVerts, 2);
    };
}// namespace murkyFramework

using namespace murkyFramework;

int main() // can't be in a namespace :(
{

    AppFramework_initStruct appInit;
    appInit.main_noGfx = main_noGfx;
    appInit.main_gfx = main_gfx;
    appInit.game = new myGame();

    AppFramework *const app = new AppFramework(appInit);

    g_appDebug->render->gfxDevice->vertexBufferManager.add(L"dynamicTextureQuad",
        GfxDevice::VertexBufferWrapper(
            GfxDevice::VertexType::posColTex,
            GfxDevice::PrimativeType::triangle,
            g_appDebug->render->gfxDevice->shaderManager.get(L"posColTex"),
            g_appDebug->render->gfxDevice->textureManager.get(L"dynamic"), 16));

    app->run();
}