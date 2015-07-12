#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
#include <murkyFramework/include/version.hpp>

#include <murkyFramework/include/common.hpp>
#include "murkyFramework/include/types.hpp"
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
        // capacity measures in 
        VertexBufferDynamic(
            VertexType vertexType, PrimativeType primativeType, 
            ShaderId shaderProgram, TextureId &texture,
            u32 nVerts );
        ~VertexBufferDynamic();
        // Methods		
        void	draw(void *data, int nPrimatives);

        s32 getCapacityBytes() const;
        // Data
        //const u32			capacity?;
        const VertexType	vertexType;		
        const PrimativeType primativeType;
        const ShaderId      shaderProgram;
        const TextureId	    texture;
        struct handleDeviceVB      *pHandle;
        u32   capacity;
    private:
      

#ifdef USE_DIRECT3D
           void *p;
#endif // USE_DIRECT3D
#ifdef USE_OPENGL
           //u32 vao;
           //u32 vbo;        
#endif // USE_OPENGL
        
        VertexBufferDynamic() = delete;
    };   
    //--------------------------------------------------------------------------
}