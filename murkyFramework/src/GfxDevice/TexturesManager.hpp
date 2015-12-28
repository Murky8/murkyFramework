#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.

//todo: implement

namespace murkyFramework {
namespace GfxDevice {
    class TexturesManager 
    {
    public:

        CollectionNamed<TextureWrapper> textures;
        //virtual void loadAndAddTexture(FilePathSplit filePath) = 0;
        virtual ~TexturesManager() {}
    };
}//namespace GfxDevice
}//namespace murkyFramework