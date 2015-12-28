#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
namespace murkyFramework {
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
    FileDirectoryWalker(const std::wstring &dirName, const std::wstring &filterRegex);
    
    // methods
    bool findNext();
    // data
    int nFilesFound = 0;
    WIN32_FIND_DATA	findData = { 0 };
private:
    // constructors
    FileDirectoryWalker() = delete;
    
    HANDLE fileHandle = nullptr;
    std::wregex regex;
    std::wstring path;
};

int getNextPosInt_incIt(std::wstring::const_iterator &it, std::wstring::const_iterator  &end);
int getNextInt_incIt(std::wstring::const_iterator &it, std::wstring::const_iterator  &end);
float getNextFloat_incIt(std::wstring::const_iterator &it, std::wstring::const_iterator  &end);
bool findNextText_incIt(std::wstring &strToFind, std::wstring::const_iterator &it, std::wstring::const_iterator  &end);

// compile if uncompiled file newer than compiled file
bool fileNeedsCompiling(FilePathSplit &pathSplit);

}//namespace murkyFramework
