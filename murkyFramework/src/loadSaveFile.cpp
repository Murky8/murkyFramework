//------------------------------------------------------------------------------
// 2014 J. Coelho.
// Platform: Windows.
#include <windows.h>
#include <string>

#include <common.hpp>
#include <loadSaveFile.hpp>
#include <debugUtils.hpp>   

namespace
{
    //std::wstring basePathName;
}

//------------------------------------------------------------------------------
// Returns false if file doesn't exist
// TODO: Doesn't handle files more than 4gig
// Trying out simpl pimpl implementation


int getFileSize(const wchar_t *const fileName)
{
    HANDLE	find;
    WIN32_FIND_DATA	findData;

    find = FindFirstFile(fileName, &findData);
    
    if (find == INVALID_HANDLE_VALUE)
        triggerBreakpoint();

    if (findData.nFileSizeHigh != 0)
        triggerBreakpoint();

    return findData.nFileSizeLow;    
}

namespace qdev
{    
    // current director starts at Z:\murkyDev\murkyFramework\build\Visual Studio 2013
    // but should be Z:\murkyDev\murkyFramework
    const std::wstring baseDir(L"../../");

    void setCurrentDirectoryToAppRoot()
    {        
        wchar_t name[1024];
        GetCurrentDirectoryW(1024, name);
        debugLog << name << L"\n";

        int	res = SetCurrentDirectory( baseDir.c_str() );                
        if(res == 0)
            triggerBreakpoint(L"Directory problem");        
    }   

    BinaryFileLoader::BinaryFileLoader( const wchar_t *const fileName )
    {
        dataLength = getFileSize( fileName );                        
        pdata      = new char[dataLength];

        FILE	*fileHandle;
        fileHandle = NULL;
        errno_t rese = _wfopen_s(&fileHandle, fileName, L"rb");
        if (fileHandle == NULL)
            triggerBreakpoint(L"File problem");

        fread( pdata, 1, dataLength, fileHandle );
        fclose( fileHandle );
    }

    BinaryFileLoader::BinaryFileLoader( const std::wstring &fileName ) :
        BinaryFileLoader( fileName.c_str() )
    {
    }

    //------------------------------------------------------------------------
    char *BinaryFileLoader::data() 
    {        
        return pdata;
    }
    
    int BinaryFileLoader::getDataLength()const
    {     
        return dataLength;
    }
        
    

    BinaryFileLoader::~BinaryFileLoader()
    {
        delete[] pdata;            
    }
}
