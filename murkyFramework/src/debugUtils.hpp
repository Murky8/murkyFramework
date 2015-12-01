#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.

class DebObj
{//dummy object to allow << operator to work on something
};

// data
extern DebObj debugLog;
extern std::wstring debugLogScreen;

// methods
void    outputDebugString(const std::wstring str);
DebObj operator << (DebObj obj, const std::wstring str);
DebObj operator << (DebObj obj, char *const str);
DebObj operator << (DebObj obj, int i);
DebObj operator << (DebObj obj, float f);
DebObj operator << (DebObj obj, double f);

//------------------------------------------------------------------------------
// debugLog2


extern std::wstring	logFilePath;

enum class DebugLog2_dest
{
	screen		= 1<<0,
	stdoutt		= 1<<1,
	file		= 1<<2
};

template<typename T>
void debug2Print(DebugLog2_dest dest, T v)
{
	debugLog << v;
	debugLog << L"\n";
}

template<typename T, typename... Args>
void debug2Print(DebugLog2_dest dest, T first, Args... args)
{
	
	debugLog << first;// << L" ";
	debug2Print(args...);
}

template<typename T, typename... Args>
void debug2Print_screen(T first, Args... args)
{
	debug2Print(DebugLog2_dest::screen, first, args);
}

template<typename T, typename... Args>
void debug2Print_file(T first, Args... args)
{
	debug2Print(DebugLog2_dest::file, first, args);
}

void debug2LogToFile(const std::wstring &textToAdd);
void debug2ResetLogFile();






