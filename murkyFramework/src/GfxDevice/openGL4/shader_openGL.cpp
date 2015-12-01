//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/src/private/pch.hpp>
#define deviceObj  g_appDebug->render->gfxDevice

#ifdef USE_OPENGL
namespace GfxDevice
{    	    
    // external forward declarations 
    extern   void GfxDevice::onGfxDeviceErrorTriggerBreakpoint();
    
    // internal forward declarations
    u32     createShader(const char* sourceText, u32 type);
    u32	    createProgram(u32 vertexShader, u32 fragmentShader);   				

    namespace Shaders
    {
        u32		uniforms_textureSamplerID;
        u32     uniformHandle_projectionMatrix;               		
    }
        
    bool checkUniform(s32 a)
    {
        if (a < 0)triggerBreakpoint();
        if (a == GL_INVALID_VALUE)triggerBreakpoint();
        if (a == GL_INVALID_OPERATION)triggerBreakpoint();
        if (a == GL_INVALID_OPERATION)triggerBreakpoint();
        return true;
    }       

    void	Shaders::initialise()
    {
        GfxDevice::onGfxDeviceErrorTriggerBreakpoint();
        debugLog << L"GfxLowLevel::Shaders::initialise" << "\n";

        {
            qdev::BinaryFileLoader vs_text_temp(L"src/GfxDevice/openGL4/shaders/posColTex.vsh");
            int nChars = vs_text_temp.getDataLength();
            char *vs_text = new char[nChars + 1];
            memcpy(vs_text, vs_text_temp.data(), nChars);
            vs_text[nChars] = 0;    // text needs to be null terminted
                        
            qdev::BinaryFileLoader fs_text_temp(L"src/GfxDevice/openGL4/shaders/posColTex.fsh");
            int nCharsFs = fs_text_temp.getDataLength();
            char *fs_text = new char[nCharsFs + 1];
            memcpy(fs_text, fs_text_temp.data(), nCharsFs);
            fs_text[nCharsFs] = 0;  // text needs to be null terminted

            u32 vs = GfxDevice::createShader(vs_text, GL_VERTEX_SHADER);
            u32 fs = GfxDevice::createShader(fs_text, GL_FRAGMENT_SHADER);

            GLuint p = GfxDevice::createProgram(vs, fs);
            deviceObj->shaderManager.add(L"posColTex", ShaderWrapper{ p });

            delete[] fs_text;
            delete[] vs_text;

            uniforms_textureSamplerID = glGetUniformLocation(p, "textureSamplerID");
            checkUniform(uniforms_textureSamplerID);

            uniformHandle_projectionMatrix = glGetUniformLocation(p, "projectionMatrix");
            checkUniform(uniformHandle_projectionMatrix);
            GfxDevice::onGfxDeviceErrorTriggerBreakpoint();
        }

        if(1)
        {
            qdev::BinaryFileLoader vs_text_temp(L"src/GfxDevice/openGL4/shaders/posCol.vsh");
            int nChars = vs_text_temp.getDataLength();
            char *vs_text = new char[nChars + 1];
            memcpy(vs_text, vs_text_temp.data(), nChars);
            vs_text[nChars] = 0;    // text needs to be null terminted

            qdev::BinaryFileLoader fs_text_temp(L"src/GfxDevice/openGL4/shaders/posCol.fsh");
            int nCharsFs = fs_text_temp.getDataLength();
            char *fs_text = new char[nCharsFs + 1];
            memcpy(fs_text, fs_text_temp.data(), nCharsFs);
            fs_text[nCharsFs] = 0;  // text needs to be null terminted

            u32 vs = GfxDevice::createShader(vs_text, GL_VERTEX_SHADER);
            u32 fs = GfxDevice::createShader(fs_text, GL_FRAGMENT_SHADER);

            GLuint p = GfxDevice::createProgram(vs, fs);
            deviceObj->shaderManager.add(L"posCol", ShaderWrapper{ p });

            //uniformHandle_projectionMatrix = glGetUniformLocation(p, "projectionMatrix");
            //checkUniform(uniformHandle_projectionMatrix);

            delete[] fs_text;
            delete[] vs_text;
            GfxDevice::onGfxDeviceErrorTriggerBreakpoint();
        }
    }
            
    void	Shaders::deinitialise()
    {
    }

    u32 createShader(const char* sourceText, u32 type)
    {
        GLuint	shader = glCreateShader(type);
        glShaderSource(shader, 1, &sourceText, NULL);
        glCompileShader(shader);

        // Check shader     
        GLint status;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
        if (status == GL_FALSE)
        {
            GLint infoLogLength;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

            GLchar *strInfoLog = new GLchar[infoLogLength + 1];
            glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

            debugLog << L"Compile failure\n";
            debugLog << strInfoLog << L"\n";
            triggerBreakpoint();

            delete[] strInfoLog;
        }
        // Check shader   
        GfxDevice::onGfxDeviceErrorTriggerBreakpoint();
        return shader;
    }

    u32	createProgram(u32 vertexShader, u32 fragmentShader)
    {
        u32 program = glCreateProgram();
        glAttachShader(program, fragmentShader);
        glAttachShader(program, vertexShader);
        glLinkProgram(program);

        // check program
        GLint status;
        glGetProgramiv(program, GL_LINK_STATUS, &status);
        if (status == GL_FALSE)
        {
            GLint infoLogLength;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

            GLchar *strInfoLog = new GLchar[infoLogLength + 1];
            glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);

            debugLog << L"Compile failure making program\n";
            debugLog << strInfoLog << L"\n";
            delete[] strInfoLog;
            triggerBreakpoint();
        }
        // check program
        GfxDevice::onGfxDeviceErrorTriggerBreakpoint();
        return program;
    }
}
#endif // USE_OPENGL
