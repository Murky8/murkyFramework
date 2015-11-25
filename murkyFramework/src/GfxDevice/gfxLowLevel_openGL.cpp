//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/src/private/pch.hpp>

#ifdef USE_OPENGL

namespace GfxDevice
{   
    // data
    mat4    projectionMatrix(unit);    

    void onGfxDeviceErrorTriggerBreakpoint()
    {
        GLenum err;
        bool stop = false;
        while(( err = glGetError() ) != GL_NO_ERROR)
        {
            std::wstring s;
            switch (err)
            {
            case 0x0500 : s = L"GL_INVALID_ENUM"; break;
            case 0x0501 : s = L"GL_INVALID_VALUE"; break;
            case 0x0502 : s = L"GL_INVALID_OPERATION"; break;
            case 0x0503 : s = L"GL_STACK_OVERFLOW"; break;
            case 0x0504 : s = L"GL_STACK_UNDERFLOW"; break;
            case 0x0505 : s = L"GL_OUT_OF_MEMORY"; break;
            default : s = L"" +err;
            }
            debugLog << L"GL error: " << s << L"\n";
            stop = true;
        }

        if( stop == true )
            triggerBreakpoint();
    }    

    void setStateToAppDefault()
    {
  
    }   

    void drawBegin()
    {
        glViewport(0, 0, g_appDebug->screenResX, g_appDebug->screenResY);

        glClearColor(0.4f, 0.6f, 0.9f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        onGfxDeviceErrorTriggerBreakpoint();	
    }

    void drawEnd()
    {
        glFlush();
        SwapBuffers(g_appDebug->render->gfxDevice->hDC);        
    }        
}

#endif // USE_OPENGL
