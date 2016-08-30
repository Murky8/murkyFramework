//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/src/pch.hpp>
#define deviceObj  g_appDebug->render->gfxDevice

namespace murkyFramework {
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
            ShaderWrapper createShaderObject(std::wstring sourceText);
        }

        bool checkUniform(s32 a)
        {
            if (a < 0)triggerBreakpoint();
            return true;
        }

        void	Shaders::initialise()
        {
            GfxDevice::onGfxDeviceErrorTriggerBreakpoint();
            debugLog << L"GfxLowLevel::Shaders::initialise" << "\n";

            deviceObj->loadShadersInDir(L"murkyFramework/src/GfxDevice/openGL4/shaders");

            {
            /*    u32 prog = deviceObj->shaderManager.get(L"posColTex").value;
                uniforms_textureSamplerID = glGetUniformLocation(
                    prog,
                    "textureSamplerID");
                onGfxDeviceErrorTriggerBreakpoint();

                uniformHandle_projectionMatrix = glGetUniformLocation(
                    prog,
                    "projectionMatrix");
                onGfxDeviceErrorTriggerBreakpoint();*/
            }
        }

        void	Shaders::deinitialise()
        {
        }

        u32 createShader(const char* sourceText, u32 type)
        {
            //GLuint	shader = glCreateShader(type);
            //glShaderSource(shader, 1, &sourceText, NULL);
            //glCompileShader(shader);

            //// Check shader     
            //GLint status;
            //glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
            //if (status == GL_FALSE)
            //{
            //    GLint infoLogLength;
            //    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

            //    GLchar *strInfoLog = new GLchar[infoLogLength + 1];
            //    glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

            //    debugLog << L"Compile failure\n";
            //    debugLog << strInfoLog << L"\n";
            //    triggerBreakpoint();

            //    delete[] strInfoLog;
            //}
            //// Check shader   
            //GfxDevice::onGfxDeviceErrorTriggerBreakpoint();
			triggerBreakpoint();
            return 0;
        }

        u32	createProgram(u32 vertexShader, u32 fragmentShader)
        {
            //u32 program = glCreateProgram();
            //glAttachShader(program, fragmentShader);
            //glAttachShader(program, vertexShader);
            //glLinkProgram(program);

            //// check program
            //GLint status;
            //glGetProgramiv(program, GL_LINK_STATUS, &status);
            //if (status == GL_FALSE)
            //{
            //    GLint infoLogLength;
            //    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

            //    GLchar *strInfoLog = new GLchar[infoLogLength + 1];
            //    glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);

            //    debugLog << L"Compile failure making program\n";
            //    debugLog << strInfoLog << L"\n";
            //    delete[] strInfoLog;
            //    triggerBreakpoint();
            //}
            //// check program
            //GfxDevice::onGfxDeviceErrorTriggerBreakpoint();
			triggerBreakpoint();
            return 0;
        }
    }//namespace GfxDevice
}//namespace murkyFramework

