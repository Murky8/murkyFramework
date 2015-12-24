#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
namespace murkyFramework {
    namespace GfxDevice
    {
        class VertexBufferWrapper
        {
        public:
            // Constructors		
            // if not using a texture, pass '0'
            // capacity measures in 		
            VertexBufferWrapper(
                VertexType vertexType, PrimativeType primativeType,
                ShaderWrapper shaderId, TextureWrapper texture,
                u32 nVerts);
            // Methods		
            void	draw(void *data, u32 nPrimatives);

            s32 getCapacityBytes() const;
            // Data
            //const u32			capacity?;
            const VertexType		vertexType;
            const PrimativeType		primativeType;
            const ShaderWrapper		shaderId;
            const TextureWrapper	texture;
            Microsoft::WRL::ComPtr<ID3D12Resource> m_vertexBuffer;
            D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
            u32   capacity;
        private:
        };

        class VertexIndexBufferWrapper
        {
        public:
            // Constructors		
            // if not using a texture, pass '0'
            // capacity measures in 		
            VertexIndexBufferWrapper(
                VertexType vertexType, PrimativeType primativeType,
                ShaderWrapper shaderId, TextureWrapper texture,
                u32 nVerts, u32 nIndicies);
            // Methods		
            
            void	draw(void *vertices, u16 nVertices, u16 *indices, u32 nPrimatives);
            s32 getVertexBufCapacityBytes() const;
            s32 getIndexBufCapacityBytes() const;
            // Data
            //const u32			capacity?;
            const VertexType		vertexType;
            const PrimativeType		primativeType;
            const ShaderWrapper		shaderId;
            const TextureWrapper	texture;
            Microsoft::WRL::ComPtr<ID3D12Resource> m_vertexBuffer;
            D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
            Microsoft::WRL::ComPtr<ID3D12Resource> m_indexBuffer;
            D3D12_INDEX_BUFFER_VIEW m_indexBufferView;
            u32   vertexBufElementCapacity;
            u32   indexBufElementCapacity;
        private:
        };

    }//namespace GfxDevice
}//namespace murkyFramework
