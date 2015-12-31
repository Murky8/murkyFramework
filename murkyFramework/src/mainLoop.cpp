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

    app->lastFrameDuration = static_cast<f32>(currentFrameClock - lastFrameClock);
    //debugLog << L"d: " << (f32)lastFrameDuration << L"\n";
    lastFrameClock = currentFrameClock;
    
    app->frameRate = static_cast<f32>(1.0/ app->lastFrameDuration);
    
    static f32 aveFR = 0.f;
    { // ave frame rate
        static f32 aveFRTimer = 0.5;
        static f32 acc = 0;
        static int nAcc = 0;
        aveFRTimer -= app->lastFrameDuration;

        acc += app->lastFrameDuration;
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
    if (app->inputDevices->keyStatus(InputDevices::KeyCode::c))
        app->flyCameraMode = true;

    if (app->inputDevices->keyStatus(InputDevices::KeyCode::v))
        app->flyCameraMode = false;

    if (app->flyCameraMode)
    {
        f32 speed = app->lastFrameDuration*50.f;
        if (app->inputDevices->keyStatus(InputDevices::KeyCode::shift))
            speed *= 10.f;

        mat4 transRot = app->game->cursorPosOri;
        vec4 pos = transRot.get_t();

        if (app->inputDevices->keyStatus(InputDevices::KeyCode::d))
            pos += transRot.get_r() * speed;

        if (app->inputDevices->keyStatus(InputDevices::KeyCode::a))
            pos -= transRot.get_r() * speed;

        if (app->inputDevices->keyStatus(InputDevices::KeyCode::w))
            pos += transRot.get_f() * speed;

        if (app->inputDevices->keyStatus(InputDevices::KeyCode::s))
            pos -= transRot.get_f() * speed;

        if (app->inputDevices->keyStatus(InputDevices::KeyCode::e))
            pos += vec::up * speed;

        if (app->inputDevices->keyStatus(InputDevices::KeyCode::q))
            pos -= vec::up * speed;

        app->game->cursorPosOri.set_t(pos);

        // free look
        vec rv3c(zero);
        int mx, my;
        app->inputDevices->consumeAllMouseDx(mx);
        app->inputDevices->consumeAllMouseDy(my);

        // rotate l/r
        rv3c.y += -0.001f*(float)mx;
        mat43 rmat = makeRotationMatrix_from3c(rv3c);

        app->game->cursorPosOri.ruf = app->game->cursorPosOri.ruf*rmat;

        // rotate l/r

        // rotate u/d
        rv3c = vec(zero);
        rv3c = app->game->cursorPosOri.get_r()*-0.001f*(float)my;
        rmat = makeRotationMatrix_from3c(rv3c);
        app->game->cursorPosOri.ruf = app->game->cursorPosOri.ruf*rmat;

        app->render->cameraPosOri = app->game->cursorPosOri;
        // rotate u/d
        // free look
    }
    //------------------------------------------------------------------------------
    app->render->drawAll();  
    
    // unlimited frame rate
    if (app->frameRateLimit <= 0.f)
    {
    }
    else
    { // limit frame rate
        f32 frameTimeMax = 1.0f / app->frameRateLimit;
        if (app->lastFrameDuration < frameTimeMax)
           Sleep(static_cast<int>((frameTimeMax - app->lastFrameDuration)*1000.f));        
    }
    app->frameCounter++;      
}
}//namespace murkyFramework