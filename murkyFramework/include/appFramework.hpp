#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
#include <murkyFramework/include/gfxLowLevel/version_gfxDevice.hpp>

#include    <thread>
#include    <murkyFramework/include/types.hpp>
#include    <murkyFramework/include/inputDevices.hpp>


class AppFramework
{
    //AppFramework();
    //AppFramework(std::thread &pmainLoopThread) : mainLoopThread(pmainLoopThread);
public:
    // Constructors
    // Destructors
    // Methods
    void    initOnce();
    
    //void    resume();
    //void    saveState();
    //void    loadState();
    //void    registerMainLoopThread();
    //void    serialise(std::vector<char> &out);

    // Data
    bool    fullScreen = false;
    int     screenResY;
    int     screenResX;


    //bool    multiThreaded = true;
    u32     frameCounter = 0;    
    float   frameRateLimit = 60; // 0= inf 
    float   frameRate;
    bool    exitWholeApp = false;
    bool    gfxInitialised = false;
    bool    initialised = false;
    //std::thread gfxLoopThread; // Currently using main.cpp instead of dedicated thread
    //std::thread thread_control;
private:
    // Constructors
    // Destructors
    // Methods
    // Data
    //bool    mainLoopThreadAttached = false;
};


extern AppFramework Gapp;
