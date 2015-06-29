//----------------------------------------------------------------------------//
// 2015 J. Coelho.
//
// Agnostic wrapper for all low level device specific graphics functions
// Stateless, ie no current texture selected, vb, etc
////////////////////////////////////////////////////////////////
#pragma once
#include <murkyFramework/include/version.hpp>

#include <vector>

// murky framework
#include <appFramework.hpp>
#include <types.hpp>  
#include <gfxLowLevel/gfxPrimativeTypes.hpp>
#include <gfxLowLevel/vertexBuffer.hpp>
#include <gfxLowLevel/shaders.hpp>
#include <gfxLowLevel/textures.hpp>


namespace GfxLowLevel //todo rename
{     
    //Data    
    void onGfxDeviceErrorTriggerBreakpoint();
    void initialise();    
    void drawBegin();        
    void drawEnd();            
	
	
    //void drawLines_pc(VertexBufferRef_Depreciate vertexBufferRef, std::vector<Line_pc> &lines);
    //void drawTriangles_pc(VertexBufferRef_Depreciate vertexBufferRef, std::vector<Triangle_pc> &triangles);    
    //void drawTriangles_pct(VertexBufferRef_Depreciate vertexBufferRef, std::vector<Triangle_pct> &triangles, TextureRef &texture);

    // void fillBuffer_start();
    // void fillBuffer_endAndDraw();
}
    //class VertexTypeInfo //todo implement ?
    //{
    //public:
    //    u16 elementSize;  
    //};

