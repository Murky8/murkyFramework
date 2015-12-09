//------------------------------------------------------------------------------
//  ??????????????????
//  ?  2015 J.Coelho ?
//  ??????????????????

#include <murkyFramework/src/pch.hpp>

namespace murkyFramework {
    // external forward declarations
    // the only global. for debugging/development purposes.
    AppFramework *g_appDebug = nullptr;

    std::vector<Triangle_pct> gdeb_tris;// todo: remove

    // testing
    void skool();

    void compileFBX(FilePathSplit pathSplit)
    {
        std::wstring binPath = makePathString(pathSplit.directoryPath, pathSplit.fileName, L"bin");

        bool binExsists;
        u64 fbxModTime = getFileModificationTime1601(pathSplit.getJoinedFilePath());
        u64 binModTime = getFileModificationTime1601(binPath, &binExsists);

        if (fbxModTime > binModTime || binExsists == false)
        {// compile to .bin
            debugLog << L"bin is not current. compiling \n";
            murkyFramework::loadFBX_tris(pathSplit.getJoinedFilePath(), gdeb_tris);
            murkyFramework::serializeTris(binPath, gdeb_tris);
        }
        else
        {
            debugLog << L"bin is current \n";
            murkyFramework::deserializeTris(binPath, gdeb_tris);
        }
    }

    void compileResources()
    {
        // scan directory	
    //	visitAllFilesInDirectory(L"data", pr);
    //	exit(0);
        std::wregex regexp{ L"FBX" };
        visitAllFilesInDirectory(L"data", compileFBX, regexp);

        murkyFramework::done = true;
    }
    //------------------------------------------------------------------------------
    // 
}//namespace murkyFramework

    int main() // can't be in a namespace :(
    {
        //skool();    
        murkyFramework::AppFramework *const app = new murkyFramework::AppFramework;
        murkyFramework::g_appDebug = app;

        murkyFramework::compileResources();// move this!	            

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