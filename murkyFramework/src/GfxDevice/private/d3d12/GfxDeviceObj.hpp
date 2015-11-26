#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11. openGL4

//namespace GfxDevice
//{
    using namespace DirectX;
    using namespace Microsoft::WRL;


    // forward declarations
    void WaitForPreviousFrame();

    inline void ThrowIfFailed(HRESULT hr)
    {
        if (FAILED(hr))
        {
            throw;
        }
    }
//}