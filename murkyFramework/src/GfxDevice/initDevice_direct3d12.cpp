//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/src/private/pch.hpp>
#ifdef USE_DIRECT3D12

struct Vertex
{
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT4 color;
};

namespace GfxDevice
{
    // forward declarations
    extern TextureWrapper createTestTextureObject();
    
    bool initialise_device(SystemSpecific * systemSpecific)	
    {
        
        return true;	
    }	

    bool deinitialise_device()
    {
        return true;
    }
} // namespace GfxDevice
#endif 
