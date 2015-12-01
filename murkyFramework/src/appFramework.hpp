#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.

class AppFramework
{
public:
    // construction
    AppFramework();

    // destruction
    ~AppFramework();
     
    // note: data/objects/components initialised in same order as listed below.

    // data
    bool    fullScreen;
    int     screenResY;
    int     screenResX;    
    u32     frameCounter = 0;    
    float   frameRateLimit = 0; // 0= inf 
    float   frameRate;
    bool    exitWholeApp = false;    
    bool    initialised = false;
    std::wstring title{ L"Murky" };

    // objects/components    
    SystemSpecific  *systemSpecific;
    RenderObj       *render;    // contains: gfxDevice object
    InputDevices    *inputDevices;
    Game            *game;
    
private:
    
};

// global used only for debuggering and developing purposes.
class AppFramework_pointer
{
    u32 nReads = 0;
    u32 nWrites = 0;

};

extern AppFramework *g_appDebug;
