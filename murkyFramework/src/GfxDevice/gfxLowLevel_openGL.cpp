//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/src/private/pch.hpp>
#include <murkyFramework/include/version.hpp>
#include <murkyFramework/include/GfxDevice/version_gfxDevice.hpp>
#ifdef USE_OPENGL

#include <windows.h>
#include <vector>

#include <murkyFramework/include/appFramework.hpp>
#include <murkyFramework/include/types.hpp>  
#include <murkyFramework/include/GfxDevice/gfxLowLevel.hpp>
#include <murkyFramework/include/GfxDevice/gfxPrimativeTypes.hpp>
#include <murkyFramework/include/GfxDevice/vertexBuffer.hpp>
#include <murkyFramework/include/GfxDevice/shaders.hpp>
#include <murkyFramework/include/GfxDevice/texture.hpp>
#include <murkyFramework/include/debugUtils.hpp>
#include <murkyFramework/include/system.hpp>
#include <murkyFramework/include/vectorMatrix.hpp>
#include <murkyFramework/include/appFramework.hpp>

#include <external/glew/include/GL/glew.h> 
#include <external/glew/include/GL/wglew.h>

#define GLM_FORCE_RADIANS
#include <external/glm/glm.hpp>
#include <external/glm/vec2.hpp>
#include <external/glm/vec3.hpp>
#include <external/glm/vec4.hpp>
#include <external/glm/mat4x4.hpp>
#include <external/glm/gtc/matrix_transform.hpp>

namespace GfxDevice
{   
    // data
    mat4    projectionMatrix(unit);    
    HDC     hDC;
    HGLRC   hRC;
    HWND    hWnd;

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
        glViewport(0, 0, Gapp->screenResX, Gapp->screenResY);

        glClearColor(0.4f, 0.6f, 0.9f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        onGfxDeviceErrorTriggerBreakpoint();	
    }

    void drawEnd()
    {
        glFlush();
        SwapBuffers(GfxDevice::hDC);
    }        
}

#endif // USE_OPENGL
