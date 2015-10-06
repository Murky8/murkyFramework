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
#include <murkyFramework/src/GfxDevice/public/textureWrappers.hpp>

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