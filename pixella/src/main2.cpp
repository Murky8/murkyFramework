#include <murkyFramework/src/pch.hpp>

namespace murkyFramework
{
    //void thread_mainGfx()
    class myGame : public Game
    {
    };
    
    class Moof
    {
    public:
        s32 x, y;
        s32 vx = 1;
        s32 vy = 1;
        Moof(s32 x, s32 y, s32 vx, s32 vy) : x(x), y(y), vx(vx), vy(vy)
        {}
    };

    boost::multi_array<u32, 2> moofLand(boost::extents[256][256]);
    std::vector<Moof> moofs;

    const s32 dimX = 256;
    const s32 dimY = 256;

    u32 getMoofLandPixel(s32 x, s32 y)
    {
        return moofLand[y & 255][x & 255];
    }
    void setMoofLandPixel(s32 x, s32 y, u32 val)
    {
        moofLand[y & 255][x & 255] = val;
    }

                        
    void  main_noGfx()
    {           
    }


    void  main_gfx()
    {
        g_appDebug->render->gfxDevice->setCurrentSlot(1);

        // erase old moof image
        for(Moof &moof : moofs)
        {            
            setMoofLandPixel(moof.x, moof.y, 0x8f);
        }
        // erase old moof image

        // Do Moofs!
        // bounce 
        for (Moof &moof : moofs)
        {
            
           /* if (getMoofLandPixel(moof.x + moof.vx, moof.y + moof.vy) != 0)
            {
                moof.vx *= -1;
                moof.vy *= -1;
            }
            else*/
            {
                bool hitX=false, hitY=false;
                if (getMoofLandPixel(moof.x, moof.y + moof.vy) == 0xffffff)
                {
                    moof.vy *= -1;
                    hitY = true;
                }

                if (getMoofLandPixel(moof.x + moof.vx, moof.y) == 0xffffff)
                {
                    moof.vx *= -1;
                    hitX = true;
                }

                if((hitX==false)&&(hitY==false))
                    if (getMoofLandPixel(moof.x + moof.vx, moof.y+moof.vy) == 0xffffff)
                    {
                        moof.vx *= -1;
                        moof.vy *= -1;
                    }
            }
            



            moof.x += moof.vx;
            moof.y += moof.vy;

        }
        // Do Moofs!

        // draw  moof image
        for (Moof &moof : moofs)
        {
            setMoofLandPixel(moof.x, moof.y, 0xffffff);
        }
        // erase moof image


    // upload moofland
    g_appDebug->render->gfxDevice->uploadDynamicTexture(moofLand.data());

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
    // upload moofland


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

    // moof land genesis
    for (int i = 0;i < 100;i++)
    {
        // draw a horz line
        u32 start = qmaths::randInt(0, 255);
        u32 finish = start +qmaths::randInt(0, 40);        
        u32 y = qmaths::randInt(0, 255);
        for (int l = start; l <= finish;++l)
        {
            moofLand[y&255][l&255] = 0xffffff;
        }
        // draw a horz line
    }
    // moof land genesis

    

    for (int i = 0;i < 10;i++)
    {
        Moof moof = { 
            qmaths::randInt(0,255),
            qmaths::randInt(0,255),
            (qmaths::randInt(0,255)==1)? 1 : -1,
            (qmaths::randInt(0,255) == 1) ? 1 : -1
        };
        moofs.push_back(moof);
    }

    // moof land genesis
    app->run();
}