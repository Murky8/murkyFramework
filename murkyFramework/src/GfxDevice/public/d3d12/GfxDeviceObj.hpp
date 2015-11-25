#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
struct GfxDeviceObj_initStruct
{
    u32 screenWidth;
    u32 screenHeight;
    WindowsSpecific* windowsSpecific;
};

class GfxDeviceObj
{
public:
    GfxDeviceObj() = delete;

    GfxDeviceObj(GfxDeviceObj_initStruct *const initStruct);
    ~GfxDeviceObj();
};
