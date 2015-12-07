//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11. openGL4
#include <murkyFramework/src/pch.hpp>
namespace murkyFramework {
void GfxDeviceObj::setUniform_projectionMatrix(const float *pMat)
{
    // note: OGL, this accepts row-major, pre-multiplying of verts and post-multi in vertex shader.
    // ie no need to transpose if post-multi (Mv) in vertex shader.

    // note:: eeek!! 
    glUseProgram(shaderManager.get(L"posColTex").value);
    glUniformMatrix4fv(GfxDevice::Shaders::uniformHandle_projectionMatrix, 1, false, pMat);
    glUseProgram(0);

    glUseProgram(shaderManager.get(L"posCol").value);
    glUniformMatrix4fv(GfxDevice::Shaders::uniformHandle_projectionMatrix, 1, false, pMat);
    glUseProgram(0);

    GfxDevice::onGfxDeviceErrorTriggerBreakpoint();
}

GfxDeviceObj::GfxDeviceObj(GfxDeviceObj_initStruct *const initStruct) :
    hDC(initStruct->windowsSpecific->gethDC())
{
    systemSpecific::WindowsSpecific *const windowsSpecific = initStruct->windowsSpecific;

    PIXELFORMATDESCRIPTOR pfd; // Create a new PIXELFORMATDESCRIPTOR (PFD)
    memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR)); // Clear our  PFD
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR); // Set the size of the PFD to the size of the class
    pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW; // Enable double buffering, opengl support and drawing to a window
    pfd.iPixelType = PFD_TYPE_RGBA; // Set our application to use RGBA pixels
    pfd.cColorBits = 32; // Give us 32 bits of color information (the higher, the more colors)
    pfd.cDepthBits = 32; // Give us 32 bits of depth information (the higher, the more depth levels)
    pfd.iLayerType = PFD_MAIN_PLANE; // Set the layer of the PFD

    int nPixelFormat = ChoosePixelFormat(windowsSpecific->gethDC(), &pfd); // Check if our PFD is valid and get a pixel format back
    if (nPixelFormat == 0)
        triggerBreakpoint(L"opengl error: GfxDeviceObj::GfxDeviceObj()");        

    bool bResult = (bool)SetPixelFormat(windowsSpecific->gethDC(), nPixelFormat, &pfd); // Try and set the pixel format based on our PFD
    if (!bResult) // If it fails
        triggerBreakpoint(L"opengl error: GfxDeviceObj::GfxDeviceObj()");        
    HGLRC tempOpenGLContext = wglCreateContext(windowsSpecific->gethDC()); // Create an OpenGL 2.1 context for our device context
    wglMakeCurrent(windowsSpecific->gethDC(), tempOpenGLContext); // Make the OpenGL 2.1 context current and active

    glewExperimental = GL_TRUE;
    GLenum error = glewInit(); // Enable GLEW
    if (error != GLEW_OK) // If GLEW fails
        triggerBreakpoint(L"opengl error: GfxDeviceObj::GfxDeviceObj()");        

    int attributes[] =
    {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 4, // Set the MAJOR version of OpenGL to 4
        WGL_CONTEXT_MINOR_VERSION_ARB, 0, // Set the MINOR version of OpenGL to 0
        WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB, // Set our OpenGL context to be forward compatible
        0
    };

    if (wglewIsSupported("WGL_ARB_create_context") == 1)
    { // If the OpenGL 3.x context creation extension is available
        windowsSpecific->hRC = wglCreateContextAttribsARB(windowsSpecific->gethDC(), NULL, attributes); // Create and OpenGL 3.x context based on the given attributes
        wglMakeCurrent(NULL, NULL); // Remove the temporary context from being active
        wglDeleteContext(tempOpenGLContext); // Delete the temporary OpenGL 2.1 context
        wglMakeCurrent(windowsSpecific->gethDC(), windowsSpecific->gethRC()); // Make our OpenGL 3.0 context current
    }
    else
    {
        windowsSpecific->hRC = tempOpenGLContext; // If we didn't have support for OpenGL 3.x and up, use the OpenGL 2.1 context
        triggerBreakpoint();
    }

    int glVersion[2] = { -1, -1 }; // Set some default values for the version
    glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]); // Get back the OpenGL MAJOR version we are using
    glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]); // Get back the OpenGL MAJOR version we are using
    debugLog << L"Using OpenGL: " << glVersion[0] << "." << glVersion[1] << "\n"; // Output which version of OpenGL we are using

                                                                                  // state
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    //glDepthFunc(GL_GREATER);
    glDisable(GL_CULL_FACE);
}

void GfxDeviceObj::initialise()
{
    GfxDevice::Shaders::initialise();
    loadTexturesInDir(L"data");    
}

GfxDeviceObj::~GfxDeviceObj()
{
}

void GfxDeviceObj::drawBegin()
{
    glViewport(0, 0, g_appDebug->screenResX, g_appDebug->screenResY);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    GfxDevice::onGfxDeviceErrorTriggerBreakpoint();
}

void GfxDeviceObj::drawEnd()
{
    glFlush();
    SwapBuffers(g_appDebug->render->gfxDevice->hDC);
}

void GfxDeviceObj::loadTexturesInDir(std::wstring directoryName)
{
    FileDirectoryWalker fileWalker(directoryName, L"\\.png$");

    while (fileWalker.findNext())
    {
        debugLog << L"RenderObj::loadTexturesInDir loaded " << fileWalker.findData.cFileName << "\n";
        FilePathSplit pathBits(std::wstring(fileWalker.findData.cFileName));

        GfxDevice::TextureWrapper newt = GfxDevice::createTextureObjectFromFile(
            directoryName, pathBits.fileName, pathBits.extensionName);

        textureManager.add(pathBits.fileName, newt);
    }
}


// temp forward dec
namespace GfxDevice
{
u32 createShader(const char* sourceText, u32 type);
u32	    createProgram(u32 vertexShader, u32 fragmentShader);
}

void GfxDeviceObj::loadShadersInDir(std::wstring directoryName)
{
    FileDirectoryWalker fileWalker(directoryName, L"\\.vsh$");

    while (fileWalker.findNext())
    {
        debugLog << L"gfxDevice::loadShadersInDir loaded " << fileWalker.findData.cFileName << "\n";
        FilePathSplit pathBits(std::wstring(fileWalker.findData.cFileName));

        {
            std::wstring vsPath = directoryName + L"/" +pathBits.fileName +L".vsh";
            qdev::BinaryFileLoader vs_text_temp(vsPath);
            int nChars = vs_text_temp.getDataLength();
            char *vs_text = new char[nChars + 1];
            memcpy(vs_text, vs_text_temp.data(), nChars);
            vs_text[nChars] = 0;    // text needs to be null terminted

            std::wstring fsPath = directoryName + L"/" + pathBits.fileName + L".fsh";
            qdev::BinaryFileLoader fs_text_temp(fsPath);
            int nCharsFs = fs_text_temp.getDataLength();
            char *fs_text = new char[nCharsFs + 1];
            memcpy(fs_text, fs_text_temp.data(), nCharsFs);
            fs_text[nCharsFs] = 0;  // text needs to be null terminted

            u32 vs = GfxDevice::createShader(vs_text, GL_VERTEX_SHADER);
            u32 fs = GfxDevice::createShader(fs_text, GL_FRAGMENT_SHADER);

            GLuint p = GfxDevice::createProgram(vs, fs);
            shaderManager.add(pathBits.fileName, GfxDevice::ShaderWrapper{ p });

            delete[] fs_text;
            delete[] vs_text;            
        }
    }
}

namespace GfxDevice
{
    void onGfxDeviceErrorTriggerBreakpoint()
    {
        GLenum err;
        bool stop = false;
        while ((err = glGetError()) != GL_NO_ERROR)
        {
            std::wstring s;
            switch (err)
            {
            case 0x0500: s = L"GL_INVALID_ENUM"; break;
            case 0x0501: s = L"GL_INVALID_VALUE"; break;
            case 0x0502: s = L"GL_INVALID_OPERATION"; break;
            case 0x0503: s = L"GL_STACK_OVERFLOW"; break;
            case 0x0504: s = L"GL_STACK_UNDERFLOW"; break;
            case 0x0505: s = L"GL_OUT_OF_MEMORY"; break;
            default: s = L"" + err;
            }
            debugLog << L"GL error: " << s << L"\n";
            stop = true;
        }

        if (stop == true)
            triggerBreakpoint();
    }


  
}
}//namespace murkyFramework