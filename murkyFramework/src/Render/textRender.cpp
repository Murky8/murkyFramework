//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/src/pch.hpp>
namespace murkyFramework {
#define deviceObj  g_appDebug->render->gfxDevice
namespace Render
{	
    // Constructors
    TextRender::TextRender(GfxDevice::TextureWrapper textureRef) 
    {											            
        textTriangleBuffer = new GfxDevice::VertexBufferWrapper(
            GfxDevice::VertexType::posColTex,
            GfxDevice::PrimativeType::triangle,			
			deviceObj->shaderManager.get(L"posColTex"),
            textureRef, 16*1024);            
    }

    void drawChar( std::vector<Triangle_pct> &triangles, 
        wchar_t ch, vec2 pos, vec3 col, vec2 charScreenDim)
    {
        //Note: -32 as 32 is start of chrsa in texture
        const   auto    charloc = ( static_cast<int>(ch) ) -32;
        const   auto    charTexDim = 1.f / 16.f;
        const   auto    tix = charloc & 15, tiy = charloc >> 4;
        const   vec3    pos3(pos.x, pos.y, 0.5f);
        const   vec2    texCoord { tix*charTexDim, tiy*charTexDim };

        // ---
        //  01
        //  23
        Vert_pct vert[] 
        {
            {pos3,col,texCoord},
            {pos3,col,texCoord},
            {pos3,col,texCoord},
            {pos3,col,texCoord}
        };
        
        vert[1].pos += vec3(charScreenDim.x, 0.f, 0.f);
        vert[2].pos += vec3(0.f, charScreenDim.y, 0.f);
        vert[3].pos += vec3(charScreenDim.x, charScreenDim.y, 0.f);

        vert[1].textCoords += vec2(charTexDim, 0.f);
        vert[2].textCoords += vec2(0.f, charTexDim);
        vert[3].textCoords += vec2(charTexDim, charTexDim);

        triangles.push_back(Triangle_pct(vert[0], vert[1], vert[2]));
        triangles.push_back(Triangle_pct(vert[1], vert[3], vert[2]));        
    }

    void TextRender::drawText(const std::wstring &text)
    {        
        // cordinates: 0,0 = top left        
        vec2    cursorPos(0.f,0.f);
        auto    it = text.cbegin();
        const   vec2    charScreenDim(1.f / 40.f, 1.f / 40.f);
               
        textTris.clear();

        while (it < text.cend())
        {
            auto ch = (*it);

            if (ch == L'\n')
            {
                cursorPos.x = 0.f;
                cursorPos.y += charScreenDim.y;

            }
            else
            {
                drawChar( textTris, ch, cursorPos, vec3(1.f, 1.f, 0.f), charScreenDim );
                cursorPos += vec2(charScreenDim.x, 0.f);
            }
            ++it;
        }        

        //setProjMatOrtho(1.f, 0.f, 0.f, 1.f, GfxLowLevel::projectionMatrix);

        this->textTriangleBuffer->draw(textTris.data(), static_cast<u32>(textTris.size()));                    
    }
}
}//namespace murkyFramework