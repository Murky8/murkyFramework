#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.

namespace murkyFramework {
    namespace GfxDevice {
        
        class TexturesManager_vulkan : public TexturesManager
        {
        public:
            void loadAndAddTexture(FilePathSplit filePath);
        };
    }//namespace GfxDevice
}//namespace murkyFrameworkwork