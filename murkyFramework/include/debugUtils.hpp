#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
#include <murkyFramework/include/version.hpp>

#include <iostream>

//------------------------------------------------------------------------------
// Output to IDE Output

#define OUTPUT_LOGGING_ENABLED
#define SCREEN_LOGGING_ENABLED
#include "inputDevices.hpp"

class DebObj
{//dummy object to allow << operator to work on something and nothing else
};

// Data
extern DebObj debugLog;

// Methods
void    outputDebugString(const std::wstring str);
DebObj operator << (DebObj obj, const std::wstring str);
DebObj operator << (DebObj obj, char *const str);
DebObj operator << (DebObj obj, int i);
DebObj operator << (DebObj obj, float f);
// Output to IDE Output
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Output to screen

// Data
extern std::wstring debugLogScreen;

// Methods

// Output to screen
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Keyboard
bool debugIsKeyPressed(InputDevices::KeyCode keyCode);
// Keyboard
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Misc
//std::wstring operator << (std::wstring &wstr, int i);
// Misc
//------------------------------------------------------------------------------

