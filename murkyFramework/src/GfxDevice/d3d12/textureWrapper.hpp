#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11

namespace GfxDevice
{
    class TextureWrapper
    {
    public:
        u32 iTexture;
        Microsoft::WRL::ComPtr<ID3D12Resource> deviceTexture;
    };
}
