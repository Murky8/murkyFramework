//------------------------------------------------------------------------------
// 2014 J. Coelho.
// Platform:
#pragma once
#include <common.hpp>

namespace GfxLowLevel
{ 
    //--------------------------------------------------------------------------
    // Vertex buffer
    enum class VertexType{ posCol, posColTex };
    enum class BufferAccessType{ dynamic };

    class VertexBufferRef   //todo rename
    {
    public:
        // Constructors
        VertexBufferRef(u32 capacity, VertexType, BufferAccessType);

        // Methods
        u32 getBufferHandle() const;

        // Data
        const u32 capacity;
        const VertexType vertexType;
        const BufferAccessType bufferAccessType;
    private:
        u32 bufferHandle;
        VertexBufferRef() = delete;
    };
    //--------------------------------------------------------------------------
}