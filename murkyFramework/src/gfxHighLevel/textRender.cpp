//------------------------------------------------------------------------------
// 2015 J. Coelho.
// C++11

#include <gfxHighLevel/textRender.hpp>

//http://glm.g-truc.net/0.9.6/api/index.html
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <debugUtils.hpp>
#include <gfxLowLevel/gfxLowLevel.hpp>
#include <gfxHighLevel/render.hpp>
#include <vectorMatrix.hpp>
#include <murkyFramework/include/gfxHighLevel/projectionMat.hpp>


namespace RenderHi
{

    // Constructors
    //TextRender::TextRender(std::wstring textureFileName) : texture(textureFileName), cursorPos(-1.0f, 1.0f)
    //{    

    //}

    //// Destructors
    //TextRender::~TextRender()
    //{
    //    //this->texture
    //}

    //// Methods     
    //void TextRender::drawAllText()
    //{
    //    text.clear();
    //}
    /*
    Result[0][0] = valType(2) / (right - left);
    Result[1][1] = valType(2) / (top - bottom);
    Result[2][2] = -valType(2) / (zFar - zNear);
    Result[3][0] = -(right + left) / (right - left);
    Result[3][1] = -(top + bottom) / (top - bottom);
    Result[3][2] = -(zFar + zNear) / (zFar - zNear);
    */

    extern GfxLowLevel::TextureRef             *font_texture;    // todo temp        
    extern RenderHi::Triangles_pctSoftBuffer   *font_softBuf;    // todo temp    

    void drawChar(RenderHi::Triangles_pctSoftBuffer &triangles, wchar_t ch, vec2 pos, vec3 col, vec2 charScreenDim)
    {
        const   auto    charloc = ( static_cast<int>(ch) ) -32; //Note: -32 as 32 is start of chrsa in texture
        const   auto    charTexDim = 1.f / 16.f;
        const   auto    tix = charloc & 15, tiy = charloc >> 4;
        const   vec3    pos3(pos);
        const   vec2    texCoord { tix*charTexDim, tiy*charTexDim };

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

        triangles.addTriangle(Triangle_pct(vert[0], vert[1], vert[2]));
        triangles.addTriangle(Triangle_pct(vert[1], vert[3], vert[2]));        
    }

    void drawText(const std::wstring &text, const GfxLowLevel::TextureRef &texture)
    {
        // cordinates: 0,0 = top left        
        vec2    cursorPos(0.f,0.f);
        auto    it = text.cbegin();
        const   vec2    charScreenDim(1.f / 20.f, 1.f / 20.f);
                               
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
                drawChar(*font_softBuf, ch, cursorPos, vec3(1.f, 1.f, 0.5f), charScreenDim);
                cursorPos += vec2(charScreenDim.x, 0.f);
            }
            ++it;
        }        

        setProjMatOrtho(1.f, 0.f, 0.f, 1.f, GfxLowLevel::projectionMatrix);
        font_softBuf->drawAll();
    }
}