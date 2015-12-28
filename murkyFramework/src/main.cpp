//------------------------------------------------------------------------------
#include <murkyFramework/src/pch.hpp>

namespace murkyFramework {
    // external forward declarations
    // the only global. for debugging/development purposes.
    AppFramework *g_appDebug = nullptr;
    
    // teaching
    void skool();   

    void compileResources()
    {        
        std::wregex regexp{ L"FBX" };
        visitAllFilesInDirectory(L"murkyFramework/data", compileFBX, regexp);        
    }
    //------------------------------------------------------------------------------
    // 
}//namespace murkyFramework

#ifdef wdwdq
    int main_old() // can't be in a namespace :(
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
        return 0;
    }

#endif
          /*
          Severity	Code	Description	Project	File	Line
Error	C2664	
'void murkyFramework::systemSpecific::WindowsSpecific::main2(murkyFramework::systemSpecific::AppFramework *const )':
cannot convert argument 1 from
'murkyFramework::AppFramework *const ' 
to
'murkyFramework::systemSpecific::AppFramework *const '	
murkyFramework	
D:\Wicked Dev\dev c++\murkyFramework\src\main.cpp	68

          */