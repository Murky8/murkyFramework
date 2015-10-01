#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
#include <murkyFramework/include/GfxDevice/version_gfxDevice.hpp>

#include <murkyFramework/include/common.hpp>
#include <murkyFramework/include/types.hpp>
#include <murkyFramework/include/GfxDevice/texture.hpp>
#include <murkyFramework/include/GfxDevice/shaders.hpp>
#include <memory>

namespace GfxDevice
{      
	class ShaderId;

    class VertexBufferDynamic
    {
    public:
        // Constructors		
        // if not using a texture, pass '0'
        // capacity measures in 
        VertexBufferDynamic(
            VertexType vertexType, PrimativeType primativeType, 
            std::wstring &shaderName, TextureId &texture,
            u32 nVerts );
        ~VertexBufferDynamic();
        // Methods		
        void	draw(void *data, u32 nPrimatives);

        s32 getCapacityBytes() const;
        // Data
        //const u32			capacity?;
        const VertexType	vertexType;		
        const PrimativeType primativeType;
		ShaderId			*pShaderId;
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