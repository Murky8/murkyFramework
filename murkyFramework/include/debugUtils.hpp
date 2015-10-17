#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
#include <murkyFramework/include/GfxDevice/version_gfxDevice.hpp>
#include <murkyFramework/include/stringHelpers.hpp>

#include <iostream>
#include "murkyFramework/include/vectorMatrix.hpp"

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

//template<typename T>
//T adder(T v) {
//	return v;
//}
//
//template<typename T, typename... Args>
//T adder(T first, Args... args) {
//	return first + adder(args...);
//}

template<typename T>
void debug2Print(T v)
{
	debugLog << v;
	debugLog << L"\n";
}

template<typename T, typename... Args>
void debug2Print(T first, Args... args)
{
	
	debugLog << first;// << L" ";
	debug2Print(args...);
}


void debug2LogToFile(const std::wstring &textToAdd);
void debug2ResetLogFile();






