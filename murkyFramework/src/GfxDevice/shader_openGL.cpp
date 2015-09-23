//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/version.hpp>
#include <murkyFramework/include/GfxDevice/version_gfxDevice.hpp>
#ifdef USE_OPENGL

#include <vector>
#include <murkyFramework/include/GfxDevice/gfxLowLevel.hpp>
#include <murkyFramework/include/GfxDevice/shaders.hpp>
#include <murkyFramework/include/debugUtils.hpp>
#include <murkyFramework/include/loadSaveFile.hpp>

#include <external/glew/include/GL/glew.h> 
#include <external/glew/include/GL/wglew.h>

namespace GfxDevice
{    	
    
    //--------------------------------------------------------------------------
    // forward declarations

    extern   void GfxDevice::onGfxDeviceErrorTriggerBreakpoint();
    u32     createShader(const char* sourceText, u32 type);
    u32	    createProgram(u32 vertexShader, u32 fragmentShader);   

    namespace Shaders
    {

        u32		uniforms_textureSamplerID;
        u32     uniformHandle_projectionMatrix;               
        ShaderId posColText;        
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

        glUseProgram(Shaders::posColText.getHandle());
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
        GLuint handle = (u32)GfxDevice::createProgram(vs, fs);        

        Shaders::posColText.handle = handle;

        uniforms_textureSamplerID = glGetUniformLocation(Shaders::posColText.getHandle(), "textureSamplerID");
        checkUniform(uniforms_textureSamplerID);

        uniformHandle_projectionMatrix = glGetUniformLocation(Shaders::posColText.getHandle(), "projectionMatrix");
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



    //void loadAndCreateShaderFromFile(const std::wstring fileName, const GLenum newShaderType, GLuint &out_shader)
 //   {
 //       std::wstring s(L"hello\n");
 //       debugLog << fileName << L"\n";

 //       onGfxDeviceErrorTriggerBreakpoint();

 //       // Load source code file
 //       qdev::BinaryFileLoader srcText(fileName);

 //       // Initialize new shader    
 //       GLuint  newShader = glCreateShader(newShaderType);

 //       // Compile shader        
 //       const char *pchar = srcText.data();
 //       int len[] { srcText.getDataLength() };

 //       glShaderSource(newShader, 1, &pchar, len);
 //       glCompileShader(newShader);

 //       // Check shader     
 //       GLint status;
 //       glGetShaderiv(newShader, GL_COMPILE_STATUS, &status);
 //       if (status == GL_FALSE)
 //       {
 //           GLint infoLogLength;
 //           glGetShaderiv(newShader, GL_INFO_LOG_LENGTH, &infoLogLength);

 //           GLchar *strInfoLog = new GLchar[infoLogLength + 1];
 //           glGetShaderInfoLog(newShader, infoLogLength, NULL, strInfoLog);

 //           debugLog << L"Compile failure\n";
 //           debugLog << strInfoLog << L"\n";
 //           triggerBreakpoint();

 //           delete[] strInfoLog;
 //       }
 //       // Check shader     

 //       onGfxDeviceErrorTriggerBreakpoint();
 //       out_shader = newShader;        
 //   }

    //void createProgram(const std::vector<GLuint> &in_shaderList, GLuint &out_program)
 //   {
 //       out_program = glCreateProgram();

 //       for (size_t iLoop = 0; iLoop < in_shaderList.size(); iLoop++)
 //                   glAttachShader(out_program, in_shaderList[iLoop]);

 //       //for( GLuint shader: in_shaderList)
 //       //{
 //           //glAttachShader(out_program, shader);
 //       //}
 //     
 //       glLinkProgram(out_program);
 //       
 //       GLint status;
 //       glGetProgramiv(out_program, GL_LINK_STATUS, &status);
 //       if(status == GL_FALSE)
 //       {
 //           GLint infoLogLength;
 //           glGetProgramiv(out_program, GL_INFO_LOG_LENGTH, &infoLogLength);
 //       
 //           GLchar *strInfoLog = new GLchar[infoLogLength + 1];
 //           glGetProgramInfoLog(out_program, infoLogLength, NULL, strInfoLog);
 //       
 //           debugLog << L"Compile failure\n";
 //           debugLog << strInfoLog << L"\n";
 //           delete[] strInfoLog;
 //           triggerBreakpoint();
 //       }
 //       
 //       for(size_t iLoop = 0; iLoop < in_shaderList.size(); iLoop++)
 //           glDetachShader(out_program, in_shaderList[iLoop]);                
 //   }

    
  //  ShaderProgram::ShaderProgram(
  //      const std::wstring &vertShaderFileName,
  //      const std::wstring &fragShaderFileName )
  //  {    
  //      std::vector<GLuint> shaders;

  //      //debugLog << vertShaderFileName << L" moof \n";

  //      GLuint shader;

  //      loadAndCreateShaderFromFile(vertShaderFileName, GL_VERTEX_SHADER, shader);
  //      shaders.push_back(shader);

  //      loadAndCreateShaderFromFile(fragShaderFileName, GL_FRAGMENT_SHADER, shader);
  //      shaders.push_back(shader);

  //      createProgram(shaders, this->handle);
  //  }    

  //  u32 ShaderProgram::getHandle() { return handle; }
  //  
  //  void loadAllShaders()
  //  {
  //      GfxLowLevel::onGfxDeviceErrorTriggerBreakpoint();
        //
        //shaderProgram_line_pc       = new ShaderProgram(L"src/shaders/test.vsh", L"src/shaders/test.fsh");
  //      shaderProgram_posColText    = new ShaderProgram(L"src/shaders/posColTex.vsh", L"src/shaders/posColTex.fsh");

  //      // Note: 'Tex1' location etc is consistant between all programs
  //      {
  //          auto loc = glGetUniformLocation(shaderProgram_posColText->getHandle(), "Tex1");
  //          checkUniform(loc);
  //          onGfxDeviceErrorTriggerBreakpoint();
  //          GfxLowLevel::uniforms_texture = loc;
  //      }
  //      {
  //          auto loc = glGetUniformLocation(shaderProgram_posColText->getHandle(), "projMatrix");
  //          checkUniform(loc);
  //          onGfxDeviceErrorTriggerBreakpoint();
  //          GfxLowLevel::uniforms_projMatrix = loc;
  //      }
  //  }
    //GLuint Shaders::CreateProgram(const std::vector<GLuint> &shaderList)	
    //{
    //    Wopengl::getAndProcessGLError();
    //    GLuint program = glCreateProgram();
    //
    //    for(size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
    //        glAttachShader(program, shaderList[iLoop]);
    //
    //    glLinkProgram(program);
    //
    //    GLint status;
    //    glGetProgramiv (program, GL_LINK_STATUS, &status);
    //    if (status == GL_FALSE)
    //    {
    //        GLint infoLogLength;
    //        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
    //
    //        GLchar *strInfoLog = new GLchar[infoLogLength + 1];
    //        glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
    //
    //        OutputDebugString(L"Linker failure\n");												   
    //        triggerBreakpoint()		
    //        delete[] strInfoLog;
    //    }
    //
    //    for(size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
    //        glDetachShader(program, shaderList[iLoop]);
    //
    //    Wopengl::getAndProcessGLError();
    //
    //    return program;
    //}

    //bool Shaders::createProgramFromVertexAndFragmentShder(
    //    const std::wstring vertexShdrName, 
    //    const std::wstring fragmentShdrName )
    //{
    //    std::vector<GLuint> shaderList;
    //    GLuint shader, program;      
    //    const std::wstring baseDir(L"src//shaders//");
    //                     
    //    shader = loadShader(baseDir +vertexShdrName, GL_VERTEX_SHADER);
    //
    //    shaderList.push_back(shader);
    //    shader = loadShader(baseDir +fragmentShdrName, GL_FRAGMENT_SHADER);
    //    
    //    shaderList.push_back(shader);
    //
    //    program = CreateProgram(shaderList);
    //    this->programs.push_back(program);
    //                     
    //    std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
    //
    //    return true;
    //}


//const char *s2 = "#version 400 core \n "
//"layout(location = 0) in vec3 position; \n "
//"layout(location = 1) in vec3 colour; \n "
//"layout(location = 2) in vec2 textCoordsIn; \n "
//"out vec3 colourx; \n "
//"out vec2 textCoords;";



//
//
//bool checkUniform(int a)
//{
//    if(a<0)triggerBreakpoint();
//    if(a == GL_INVALID_VALUE)triggerBreakpoint();
//    if(a == GL_INVALID_OPERATION)triggerBreakpoint();
//    if(a == GL_INVALID_OPERATION)triggerBreakpoint();
//    return true;
//}
//
//void Shaders::loadShaders()
//{
//    createProgramFromVertexAndFragmentShder(L"test.vsh", L"test.fsh");
//    uniforms.Tex1 = glGetUniformLocation(programs[namePrograms::TEST], "Tex1");
//
//    createProgramFromVertexAndFragmentShder(L"line.vsh", L"line.fsh");
//    // Get uniform vars    
//    if((uniforms.idProjMat = glGetUniformLocation(programs[namePrograms::LINE],"projectionMatrix")) <0)
//            triggerBreakpoint();
//    
//    createProgramFromVertexAndFragmentShder(L"quad.vsh", L"quad.fsh");
//
//    createProgramFromVertexAndFragmentShder(L"projector.vsh", L"projector.fsh");
//    // Get uniform vars    
//    if((uniforms.id_projectorProjmat = glGetUniformLocation(programs[namePrograms::PROJECTIVE],"projectionMatrix2")) <0)    
//        triggerBreakpoint();
//
//    if((uniforms.id_projectorLightTX = glGetUniformLocation(programs[namePrograms::PROJECTIVE],"lightProjMatrix")) <0)    
//        triggerBreakpoint();
//
//    if((uniforms.id_projectorTex = glGetUniformLocation(programs[namePrograms::PROJECTIVE], "Tex")) <0) 
//        triggerBreakpoint();
//                          
//    createProgramFromVertexAndFragmentShder(L"lightZWrite.vsh", L"lightZWrite.fsh");        
//    // Get uniform vars    
//    uniforms.lightZWriteProjMat = glGetUniformLocation(programs[namePrograms::lightZWrite],"projectionMatrix3");
//    checkUniform(uniforms.lightZWriteProjMat);
//        
//    createProgramFromVertexAndFragmentShder(L"drawWithShadow.vsh", L"drawWithShadow.fsh");        
//    // Get uniform vars    
//    uniforms.drawWithShadowProjMat = glGetUniformLocation(programs[namePrograms::drawWithShadow],"projectionMat4");
//    checkUniform(uniforms.drawWithShadowProjMat);
//    uniforms.drawWithShadowLightProjMat = glGetUniformLocation(programs[namePrograms::drawWithShadow],"lightProjectionMat");
//    checkUniform(uniforms.drawWithShadowLightProjMat);
//
//    checkUniform(
//        uniforms.shadow_texture = glGetUniformLocation(programs[namePrograms::drawWithShadow], "text")
//        );
//
//    checkUniform(
//        uniforms.shadow_lightDepth = glGetUniformLocation(programs[namePrograms::drawWithShadow], "depth")
//        );    
//}
//
//bool Shaders::createProgramFromVertexAndFragmentShder(
//    const std::wstring vertexShdrName, 
//    const std::wstring fragmentShdrName )
//{
//    std::vector<GLuint> shaderList;
//    GLuint shader, program;      
//    const std::wstring baseDir(L"src//shaders//");
//                     
//    shader = loadShader(baseDir +vertexShdrName, GL_VERTEX_SHADER);
//
//    shaderList.push_back(shader);
//    shader = loadShader(baseDir +fragmentShdrName, GL_FRAGMENT_SHADER);
//    
//    shaderList.push_back(shader);
//
//    program = CreateProgram(shaderList);
//    this->programs.push_back(program);
//                     
//    std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
//
//    return true;
//}
//
//GLuint Shaders::loadShader(const std::wstring fileName, GLenum shaderType)
//{
//    char *shaderSource;
//    int	shaderSourceLen;
//
//    Misc::loadFileBinary(fileName.c_str(), shaderSource, shaderSourceLen);
//
//    std::string shaderStr( shaderSource, shaderSourceLen );		
//    delete [] shaderSource;
//
//    return CreateShader(shaderType, shaderStr);
//}
//
////http://thisdir/info/createshader.html
//GLuint Shaders::CreateShader(GLenum eShaderType, const std::string &strShaderFile2)
//{
//    Wopengl::getAndProcessGLError();
//
//    GLuint shader = glCreateShader(eShaderType);
//    const char *strFileData = strShaderFile2.c_str();
//    glShaderSource(shader, 1, &strFileData, NULL);
//
//    glCompileShader(shader);
//
//    GLint status;
//    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
//    if(status == GL_FALSE)
//    {
//        GLint infoLogLength;
//        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
//
//        GLchar *strInfoLog = new GLchar[infoLogLength + 1];
//        glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);
//
//        const char *strShaderType = NULL;
//        switch(eShaderType)
//        {
//        case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
//        case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
//        case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
//        }
//
//        OutputDebugString(L"Compile failure\n");
//        debugLogc("%s", strInfoLog );
//
//        triggerBreakpoint();
//
//        delete[] strInfoLog;
//    }
//
//    Wopengl::getAndProcessGLError();
//
//    return shader;
//}
//
///*
//GLuint Shaders::LoadAndCreateShader(GLenum eShaderType, const std::string &fileName)
//{
//    //Misc::loadFileBinary( )
//    //GLuint CreateShader(GLenum eShaderType, const std::string &strShaderFile)
//    return 0;
//}
//  */
//GLuint Shaders::CreateProgram(const std::vector<GLuint> &shaderList)	
//{
//    Wopengl::getAndProcessGLError();
//    GLuint program = glCreateProgram();
//
//    for(size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
//        glAttachShader(program, shaderList[iLoop]);
//
//    glLinkProgram(program);
//
//    GLint status;
//    glGetProgramiv (program, GL_LINK_STATUS, &status);
//    if (status == GL_FALSE)
//    {
//        GLint infoLogLength;
//        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
//
//        GLchar *strInfoLog = new GLchar[infoLogLength + 1];
//        glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
//
//        OutputDebugString(L"Linker failure\n");												   
//        triggerBreakpoint()		
//        delete[] strInfoLog;
//    }
//
//    for(size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
//        glDetachShader(program, shaderList[iLoop]);
//
//    Wopengl::getAndProcessGLError();
//
//    return program;
//}
//
//void Shaders::releaseAllResources()
//{
//    for_each(programs.begin(), programs.end(), glDeleteProgram);
//}

