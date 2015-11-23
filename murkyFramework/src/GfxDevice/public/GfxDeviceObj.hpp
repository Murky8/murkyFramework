#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11

class GfxDeviceObj
{
    public:
    GfxDeviceObj();
    ~GfxDeviceObj();

    int     a;
    HDC		hDC = nullptr;
};

