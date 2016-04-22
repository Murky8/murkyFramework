#include <murkyFramework/src/pch.hpp>

namespace murkyFramework
{
    using namespace qmaths;
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
        u32 col = 0x808080;
        Moof() {}
        Moof(s32 x, s32 y, s32 vx, s32 vy) : x(x), y(y), vx(vx), vy(vy)
        {}
    };

    const s32 dimX = 1024;
    const s32 maskX = dimX - 1;
    const s32 dimY = 1024;
    const s32 maskY = dimY - 1;

    boost::multi_array<u32, 2> moofLand(boost::extents[dimY][dimX]);
    std::vector<Moof> moofs;

    u32 getMoofLandPixel(s32 x, s32 y)
    {
        return moofLand[y & maskY][x & maskX];
    }
    void setMoofLandPixel(s32 x, s32 y, u32 val)
    {
        moofLand[y & maskY][x & maskX] = val;
    }

                        
    void  main_noGfx()
    {           
    }

    //alpha bits:   
    enum eType
    {
        empty,
        wall,
        moof
    };

    bool isWall(u32 pixy)
    {
        u8 alpha = pixy >> 24;
        if ((alpha & 0b11) == eType::wall)
            return true;
        else
            return false;
    }
    
    bool isMoof(u32 pixy)
    {           
        u8 alpha = pixy >> 24;
        if ((alpha & 0b11) == eType::moof)
            return true;
        else
            return false;        
    }

    bool isBounceoffable(u32 pixy)
    {
        u8 alpha = pixy >> 24;
        if ((alpha & 0b11) != eType::empty)
            return true;
        else
            return false;
    }

    void  main_gfx()
    {
        g_appDebug->render->gfxDevice->setCurrentSlot(1);

        // erase old moof image
        for (Moof &moof : moofs)
        {
            // moof poo
            //if( ::randInt(0, 999)>4)
        }
        // erase old moof image
        static s32 directionsX[] = { 0, 1, 1, 1,   0, -1, -1, -1 };
        static s32 directionsY[] = { 1, 1, 0, -1, -1, -1,  0,  1 };
        // Do Moofs!
        // bounce 
        for (Moof &moof : moofs)
        {
            if (isMoof(getMoofLandPixel(moof.x, moof.y)))
                //setMoofLandPixel(moof.x, moof.y, moof.col + (eType::empty << 24));
            setMoofLandPixel(moof.x, moof.y, 0x000000 +(eType::empty<<24));

             // moof-moof
            if (isMoof(getMoofLandPixel(moof.x + moof.vx, moof.y + moof.vy)))
            {            
                u32 a = randInt(0, 8);
                moof.vx = directionsX[a];
                moof.vy = directionsY[a];
            }

            if(std::abs(moof.vy*moof.vx)>0) //horaz==0
            {// diagonal movement                
                u32 pixl = getMoofLandPixel(moof.x, moof.y + moof.vy);
                u32 pixs = getMoofLandPixel(moof.x + moof.vx, moof.y + moof.vy);
                u32 pixr = getMoofLandPixel(moof.x + moof.vx, moof.y);

                if(isBounceoffable(pixl))
                {
                    moof.vy *= -1;                    
                }

                if(isBounceoffable(pixr))
                {
                    moof.vx *= -1;                    
                }

                // outer corner case
                if((isBounceoffable(pixl)==false) && (isBounceoffable(pixr) == false))
                if (isBounceoffable(pixs) == true)
                    {
                        if( randInt(0, 100)>50)
                            moof.vx *= -1;

                        if ( randInt(0, 100)>50)
                            moof.vy *= -1;
                    }
            }
            else
            {// ortho movement
                bool resl, ress, resr;


                if (moof.vx == 0)// vertical
                {
                u32 pixl = getMoofLandPixel(moof.x - 1, moof.y);
                u32 pixs = getMoofLandPixel(moof.x, moof.y +moof.vy);
                u32 pixr = getMoofLandPixel(moof.x + 1, moof.y);
                resl = isBounceoffable(pixl);
                ress = isBounceoffable(pixs);
                resr = isBounceoffable(pixr);

                    if(resl==true && ress==true && resr==false)
                    {
                        moof.vy = 0;
                        moof.vx = 1;
                    }

                    if (resl == false && ress == true && resr == true)
                    {
                        moof.vy = 0;
                        moof.vx = -1;
                    }

                    if (resl == false && ress == true && resr == false)
                    {
                        moof.vy *= -1;
                    }
                }
                else
                {
                    u32 pixl = getMoofLandPixel(moof.x, moof.y - 1);
                    u32 pixs = getMoofLandPixel(moof.x +moof.vx, moof.y);
                    u32 pixr = getMoofLandPixel(moof.x, moof.y +1);

                    resl = isBounceoffable(pixl);
                    ress = isBounceoffable(pixs);
                    resr = isBounceoffable(pixr);

                    if (resl == true && ress == true && resr == false)
                    {
                        moof.vy = 1;
                        moof.vx = 0;
                    }

                    if (resl == false && ress == true && resr == true)
                    {
                        moof.vy = -1;
                        moof.vx = 0;
                    }

                    if (resl == false && ress == true && resr == false)
                    {
                        moof.vx *= -1;
                    }
                }
            }

            if (moof.vx == 0 && moof.vy == 0)
            {
                u32 a = randInt(0, 8);
                moof.vx = directionsX[a];
                moof.vy = directionsY[a];
            }
            
            moof.x += moof.vx;
            moof.y += moof.vy;

            if(!isBounceoffable(getMoofLandPixel(moof.x, moof.y)))          
                setMoofLandPixel(moof.x, moof.y, moof.col + (eType::moof << 24));            
        }
        // Do Moofs!

        // draw  moof image
        for (Moof &moof : moofs)
        {
        }
        // draw moof image


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

void drawBox(s32 botlx, s32 botly, s32 toprx, s32 topry, u32 col)
{
    for (int y = botly; y < topry;++y)
        for (int x = botlx; x < toprx;++x)
            setMoofLandPixel(x, y, col);

}

void drawBox45(s32 xmin, s32 xmax, s32 ymin, s32 ymax, u32 col)
{
    //for (int y = botly; y < topry;++y)
        //for (int x = botlx; x < toprx;++x)
            //setMoofLandPixel(x, y, col);

}

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
    if(0)
    for (int i = 0;i < 1000;i++)
    {
/*
        if(1)
        {
            // draw a horz line
            u32 start =  randInt(0, maskY);
            u32 finish = start +  randInt(0, 100);
            u32 y =  randInt(0, 1023);
            for (int l = start; l <= finish;++l)
            {
                setMoofLandPixel(l, y, 0xffffff);
            }
            // draw a horz line
        }

        if(1)
        {
            // draw a horz line
            u32 start =  randInt(0, maskX);
            u32 finish = start +  randInt(0, 100);
            u32 x =  randInt(0, 1023);
            for (int l = start; l <= finish;++l)
            {
                setMoofLandPixel(x, l, 0xffffff);
            }
            // draw a horz line
        }

        {
         //didagonal
            s32 x =  randInt(0, maskY);            
            s32 y =  randInt(0, maskX);
            s32 l =  randInt(0, 300);
            s32 vy = ( randInt(0, 1) == 1) ? 1 : -1;

            for (int i = 0; i <= l;++i)
            {
                setMoofLandPixel(x, y, 0xffffff);
                x++;
                y += vy;
            }
            //didagonal            
        }
  */
        // lines
        s32 x = randInt(0, maskY);
        s32 y = randInt(0, maskX);
        s32 vx = 0;
        s32 vy = 0;
        while ((vx == 0) && (vy == 0))
        {
            vx = randInt(-1, 1);
            vy = randInt(-1, 1);
        }

        s32 l = randInt(0, 300);
        for (int i = 0; i <= l;++i)
        {
            if (getMoofLandPixel(x, y) == 0xffffff)
                i = 999999;

            setMoofLandPixel(x, y, 0xffffff);
            x += vx;
            y += vy;
        }
        // lines        
    }

    // boxes
    drawBox(0, 0, maskX, maskY, 0xffffff + (eType::wall << 24));
    for (int i = 0 ; i < 400 ; i++)
    {        
        s32 botlx;
        s32 botly;
        s32 toprx;
        s32 topry;

        if (randInt(0, 150) > 80)
        {   // walls
            s32 w = randInt(4, 30);
            s32 h = 34 -w;

            botlx = randInt(0, 1000);
            toprx = botlx + w;

            botly = randInt(0, 1000);
            topry = botly + h;
            drawBox(botlx, botly, toprx, topry, 0x909090+(eType::wall<<24));

        }
        else
        {   // space
            s32 w = randInt(4, 200);
            s32 h = 204 - w;

            botlx = randInt(0, 1000);
            toprx = botlx + w;

            botly = randInt(0, 1000);
            topry = botly + h;
            drawBox(botlx, botly, toprx, topry, 0x000000 + (eType::empty << 24));
        }

        
        /*botlx += 2;
        botly += 2;
        toprx -= 2;
        topry -= 2;

        drawBox(botlx, botly, toprx, topry, 0);*/
    }
    // boxes

    // moof land genesis

    
    for (int i = 0;i < 10000;i++)
    {
        Moof moof = { 
             randInt(0,100),
             randInt(0,100),
             randInt(-1,1),
             randInt(-1,1)
        };
        
        /*Moof moof = {
            i*2, i*2, 1, 1
        };
*/
        moof.col = randInt(0, 0xffffff);
        moofs.push_back(moof);
    }

    // moof land genesis
    app->run();
}