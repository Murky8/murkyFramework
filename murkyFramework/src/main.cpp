//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/src/private/pch.hpp>

// external forward declarations

// forward declarations
void mainLoop_threadMain(AppFramework  *const app);

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
        
    if (fbxModTime>binModTime || binExsists == false)
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
    std::wregex regexp {L"FBX"};
    visitAllFilesInDirectory(L"data", compileFBX, regexp);

    murkyFramework::done = true;
}

//------------------------------------------------------------------------------
// 
int main()
{    
    //skool();    
    AppFramework *const app = new AppFramework;
    g_appDebug = app;
       
    compileResources();// move this!	            
    
    SetWindowLongPtr( //note: windows msg loop inactive until following is set.
        (dynamic_cast<WindowsSpecific*>(app->systemSpecific))->gethWnd(), 
        GWLP_USERDATA, 
        (LONG_PTR)app); 

    while (app->exitWholeApp==false)
    {
    MSG		msg;
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {	// Is There A Message Waiting?        
            if (msg.message == WM_QUIT)
            {				// Have We Received A Quit Message?
                app->exitWholeApp = TRUE;							// If So done=TRUE
            }
            else
            {
                TranslateMessage(&msg);				// Translate The Message
                DispatchMessage(&msg);				// Dispatch The Message
            }
        }
        else
        {										// If There Are No Messages                                
            mainLoop_threadMain(app);
        }
    }

    delete app;    
    debugLog << L"Finished\n";
}
