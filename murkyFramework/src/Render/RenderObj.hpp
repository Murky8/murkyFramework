#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
namespace murkyFramework {
class RenderObj
{
public:
    // construction
    RenderObj() = delete;
    RenderObj(GfxDevice::GfxDeviceObj_initStruct  *const initStruct);
    // destruction
    ~RenderObj();

    // methods    

    // objects/components
    GfxDevice::GfxDeviceObj *gfxDevice;

    // data
    //vec4 cursorPos{ 0, 0, -5.f };
    //mat3 cursorOri{ unit };
};
}//namespace murkyFramework