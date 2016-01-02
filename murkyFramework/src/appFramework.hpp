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

    struct AppFramework_initStruct
    {
        void(*main_noGfx)();
        void(*main_gfx)();
        class Game *game;
    };

    class AppFramework
    {
    public:
        // construction
        AppFramework() = delete;
        AppFramework(AppFramework_initStruct &appInit);

        // destruction
        ~AppFramework();

        // methods
        void run();
        // note: data/objects/components initialised in same order as listed below.

        // data
        bool    fullScreen;
        int     screenResY;
        int     screenResX;
        u32     frameCounter = 0;
        float   frameRateLimit = 0; // 0= inf 
        float   frameRate;        
        bool    flyCameraMode = { true };
        float   lastFrameDuration = 0.1f;
        bool    exitWholeApp = false;
        bool    initialised = false;
        std::wstring title{ L"Murky" };
        std::wstring projectDirectory{ L"" };
        std::wstring frameworkDirectory{ L"" };

        // objects/components    
        class   systemSpecific::SystemSpecificObj  *system;
        class   Render::RenderObj   *render;    // contains: gfxDevice object
        class   InputDevices    *inputDevices;
        class   Game            *game;
        class   AudioObj        *audio;

        // threads
        void (*main_noGfx)();
        void (*main_gfx)();

    private:

    };

    extern AppFramework *g_appDebug;

}//namespace murkyFramework
