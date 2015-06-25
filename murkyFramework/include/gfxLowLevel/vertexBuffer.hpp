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
	
	class VertexBufferDynamic
	{
	public:
		// Constructors		
		VertexBufferDynamic(VertexType vertexType, u32 capacity);

		// Methods		

		// Data
		const u32 capacity;
		const VertexType vertexType;		
	private:
		u32 vbo;
		u32 vao;
		VertexBufferDynamic() = delete;
	};

    class VertexBufferRef_Depreciate   //todo rename
    {
    public:
        // Constructors
        VertexBufferRef_Depreciate(u32 capacity, VertexType, BufferAccessType);

        // Methods
        u32 getBufferHandle() const;

        // Data
        const u32 capacity;
        const VertexType vertexType;
        const BufferAccessType bufferAccessType;
    private:
        u32 bufferHandle;
        VertexBufferRef_Depreciate() = delete;
    };
    //--------------------------------------------------------------------------
}