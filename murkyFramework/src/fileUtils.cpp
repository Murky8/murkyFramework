//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/src/pch.hpp>

namespace murkyFramework {
// note: do recursive?
void visitAllFilesInDirectory(std::wstring startDir, void(*funct)(FilePathSplit), std::wregex &extensionName_regex)
{
	WIN32_FIND_DATA	findData;
	HANDLE fileHandle = nullptr;
	std::wstring searchfolderPath = startDir + L"\\*";

	fileHandle = FindFirstFile(searchfolderPath.c_str(), &findData);
	if (fileHandle == INVALID_HANDLE_VALUE)
	{
		return;
	}

	// first match should be '.' (check this...). we ignore it.
	if (std::wstring(findData.cFileName) != L".")
		triggerBreakpoint();

	while (1)
	{
		HRESULT res;
		res = FindNextFile(fileHandle, &findData);
		if (res == FALSE)
			break;

		std::wstring fileName;
		fileName = startDir + L"/" + (findData.cFileName);
		FilePathSplit splitPath(fileName);
		if (std::regex_search(splitPath.extensionName, extensionName_regex) == false)
			continue;

        if(funct!= nullptr)
		    funct(splitPath);
	}
}

#if 1 // warning_not_thread_safe
u32 fileCount;
void fileCounterIncrementer(FilePathSplit path)
{
    ++fileCount;
}

u32     countFilesInDirectory(std::wstring startDir, std::wregex &extensionName_regex)
{
    fileCount = 0;
    visitAllFilesInDirectory(startDir, fileCounterIncrementer, extensionName_regex);
    return fileCount;
}
#endif

FilePathSplit::FilePathSplit(std::wstring pathFileNameExt)
{
	decomposeFilePath(pathFileNameExt,
		directoryPath,
		fileName,
		extensionName);
}

    std::wstring FilePathSplit::getJoinedFilePath()
    {
	    return makePathString(directoryPath, fileName, extensionName);
    }

    void removeInstsOfChar(std::wstring &str, wchar_t chr)
    {
	    str.erase(std::remove(str.begin(), str.end(), chr),
		    str.end());
    }

    void decomposeFilePath(std::wstring in_string, std::wstring &out_directoryPath,
	    std::wstring &out_fileNameNaked, std::wstring &out_extensionName)
    {
	    wchar_t dirPath[200];	// todo: stack capacity, threading?
	    wchar_t fileName[30];
	    wchar_t extension[10];
	
	    _wsplitpath_s(	// todo: windows only :(
		    in_string.c_str(),
		    nullptr,
		    0,
		    dirPath,
		    sizeof(dirPath) / sizeof(wchar_t),
		    fileName,
		    sizeof(fileName) / sizeof(wchar_t),
		    extension,
		    sizeof(extension) / sizeof(wchar_t)
		    );
	    //_splitpath

	    out_directoryPath = std::wstring(dirPath);
	    
        if(out_directoryPath.size()>0)
        if(out_directoryPath.back() == '/')
	    {
		    out_directoryPath.pop_back();
	    }

	    out_fileNameNaked = std::wstring(fileName);

	    out_extensionName = std::wstring(extension);
	    removeInstsOfChar(out_extensionName, '.');		
    }

    std::wstring makePathString(const std::wstring &dirName, const std::wstring &fileName, 
        const std::wstring &extensionName)
    {
	    std::wstring str;
	    if (dirName.length() != 0)
		    str += dirName + L"/";

	    str += fileName + L"." + extensionName;

        return str;
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

    FileDirectoryWalker::FileDirectoryWalker(const std::wstring &startDirName, const std::wstring &filterRegex)
    {
        /*
        WIN32_FIND_DATA	findData;
        ZeroMemory(&findData, sizeof(findData));
        path = dirName;   
        */            
        path = startDirName + L"\\*";
        regex = std::wregex(filterRegex);

        fileHandle = FindFirstFile(path.c_str(), &findData);
        if (fileHandle == INVALID_HANDLE_VALUE)
        {
            return;
        }

        // first match should be '.' (check this!!!). we ignore it.
        if (std::wstring(findData.cFileName) != L".")
            triggerBreakpoint();
    }

    bool FileDirectoryWalker::findNext()
    {
        HRESULT res;
        while (1)
        {
            res = FindNextFile(fileHandle, &findData);
            // eg findData.cFileName is L"font 4c.png" 
            if (res == FALSE)
                return false;

            std::wstring foundName(findData.cFileName);
            //bbbfileName = startDir + L"/" + (findData.cFileName);
            //FilePathSplit splitPath(fileName);
            if (std::regex_search(foundName,regex) == false)
                continue;
            return true;
        }
    }

    int getNextPosInt_incIt(std::wstring::const_iterator &it, std::wstring::const_iterator  &end)
    {
        static std::wregex regx_int(L"[0-9]+");

        int i{ 0 };
        std::wsmatch match;

        if (std::regex_search(it, end, match, regx_int))
        {
            std::wstring substr(match[0].first, match[0].second);
            i = _wtoi(substr.c_str());

            it = match[0].second;
        }
        return i;
    }

    int getNextInt_incIt(std::wstring::const_iterator &it, std::wstring::const_iterator  &end)
    {
        static std::wregex regx_int(L"[-\\+]?[0-9]+");

        int i{ 0 };
        std::wsmatch match;

        if (std::regex_search(it, end, match, regx_int))
        {
            std::wstring substr(match[0].first, match[0].second);
            i = _wtoi(substr.c_str());

            it = match[0].second;
        }

        return i;
    }

    float getNextFloat_incIt(std::wstring::const_iterator &it, std::wstring::const_iterator  &end)
    {
        // "-+[0-9]+\\.+[0-9]+"
        static std::wregex regx_float(L"[-\\+]?[0-9]*\\.?[0-9]+");

        float f{ 0 };
        std::wsmatch match;

        if (std::regex_search(it, end, match, regx_float))
        {
            std::wstring substr(match[0].first, match[0].second);
            f = (float)_wtof(substr.c_str());

            it = match[0].second;
        }

        return f;
    }

    bool findNextText_incIt(std::wstring &strToFind, std::wstring::const_iterator &it, std::wstring::const_iterator  &end)
    {
        std::wstring regex_string;
        regex_string += L"[\\s]";
        regex_string += strToFind;
        std::wregex regx_text(regex_string);

        std::wsmatch match;

        if (std::regex_search(it, end, match, regx_text))
        {
            std::wstring substr(match[0].first, match[0].second);
            it = match[0].second;
            return true;
        }
        else
        {
            return false;
        }
    }

    // compile if uncompiled file newer than compiled file
    bool fileNeedsCompiling(FilePathSplit &pathSplit)
    {
        std::wstring binPath = makePathString(pathSplit.directoryPath, pathSplit.fileName, L"bin");

        bool binExsists;
        u64 fbxModTime = getFileModificationTime1601(pathSplit.getJoinedFilePath());
        u64 binModTime = getFileModificationTime1601(binPath, &binExsists);

        // todo: temp!!!!
        if (fbxModTime > binModTime || binExsists == false)
            return true;
        else
            return false;
    }

    /*
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
    */

}//namespace murkyFramework