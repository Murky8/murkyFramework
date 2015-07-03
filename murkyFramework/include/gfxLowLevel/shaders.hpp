//------------------------------------------------------------------------------
// 2014 J. Coelho.
// Platform:
#pragma once

#include <types.hpp>
#include <gfxLowLevel/gfxLowLevel.hpp>
#include <vector>
#include <string>


namespace GfxLowLevel
{
    // forward declarations
    class ShaderId;

    namespace Shaders
    {    
        extern	u32	uniforms_textureSamplerID;		
        extern	u32	uniformHandle_projectionMatrix;
        extern	ShaderId posColText;
        void	initialise();
        void	deinitialise();
    }

    class ShaderId
    {
    public:
        u32 getHandle() const { return handle; }
        u32 handle;
    };        

    class ShaderManager
    {
        //TextureId &TextureManager::getTextureByName(const std::wstring &name);
        //void        setGfxDeviceState_currentTexture(const TextureId &texture);
        void       createShaderProgram(const std::wstring &dirName, const std::wstring &fileName);
        void    initialise();
        //void        deleteAllTextures();

        //std::vector<TextureRef> textures;
        std::map< std::wstring, GfxLowLevel::TextureId> shaderPrograms;

    private:

    };

    void setUniform_projectionMatrix(const mat4 *pMat);

    u32 createShader(const char* sourceText, u32 type);
    u32	createProgram(u32 vertexShader, u32 fragmentShader);

    class ShaderProgram
    {        
   public:
        // Constructors
        ShaderProgram::ShaderProgram(
            const std::wstring &vertShaderFileNam,
            const std::wstring &fragShaderFileNam);
        // Destructors
        // Methods
        u32 getHandle();        
        // Data
   
    private:
        // Constructors
        ShaderProgram() = delete;
        // Destructors
        // Methods
        // Data
    u32     handle;     
    };        

    
}

/*
class Shaders
{
public:    
    //--------------------------------------------------------------------------
    // Data
    enum namePrograms
    {
        TEST, LINE, QUAD, PROJECTIVE, lightZWrite, drawWithShadow,
    };

    std::vector<GLuint> programs;
    
    //--------------------------------------------------------------------------
    // Uniform Data
    struct 
    {
        GLuint idProjMat;
        GLuint Tex1;
        GLuint id_projectorProjmat;
        GLuint id_projectorLightTX;
        GLuint id_projectorTex;

        GLuint lightZWriteProjMat;
        GLuint drawWithShadowProjMat;
        GLuint drawWithShadowLightProjMat;
        GLuint shadow_texture;
        GLuint shadow_lightDepth;
    }uniforms;

    //--------------------------------------------------------------------------
    // Functions
    void   loadShaders();
    void   releaseAllResources();

private:
    bool    Shaders::createProgramFromVertexAndFragmentShder(
    const   std::wstring vertexShdrName, 
    const   std::wstring fragmentShdrName );
    GLuint  CreateShader(GLenum eShaderType, const std::string &strShaderFile2);
    GLuint   loadShader(const std::wstring fileName,GLenum shaderType);
    GLuint  CreateProgram(const std::vector<GLuint> &shaderList);

};
*/


