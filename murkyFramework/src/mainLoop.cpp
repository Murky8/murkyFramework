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
void mainLoop(InputDevices &inputDevices, State &state)
{    
    static f64 lastFrameClock = system2::readTimeSecondsSinceAppStart();

    f64 currentFrameClock = system2::readTimeSecondsSinceAppStart();
    f64 lastFrameDuration = currentFrameClock - lastFrameClock;

        lastFrameClock = currentFrameClock;
    clamp(lastFrameDuration, 0.0000001, 1.0);


    if (Gapp.frameRateLimit == 0) 
    {
        Gapp.frameRate = 1.0 / lastFrameDuration;
    }
    else
    {
        static f64 timer = 0.0f;

        timer -= lastFrameDuration;

        if (timer > 0)
            return;
    
        timer += 1.0f/Gapp.frameRateLimit;
        Gapp.frameRate = Gapp.frameRateLimit;
        
    }


    
    debugLogScreen.clear();    
    
    debugLogScreen <<(f32)system2::readTimeSecondsSinceAppStart() << L"\n";
    debugLogScreen << L"framerate: " << Gapp.frameRate << L"\n";
    debugLogScreen << L"frame: " << (int)Gapp.frameCounter << L"\n";


    RenderHi::drawAll(state);    
    
    Gapp.frameCounter++;
            
  
    
}