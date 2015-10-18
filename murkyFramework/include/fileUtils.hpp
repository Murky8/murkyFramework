#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
#include <murkyFramework/include/GfxDevice/version_gfxDevice.hpp>

#include <regex>
#include <wtypes.h>
#include <murkyFramework/include/common.hpp>
#include <murkyFramework/include/debugUtils.hpp>


class PathFileNameExtComponents
{
public:
	std::wstring	directoryPath;
	std::wstring	fileName;
	std::wstring	extensionName;
	std::wstring	wholePathName();

	PathFileNameExtComponents(){}
	PathFileNameExtComponents(std::wstring pathFileNameExt);

};

void visitAllFilesInDirectory(std::wstring startDir, void (*funct) (PathFileNameExtComponents), std::wregex &regexp);
void decomposeFilePath(std::wstring in_string, std::wstring &out_directoryPath,
	std::wstring &out_fileNameNaked, std::wstring &out_extensionName);

std::wstring makePathString(const std::wstring &dirName, const std::wstring &fileName,
    const std::wstring &extensionName);

u64 getFileModificationTime1601(std::wstring fileNameWithPath, bool *out_fileExists = nullptr);

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
