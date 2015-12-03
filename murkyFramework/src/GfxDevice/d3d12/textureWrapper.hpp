#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
namespace murkyFramework {}//namespace murkyFramework
namespace GfxDevice
{
    class TextureWrapper
    {
    public:
        u32 iTexture;
        Microsoft::WRL::ComPtr<ID3D12Resource> deviceTexture;
    };
}
