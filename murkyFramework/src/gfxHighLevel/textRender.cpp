//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/version.hpp>
#include <murkyFramework/include/gfxHighLevel/textRender.hpp>

//http://glm.g-truc.net/0.9.6/api/index.html
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <murkyFramework/include/debugUtils.hpp>
#include <murkyFramework/include/gfxLowLevel/gfxLowLevel.hpp>
#include <murkyFramework/include/gfxLowLevel/texture.hpp>
#include <murkyFramework/include/gfxLowLevel/gfxPrimativeTypes.hpp>

#include <murkyFramework/include/gfxHighLevel/render.hpp>
#include <murkyFramework/include/vectorMatrix.hpp>
#include <murkyFramework/include/gfxHighLevel/projectionMat.hpp>

namespace RenderHi
{
    // Forward declarations
    extern GfxLowLevel::TextureManager textureManager;

    // Constructors
    TextRender::TextRender(GfxLowLevel::TextureId textureRef) 
    {											
        GfxLowLevel::onGfxDeviceErrorTriggerBreakpoint();
        
        textTriangleBuffer = new GfxLowLevel::VertexBufferDynamic(
            GfxLowLevel::VertexType::posColTex,
            GfxLowLevel::PrimativeType::triangle,
            GfxLowLevel::Shaders::posColText,
            textureRef);    

        GfxLowLevel::onGfxDeviceErrorTriggerBreakpoint();
    }

    // data
    // methods
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

//    extern GfxLowLevel::TextureRef             *font_texture;    // todo temp        
 //   extern RenderHi::Triangles_pctSoftBuffer   *font_softBuf;    // todo temp    

    void drawChar( std::vector<Triangle_pct> &triangles, 
        wchar_t ch, vec2 pos, vec3 col, vec2 charScreenDim)
    {
        //Note: -32 as 32 is start of chrsa in texture
        const   auto    charloc = ( static_cast<int>(ch) ) -32;
        const   auto    charTexDim = 1.f / 16.f;
        const   auto    tix = charloc & 15, tiy = charloc >> 4;
        const   vec3    pos3(pos);
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
        GfxLowLevel::onGfxDeviceErrorTriggerBreakpoint();

        // cordinates: 0,0 = top left        
        vec2    cursorPos(0.f,0.f);
        auto    it = text.cbegin();
        const   vec2    charScreenDim(1.f / 20.f, 1.f / 20.f);
               
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
                drawChar( textTris, ch, cursorPos, vec3(1.f, 1.f, 0.5f), charScreenDim );
                cursorPos += vec2(charScreenDim.x, 0.f);
            }
            ++it;
        }        

        //setProjMatOrtho(1.f, 0.f, 0.f, 1.f, GfxLowLevel::projectionMatrix);

        this->textTriangleBuffer->draw(textTris.data(), textTris.size());            
        GfxLowLevel::onGfxDeviceErrorTriggerBreakpoint();

    }
}