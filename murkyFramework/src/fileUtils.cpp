//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/common.hpp>
#include <murkyFramework/include/version.hpp>
#include <murkyFramework/include/GfxDevice/version_gfxDevice.hpp>

#include <murkyFramework/include/fileUtils.hpp>
#include <murkyFramework/include/debugUtils.hpp>
    

std::wstring makePathString(const std::wstring &dirName, const std::wstring &fileName, 
    const std::wstring &extensionName)
{
    return dirName + L"/" + fileName + L"." + extensionName;
}

u64 getFileModificationTime1601(std::wstring fileNameWithPath, bool *out_fileExists)
{
    WIN32_FILE_ATTRIBUTE_DATA	info;
    bool res = GetFileAttributesEx(fileNameWithPath.c_str(), GetFileExInfoStandard, &info);

    if(res==0)
    {
        auto res2 = GetLastError();
		if(out_fileExists!=nullptr)
			*out_fileExists = false;
        return 0;
    }
            
    FILETIME time = info.ftLastWriteTime;
    u64 time2 = (static_cast<u64>(time.dwHighDateTime) << 32) + static_cast<u64>(time.dwLowDateTime);

	if (out_fileExists != nullptr)
		*out_fileExists = true;
    return time2;	
}

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
