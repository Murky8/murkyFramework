//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/src/private/pch.hpp>

#include <murkyFramework/include/Render/render.hpp>
#include <murkyFramework/include/vectorMatrix_rotation.hpp>

// called from windows loop in main.cpp
void mainLoop_threadMain(InputDevices &inputDevices, State &state)
{        
    debugLogScreen.clear();    
    static f64 lastFrameClock = 0;
    f64 currentFrameClock = system2::readTimeSecondsSinceAppStart();

    f32 lastFrameDuration = static_cast<f32>(currentFrameClock - lastFrameClock);
    //debugLog << L"d: " << (f32)lastFrameDuration << L"\n";
    lastFrameClock = currentFrameClock;
    
    Gapp->frameRate = static_cast<f32>(1.0/lastFrameDuration);
    
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
    if (inputDevices.keyStatus(InputDevices::KeyCode::shift))
        speed *= 10.f;

    if (inputDevices.keyStatus(InputDevices::KeyCode::d))
        state.cursorPos += state.cursorOri.get_r() * speed;	
    
    if (inputDevices.keyStatus(InputDevices::KeyCode::a))
        state.cursorPos -= state.cursorOri.get_r() * speed;

    if (inputDevices.keyStatus(InputDevices::KeyCode::w))
        state.cursorPos += state.cursorOri.get_f() * speed;
    
    if (inputDevices.keyStatus(InputDevices::KeyCode::s))
        state.cursorPos -= state.cursorOri.get_f() * speed;

    if (inputDevices.keyStatus(InputDevices::KeyCode::e))
        state.cursorPos += vec::up * speed;

    if (inputDevices.keyStatus(InputDevices::KeyCode::q))
        state.cursorPos -= vec::up * speed;
	
    
	// free look
	vec rv(zero);	
	int mx, my;
	inputDevices.consumeAllMouseDx(mx);
	inputDevices.consumeAllMouseDy(my);

   
	// rotate l/r
	rv.y	+= -0.001f*(float)mx;
	mat3 rmat = makeRotationMatrix3c(rv);
	state.cursorOri = state.cursorOri*rmat;
	// rotate l/r
    
	// rotate u/d
	rv = vec(zero);
	rv = state.cursorOri.get_r()*-0.001f*(float)my;
	rmat = makeRotationMatrix3c(rv);
	state.cursorOri = state.cursorOri*rmat;
	// rotate u/d
	// free look

	debugLogScreen << rmat << L"\n";
	//debugLogScreen << state.cursorOri << L"\n";
    //state.cursorOri 

	//------------------------------------------------------------------------------
    Render::drawAll(state);  
    
    // unlimited frame rate
    if (Gapp->frameRateLimit == 0)
    {
    }
    else
    { // limit frame rate
        f32 frameTimeMax = 1.0f / Gapp->frameRateLimit;
        if (lastFrameDuration < frameTimeMax)
           Sleep(static_cast<int>((frameTimeMax - lastFrameDuration)*1000.f));        
    }
    Gapp->frameCounter++;      
}

void mainLoop_threadSecond(InputDevices &inputDevices, State &state)
{
}
