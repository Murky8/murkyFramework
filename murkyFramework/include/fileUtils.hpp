//------------------------------------------------------------------------------
// 2015 J. Coelho.
// C++11. Windows. /*Android.*/
// pragma once
#include <wtypes.h>

#include <version.hpp>
#include <common.hpp>
#include <debugUtils.hpp>

class FileDirectoryWalker
{
public:
    // Constructors
    FileDirectoryWalker(const std::wstring &dirName, const std::wstring &filterRegex);
    // Destructors
    // Methods
    bool findNext(std::wstring &outString);
    // Data
    int nFilesFound = 0;
private:
    // Constructors
    FileDirectoryWalker() = delete;
    // Destructors
    // Methods
    // Data
    HANDLE fileHandle = nullptr;
    std::wstring path;
};
