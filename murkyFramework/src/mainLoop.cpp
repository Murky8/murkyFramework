//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/src/pch.hpp>
namespace murkyFramework {
// called from windows loop in main.cpp
void mainLoop_threadMain(AppFramework  *const app)
{        
    debugLogScreen.clear();    
    static f64 lastFrameClock = 0;
    f64 currentFrameClock = app->system->readTimeSecondsSinceAppStart();

    f32 lastFrameDuration = static_cast<f32>(currentFrameClock - lastFrameClock);
    //debugLog << L"d: " << (f32)lastFrameDuration << L"\n";
    lastFrameClock = currentFrameClock;
    
    app->frameRate = static_cast<f32>(1.0/lastFrameDuration);
    
    static f32 aveFR = 0.f;
    { // ave frame rate
        static f32 aveFRTimer = 0.5;
        static f32 acc = 0;
        static int nAcc = 0;
        aveFRTimer -= lastFrameDuration;

        acc += lastFrameDuration;
        nAcc++;
        if (aveFRTimer < 0)
        {
            aveFRTimer += 0.3f;           
            aveFR = nAcc/acc;
            nAcc = 0;
            acc = 0.f;
        }
    }
        
    debugLogScreen << L"Framerate: " << aveFR << L"\n";    
    
    //------------------------------------------------------------------------------
    f32 speed = lastFrameDuration*50.f;
    if (app->inputDevices->keyStatus(InputDevices::KeyCode::shift))
        speed *= 10.f;

    if (app->inputDevices->keyStatus(InputDevices::KeyCode::d))
        app->game->cursorPos += app->game->cursorOri.get_r() * speed;
    
    if (app->inputDevices->keyStatus(InputDevices::KeyCode::a))
        app->game->cursorPos -= app->game->cursorOri.get_r() * speed;

    if (app->inputDevices->keyStatus(InputDevices::KeyCode::w))
        app->game->cursorPos += app->game->cursorOri.get_f() * speed;
    
    if (app->inputDevices->keyStatus(InputDevices::KeyCode::s))
        app->game->cursorPos -= app->game->cursorOri.get_f() * speed;

    if (app->inputDevices->keyStatus(InputDevices::KeyCode::e))
        app->game->cursorPos += vec::up * speed;

    if (app->inputDevices->keyStatus(InputDevices::KeyCode::q))
        app->game->cursorPos -= vec::up * speed;
        
    // free look
    vec rv(zero);	
    int mx, my;
    app->inputDevices->consumeAllMouseDx(mx);
    app->inputDevices->consumeAllMouseDy(my);

   
    // rotate l/r
    rv.y	+= -0.001f*(float)mx;
    mat3 rmat = makeRotationMatrix3c(rv);
    app->game->cursorOri = app->game->cursorOri*rmat;
    // rotate l/r
    
    // rotate u/d
    rv = vec(zero);
    rv = app->game->cursorOri.get_r()*-0.001f*(float)my;
    rmat = makeRotationMatrix3c(rv);
    app->game->cursorOri = app->game->cursorOri*rmat;
    // rotate u/d
    // free look

    debugLogScreen << rmat << L"\n";
    //debugLogScreen << state.cursorOri << L"\n";
    //state.cursorOri 

    //------------------------------------------------------------------------------
    Render::drawAll();  
    
    // unlimited frame rate
    if (app->frameRateLimit <= 0.f)
    {
    }
    else
    { // limit frame rate
        f32 frameTimeMax = 1.0f / app->frameRateLimit;
        if (lastFrameDuration < frameTimeMax)
           Sleep(static_cast<int>((frameTimeMax - lastFrameDuration)*1000.f));        
    }
    app->frameCounter++;      
}
}//namespace murkyFramework