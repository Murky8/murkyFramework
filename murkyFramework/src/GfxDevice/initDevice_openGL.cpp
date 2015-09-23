//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/version.hpp>
#include <murkyFramework/include/GfxDevice/version_gfxDevice.hpp>
#ifdef USE_OPENGL

#include <murkyFramework/include/common.hpp>
#include <murkyFramework/include/debugUtils.hpp>
#include <windows.h>
#include <external/glew/include/GL/glew.h> 
#include <external/glew/include/GL/wglew.h>
#include <murkyFramework/include/GfxDevice/gfxLowLevel.hpp>


namespace GfxDevice
{
    // forward declarations    
    extern  HDC     hDC;
    extern  HGLRC   hRC;
    extern  HWND    hWnd;
    
    bool initialise_device(HDC &hDC, HGLRC &hRC, HWND &hWnd)         
    {
            GfxDevice::hDC = hDC;//hDC = GetDC(hWnd); // Get the device context for our window
            GfxDevice::hRC = hRC;
            GfxDevice::hWnd = hWnd;

            PIXELFORMATDESCRIPTOR pfd; // Create a new PIXELFORMATDESCRIPTOR (PFD)
            memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR)); // Clear our  PFD
            pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR); // Set the size of the PFD to the size of the class
            pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW; // Enable double buffering, opengl support and drawing to a window
            pfd.iPixelType = PFD_TYPE_RGBA; // Set our application to use RGBA pixels
            pfd.cColorBits = 32; // Give us 32 bits of color information (the higher, the more colors)
            pfd.cDepthBits = 32; // Give us 32 bits of depth information (the higher, the more depth levels)
            pfd.iLayerType = PFD_MAIN_PLANE; // Set the layer of the PFD

            int nPixelFormat = ChoosePixelFormat(hDC, &pfd); // Check if our PFD is valid and get a pixel format back
            if (nPixelFormat == 0) // If it fails
                return false;

            bool bResult = (bool)SetPixelFormat(hDC, nPixelFormat, &pfd); // Try and set the pixel format based on our PFD
            if (!bResult) // If it fails
                return false;

            HGLRC tempOpenGLContext = wglCreateContext(hDC); // Create an OpenGL 2.1 context for our device context
            wglMakeCurrent(hDC, tempOpenGLContext); // Make the OpenGL 2.1 context current and active

            glewExperimental = GL_TRUE;
            GLenum error = glewInit(); // Enable GLEW
            if (error != GLEW_OK) // If GLEW fails
                return false;

            int attributes[] =
            {
                WGL_CONTEXT_MAJOR_VERSION_ARB, 4, // Set the MAJOR version of OpenGL to 4
                WGL_CONTEXT_MINOR_VERSION_ARB, 0, // Set the MINOR version of OpenGL to 0
                WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB, // Set our OpenGL context to be forward compatible
                0
            };

            if (wglewIsSupported("WGL_ARB_create_context") == 1)
            { // If the OpenGL 3.x context creation extension is available
                hRC = wglCreateContextAttribsARB(hDC, NULL, attributes); // Create and OpenGL 3.x context based on the given attributes
                wglMakeCurrent(NULL, NULL); // Remove the temporary context from being active
                wglDeleteContext(tempOpenGLContext); // Delete the temporary OpenGL 2.1 context
                wglMakeCurrent(hDC, hRC); // Make our OpenGL 3.0 context current
            }
            else
            {
                hRC = tempOpenGLContext; // If we didn't have support for OpenGL 3.x and up, use the OpenGL 2.1 context
                triggerBreakpoint();
            }

            int glVersion[2] = { -1, -1 }; // Set some default values for the version
            glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]); // Get back the OpenGL MAJOR version we are using
            glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]); // Get back the OpenGL MAJOR version we are using
            debugLog << L"Using OpenGL: " << glVersion[0] << "." << glVersion[1] << "\n"; // Output which version of OpenGL we are using


            glEnable(GL_DEPTH_TEST);
            return true; // We have successfully created a context, return true
        }

    bool deinitialise_device()
    {
        return false;

    }
} // namespace GfxLowLevel
#endif