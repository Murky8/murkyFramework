#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
namespace murkyFramework {}//namespace murkyFramework
class FilePathSplit
{
public:
	std::wstring	directoryPath;
	std::wstring	fileName;
	std::wstring	extensionName;
	std::wstring	getJoinedFilePath();

	FilePathSplit(){}
	FilePathSplit(std::wstring pathFileNameExt);

};

void visitAllFilesInDirectory(std::wstring startDir, void (*funct) (FilePathSplit), std::wregex &extensionName_regex);
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
