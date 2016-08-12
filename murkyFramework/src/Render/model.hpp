#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
namespace murkyFramework {
    namespace Render {
        class Model
        {
        public:
            Model() {}
            //Model(std::vector<Triangle_pct> tris, std::vector<u16> indicies, GfxDevice::TextureWrapper texture);
            std::vector<Vert_pct> vertices;
            std::vector<u16> indicies;
            GfxDevice::TextureWrapper texture{};
        };    
    }//namespace Render 
}//namespace murkyFramework 