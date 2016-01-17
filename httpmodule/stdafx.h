// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <assertions.h>
#include <cassert>
#include <assert.h> 
#include <Windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "Shlwapi.h"
#include "Enums.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
//#include <windows.h>

using namespace std;

#define BUFSIZMAX		8*1024
#define BUFSIZMIN		1024
#define DATA_BUFSIZE	8192
#define DATASIZMIN		128

// TODO: reference additional headers your program requires here
#ifdef HTTPMODULE_EXPORTS
#define HTTPMODULE_API __declspec(dllexport)
#else
#define HTTPMODULE_API __declspec(dllimport)
#endif