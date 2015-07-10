//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/version.hpp>

#include <murkyFramework/include/fileUtils.hpp>
#include <murkyFramework/include/common.hpp>
#include <murkyFramework/include/debugUtils.hpp>
    
FileDirectoryWalker::FileDirectoryWalker(const std::wstring &dirName, const std::wstring &filterRegex)
{
    WIN32_FIND_DATA	findData;
    ZeroMemory(&findData, sizeof(findData));
    path = dirName;   
}

bool FileDirectoryWalker::findNext(std::wstring &outString)
{
    WIN32_FIND_DATA	findData;
    ZeroMemory(&findData, sizeof(findData));
    
    if (nFilesFound == 0)
    {// 1st pass only
        fileHandle = FindFirstFile(path.c_str(), &findData);
        if (fileHandle != INVALID_HANDLE_VALUE)
        {//matched a file
            outString = findData.cFileName;
            ++nFilesFound;
            return true;
        }
        else
        {
            return false;            
        }
    }
    else
    {// Not 1st pass
        auto result = FindNextFile(fileHandle, &findData);

        if (result != 0)
        {//matched a file            
            outString = findData.cFileName;
            ++nFilesFound;
            return true;
        }
        else
        {
            return false;
        }
    }
}
