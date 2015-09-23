#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
#include <murkyFramework/include/GfxDevice/version_gfxDevice.hpp>

#include    <thread>
#include    <murkyFramework/include/types.hpp>
#include    <murkyFramework/include/inputDevices.hpp>


class AppFramework
{
public:
 
    void    initOnce();
 
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

private:
    
};


extern AppFramework Gapp;
