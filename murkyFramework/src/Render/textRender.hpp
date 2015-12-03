#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
namespace murkyFramework {}//namespace murkyFramework
namespace Render
{
    class TextRender
    {
    public:
        // constructors
        TextRender() = delete;
        TextRender(GfxDevice::TextureWrapper textureRef);

        // methods
        void drawText(const std::wstring &text);

        // data        
        std::vector<Triangle_pct>				textTris;
        GfxDevice::VertexBufferWrapper	*textTriangleBuffer;                
    private:        
    };		

    //void drawText(const std::wstring &text, const GfxLowLevel::TextureRef &texture);
}