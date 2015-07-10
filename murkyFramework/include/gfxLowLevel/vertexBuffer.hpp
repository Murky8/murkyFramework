#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
#include <murkyFramework/include/version.hpp>

#include <murkyFramework/include/common.hpp>
#include <murkyFramework/include/gfxLowLevel/texture.hpp>
#include <murkyFramework/include/gfxLowLevel/shaders.hpp>

namespace GfxLowLevel
{ 
    enum class VertexType{ posCol, posColTex };
    enum class PrimativeType{ triangle, line, point };
    
    class VertexBufferDynamic
    {
    public:
        // Constructors		
        // if not using a texture, pass '0'
        VertexBufferDynamic(VertexType vertexType, PrimativeType primativeType, ShaderId shaderProgram, TextureId &texture);

        // Methods		
        void	draw(void *data, int nPrimatives);

        // Data
        //const u32			capacity?;
        const VertexType	vertexType;		
        const PrimativeType primativeType;
        const ShaderId      shaderProgram;
        const TextureId	    texture;
    private:
        u32 vbo;
        u32 vao;
        VertexBufferDynamic() = delete;
    };
  
    //--------------------------------------------------------------------------
}