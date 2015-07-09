//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/version.hpp>
#include <murkyFramework/include/inputDevices.hpp>

#include <version.hpp>
#include <windows.h>


   
// Constructors
InputDevices::InputDevices()
{    
}

 
bool InputDevices::keyStatus(InputDevices::KeyCode iKey)
{
#ifdef _WINDOWS
    if (GetAsyncKeyState(static_cast<int>(iKey)) != 0)
        return true;
    else
        return false;
#endif

    //#ifdef ANDROID
    //#endif
};
