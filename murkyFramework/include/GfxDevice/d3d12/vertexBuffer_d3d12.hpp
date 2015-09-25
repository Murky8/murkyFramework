#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
#include <murkyFramework/include/GfxDevice/version_gfxDevice.hpp>

#include <murkyFramework/include/common.hpp>
#include <murkyFramework/include/types.hpp>
#include <murkyFramework/include/GfxDevice/texture.hpp>
#include <murkyFramework/include/GfxDevice/shaders.hpp>

namespace GfxDevice
{      
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
        struct handleDeviceVB *pHandle;
        u32   capacity;
    private:
      

#ifdef USE_DIRECT3D11
           
#endif // USE_DIRECT3D11
#ifdef USE_OPENGL
           //u32 vao;
           //u32 vbo;        
#endif // USE_OPENGL
        
        VertexBufferDynamic() = delete;
    };   
    //--------------------------------------------------------------------------
}