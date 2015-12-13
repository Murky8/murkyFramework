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

    struct main_noGfx_parameters
    {
        class AppFramework *app;
        f32 deltaTime;
    };
    
    struct main_gfx_parameters
    {
        class AppFramework *app;
        f32 deltaTime;
    };

    struct AppFramework_initStruct
    {
        void(*main_noGfx)(main_noGfx_parameters* parmeters);
        void(*main_gfx)(main_gfx_parameters* parmeters);
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
        float   lastFrameDuration = 0;
        bool    exitWholeApp = false;
        bool    initialised = false;
        std::wstring title{ L"Murky" };

        // objects/components    
        class   systemSpecific::SystemSpecificObj  *system;
        class   Render::RenderObj   *render;    // contains: gfxDevice object
        class   InputDevices    *inputDevices;
        class   Game            *game;

        // threads
        void (*main_noGfx)(main_noGfx_parameters *const);
        void (*main_gfx)(main_gfx_parameters *const);

    private:

    };

    extern AppFramework *g_appDebug;

}//namespace murkyFramework
