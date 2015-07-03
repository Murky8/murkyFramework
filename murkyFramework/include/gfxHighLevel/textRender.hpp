//------------------------------------------------------------------------------
// 2014 J. Coelho.
// Platform:
#pragma once
#include <gfxLowLevel/textures.hpp>
#include <gfxLowLevel/gfxPrimativeTypes.hpp>
#include <gfxLowLevel/vertexBuffer.hpp>
#include <gfxLowLevel/textures.hpp>

#include <string>
#include <vector>
#include <vectorMatrix.hpp>

namespace RenderHi
{
    class TextRender
    {
    public:
        // Constructors
        TextRender();

        // Destructors
        //~TextRender();
        // Methods
        void drawText(const std::wstring &text);
        //void drawAllText();
        // Data
        
        std::vector<Triangle_pct>			textTris;
        GfxLowLevel::VertexBufferDynamic	*textTriangleBuffer;
        GfxLowLevel::TextureRef				textureRef;		
        //std::wstring text;
    private:
        // Constructors        
        // Destructors
        // Methods
        // Data                
    };		

    //void drawText(const std::wstring &text, const GfxLowLevel::TextureRef &texture);
}