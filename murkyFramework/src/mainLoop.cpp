//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/version.hpp>
#include <murkyFramework/include/inputDevices.hpp>
#include <murkyFramework/include/gfxLowLevel/version_gfxDevice.hpp>

#include <iostream>
#include <vector>
#include <thread>

#include <windows.h>
#include <stdlib.h>

#include <murkyFramework/include/debugUtils.hpp>
#include <murkyFramework/include/appFramework.hpp>
#include <murkyFramework/include/types.hpp>

#include <murkyFramework/include/loadSaveFile.hpp>
#include <murkyFramework/include/inputDevices.hpp>
#include <murkyFramework/include/system.hpp>
#include <murkyFramework/include/gfxLowLevel/gfxPrimativeTypes.hpp>
#include <murkyFramework/include/gfxLowLevel/gfxLowLevel.hpp>
#include <murkyFramework/include/gfxHighLevel/render.hpp>
#include <murkyFramework/include/vectorMatrix.hpp>
#include <murkyFramework/include/state.hpp>
#include <murkyFramework/include/maths.hpp>

// called from windows loop in main.cpp
void mainLoop_threadMain(InputDevices &inputDevices, State &state)
{    
    
    static f64 lastFrameClock = 0;
    f64 currentFrameClock = system2::readTimeSecondsSinceAppStart();

    f64 lastFrameDuration = currentFrameClock - lastFrameClock;
    //debugLog << L"d: " << (f32)lastFrameDuration << L"\n";
    lastFrameClock = currentFrameClock;
    
    Gapp.frameRate = 1.0 / lastFrameDuration;


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
            
            aveFR = nAcc / (f32)acc;
            nAcc = 0;
            acc = 0.f;
        }
    }
        

    //clamp(lastFrameDuration, 0.0000001, 1.0);

    //// unlimited frame rate
    //if (Gapp.frameRateLimit == 0) 
    //{
    //}
    //else
    //{
    //    static f64 timer = 0.0f;

    //    timer -= lastFrameDuration;

    //    if (timer > 0)
    //        return;
    //
    //    timer += 1.0f/Gapp.frameRateLimit;
    //    Gapp.frameRate = Gapp.frameRateLimit;
    //    
    //}

    //static f64 lastFrameClock = system2::readTimeSecondsSinceAppStart();

    //f64 currentFrameClock = system2::readTimeSecondsSinceAppStart();
    //f64 lastFrameDuration = currentFrameClock - lastFrameClock;

    //lastFrameClock = currentFrameClock;
    //clamp(lastFrameDuration, 0.0000001, 1.0);

    //// unlimited frame rate
    //if (Gapp.frameRateLimit == 0)
    //{
    //    Gapp.frameRate = 1.0 / lastFrameDuration;
    //}
    //else
    //{
    //    static f64 timer = 0.0f;

    //    timer -= lastFrameDuration;

    //    if (timer > 0)
    //        return;

    //    timer += 1.0f / Gapp.frameRateLimit;
    //    Gapp.frameRate = Gapp.frameRateLimit;

    //}
    
    debugLogScreen.clear();    
    
    debugLogScreen <<(f32)system2::readTimeSecondsSinceAppStart() << L"\n";
    debugLogScreen << L"framerate: " << aveFR << L"\n";
    //debugLog << L"framerate: " << Gapp.frameRate << L"\n";
    debugLogScreen << L"frame: " << (int)Gapp.frameCounter << L"\n";

    RenderHi::drawAll(state);    
    
    // unlimited frame rate
    if (Gapp.frameRateLimit == 0)
    {
    }
    else
    { // limit frame rate
        f64 frameTimeMax = 1.0 / Gapp.frameRateLimit;
        if (lastFrameDuration < frameTimeMax)
           Sleep((float)(frameTimeMax - lastFrameDuration)*1000.f);        
    }
    Gapp.frameCounter++;      
}

void mainLoop_threadSecond(InputDevices &inputDevices, State &state)
{
}
