#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.

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
            ShaderId2 shaderProgram, TextureId &texture,
            u32 nVerts );
        ~VertexBufferDynamic();
        // Methods		
        void	draw(void *data, int nPrimatives);

        s32 getCapacityBytes() const;
        // Data
        //const u32			capacity?;
        const VertexType	vertexType;		
        const PrimativeType primativeType;
        ShaderId2      shaderProgram;
        const TextureId	    texture;
        struct handleDeviceVB *pHandle;
        u32   capacity;
    private:             
        VertexBufferDynamic() = delete;
    };   
    //--------------------------------------------------------------------------
}