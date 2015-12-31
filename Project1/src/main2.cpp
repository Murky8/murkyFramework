#include <murkyFramework/src/pch.hpp>

namespace murkyFramework
{
    //void thread_mainGfx()
    class myGame : public Game
    {
    };
          
    class Monk : public EntityBase
    {
    public:
        Monk(const mat4& transform) : EntityBase(transform)
        {
        }

    };

    void  main_noGfx()
    {           
        EntityBase* player = *g_appDebug->game->entities.begin();

        int dmx;
        g_appDebug->inputDevices->consumeAllMouseDx(dmx);
        player->transform.trans += vec4(dmx*0.1f, 0, 0);

        //app->inputDevices->consumeAllMouseDy(my);

        for (auto &entity : g_appDebug->game->entities)
        {
            // do physics
            //mat43 rmat = makeRotationMatrix_from3c(vec4(0.f, 0.001f, 0.f));
            //entity->transform.ruf = entity->transform.ruf*rmat;
        }        
    }

    void  main_gfx()
    {

        EntityBase* ent = *g_appDebug->game->entities.begin();

        //focus on enitiyty
        //g_appDebug->render->cameraPosOri = Render::makeLookAtPosOri(
        //  ent->transform.trans +vec4(0,20, -20), ent->transform.trans);

        if (!g_appDebug->flyCameraMode)
        {
     
        g_appDebug->render->cameraPosOri = Render::makeLookAtPosOri(
            vec4(0, 20, -20), vec4(0, 0, 0));
        }

        for (auto *ent : g_appDebug->game->entities)
        {
            // display
            vec verts[4] = { vec{1, 0, 2}, vec{1, 0, -2},vec{-1, 0, -2},vec{-1, 0, 2} };

            for (int i = 0;i < 4;i++)
            {
                verts[i] = verts[i] * ent->transform.ruf +ent->transform.trans;
            }

            auto &lines = g_appDebug->render->defaultLines;
            lines.push_back(Line_pc(
                Vert_pc((vec3)verts[0], vec3(1, 1, 1)),
                Vert_pc((vec3)verts[1], vec3(1, 1, 1))
                ));

            lines.push_back(Line_pc(
                Vert_pc((vec3)verts[1], vec3(1, 1, 1)),
                Vert_pc((vec3)verts[2], vec3(1, 1, 1))
                ));

            lines.push_back(Line_pc(
                Vert_pc((vec3)verts[2], vec3(1, 1, 1)),
                Vert_pc((vec3)verts[3], vec3(1, 1, 1))
                ));

            lines.push_back(Line_pc(
                Vert_pc((vec3)verts[3], vec3(1, 1, 1)),
                Vert_pc((vec3)verts[0], vec3(1, 1, 1))
                ));
        }
    }  
}// namespace murkyFramework

using namespace murkyFramework;

int main() // can't be in a namespace :(
{
    //skool();    
    AppFramework_initStruct appInit;
    appInit.main_noGfx  = main_noGfx;
    appInit.main_gfx    = main_gfx;
    appInit.game        = new myGame();

    AppFramework *const app = new AppFramework(appInit);
    app->game->entities.push_back(new Monk(mat4(mat43(unit), vec4(5, 1, 0))));
    app->game->entities.push_back(new Monk(mat4(mat43(unit), vec4(0, 1, 0))));
                                                                                     
    app->run();

    murkyFramework::debugLog << L"Finished\n";
}