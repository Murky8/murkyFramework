#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
namespace murkyFramework
{
    //forward declarations
    namespace systemSpecific
    {
        class SystemSpecificObj;
    }
    namespace Render
    {
        class RenderObj;
    }

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
        class   systemSpecific::SystemSpecificObj  *system;
        class   Render::RenderObj   *render;    // contains: gfxDevice object
        class   InputDevices    *inputDevices;
        class   Game            *game;

    private:

    };

    extern AppFramework *g_appDebug;

}//namespace murkyFramework
