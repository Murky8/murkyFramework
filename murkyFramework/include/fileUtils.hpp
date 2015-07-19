#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
#include <murkyFramework/include/gfxLowLevel/version_gfxDevice.hpp>

#include <wtypes.h>
#include <murkyFramework/include/common.hpp>
#include <murkyFramework/include/debugUtils.hpp>

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
