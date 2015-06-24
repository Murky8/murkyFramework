//------------------------------------------------------------------------------
// 2014 J. Coelho.
// Platform:
#pragma once
#include <murkyFramework/include/gfxLowLevel/textures.hpp>

#include <string>

#include <vectorMatrix.hpp>



namespace RenderHi
{
    class TextRender
    {
    public:
        // Constructors
        //explicit TextRender::TextRender(std::wstring textureFileName);

        // Destructors
        //~TextRender();
        // Methods
        //void drawAllText();
        // Data
        //std::wstring text;
    private:
        // Constructors
        // TextRender() = delete;
        // Destructors
        // Methods
        // Data                
    };

    void drawText(const std::wstring &text, const GfxLowLevel::TextureRef &texture);
}