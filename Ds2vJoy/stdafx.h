﻿#pragma once
#include "targetver.h"

#define WIN32_LEAN_AND_MEAN				// Exclude rarely-used parts from the header
#pragma warning(disable : 4244)

#define OEMRESOURCE
#define OCR_HELP	32651
//////////////////////////////////////////////////////////////////////
//#include <windows.h>
#include "windowsx.h"
//#include <commctrl.h>
//#pragma comment(lib, "comctl32.lib")
#include "Resource.h"
#include <atlbase.h>					//A2T T2A W2T T2W A2W W2A
#include <sstream>						//std+Settings stringstream
#include "Language.h"
//////////////////////////////////////////////////////////////////////
#include <set>
#include <future>						// std::async, std::future
#include <filesystem>					//filesystem
#include <condition_variable>			//mutex
//#include <fstream>					//<<
//#include <codecvt>					//convert string wstring
//#include <iostream>					//std::cout & std::err
//#include <psapi.h>					//EnumDeviceDrivers & GetDeviceDriverBaseName
//#include <stdlib.h>
//#include <malloc.h>
//#include <memory.h>
//#include <tchar.h>
//#include <stdio.h>
//#include <iomanip>
//#include <string>
//#include <locale>
//#include <process.h>
//#include <Dbt.h>
//#include <comdef.h >
//#include <comutil.h>
//////////////////////////////////////////////////////////////////////
#include <atlimage.h>					//Screenshot
//#include <gdiplus.h>
//////////////////////////////////////////////////////////////////////
#include <mmeapi.h>						//Sound control
#pragma comment (lib, "winmm.lib")
//////////////////////////////////////////////////////////////////////
//#include "magnification.h"			//Magnification
//#pragma comment(lib, "Magnification.lib")
//////////////////////////////////////////////////////////////////////
#include "Zip Utils/unzip.h"			//Zip Utils
#include "CPULimiter/CPULimiter.h"
//////////////////////////////////////////////////////////////////////
#include "HTTPRequest/HTTPRequest.hpp"	//HTTPRequest (https://github.com/elnormous/HTTPRequest)
#pragma comment(lib, "Ws2_32.lib")
//#include <chrono>						//HTTPRequest timeout
//////////////////////////////////////////////////////////////////////
#include <Hidsdi.h>						//DS
#pragma comment(lib, "hid.lib")
#include <BluetoothAPIs.h>
#pragma comment(lib, "Bthprops")
#include <SetupAPI.h>
#pragma comment(lib, "SetupAPI.lib")
//////////////////////////////////////////////////////////////////////
#include "Crc32/Crc32.h"				//Source
//////////////////////////////////////////////////////////////////////
//#include <Xinput.h>					//Xinput
//#pragma comment(lib, "Xinput.lib")
//////////////////////////////////////////////////////////////////////
//#include <winioctl.h>					//vJoyInterface
//////////////////////////////////////////////////////////////////////
#include "vJoy/vJoyInterface.h"			//vJoy
#pragma comment(lib, "vJoy/vJoyInterface.lib")
//////////////////////////////////////////////////////////////////////
#include "LogDlg.h"						//Log
//////////////////////////////////////////////////////////////////////
#include "Settings.h"					//Settings
//////////////////////////////////////////////////////////////////////
#include <Commdlg.h>					//SettingDlg
//////////////////////////////////////////////////////////////////////
#include "time.h"						//Mapping & dsButton
//////////////////////////////////////////////////////////////////////
#include <bitset>						//MappingDlg
//////////////////////////////////////////////////////////////////////
//#include <vector>						//Keymap
//////////////////////////////////////////////////////////////////////
//#include <map>						//RapidFire
//////////////////////////////////////////////////////////////////////
#include "ViGEm/Client.h"				//ViGem
#pragma comment(lib, "ViGEm/ViGEmClient.lib")
//////////////////////////////////////////////////////////////////////
#include <psapi.h>						//Guardian (EnumProcesses, EnumProcessModules & GetModuleBaseName)
//////////////////////////////////////////////////////////////////////
#include <DispatcherQueue.h>			//ExplorerDlg
#include <regex>
#include <ShObjIdl_core.h>
#include <pathcch.h>
//////////////////////////////////////////////////////////////////////
//#include "dwmapi.h"					//Thumbnail
//#pragma comment(lib, "Dwmapi.lib")
//////////////////////////////////////////////////////////////////////
#include <Shellapi.h>					//Tasktray
//////////////////////////////////////////////////////////////////////
#include "uxtheme.h"					//WindowsTools
#pragma comment (lib, "UxTheme.lib")
#include "WindowsTools.h"

#pragma warning(default : 4244)