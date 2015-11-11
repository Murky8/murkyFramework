//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/src/private/pch.hpp>
#include <murkyFramework/include/version.hpp>
#include <murkyFramework/include/GfxDevice/version_gfxDevice.hpp>
#ifdef USE_OPENGL

//#include <vector>
#include <memory>
#include <murkyFramework/include/GfxDevice/gfxLowLevel.hpp>
#include <murkyFramework/include/GfxDevice/shaders.hpp>
#include <murkyFramework/include/debugUtils.hpp>
#include <murkyFramework/include/loadSaveFile.hpp>

#include <external/glew/include/GL/glew.h> 
#include <external/glew/include/GL/wglew.h>
#include <murkyFramework/include/collectionNamed.hpp>
#include <murkyFramework/src/GfxDevice/public/gfxDevice.hpp>

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
	
    const char* vertex_shader =
        "#version 400 core\n"
        "layout(location = 0) in vec3 in_pos;"
        "layout(location = 1) in vec3 in_col;"
        "layout(location = 2) in vec2 in_textCoords;"
        "uniform mat4 projectionMatrix;"
        "out vec3 colour;"
        "out vec2 textCoords;"
        ""
        "void main()"
        "{"
        "	colour = in_col;"
        "	textCoords = in_textCoords;"
        "	gl_Position = projectionMatrix*vec4(in_pos, 1.0);"
        "};";

    const char* fragment_shader =
        "#version 400 core\n"
        "in vec3 colour;"
        "in vec2 textCoords;"
        "uniform sampler2D textureSamplerID;"
        "out vec4 frag_colour;"
        ""
        "void main ()"
        "{"
        "  frag_colour = vec4 (colour, 1.0)*texture( textureSamplerID, textCoords );"
        "}";

    bool checkUniform(s32 a)
    {
        if (a < 0)triggerBreakpoint();
        if (a == GL_INVALID_VALUE)triggerBreakpoint();
        if (a == GL_INVALID_OPERATION)triggerBreakpoint();
        if (a == GL_INVALID_OPERATION)triggerBreakpoint();
        return true;
    }
    
    void setUniform_projectionMatrix(const float *pMat)
    {
		// note: OGL, this accepts row-major, pre-multiplying of verts and post-multi in vertex shader.
		// ie no need to transpose if post-multi (Mv) in vertex shader.

        glUseProgram(shaderManager.get( L"posColTex").value);
        glUniformMatrix4fv(Shaders::uniformHandle_projectionMatrix, 1, false, pMat);		
        glUseProgram(0);
        GfxDevice::onGfxDeviceErrorTriggerBreakpoint();
    }

    void	Shaders::initialise()
    {
        GfxDevice::onGfxDeviceErrorTriggerBreakpoint();
        debugLog << L"GfxLowLevel::Shaders::initialise" << "\n";
        
        u32 vs = GfxDevice::createShader(vertex_shader, GL_VERTEX_SHADER);
        u32 fs = GfxDevice::createShader(fragment_shader, GL_FRAGMENT_SHADER);			
		GLuint p = GfxDevice::createProgram(vs, fs);			
		shaderManager.add(L"posColTex", ShaderWrapper{ p });

        uniforms_textureSamplerID = glGetUniformLocation(p, "textureSamplerID");
        checkUniform(uniforms_textureSamplerID);

        uniformHandle_projectionMatrix = glGetUniformLocation(p, "projectionMatrix");
        checkUniform(uniformHandle_projectionMatrix);
        GfxDevice::onGfxDeviceErrorTriggerBreakpoint();
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

            debugLog << L"Compile failure\n";
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

