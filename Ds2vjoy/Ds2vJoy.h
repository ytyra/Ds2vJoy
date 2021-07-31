﻿#pragma once
#include "DS.h"

#define WM_DEVICE_VJOY_START WM_USER + 1
#define WM_DEVICE_DS_START WM_USER + 2
#define WM_RELOAD WM_USER + 3
#define WM_LOGNEW WM_USER + 4
#define WM_SETORANGELED WM_USER + 5
#define WM_SETWHITELED WM_USER + 6
#define WM_SETTRIGGERS WM_USER + 7
#define WM_ADDMAPPING WM_USER + 8
#define WM_CHANGE_PAD WM_USER + 9
#define WM_ADDKEYMAP WM_USER + 10
#define WM_CHANGE_HIDS WM_USER + 11
#define WM_ADDRAPIDFIRE WM_USER + 12
#define WM_CREATE_MENU WM_USER + 13
#define WM_TASKTRAY WM_USER + 14
#define ID_LOGEDITWINDOW 1
#define ID_TABMENU 2
#define ID_CHKBOXW 3
#define ID_STATUS 4

void OutRun(int comeback, bool verbose = false);
