#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
#include <murkyFramework/include/gfxLowLevel/version_gfxDevice.hpp>

#include <string>
#include <vector>

#include <murkyFramework/include/vectorMatrix.hpp>
#include <murkyFramework/include/gfxLowLevel/gfxPrimativeTypes.hpp>
#include <murkyFramework/include/gfxLowLevel/vertexBuffer.hpp>
#include <murkyFramework/include/gfxLowLevel/texture.hpp>


namespace RenderHi
{
    class TextRender
    {
    public:
        // Constructors
        TextRender() = delete;
        TextRender( GfxLowLevel::TextureId textureRef);

        // Destructors
        //~TextRender();
        // Methods
        void drawText(const std::wstring &text);
        //void drawAllText();
        // Data
        
        std::vector<Triangle_pct>			textTris;
        GfxLowLevel::VertexBufferDynamic	*textTriangleBuffer;        
        //std::wstring text;
    private:
        // Constructors        
        // Destructors
        // Methods
        // Data                
    };		

    //void drawText(const std::wstring &text, const GfxLowLevel::TextureRef &texture);
}