//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/src/pch.hpp>
namespace murkyFramework {
// returns -1 if unsuccessful
int getFileSize(const wchar_t *const fileName)
{
    HANDLE	find;
    WIN32_FIND_DATA	findData;

    find = FindFirstFile(fileName, &findData);
    
    if (find == INVALID_HANDLE_VALUE)
        return -1;

    if (findData.nFileSizeHigh != 0)
        return -1;

    return findData.nFileSizeLow;    
}

//http://www.jose.it-berater.org/sysinfo/pages/getfiletime.htm
long int getFileModTime(const wchar_t *const fileName)
{
    HANDLE	hFile;
    WIN32_FIND_DATA	findData;

    hFile = FindFirstFile(fileName, &findData);

    if (hFile == INVALID_HANDLE_VALUE)
        return -1;

    FILETIME ftCreate, ftAccess, ftWrite;
    SYSTEMTIME stUTC, stLocal;
    //DWORD dwRet;

    if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite))
        return -1;

    // Convert the last-write time to local time.
    FileTimeToSystemTime(&ftWrite, &stUTC);
    SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
    triggerBreakpoint();
    return -1;
    //GetFileTime();
}

namespace qdev
{    
    // current director starts at Z:\murkyDev\murkyFramework\build\Visual Studio 2013
    // but should be Z:\murkyDev\murkyFramework
    const std::wstring baseDir(L"../../");

#ifdef WINDOWS
    void setCurrentDirectoryToAppRoot()
    {        
        wchar_t name[1024];
        GetCurrentDirectoryW(1024, name);
        debugLog << name << L"\n";

        int	res = SetCurrentDirectory( baseDir.c_str() );                
        if(res == 0)
            triggerBreakpoint(L"Directory problem");        
    } 
#endif
#ifdef ANDROID
#endif

    BinaryFileLoader::BinaryFileLoader(const std::wstring &dirName, const std::wstring &fileName,
        const std::wstring &extensionName) : BinaryFileLoader
        (
        dirName + L"/" + fileName + L"." + extensionName
        )
    {
    }

    BinaryFileLoader::BinaryFileLoader( const wchar_t *const fileName )
    {
        int fileSize = getFileSize( fileName );                        
        if (fileSize == -1)
        {
            pdata = 0;
            debugLog << L"couldn't load file !\n";
            return;
        }

        dataLength = fileSize;

        FILE	*fileHandle;
        fileHandle = NULL;
        errno_t rese = _wfopen_s(&fileHandle, fileName, L"rb");
        if (fileHandle == NULL)
        {
            pdata = 0;
            debugLog << L"couldn't load file\n";
            return;
        }

        pdata = new char[dataLength];
        fread( pdata, 1, dataLength, fileHandle );
        fclose( fileHandle );

        debugLog << L"loaded file" << fileName << "\n";
        // todo: more checks please
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

}//namespace murkyFramework