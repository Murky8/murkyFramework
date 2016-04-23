#include <murkyFramework/src/pch.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
          
namespace murkyFramework
{
    //void thread_mainGfx()
    class myGame : public Game
    {
    };
          
    class Monk : public EntityBase
    {
    public:
        f32 desiredVel = { 0 };
        void doFrame()
        {
            f32 deltaTime = g_appDebug->lastFrameDuration;
            //if (vel.z < desiredVel)
              //  vel.z += 4.1f*deltaTime;

            //if (vel.z > desiredVel)
                //vel.z -= 4.1f*deltaTime;
        }

        Monk(const mat4& transform) : EntityBase(transform)
        {            
        }

        Monk(const mat4& transform, vec4 v, vec4 w) : EntityBase(transform,v,w)
        {            
        }
    };

    void  main_noGfx()
    {           
        EntityBase* player = *g_appDebug->game->entities.begin();
        f32 deltaTime = g_appDebug->lastFrameDuration;

        vec4 oldPosPlayer = player->transform.trans;

        int dmx;
        g_appDebug->inputDevices->consumeAllMouseDx(dmx);
        player->transform.trans += vec4(dmx*0.1f, 0, 0);

        if (g_appDebug->inputDevices->keyStatus(InputDevices::KeyCode::q))
        {
            player->vel.z += 10.1*deltaTime;
        }
        if (g_appDebug->inputDevices->keyStatus(InputDevices::KeyCode::a))
        {
            player->vel.z -= 20.1*deltaTime;
            
        }
        //app->inputDevices->consumeAllMouseDy(my);

        if (g_appDebug->inputDevices->keyStatus(InputDevices::KeyCode::p))
        {
            player->vel.x = 22.f;
        }

        if (g_appDebug->inputDevices->keyStatus(InputDevices::KeyCode::o))
        {
            player->vel.x = -22.f;
        }

        for (auto *ent : g_appDebug->game->entities)
        {
            ent->doFrame();
            // do physics
                                               
            ent->transform.trans += ent->vel*deltaTime;

            if (ent == player)
                continue;

            if (ent->transform.trans.z > player->transform.trans.z+ 100.f)
                ent->transform.trans.z -= 200.f;

            if (ent->transform.trans.z < player->transform.trans.z -100.f)
                ent->transform.trans.z += 200.f;
            //mat43 rmat = makeRotationMatrix_from3c(vec4(0.f, 0.001f, 0.f));
            //entity->transform.ruf = entity->transform.ruf*rmat;
            // do physics
        }        

        // if player reached new lane stop lat vel
        if ((int)(player->transform.trans.x/3.f) != (int)(oldPosPlayer.x/3.f))
            player->vel.x = 0;
    }

    void  main_gfx()
    {
        EntityBase* player = *g_appDebug->game->entities.begin();

        //focus on enitiyty
        //g_appDebug->render->cameraPosOri = Render::makeLookAtPosOri(
        //  ent->transform.trans +vec4(0,20, -20), ent->transform.trans);

        if (!g_appDebug->flyCameraMode)
        {

            f32 speedOffset = -player->vel.z*0.f;
            g_appDebug->render->cameraPosOri = Render::makeLookAtPosOri(
                //vec4(15, 20, -10), vec4(15, 0, 0));
                player->transform.trans + vec4(0, 20, -10.f + speedOffset), player->transform.trans + vec4(0, 0, speedOffset));
        }


        auto &lines = g_appDebug->render->defaultLines;
        //for (auto *ent : g_appDebug->game->entities)
        //{
        //    // display
        //    vec verts[4] = { vec{1, 0, 2}, vec{1, 0, -2},vec{-1, 0, -2},vec{-1, 0, 2} };

        //    for (int i = 0;i < 4;i++)
        //    {
        //        verts[i] = verts[i] * ent->transform.ruf +ent->transform.trans;
        //    }

        //    lines.push_back(Line_pc(
        //        Vert_pc((vec3)verts[0], vec3(1, 1, 1)),
        //        Vert_pc((vec3)verts[1], vec3(1, 1, 1))
        //        ));

        //    lines.push_back(Line_pc(
        //        Vert_pc((vec3)verts[1], vec3(1, 1, 1)),
        //        Vert_pc((vec3)verts[2], vec3(1, 1, 1))
        //        ));

        //    lines.push_back(Line_pc(
        //        Vert_pc((vec3)verts[2], vec3(1, 1, 1)),
        //        Vert_pc((vec3)verts[3], vec3(1, 1, 1))
        //        ));

        //    lines.push_back(Line_pc(
        //        Vert_pc((vec3)verts[3], vec3(1, 1, 1)),
        //        Vert_pc((vec3)verts[0], vec3(1, 1, 1))
        //        ));
        //}

        //void hfe_createLayerVerts(std::vector<vec4> &verts, f32 rMajor, f32 rMinor, f32 phase, f32 z)
        // make  helfire rotor
        f32 rMajor = 25.0f;
        f32 rMinor = 5.0f;
        f32 r = rMajor + rMinor;
        int nMajorSegments = 20;
        int nMinorSegments = 20;

        static f32 phase = 0.0f;
        phase += 0.001f;        

        // create minor verts
        vec basisX = { cos(phase), sin(phase), 0 };
        vec basisY = { -sin(phase), cos(phase), 0 };

        float dtheta = 3.141f*0.5f / ((f32)nMinorSegments);

        std::vector<vec4> verts;        
        
        f32 theta;
        int i;

        // right minor        
        for (theta = 0, i = 0; i < nMinorSegments; ++i)
        {
            vec offset = { r*0.5f -rMinor, r*0.5f - rMinor, 0.0f };
            f32 x = rMinor*sin(theta);
            f32 y = rMinor*cos(theta);
            
            vec vert = basisX*(x+offset.x) + basisY*(y+offset.y);
                        
            theta += dtheta;
            verts.push_back(vert);
        }

        // left minor        
        for (theta = 0,i = 0; i < nMinorSegments; ++i)
        {            
            vec offset = { -1.0f*(r*0.5f - rMinor), -1.0f*(r*0.5f - rMinor), 0.0f };            
            f32 x = rMinor*sin(theta+ M_PI);
            f32 y = rMinor*cos(theta+ M_PI);

            vec vert = basisX*(x + offset.x) + basisY*(y + offset.y);

            theta += dtheta;
            verts.push_back(vert);
        }

        // top major        
        for (theta = 0,i = 0; i < nMinorSegments; ++i)
        {         
            vec offset = { rMajor -0.5f*r, -1.0f*(rMajor - 0.5f*r), 0.0f };
            f32 x = rMajor*sin(theta -M_PI_2);
            f32 y = rMajor*cos(theta -M_PI_2);

            vec vert = basisX*(x + offset.x) + basisY*(y + offset.y);

            theta += dtheta;
            verts.push_back(vert);
        }

        // bottom major        
        for (theta = 0,i = 0; i < nMinorSegments; ++i)
        {         
            vec offset = { -1.0f*(rMajor - 0.5f*r), 1.0f*(rMajor - 0.5f*r), 0.0f };
            f32 x = rMajor*sin(theta +M_PI_2);
            f32 y = rMajor*cos(theta +M_PI_2);

            vec vert = basisX*(x + offset.x) + basisY*(y + offset.y);

            theta += dtheta;
            verts.push_back(vert);
        }


        for each (vec v in verts)
        {
            Render::drawCrosshair(vec3(v.x, v.y, 0), vec3(1, 1, 1), 0.1f, lines);
        }
            
    }  
}// namespace murkyFramework

using namespace murkyFramework;

int main() // can't be in a namespace :(
{
    
    //     

    //skool();    
    AppFramework_initStruct appInit;
    appInit.main_noGfx  = main_noGfx;
    appInit.main_gfx    = main_gfx;
    appInit.game        = new myGame();

    AppFramework *const app = new AppFramework(appInit);
    for (int j = 0;j < 10;++j)
    {
        //auto speed = vec4(0, 0, qmaths::randInt(0, (j<5)?10:-10));
            auto speed = vec4(0, 0, j * 2);
            if (j > 5)speed = speed*-1.f;
        //auto speed = vec4(0, 0, qmaths::randInt(0, 30));

        for (int i = 0;i < 10;++i)
        {
            app->game->entities.push_back(new Monk(
                mat4(mat43(unit), vec4(j*3.f, 0, qmaths::randInt(0, 80))), speed,vec4(0.f)
                    ));            
        }
    }
                                                                                         
    app->run();
    murkyFramework::debugLog << L"Finished\n";
     
}