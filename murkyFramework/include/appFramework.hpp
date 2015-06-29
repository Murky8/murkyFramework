//------------------------------------------------------------------------------
// 2015 J. Coelho
// Platform: Windows 32/64
#pragma once

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
    InputDevices inputDevices;
    //void    resume();
    //void    saveState();
    //void    loadState();
    //void    registerMainLoopThread();
    //void    serialise(std::vector<char> &out);

    // Data
    bool    fullScreen = false;
    int     screenResY;
    int     screenResX ;

    //bool    multiThreaded = true;
    u32     frameCounter = 0;
    bool    exitWholeApp = false;
    bool    gfxInitialised = false;    
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
