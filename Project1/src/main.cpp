#include <murkyFramework/src/pch.hpp>

int main() // can't be in a namespace :(
{
    //skool();    
    murkyFramework::AppFramework *const app = new murkyFramework::AppFramework;

#ifdef WINDOWS
    murkyFramework::systemSpecific::WindowsSpecific* windowsSpecific{
        dynamic_cast<murkyFramework::systemSpecific::WindowsSpecific*>(app->system) };

    //note: windows msg loop inactive until following is set. needed to pass info between 
    SetWindowLongPtr(windowsSpecific->gethWnd(), GWLP_USERDATA, (LONG_PTR)app);

    windowsSpecific->main2(app);

    delete app;
#endif

#ifdef ANDROID
#endif

    murkyFramework::debugLog << L"Finished\n";
}