﻿#include "stdafx.h"
#include "Language.h"

struct LANGSTRS I18N = { 0 };

WCHAR* GetText(int _id, WCHAR* txt)
{
	WCHAR buffer[1024];
	int len = LoadStringW(hInst, _id, buffer, 1024);
	WCHAR* ret;
	if (len == 0)
	{
		ret = new WCHAR[lstrlenW(txt) + 1];
		lstrcpyW(ret, txt);
	}
	else
	{
		ret = new WCHAR[lstrlenW(buffer) + 1];
		lstrcpyW(ret, buffer);
	}
	return ret;
}

void LoadLanguage()
{
	I18N.DS2VJOY = GetText(IDS_DS2VJOY, L"DS2VJOY");
	I18N.APP_TITLE = GetText(IDS_APP_TITLE, L"Ds2vJoy");
	I18N.RapidFire_State = GetText(IDS_RAPIDFIRE_STATE, L"First:%dms UP:%dms DOWN:%dms");
	I18N.BT_Size = GetText(IDS_BT_SIZE, L"The size of the received data was not enough.");
	I18N.BT_CRC = GetText(IDS_BT_CRC, L"BT received data with crc error: %d");
	I18N.Button_LX = GetText(IDS_BUTTON_LX, L"AXISL X");
	I18N.Button_LY = GetText(IDS_BUTTON_LY, L"AXISL Y");
	I18N.Button_RX = GetText(IDS_BUTTON_RX, L"AXISR X");
	I18N.Button_RY = GetText(IDS_BUTTON_RY, L"AXISR Y");
	I18N.Button_L3 = GetText(IDS_BUTTON_L3, L"L3");
	I18N.Button_DPAD_LEFT = GetText(IDS_BUTTON_DPAD_LEFT, L"©");
	I18N.Button_DPAD_UP = GetText(IDS_BUTTON_DPAD_UP, L"ª");
	I18N.Button_DPAD_RIGHT = GetText(IDS_BUTTON_DPAD_RIGHT, L"¨");
	I18N.Button_DPAD_DOWN = GetText(IDS_BUTTON_DPAD_DOWN, L"«");
	I18N.Button_SHARE = GetText(IDS_BUTTON_SHARE, L"SHARE");
	I18N.Button_OPTIONS = GetText(IDS_BUTTON_OPTIONS, L"OPTIONS");
	I18N.Button_SQUARE = GetText(IDS_BUTTON_SQUARE, L"□");
	I18N.Button_TRIANGLE = GetText(IDS_BUTTON_TRIANGLE, L"△");
	I18N.Button_CIRCLE = GetText(IDS_BUTTON_CIRCLE, L"○");
	I18N.Button_CROSS = GetText(IDS_BUTTON_CROSS, L"✕");
	I18N.Button_R3 = GetText(IDS_BUTTON_R3, L"R3");
	I18N.Button_PS = GetText(IDS_BUTTON_PS, L"PS");
	I18N.Button_TOUCH = GetText(IDS_BUTTON_TOUCH, L"TOUCH");
	I18N.Button_MUTE = GetText(IDS_BUTTON_MUTE, L"MUTE");
	I18N.Button_L1 = GetText(IDS_BUTTON_L1, L"L1");
	I18N.Button_R1 = GetText(IDS_BUTTON_R1, L"R1");
	I18N.Button_L2 = GetText(IDS_BUTTON_L2, L"L2");
	I18N.Button_R2 = GetText(IDS_BUTTON_R2, L"R2");
	I18N.Button_L2TRIGGER = GetText(IDS_BUTTON_L2TRIGGER, L"L2TRIGGER");
	I18N.Button_R2TRIGGER = GetText(IDS_BUTTON_R2TRIGGER, L"R2TRIGGER");
	I18N.Button_EMPTY = GetText(IDS_BUTTON_EMPTY, L"NULL");
	I18N.Button_MIDDLE = GetText(IDS_BUTTON_MIDDLE, L"HALF");
	I18N.Button_FULL = GetText(IDS_BUTTON_FULL, L"FULL");
	I18N.Button_LX_INV = GetText(IDS_BUTTON_LX_INV, L"AXISL XINV");
	I18N.Button_LY_INV = GetText(IDS_BUTTON_LY_INV, L"AXISL YINV");
	I18N.Button_RX_INV = GetText(IDS_BUTTON_RX_INV, L"AXISR XINV");
	I18N.Button_RY_INV = GetText(IDS_BUTTON_RY_INV, L"AXISR YINV");
	I18N.Button_AXISL_LEFT = GetText(IDS_BUTTON_AXISL_LEFT, L"AXISL ←");
	I18N.Button_AXISL_UP_LEFT = GetText(IDS_BUTTON_AXISL_UP_LEFT, L"AXISL ←↑");
	I18N.Button_AXISL_UP = GetText(IDS_BUTTON_AXISL_UP, L"AXISL ↑");
	I18N.Button_AXISL_UP_RIGHT = GetText(IDS_BUTTON_AXISL_UP_RIGHT, L"AXISL ↑→");
	I18N.Button_AXISL_RIGHT = GetText(IDS_BUTTON_AXISL_RIGHT, L"AXISL →");
	I18N.Button_AXISL_DOWN_RIGHT = GetText(IDS_BUTTON_AXISL_DOWN_RIGHT, L"AXISL ↓→");
	I18N.Button_AXISL_DOWN = GetText(IDS_BUTTON_AXISL_DOWN, L"AXISL ↓");
	I18N.Button_AXISL_DOWN_LEFT = GetText(IDS_BUTTON_AXISL_DOWN_LEFT, L"AXISL ←↓");
	I18N.Button_AXISR_LEFT = GetText(IDS_BUTTON_AXISR_LEFT, L"AXISR ←");
	I18N.Button_AXISR_UP_LEFT = GetText(IDS_BUTTON_AXISR_UP_LEFT, L"AXISR ←↑");
	I18N.Button_AXISR_UP = GetText(IDS_BUTTON_AXISR_UP, L"AXISR ↑");
	I18N.Button_AXISR_UP_RIGHT = GetText(IDS_BUTTON_AXISR_UP_RIGHT, L"AXISR ↑→");
	I18N.Button_AXISR_RIGHT = GetText(IDS_BUTTON_AXISR_RIGHT, L"AXISR →");
	I18N.Button_AXISR_DOWN_RIGHT = GetText(IDS_BUTTON_AXISR_DOWN_RIGHT, L"AXISR ↓→");
	I18N.Button_AXISR_DOWN = GetText(IDS_BUTTON_AXISR_DOWN, L"AXISR ↓");
	I18N.Button_AXISR_DOWN_LEFT = GetText(IDS_BUTTON_AXISR_DOWN_LEFT, L"AXISR ←↓");
	I18N.Button_TOUCH_UP = GetText(IDS_BUTTON_TOUCH_UP, L"TOUCHUP");
	I18N.Button_TOUCHPAD = GetText(IDS_BUTTON_TOUCHPAD, L"TOUCHPAD");
	I18N.Button_TOUCH_1 = GetText(IDS_BUTTON_TOUCH_1, L"TOUCH1");
	I18N.Button_TOUCH_2 = GetText(IDS_BUTTON_TOUCH_2, L"TOUCH2");
	I18N.Button_TOUCH_3 = GetText(IDS_BUTTON_TOUCH_3, L"TOUCH3");
	I18N.Button_TOUCH_4 = GetText(IDS_BUTTON_TOUCH_4, L"TOUCH4");
	I18N.Button_TOUCH_L = GetText(IDS_BUTTON_TOUCH_L, L"TOUCHL");
	I18N.Button_TOUCH_R = GetText(IDS_BUTTON_TOUCH_R, L"TOUCHR");
	I18N.Button_L2TRIGGER_33 = GetText(IDS_BUTTON_L2TRIGGER_33, L"L2LOW");
	I18N.Button_R2TRIGGER_33 = GetText(IDS_BUTTON_R2TRIGGER_33, L"R2LOW");
	I18N.Button_L2TRIGGER_66 = GetText(IDS_BUTTON_L2TRIGGER_66, L"L2HIGH");
	I18N.Button_R2TRIGGER_66 = GetText(IDS_BUTTON_R2TRIGGER_66, L"R2HIGH");
	I18N.Button_L2TRIGGER_100 = GetText(IDS_BUTTON_L2TRIGGER_100, L"L2FULL");
	I18N.Button_R2TRIGGER_100 = GetText(IDS_BUTTON_R2TRIGGER_100, L"R2FULL");
	I18N.Button_L1R1 = GetText(IDS_BUTTON_L1R1, L"L1R1");
	I18N.Button_L2R2 = GetText(IDS_BUTTON_L2R2, L"L2R2");
	I18N.Button_BATTERY = GetText(IDS_BUTTON_BATTERY, L"BATTERY");
	I18N.Gamepad_LEFT_STICK = GetText(IDS_GAMEPAD_LEFT_BUMPER, L"Lstick");
	I18N.Gamepad_BACK = GetText(IDS_GAMEPAD_BACK, L"Back");
	I18N.Gamepad_START = GetText(IDS_GAMEPAD_START, L"Start");
	I18N.Gamepad_X = GetText(IDS_GAMEPAD_X, L"X");
	I18N.Gamepad_Y = GetText(IDS_GAMEPAD_Y, L"Y");
	I18N.Gamepad_B = GetText(IDS_GAMEPAD_B, L"B");
	I18N.Gamepad_A = GetText(IDS_GAMEPAD_A, L"A");
	I18N.Gamepad_RIGHT_STICK = GetText(IDS_GAMEPAD_RIGHT_STICK, L"Rstick");
	I18N.Gamepad_GUIDE = GetText(IDS_GAMEPAD_GUIDE, L"Guide");
	I18N.Gamepad_LEFT_BUMPER = GetText(IDS_GAMEPAD_LEFT_BUMPER, L"Lbumper");
	I18N.Gamepad_RIGHT_BUMPER = GetText(IDS_GAMEPAD_RIGHT_BUMPER, L"Rbumper");
	I18N.Gamepad_LEFT_TRIGGER = GetText(IDS_GAMEPAD_LEFT_TRIGGER, L"Ltrigger");
	I18N.Gamepad_RIGHT_TRIGGER = GetText(IDS_GAMEPAD_RIGHT_TRIGGER, L"Rtrigger");
	I18N.Change_Settings = GetText(IDS_LOG_SETTINGS, L"Reconnecting for change settings.");
	I18N.Clone_of_Tab = GetText(IDS_CLONE_OF_TAB, L" Clone of Tab ");
	I18N.ConnectController = GetText(IDS_CONNECTCONTROLLER, L"Connect %s Serial %s");
	I18N.ConnectionStarted = GetText(IDS_CONNECTIONSTARTED, L"Connection started %s");
	I18N.DisconnectController = GetText(IDS_DISCONNECTCONTROLLER, L"Disconnect %s");
	I18N.FailedParse = GetText(IDS_FAILEDPARSE, L"Failed to parse %s");
	I18N.FailedRecv = GetText(IDS_FAILEDRECV, L"Failed to recv data from %s : %d");
	I18N.FailedSendCmd = GetText(IDS_FAILEDSENDCMD, L"Failed to send command to %s : %d");
	I18N.FailedThread = GetText(IDS_FAILEDTHREAD, L"Failed to create thread");
	I18N.Firmware = GetText(IDS_FIRMWARE, L"Firmware Ver %02X%02X (%s)");
	I18N.FirmwareFail = GetText(IDS_FIRMWAREFAIL, L"Cannot get firmware version");
	I18N.Log_Wait_ds = GetText(IDS_LOG_WAIT_DS, L"Waiting for a valid DS device.");
	I18N.Log_Wait_vJoy = GetText(IDS_LOG_WAIT_VJOY, L"Waiting for a valid vJoy device.");
	I18N.MBOX_Delete = GetText(IDS_MBOX_Delete, L"Are you sure you want to delete the selected item?");
	I18N.MBOX_ErrTitle = GetText(IDS_MBOX_ErrTitle, L"Error");
	I18N.MBOX_NoButtonSelected = GetText(IDS_MBOX_NOBUTTONSELECTED, L"No button selected");
	I18N.MBOX_Save = GetText(IDS_MBOX_Save, L"Save?");
	I18N.MouseAction_ACTIVE_MOUSE = GetText(IDS_MOUSE_ACTIVE_MOUSE, L"Active mouse");
	I18N.MouseAction_SAVE_POSITION = GetText(IDS_MOUSE_SAVE_POSITION, L"Save position");
	I18N.MouseAction_MOVE_TO_XY = GetText(IDS_MOUSE_MOVE_TO_XY, L"Move to x,y");
	I18N.MouseAction_SAVE_AND_MOVE_TO_XY = GetText(IDS_MOUSE_SAVE_AND_MOVE_TO_XY, L"S&Move to xy");
	I18N.MouseAction_MOVE_TO_WH = GetText(IDS_MOUSE_MOVE_TO_WH, L"Move to w,h");
	I18N.MouseAction_SAVE_AND_MOVE_TO_WH = GetText(IDS_MOUSE_SAVE_AND_MOVE_TO_WH, L"S&Move to wh");
	I18N.MouseAction_MOVE_BACK = GetText(IDS_MOUSE_MOVE_BACK, L"Move back");
	I18N.MouseAction_LEFT_CLICK = GetText(IDS_MOUSE_LEFT_CLICK, L"Left Click");
	I18N.MouseAction_MIDDLE_CLICK = GetText(IDS_MOUSE_MIDDLE_CLICK, L"Middle Click");
	I18N.MouseAction_RIGHT_CLICK = GetText(IDS_MOUSE_RIGHT_CLICK, L"Right Click");
	I18N.MouseAction_X1_CLICK = GetText(IDS_MOUSE_X1_CLICK, L"X1 Click");
	I18N.MouseAction_X2_CLICK = GetText(IDS_MOUSE_X2_CLICK, L"X2 Click");
	I18N.MouseAction_SCROLL_UP = GetText(IDS_MOUSE_SCROLL_UP, L"Scroll ↑");
	I18N.MouseAction_SCROLL_DOWN = GetText(IDS_MOUSE_SCROLL_DOWN, L"Scroll ↓");
	I18N.MouseAction_LEFT_DOUBLE_CLICK = GetText(IDS_MOUSE_LEFT_DOUBLE_CLICK, L"Left Double");
	I18N.MouseAction_MIDDLE_DOUBLE_CLICK = GetText(IDS_MOUSE_MIDDLE_DOUBLE_CLICK, L"Middle Double");
	I18N.MouseAction_RIGHT_DOUBLE_CLICK = GetText(IDS_MOUSE_RIGHT_DOUBLE_CLICK, L"Right Double");
	I18N.MouseAction_X1_DOUBLE_CLICK = GetText(IDS_MOUSE_X1_DOUBLE_CLICK, L"X1 Double");
	I18N.MouseAction_X2_DOUBLE_CLICK = GetText(IDS_MOUSE_X2_DOUBLE_CLICK, L"X2 Double");
	I18N.MouseAction_SCROLL_UP_x5 = GetText(IDS_MOUSE_SCROLL_UP_x5, L"Scroll ↑ x5");
	I18N.MouseAction_SCROLL_DOWN_x5 = GetText(IDS_MOUSE_SCROLL_DOWN_x5, L"Scroll ↓ x5");
	I18N.MouseAction_LEFT_DOWN = GetText(IDS_MOUSE_LEFT_DOWN, L"Left Down");
	I18N.MouseAction_MIDDLE_DOWN = GetText(IDS_MOUSE_MIDDLE_DOWN, L"Middle Down");
	I18N.MouseAction_RIGHT_DOWN = GetText(IDS_MOUSE_RIGHT_DOWN, L"Right Down");
	I18N.MouseAction_X1_DOWN = GetText(IDS_MOUSE_X1_DOWN, L"X1 Down");
	I18N.MouseAction_X2_DOWN = GetText(IDS_MOUSE_X2_DOWN, L"X2 Down");
	I18N.MouseAction_SCROLL_UP_VARIABLE = GetText(IDS_MOUSE_SCROLL_UP_VARIABLE, L"Scroll ↑ incr");
	I18N.MouseAction_SCROLL_DOWN_VARIABLE = GetText(IDS_MOUSE_SCROLL_DOWN_VARIABLE, L"Scroll ↓ incr");
	I18N.MouseAction_MUTE_SOUND = GetText(IDS_MOUSE_MUTE_SOUND, L"Mute/Unmute");
	I18N.MouseAction_VOLUME_UP = GetText(IDS_MOUSE_VOLUME_UP, L"Volume up");
	I18N.MouseAction_VOLUME_DOWN = GetText(IDS_MOUSE_VOLUME_DOWN, L"Volume down");
	I18N.MouseAction_MEMORIZE_MODE = GetText(IDS_MOUSE_MEMORIZE_MODE, L"Memorize mode");
	I18N.MouseAction_TO_MODE = GetText(IDS_MOUSE_TO_MODE, L"To Mode: ");
	I18N.MouseAction_TO_LAST_MODE = GetText(IDS_MOUSE_TO_LAST_MODE, L"To Last Mode");
	I18N.Setting = GetText(IDS_SETTING, L"Setting");
	I18N.SkipController = GetText(IDS_SKIPCONTROLLER, L"Skip %s Serial %s");
	I18N.Status_Wait = GetText(IDS_STATUS_WAIT, L"waiting");
	I18N.TabRapidFire = GetText(IDS_TAB_RAPIDFIRE, L"RapidFire");
	I18N.TabGuardian = GetText(IDS_TAB_GUARDIAN, L"Guardian");
	I18N.TabKeymap = GetText(IDS_TAB_KEYMAP, L"Keymap");
	I18N.TabLinks = GetText(IDS_TAB_LINKS, L"Links");
	I18N.TabLog = GetText(IDS_TAB_LOG, L"Log");
	I18N.TabMapping = GetText(IDS_TAB_MAPPING, L"Mapping");
	I18N.TabSettings = GetText(IDS_TAB_SETTINGS, L"Settings");
	I18N.TabViGEm = GetText(IDS_TAB_VIGEM, L"ViGEm");
	I18N.TagsButton = GetText(IDS_TAGS_BUTTON, L"Tags");
	I18N.TT_AppNotFound = GetText(IDS_TT_APPNOTFOUND, L"%s not found.");
	I18N.TT_ProfileChanged = GetText(IDS_TT_PROFILECHANGED, L"Profile changed to: %d");
	I18N.dsButton = GetText(IDS_DS_BUTTON, L"DS Button");
	I18N.vJoyButton = GetText(IDS_VJOY_BUTTON, L"vJoy Button");
	I18N.vJoy_acquire = GetText(IDS_VJOY_ACQUIRE, L"Acquired vJoy (ID:%d)");
	I18N.vJoy_busy = GetText(IDS_VJOY_BUSY, L"vJoy (ID:%d) is busy");
	I18N.vJoy_cantuse = GetText(IDS_VJOY_CANT_USE, L"Can not use vJoy (ID:%d) ");
	I18N.vJoy_failed_acquire = GetText(IDS_VJOY_FAILED_ACQUIRE, L"Failed to acquire vJoy (ID:%d)");
	I18N.vJoy_failed_update = GetText(IDS_VJOY_FAILED_UPDATE, L"Failed to update vJoy (ID:%d)");
	I18N.vJoy_version = GetText(IDS_VJOY_VERSION, L"vJoy version %d.%d.%d");
	I18N.vJoy_version_notmatch = GetText(IDS_VJOY_NOMATCH, L"vJoy version not match. DLL(%d.%d.%d), Driver(%d.%d.%d)");
	I18N.vJoy_X = GetText(IDS_VJOY_X, L"X");
	I18N.vJoy_Y = GetText(IDS_VJOY_Y, L"Y");
	I18N.vJoy_Z = GetText(IDS_VJOY_Z, L"Z");
	I18N.vJoy_RX = GetText(IDS_VJOY_RX, L"RX");
	I18N.vJoy_RY = GetText(IDS_VJOY_RY, L"RY");
	I18N.vJoy_RZ = GetText(IDS_VJOY_RZ, L"RZ");
	I18N.vJoy_SL0 = GetText(IDS_VJOY_SL0, L"SL0");
	I18N.vJoy_SL1 = GetText(IDS_VJOY_SL1, L"SL1");
	I18N.vJoy_XTR = GetText(IDS_VJOY_XTR, L"XTR");
	I18N.vJoy_YTR = GetText(IDS_VJOY_YTR, L"YTR");
	I18N.vJoy_ZTR = GetText(IDS_VJOY_ZTR, L"ZTR");
	I18N.vJoy_RXTR = GetText(IDS_VJOY_RXTR, L"RXTR");
	I18N.vJoy_RYTR = GetText(IDS_VJOY_RYTR, L"RYTR");
	I18N.vJoy_RZTR = GetText(IDS_VJOY_RZTR, L"RZTR");
	I18N.vJoy_SL0TR = GetText(IDS_VJOY_SL0TR, L"SL0TR");
	I18N.vJoy_SL1TR = GetText(IDS_VJOY_SL1TR, L"SL1TR");
	I18N.vJoy_XINV = GetText(IDS_VJOY_XINV, L"XINV");
	I18N.vJoy_YINV = GetText(IDS_VJOY_YINV, L"YINV");
	I18N.vJoy_ZINV = GetText(IDS_VJOY_ZINV, L"ZINV");
	I18N.vJoy_RXINV = GetText(IDS_VJOY_RXINV, L"RXINV");
	I18N.vJoy_RYINV = GetText(IDS_VJOY_RYINV, L"RYINV");
	I18N.vJoy_RZINV = GetText(IDS_VJOY_RZINV, L"RZINV");
	I18N.vJoy_SL0INV = GetText(IDS_VJOY_SL0INV, L"SL0INV");
	I18N.vJoy_SL1INV = GetText(IDS_VJOY_SL1INV, L"SL1INV");
	I18N.vJoy_DPAD1_LEFT = GetText(IDS_VJOY_DPAD1_LEFT, L"POV1 ←");
	I18N.vJoy_DPAD1_UP_LEFT = GetText(IDS_VJOY_DPAD1_UP_LEFT, L"POV1 ←↑");
	I18N.vJoy_DPAD1_UP = GetText(IDS_VJOY_DPAD1_UP, L"POV1 ↑");
	I18N.vJoy_DPAD1_UP_RIGHT = GetText(IDS_VJOY_DPAD1_UP_RIGHT, L"POV1 ↑→");
	I18N.vJoy_DPAD1_RIGHT = GetText(IDS_VJOY_DPAD1_RIGHT, L"POV1 →");
	I18N.vJoy_DPAD1_DOWN_RIGHT = GetText(IDS_VJOY_DPAD1_DOWN_RIGHT, L"POV1 ↓→");
	I18N.vJoy_DPAD1_DOWN = GetText(IDS_VJOY_DPAD1_DOWN, L"POV1 ↓");
	I18N.vJoy_DPAD1_DOWN_LEFT = GetText(IDS_VJOY_DPAD1_DOWN_LEFT, L"POV1 ←↓");
	I18N.vJoy_DPAD2_LEFT = GetText(IDS_VJOY_DPAD2_LEFT, L"POV2 ←");
	I18N.vJoy_DPAD2_UP_LEFT = GetText(IDS_VJOY_DPAD2_UP_LEFT, L"POV2 ←↑");
	I18N.vJoy_DPAD2_UP = GetText(IDS_VJOY_DPAD2_UP, L"POV2 ↑");
	I18N.vJoy_DPAD2_UP_RIGHT = GetText(IDS_VJOY_DPAD2_UP_RIGHT, L"POV2 ↑→");
	I18N.vJoy_DPAD2_RIGHT = GetText(IDS_VJOY_DPAD2_RIGHT, L"POV2 →");
	I18N.vJoy_DPAD2_DOWN_RIGHT = GetText(IDS_VJOY_DPAD2_DOWN_RIGHT, L"POV2 ↓→");
	I18N.vJoy_DPAD2_DOWN = GetText(IDS_VJOY_DPAD2_DOWN, L"POV2 ↓");
	I18N.vJoy_DPAD2_DOWN_LEFT = GetText(IDS_VJOY_DPAD2_DOWN_LEFT, L"POV2 ←↓");
	I18N.vJoy_DPAD3_LEFT = GetText(IDS_VJOY_DPAD3_LEFT, L"POV3 ←");
	I18N.vJoy_DPAD3_UP_LEFT = GetText(IDS_VJOY_DPAD3_UP_LEFT, L"POV2 ←↑");
	I18N.vJoy_DPAD3_UP = GetText(IDS_VJOY_DPAD3_UP, L"POV3 ↑");
	I18N.vJoy_DPAD3_UP_RIGHT = GetText(IDS_VJOY_DPAD3_UP_RIGHT, L"POV3 ↑→");
	I18N.vJoy_DPAD3_RIGHT = GetText(IDS_VJOY_DPAD3_RIGHT, L"POV3 →");
	I18N.vJoy_DPAD3_DOWN_RIGHT = GetText(IDS_VJOY_DPAD3_DOWN_RIGHT, L"POV3 ↓→");
	I18N.vJoy_DPAD3_DOWN = GetText(IDS_VJOY_DPAD3_DOWN, L"POV3 ↓");
	I18N.vJoy_DPAD3_DOWN_LEFT = GetText(IDS_VJOY_DPAD3_DOWN_LEFT, L"POV3 ←↓");
	I18N.vJoy_DPAD4_LEFT = GetText(IDS_VJOY_DPAD4_LEFT, L"POV4 ←");
	I18N.vJoy_DPAD4_UP_LEFT = GetText(IDS_VJOY_DPAD4_UP_LEFT, L"POV4 ←↑");
	I18N.vJoy_DPAD4_UP = GetText(IDS_VJOY_DPAD4_UP, L"POV4 ↑");
	I18N.vJoy_DPAD4_UP_RIGHT = GetText(IDS_VJOY_DPAD4_UP_RIGHT, L"POV4 ↑→");
	I18N.vJoy_DPAD4_RIGHT = GetText(IDS_VJOY_DPAD4_RIGHT, L"POV4 →");
	I18N.vJoy_DPAD4_DOWN_RIGHT = GetText(IDS_VJOY_DPAD4_DOWN_RIGHT, L"POV4 ↓→");
	I18N.vJoy_DPAD4_DOWN = GetText(IDS_VJOY_DPAD4_DOWN, L"POV4 ↓");
	I18N.vJoy_DPAD4_DOWN_LEFT = GetText(IDS_VJOY_DPAD4_DOWN_LEFT, L"POV4 ←↓");
}

void FreeLanguage()
{
	if (I18N.SkipController == 0)
		return;
	delete[] I18N.DS2VJOY;
	delete[] I18N.APP_TITLE;
	delete[] I18N.RapidFire_State;
	delete[] I18N.BT_Size;
	delete[] I18N.BT_CRC;
	delete[] I18N.Button_LX;
	delete[] I18N.Button_LY;
	delete[] I18N.Button_RX;
	delete[] I18N.Button_RY;
	delete[] I18N.Button_L3;
	delete[] I18N.Button_DPAD_LEFT;
	delete[] I18N.Button_DPAD_UP;
	delete[] I18N.Button_DPAD_RIGHT;
	delete[] I18N.Button_DPAD_DOWN;
	delete[] I18N.Button_SHARE;
	delete[] I18N.Button_OPTIONS;
	delete[] I18N.Button_SQUARE;
	delete[] I18N.Button_TRIANGLE;
	delete[] I18N.Button_CIRCLE;
	delete[] I18N.Button_CROSS;
	delete[] I18N.Button_R3;
	delete[] I18N.Button_PS;
	delete[] I18N.Button_TOUCH;
	delete[] I18N.Button_MUTE;
	delete[] I18N.Button_L1;
	delete[] I18N.Button_R1;
	delete[] I18N.Button_L2;
	delete[] I18N.Button_R2;
	delete[] I18N.Button_L2TRIGGER;
	delete[] I18N.Button_R2TRIGGER;
	delete[] I18N.Button_EMPTY;
	delete[] I18N.Button_MIDDLE;
	delete[] I18N.Button_FULL;
	delete[] I18N.Button_LX_INV;
	delete[] I18N.Button_LY_INV;
	delete[] I18N.Button_RX_INV;
	delete[] I18N.Button_RY_INV;
	delete[] I18N.Button_AXISL_LEFT;
	delete[] I18N.Button_AXISL_UP_LEFT;
	delete[] I18N.Button_AXISL_UP;
	delete[] I18N.Button_AXISL_UP_RIGHT;
	delete[] I18N.Button_AXISL_RIGHT;
	delete[] I18N.Button_AXISL_DOWN_RIGHT;
	delete[] I18N.Button_AXISL_DOWN;
	delete[] I18N.Button_AXISL_DOWN_LEFT;
	delete[] I18N.Button_AXISR_LEFT;
	delete[] I18N.Button_AXISR_UP_LEFT;
	delete[] I18N.Button_AXISR_UP;
	delete[] I18N.Button_AXISR_UP_RIGHT;
	delete[] I18N.Button_AXISR_RIGHT;
	delete[] I18N.Button_AXISR_DOWN_RIGHT;
	delete[] I18N.Button_AXISR_DOWN;
	delete[] I18N.Button_AXISR_DOWN_LEFT;
	delete[] I18N.Button_TOUCH_UP;
	delete[] I18N.Button_TOUCHPAD;
	delete[] I18N.Button_TOUCH_1;
	delete[] I18N.Button_TOUCH_2;
	delete[] I18N.Button_TOUCH_3;
	delete[] I18N.Button_TOUCH_4;
	delete[] I18N.Button_TOUCH_L;
	delete[] I18N.Button_TOUCH_R;
	delete[] I18N.Button_L2TRIGGER_33;
	delete[] I18N.Button_R2TRIGGER_33;
	delete[] I18N.Button_L2TRIGGER_66;
	delete[] I18N.Button_R2TRIGGER_66;
	delete[] I18N.Button_L2TRIGGER_100;
	delete[] I18N.Button_R2TRIGGER_100;
	delete[] I18N.Button_L1R1;
	delete[] I18N.Button_L2R2;
	delete[] I18N.Button_BATTERY;
	delete[] I18N.Gamepad_LEFT_STICK;
	delete[] I18N.Gamepad_BACK;
	delete[] I18N.Gamepad_START;
	delete[] I18N.Gamepad_X;
	delete[] I18N.Gamepad_Y;
	delete[] I18N.Gamepad_B;
	delete[] I18N.Gamepad_A;
	delete[] I18N.Gamepad_RIGHT_STICK;
	delete[] I18N.Gamepad_GUIDE;
	delete[] I18N.Gamepad_LEFT_BUMPER;
	delete[] I18N.Gamepad_RIGHT_BUMPER;
	delete[] I18N.Gamepad_LEFT_TRIGGER;
	delete[] I18N.Gamepad_RIGHT_TRIGGER;
	delete[] I18N.Change_Settings;
	delete[] I18N.Clone_of_Tab;
	delete[] I18N.ConnectController;
	delete[] I18N.ConnectionStarted;
	delete[] I18N.DisconnectController;
	delete[] I18N.FailedParse;
	delete[] I18N.FailedRecv;
	delete[] I18N.FailedSendCmd;
	delete[] I18N.FailedThread;
	delete[] I18N.Firmware;
	delete[] I18N.FirmwareFail;
	delete[] I18N.Log_Wait_ds;
	delete[] I18N.Log_Wait_vJoy;
	delete[] I18N.MBOX_Delete;
	delete[] I18N.MBOX_ErrTitle;
	delete[] I18N.MBOX_NoButtonSelected;
	delete[] I18N.MBOX_Save;
	delete[] I18N.MouseAction_ACTIVE_MOUSE;
	delete[] I18N.MouseAction_SAVE_POSITION;
	delete[] I18N.MouseAction_MOVE_TO_XY;
	delete[] I18N.MouseAction_SAVE_AND_MOVE_TO_XY;
	delete[] I18N.MouseAction_MOVE_TO_WH;
	delete[] I18N.MouseAction_SAVE_AND_MOVE_TO_WH;
	delete[] I18N.MouseAction_MOVE_BACK;
	delete[] I18N.MouseAction_LEFT_CLICK;
	delete[] I18N.MouseAction_MIDDLE_CLICK;
	delete[] I18N.MouseAction_RIGHT_CLICK;
	delete[] I18N.MouseAction_X1_CLICK;
	delete[] I18N.MouseAction_X2_CLICK;
	delete[] I18N.MouseAction_SCROLL_UP;
	delete[] I18N.MouseAction_SCROLL_DOWN;
	delete[] I18N.MouseAction_LEFT_DOUBLE_CLICK;
	delete[] I18N.MouseAction_MIDDLE_DOUBLE_CLICK;
	delete[] I18N.MouseAction_RIGHT_DOUBLE_CLICK;
	delete[] I18N.MouseAction_X1_DOUBLE_CLICK;
	delete[] I18N.MouseAction_X2_DOUBLE_CLICK;
	delete[] I18N.MouseAction_SCROLL_UP_x5;
	delete[] I18N.MouseAction_SCROLL_DOWN_x5;
	delete[] I18N.MouseAction_LEFT_DOWN;
	delete[] I18N.MouseAction_MIDDLE_DOWN;
	delete[] I18N.MouseAction_RIGHT_DOWN;
	delete[] I18N.MouseAction_X1_DOWN;
	delete[] I18N.MouseAction_X2_DOWN;
	delete[] I18N.MouseAction_SCROLL_UP_VARIABLE;
	delete[] I18N.MouseAction_SCROLL_DOWN_VARIABLE;
	delete[] I18N.MouseAction_MUTE_SOUND;
	delete[] I18N.MouseAction_VOLUME_UP;
	delete[] I18N.MouseAction_VOLUME_DOWN;
	delete[] I18N.MouseAction_MEMORIZE_MODE;
	delete[] I18N.MouseAction_TO_MODE;
	delete[] I18N.MouseAction_TO_LAST_MODE;
	delete[] I18N.Setting;
	delete[] I18N.SkipController;
	delete[] I18N.Status_Wait;
	delete[] I18N.TabRapidFire;
	delete[] I18N.TabGuardian;
	delete[] I18N.TabKeymap;
	delete[] I18N.TabLinks;
	delete[] I18N.TabLog;
	delete[] I18N.TabMapping;
	delete[] I18N.TabSettings;
	delete[] I18N.TabViGEm;
	delete[] I18N.TagsButton;
	delete[] I18N.TT_AppNotFound;
	delete[] I18N.TT_ProfileChanged;
	delete[] I18N.dsButton;
	delete[] I18N.vJoyButton;
	delete[] I18N.vJoy_acquire;
	delete[] I18N.vJoy_busy;
	delete[] I18N.vJoy_cantuse;
	delete[] I18N.vJoy_failed_acquire;
	delete[] I18N.vJoy_failed_update;
	delete[] I18N.vJoy_version;
	delete[] I18N.vJoy_version_notmatch;
	delete[] I18N.vJoy_X;
	delete[] I18N.vJoy_Y;
	delete[] I18N.vJoy_Z;
	delete[] I18N.vJoy_RX;
	delete[] I18N.vJoy_RY;
	delete[] I18N.vJoy_RZ;
	delete[] I18N.vJoy_SL0;
	delete[] I18N.vJoy_SL1;
	delete[] I18N.vJoy_XTR;
	delete[] I18N.vJoy_YTR;
	delete[] I18N.vJoy_ZTR;
	delete[] I18N.vJoy_RXTR;
	delete[] I18N.vJoy_RYTR;
	delete[] I18N.vJoy_RZTR;
	delete[] I18N.vJoy_SL0TR;
	delete[] I18N.vJoy_SL1TR;
	delete[] I18N.vJoy_XINV;
	delete[] I18N.vJoy_YINV;
	delete[] I18N.vJoy_ZINV;
	delete[] I18N.vJoy_RXINV;
	delete[] I18N.vJoy_RYINV;
	delete[] I18N.vJoy_RZINV;
	delete[] I18N.vJoy_SL0INV;
	delete[] I18N.vJoy_SL1INV;
	delete[] I18N.vJoy_DPAD1_LEFT;
	delete[] I18N.vJoy_DPAD1_UP_LEFT;
	delete[] I18N.vJoy_DPAD1_UP;
	delete[] I18N.vJoy_DPAD1_UP_RIGHT;
	delete[] I18N.vJoy_DPAD1_RIGHT;
	delete[] I18N.vJoy_DPAD1_DOWN_RIGHT;
	delete[] I18N.vJoy_DPAD1_DOWN;
	delete[] I18N.vJoy_DPAD1_DOWN_LEFT;
	delete[] I18N.vJoy_DPAD2_LEFT;
	delete[] I18N.vJoy_DPAD2_UP_LEFT;
	delete[] I18N.vJoy_DPAD2_UP;
	delete[] I18N.vJoy_DPAD2_UP_RIGHT;
	delete[] I18N.vJoy_DPAD2_RIGHT;
	delete[] I18N.vJoy_DPAD2_DOWN_RIGHT;
	delete[] I18N.vJoy_DPAD2_DOWN;
	delete[] I18N.vJoy_DPAD2_DOWN_LEFT;
	delete[] I18N.vJoy_DPAD3_LEFT;
	delete[] I18N.vJoy_DPAD3_UP_LEFT;
	delete[] I18N.vJoy_DPAD3_UP;
	delete[] I18N.vJoy_DPAD3_UP_RIGHT;
	delete[] I18N.vJoy_DPAD3_RIGHT;
	delete[] I18N.vJoy_DPAD3_DOWN_RIGHT;
	delete[] I18N.vJoy_DPAD3_DOWN;
	delete[] I18N.vJoy_DPAD3_DOWN_LEFT;
	delete[] I18N.vJoy_DPAD4_LEFT;
	delete[] I18N.vJoy_DPAD4_UP_LEFT;
	delete[] I18N.vJoy_DPAD4_UP;
	delete[] I18N.vJoy_DPAD4_UP_RIGHT;
	delete[] I18N.vJoy_DPAD4_RIGHT;
	delete[] I18N.vJoy_DPAD4_DOWN_RIGHT;
	delete[] I18N.vJoy_DPAD4_DOWN;
	delete[] I18N.vJoy_DPAD4_DOWN_LEFT;
}
