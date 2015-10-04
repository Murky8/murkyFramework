#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
#include <murkyFramework/include/GfxDevice/version_gfxDevice.hpp>

#include <string>
#include <vector>

#include <murkyFramework/include/vectorMatrix.hpp>
#include <murkyFramework/include/GfxDevice/gfxPrimativeTypes.hpp>
#include <murkyFramework/include/GfxDevice/vertexBuffer.hpp>
#include <murkyFramework/include/GfxDevice/texture.hpp>

namespace Render
{
    class TextRender
    {
    public:
        // Constructors
        TextRender() = delete;
        TextRender( GfxDevice::TextureId textureRef);

        // Destructors
        //~TextRender();
        // Methods
        void drawText(const std::wstring &text);
        //void drawAllText();
        // Data
        
        std::vector<Triangle_pct>			textTris;
        GfxDevice::VertexBufferDynamic	*textTriangleBuffer;        
        //std::wstring text;
    private:
        // Constructors        
        // Destructors
        // Methods
        // Data                
    };		

    //void drawText(const std::wstring &text, const GfxLowLevel::TextureRef &texture);
}