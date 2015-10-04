#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
#include <murkyFramework/include/GfxDevice/version_gfxDevice.hpp>

#include <murkyFramework/include/common.hpp>
#include <murkyFramework/include/types.hpp>
#include <murkyFramework/include/GfxDevice/texture.hpp>
#include <murkyFramework/src/GfxDevice/public/shaderId.hpp>
#include <murkyFramework/include/GfxDevice/shaders.hpp>
#include <memory>

namespace GfxDevice
{      
	class ShaderId2;

    class VertexBufferDynamic
    {
    public:
        // Constructors		
        // if not using a texture, pass '0'
        // capacity measures in 
        VertexBufferDynamic(
            VertexType vertexType, PrimativeType primativeType, 
			ShaderId_private3 shaderId, TextureId &texture,
            u32 nVerts );
        ~VertexBufferDynamic();
        // Methods		
        void	draw(void *data, u32 nPrimatives);

        s32 getCapacityBytes() const;
        // Data
        //const u32			capacity?;
        const VertexType	vertexType;		
        const PrimativeType primativeType;
		const ShaderId_private3	shaderId;
        const TextureId	    texture;
        struct handleDeviceVB *pHandle;
        u32   capacity;
    private:
              
        VertexBufferDynamic() = delete;
    };   
    //--------------------------------------------------------------------------
}