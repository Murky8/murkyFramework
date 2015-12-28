//#include <murkyFramework/src/pch.hpp>
#include <windows.h>
#include <string>
#include <vector>
#include <list>

#include <murkyFramework/src/debugUtils.hpp>
#include <murkyFramework/src/version.hpp>
#include <murkyFramework/src/types.hpp>
#include <murkyFramework/src/appFramework.hpp>
#include <murkyFramework/src/SystemSpecific/Windows/WindowsSpecific.hpp>

#include <murkyFramework/src/vectorMatrix.hpp>
#include <murkyFramework/src/Game/EntityBase.hpp>
#include <murkyFramework/src/Game/Game.hpp>
#include <murkyFramework/src/stringHelpers.hpp>

namespace murkyFramework
{
    //void thread_mainGfx()
    class myGame : public Game
    {
    };
      
    void  main_noGfx(struct main_noGfx_parameters* parameters)
    {
        myGame *game = dynamic_cast<myGame*>(parameters->app->game);        
    }

    void  main_gfx(struct main_gfx_parameters* parameters)
    {        
    }
}

using namespace murkyFramework;

class Monk : public EntityBase
{

};

int main() // can't be in a namespace :(
{
    //skool();    
    AppFramework_initStruct appInit;
    appInit.main_noGfx = main_noGfx;
    appInit.game = new myGame();

    AppFramework *const app = new AppFramework(appInit);
    app->game->entities.push_back(new Monk());

    app->run();

    murkyFramework::debugLog << L"Finished\n";
}