#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
namespace murkyFramework {
    namespace Render {
        class Model
        {
        public:
            std::vector<Triangle_pct> tris;
            std::vector<u16> indicies;
            GfxDevice::TextureWrapper texture;
        };
    
    }//namespace Render 
}//namespace murkyFramework 