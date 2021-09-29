﻿#include "stdafx.h"
#include "MappingDataDlg.h"
#include "Ds2vJoy.h"

MappingDataDlg::MappingDataDlg()
	:m_hWnd()
	, m_hDlg()
	, m_parent()
	, firsttime(false)
	, canprint(false)
	, docked(0)
	, docked_last(0)
	, movable(true)
	, m_y(0)
	, mappingData()
	, Modified()
	, m_isClonedList(false)
	, m_mode(0)
{
	for (int i = 0; i < Mofified_count; i++) Modified[i] = false;
}

MappingDataDlg::~MappingDataDlg()
{
}

void MappingDataDlg::Init(HINSTANCE hInst, HWND hWnd)
{
	docked = 1;
	docked_last = 1;
	m_mode = 0;
	m_hWnd = hWnd;
	m_hDlg = CreateDialogParam(hInst, MAKEINTRESOURCE(IDD_MAPPING_ADD), hWnd, (DLGPROC)Proc, (LPARAM)this);
	long lStyle = GetWindowLong(m_hDlg, GWL_STYLE);
//	lStyle |= WS_THICKFRAME;
	lStyle = lStyle & ~WS_CAPTION;
	SetWindowLong(m_hDlg, GWL_STYLE, lStyle);
	Hide();
}

void MappingDataDlg::Open(HWND parent, int mode)
{
	m_mode = mode;
	firsttime = false;
	canprint = false;
//	ShowWindow(parent, SW_HIDE);
	ShowWindow(m_hDlg, SW_SHOW);
	PostMessage(m_hWnd, WM_SIZE, -1, -2);
	danger = false;
	capturehasbeendone = false;
	whohasfocus = 0;
	SetTimer(m_hDlg, 1, 100, NULL);
}

INT_PTR MappingDataDlg::Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	MappingDataDlg* dlg;
	if (uMsg == WM_INITDIALOG)
	{
		SetWindowLongPtr(hWnd, GWLP_USERDATA, lParam);
		dlg = (MappingDataDlg*)lParam;
	}
	else
		dlg = (MappingDataDlg*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if (dlg)
		return dlg->_proc(hWnd, uMsg, wParam, lParam);
	else
		return NULL;
}

INT_PTR MappingDataDlg::_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static POINT p;
	static int m_x;
	static int m_y;
	static int m_z;
	static int m_t;
	switch (uMsg)
	{
	case WM_NCCALCSIZE:
		if (wParam)
		{
//			NCCALCSIZE_PARAMS FAR* lpncsp = (NCCALCSIZE_PARAMS FAR*)lParam;
//			lpncsp->rgrc[0].top -= 6;
//			return FALSE;
			return TRUE;
		}
		return FALSE;
	case WM_CTLCOLORDLG:
	case WM_CTLCOLORMSGBOX:
	case WM_CTLCOLORSCROLLBAR:
	{
		HDC hdcStatic = (HDC)wParam;
		static HBRUSH hBrushColor;
		hBrushColor = CreateSolidBrush(RGB(191, 200, 196));
		SetTextColor(hdcStatic, RGB(10, 10, 10));
		SetBkMode(hdcStatic, TRANSPARENT);
		SetBkColor(hdcStatic, RGB(191, 200, 196));
		return (LRESULT)hBrushColor;
	}
	case WM_CTLCOLORBTN:
	case WM_CTLCOLORSTATIC:
	{
		HDC hdcStatic = (HDC)wParam;
		static HBRUSH hBrushColor;
		DWORD CtrlID = GetDlgCtrlID((HWND)lParam);
		if (CtrlID == IDC_MAPPING_X || CtrlID == IDC_MAPPING_Y)
		{
			hBrushColor = (danger) ? CreateSolidBrush(RGB(180, 0, 180)) : CreateSolidBrush(RGB(180, 180, 0));
			if (danger)
				SetTextColor(hdcStatic, RGB(224, 224, 255));
			else
				SetTextColor(hdcStatic, RGB(24, 24, 92));
		}
		else
		{
			hBrushColor = CreateSolidBrush(RGB(210, 210, 215));
			SetTextColor(hdcStatic, RGB(24, 24, 92));
		}
		SetBkMode(hdcStatic, TRANSPARENT);
		SetBkColor(hdcStatic, RGB(36, 163, 163));
		return (LRESULT)hBrushColor;
	}
	case WM_CTLCOLOREDIT:
	{
		HDC hdcStatic = (HDC)wParam;
		static HBRUSH hBrushColor;
		hBrushColor = CreateSolidBrush(RGB(228, 228, 232));
		SetTextColor(hdcStatic, RGB(62, 20, 102));
		SetBkMode(hdcStatic, TRANSPARENT);
		SetBkColor(hdcStatic, RGB(255, 255, 0));
		return (LRESULT)hBrushColor;
	}
	case WM_CTLCOLORLISTBOX:
	{
		HDC hdcStatic = (HDC)wParam;
		static HBRUSH hBrushColor;
		hBrushColor = CreateSolidBrush(RGB(216, 215, 220));
		SetTextColor(hdcStatic, RGB(10, 10, 10));
		SetBkMode(hdcStatic, TRANSPARENT);
		SetBkColor(hdcStatic, RGB(255, 255, 0));
		return (LRESULT)hBrushColor;
	}
	case WM_PAINT:
	{
		if (!IsIconic(hWnd))
		{
			PAINTSTRUCT ps;
			HDC hDC = BeginPaint(hWnd, &ps);

			RECT rect;

			GetClientRect(hWnd, &rect);

			HBRUSH brush = CreateSolidBrush(RGB(210, 210, 215));
			FillRect(hDC, &rect, brush);
			DeleteObject(brush);
			EndPaint(hWnd, &ps);
		}
		return FALSE;
	}
	case WM_LBUTTONDOWN:
	{
		SetCapture(m_hDlg);
		GetCursorPos(&p);
		RECT rect;
		GetWindowRect(hWnd, &rect);
		m_x = rect.left - p.x;
		m_y = rect.top - p.y;
		m_z = rect.right - p.x;
		m_t = rect.bottom - p.y;
		break;
	}
	case WM_NOTIFY:
	{
		switch (((LPNMLISTVIEW)lParam)->hdr.code)
		{
		case BCN_HOTITEMCHANGE:
		{
			bool whichone = false;
			static byte state[16] = { 0 };
			switch (((NMBCHOTITEM*)lParam)->dwFlags)
				// HICF_OTHER			= 0x00000000
				// HICF_MOUSE			= 0x00000001	Triggered by mouse
				// HICF_ARROWKEYS		= 0x00000002	Triggered by arrow keys
				// HICF_ACCELERATOR		= 0x00000004	Triggered by accelerator
				// HICF_DUPACCEL		= 0x00000008	This accelerator is not unique
				// HICF_ENTERING		= 0x00000010	idOld is invalid
				// HICF_LEAVING			= 0x00000020	idNew is invalid
				// HICF_RESELECT		= 0x00000040	hot item reselected
				// HICF_LMOUSE			= 0x00000080	left mouse button selected
				// HICF_TOGGLEDROPDOWN	= 0x00000100	Toggle button's dropdown state
			{
			case (HICF_ENTERING | HICF_MOUSE):
				switch (((LPNMHDR)lParam)->idFrom)
				{
				case IDC_MAPPING_ONRELEASE_1:state[0] = state[0] | 0x1; break;
				case IDC_MAPPING_ONRELEASE_2:state[1] = state[1] | 0x1; break;
				case IDC_MAPPING_ONRELEASE_3:state[2] = state[2] | 0x1; break;
				case IDC_MAPPING_ONRELEASE_4:state[3] = state[3] | 0x1; break;
				case IDC_MAPPING_ONRELEASE_5:state[4] = state[4] | 0x1; break;
				case IDC_MAPPING_ONRELEASE_6:state[5] = state[5] | 0x1; break;
				case IDC_MAPPING_ONRELEASE_7:state[6] = state[6] | 0x1; break;
				case IDC_MAPPING_ONRELEASE_8:state[7] = state[7] | 0x1; break;
				case IDC_MAPPING_NORELEASE_1:state[0] = state[0] | 0x2; break;
				case IDC_MAPPING_NORELEASE_2:state[1] = state[1] | 0x2; break;
				case IDC_MAPPING_NORELEASE_3:state[2] = state[2] | 0x2; break;
				case IDC_MAPPING_NORELEASE_4:state[3] = state[3] | 0x2; break;
				case IDC_MAPPING_NORELEASE_5:state[4] = state[4] | 0x2; break;
				case IDC_MAPPING_NORELEASE_6:state[5] = state[5] | 0x2; break;
				case IDC_MAPPING_NORELEASE_7:state[6] = state[6] | 0x2; break;
				case IDC_MAPPING_NORELEASE_8:state[7] = state[7] | 0x2; break;
				case IDC_MAPPING_NLRELEASE_1:state[0] = state[0] | 0x4; break;
				case IDC_MAPPING_NLRELEASE_2:state[1] = state[1] | 0x4; break;
				case IDC_MAPPING_NLRELEASE_3:state[2] = state[2] | 0x4; break;
				case IDC_MAPPING_NLRELEASE_4:state[3] = state[3] | 0x4; break;
				case IDC_MAPPING_NLRELEASE_5:state[4] = state[4] | 0x4; break;
				case IDC_MAPPING_NLRELEASE_6:state[5] = state[5] | 0x4; break;
				case IDC_MAPPING_NLRELEASE_7:state[6] = state[6] | 0x4; break;
				case IDC_MAPPING_NLRELEASE_8:state[7] = state[7] | 0x4; break;
				case IDC_MAPPING_MOUSE_ACTION_1:state[8] = state[8] | 0x1; whichone = true; break;
				case IDC_MAPPING_MOUSE_ACTION_2:state[9] = state[9] | 0x1; whichone = true; break;
				case IDC_MAPPING_MOUSE_ACTION_3:state[10] = state[10] | 0x1; whichone = true; break;
				case IDC_MAPPING_MOUSE_ACTION_4:state[11] = state[11] | 0x1; whichone = true; break;
				case IDC_MAPPING_MOUSE_ACTION_5:state[12] = state[12] | 0x1; whichone = true; break;
				case IDC_MAPPING_MOUSE_ACTION_6:state[13] = state[13] | 0x1; whichone = true; break;
				case IDC_MAPPING_MOUSE_ACTION_7:state[14] = state[14] | 0x1; whichone = true; break;
				case IDC_MAPPING_MOUSE_ACTION_8:state[15] = state[15] | 0x1; whichone = true; break;
				case IDC_MAPPING_OVERCONTROL_1:state[8] = state[8] | 0x2; whichone = true; break;
				case IDC_MAPPING_OVERCONTROL_2:state[9] = state[9] | 0x2; whichone = true; break;
				case IDC_MAPPING_OVERCONTROL_3:state[10] = state[10] | 0x2; whichone = true; break;
				case IDC_MAPPING_OVERCONTROL_4:state[11] = state[11] | 0x2; whichone = true; break;
				case IDC_MAPPING_OVERCONTROL_5:state[12] = state[12] | 0x2; whichone = true; break;
				case IDC_MAPPING_OVERCONTROL_6:state[13] = state[13] | 0x2; whichone = true; break;
				case IDC_MAPPING_OVERCONTROL_7:state[14] = state[14] | 0x2; whichone = true; break;
				case IDC_MAPPING_OVERCONTROL_8:state[15] = state[15] | 0x2; whichone = true; break;
				case IDC_MAPPING_SWITCH_1:state[8] = state[8] | 0x4; whichone = true; break;
				case IDC_MAPPING_SWITCH_2:state[9] = state[9] | 0x4; whichone = true; break;
				case IDC_MAPPING_SWITCH_3:state[10] = state[10] | 0x4; whichone = true; break;
				case IDC_MAPPING_SWITCH_4:state[11] = state[11] | 0x4; whichone = true; break;
				case IDC_MAPPING_SWITCH_5:state[12] = state[12] | 0x4; whichone = true; break;
				case IDC_MAPPING_SWITCH_6:state[13] = state[13] | 0x4; whichone = true; break;
				case IDC_MAPPING_SWITCH_7:state[14] = state[14] | 0x4; whichone = true; break;
				case IDC_MAPPING_SWITCH_8:state[15] = state[15] | 0x4; whichone = true; break;
				default:
					return FALSE;
				}
				break;
			case (HICF_LEAVING | HICF_MOUSE):
				switch (((LPNMHDR)lParam)->idFrom)
				{
				case IDC_MAPPING_ONRELEASE_1:state[0] = state[0] ^ 0x1; break;
				case IDC_MAPPING_ONRELEASE_2:state[1] = state[1] ^ 0x1; break;
				case IDC_MAPPING_ONRELEASE_3:state[2] = state[2] ^ 0x1; break;
				case IDC_MAPPING_ONRELEASE_4:state[3] = state[3] ^ 0x1; break;
				case IDC_MAPPING_ONRELEASE_5:state[4] = state[4] ^ 0x1; break;
				case IDC_MAPPING_ONRELEASE_6:state[5] = state[5] ^ 0x1; break;
				case IDC_MAPPING_ONRELEASE_7:state[6] = state[6] ^ 0x1; break;
				case IDC_MAPPING_ONRELEASE_8:state[7] = state[7] ^ 0x1; break;
				case IDC_MAPPING_NORELEASE_1:state[0] = state[0] ^ 0x2; break;
				case IDC_MAPPING_NORELEASE_2:state[1] = state[1] ^ 0x2; break;
				case IDC_MAPPING_NORELEASE_3:state[2] = state[2] ^ 0x2; break;
				case IDC_MAPPING_NORELEASE_4:state[3] = state[3] ^ 0x2; break;
				case IDC_MAPPING_NORELEASE_5:state[4] = state[4] ^ 0x2; break;
				case IDC_MAPPING_NORELEASE_6:state[5] = state[5] ^ 0x2; break;
				case IDC_MAPPING_NORELEASE_7:state[6] = state[6] ^ 0x2; break;
				case IDC_MAPPING_NORELEASE_8:state[7] = state[7] ^ 0x2; break;
				case IDC_MAPPING_NLRELEASE_1:state[0] = state[0] ^ 0x4; break;
				case IDC_MAPPING_NLRELEASE_2:state[1] = state[1] ^ 0x4; break;
				case IDC_MAPPING_NLRELEASE_3:state[2] = state[2] ^ 0x4; break;
				case IDC_MAPPING_NLRELEASE_4:state[3] = state[3] ^ 0x4; break;
				case IDC_MAPPING_NLRELEASE_5:state[4] = state[4] ^ 0x4; break;
				case IDC_MAPPING_NLRELEASE_6:state[5] = state[5] ^ 0x4; break;
				case IDC_MAPPING_NLRELEASE_7:state[6] = state[6] ^ 0x4; break;
				case IDC_MAPPING_NLRELEASE_8:state[7] = state[7] ^ 0x4; break;
				case IDC_MAPPING_MOUSE_ACTION_1:state[8] = state[8] ^ 0x1; whichone = true; break;
				case IDC_MAPPING_MOUSE_ACTION_2:state[9] = state[9] ^ 0x1; whichone = true; break;
				case IDC_MAPPING_MOUSE_ACTION_3:state[10] = state[10] ^ 0x1; whichone = true; break;
				case IDC_MAPPING_MOUSE_ACTION_4:state[11] = state[11] ^ 0x1; whichone = true; break;
				case IDC_MAPPING_MOUSE_ACTION_5:state[12] = state[12] ^ 0x1; whichone = true; break;
				case IDC_MAPPING_MOUSE_ACTION_6:state[13] = state[13] ^ 0x1; whichone = true; break;
				case IDC_MAPPING_MOUSE_ACTION_7:state[14] = state[14] ^ 0x1; whichone = true; break;
				case IDC_MAPPING_MOUSE_ACTION_8:state[15] = state[15] ^ 0x1; whichone = true; break;
				case IDC_MAPPING_OVERCONTROL_1:state[8] = state[8] ^ 0x2; whichone = true; break;
				case IDC_MAPPING_OVERCONTROL_2:state[9] = state[9] ^ 0x2; whichone = true; break;
				case IDC_MAPPING_OVERCONTROL_3:state[10] = state[10] ^ 0x2; whichone = true; break;
				case IDC_MAPPING_OVERCONTROL_4:state[11] = state[11] ^ 0x2; whichone = true; break;
				case IDC_MAPPING_OVERCONTROL_5:state[12] = state[12] ^ 0x2; whichone = true; break;
				case IDC_MAPPING_OVERCONTROL_6:state[13] = state[13] ^ 0x2; whichone = true; break;
				case IDC_MAPPING_OVERCONTROL_7:state[14] = state[14] ^ 0x2; whichone = true; break;
				case IDC_MAPPING_OVERCONTROL_8:state[15] = state[15] ^ 0x2; whichone = true; break;
				case IDC_MAPPING_SWITCH_1:state[8] = state[8] ^ 0x4; whichone = true; break;
				case IDC_MAPPING_SWITCH_2:state[9] = state[9] ^ 0x4; whichone = true; break;
				case IDC_MAPPING_SWITCH_3:state[10] = state[10] ^ 0x4; whichone = true; break;
				case IDC_MAPPING_SWITCH_4:state[11] = state[11] ^ 0x4; whichone = true; break;
				case IDC_MAPPING_SWITCH_5:state[12] = state[12] ^ 0x4; whichone = true; break;
				case IDC_MAPPING_SWITCH_6:state[13] = state[13] ^ 0x4; whichone = true; break;
				case IDC_MAPPING_SWITCH_7:state[14] = state[14] ^ 0x4; whichone = true; break;
				case IDC_MAPPING_SWITCH_8:state[15] = state[15] ^ 0x4; whichone = true; break;
				default:
					return FALSE;
				}
				break;
			default:
				return FALSE;
			}
			if (whichone)
			{
				bool oc[8];
				bool sw[8];
				for (int i = 0; i < 8; i++)
				{
					oc[i] = (WPARAM)mappingData.Overcontrol[i];
					sw[i] = (WPARAM)mappingData.Switch[i];
				}
				if (oc[0] || state[8] & 0x1 || state[8] & 0x2) { ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_OVERCONTROL_1), SW_SHOW); } else { ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_OVERCONTROL_1), SW_HIDE); }
				if (oc[1] || state[9] & 0x1 || state[9] & 0x2) { ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_OVERCONTROL_2), SW_SHOW); } else { ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_OVERCONTROL_2), SW_HIDE); }
				if (oc[2] || state[10] & 0x1 || state[10] & 0x2) { ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_OVERCONTROL_3), SW_SHOW); } else { ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_OVERCONTROL_3), SW_HIDE); }
				if (oc[3] || state[11] & 0x1 || state[11] & 0x2) { ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_OVERCONTROL_4), SW_SHOW); } else { ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_OVERCONTROL_4), SW_HIDE); }
				if (oc[4] || state[12] & 0x1 || state[12] & 0x2) { ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_OVERCONTROL_5), SW_SHOW); } else { ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_OVERCONTROL_5), SW_HIDE); }
				if (oc[5] || state[13] & 0x1 || state[13] & 0x2) { ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_OVERCONTROL_6), SW_SHOW); } else { ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_OVERCONTROL_6), SW_HIDE); }
				if (oc[6] || state[14] & 0x1 || state[14] & 0x2) { ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_OVERCONTROL_7), SW_SHOW); } else { ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_OVERCONTROL_7), SW_HIDE); }
				if (oc[7] || state[15] & 0x1 || state[15] & 0x2) { ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_OVERCONTROL_8), SW_SHOW); } else { ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_OVERCONTROL_8), SW_HIDE); }
				if (sw[0] || state[8] & 0x1 || state[8] & 0x4) { ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_SWITCH_1), SW_SHOW); } else { ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_SWITCH_1), SW_HIDE); }
				if (sw[1] || state[9] & 0x1 || state[9] & 0x4) { ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_SWITCH_2), SW_SHOW); } else { ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_SWITCH_2), SW_HIDE); }
				if (sw[2] || state[10] & 0x1 || state[10] & 0x4) { ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_SWITCH_3), SW_SHOW); } else { ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_SWITCH_3), SW_HIDE); }
				if (sw[3] || state[11] & 0x1 || state[11] & 0x4) { ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_SWITCH_4), SW_SHOW); } else { ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_SWITCH_4), SW_HIDE); }
				if (sw[4] || state[12] & 0x1 || state[12] & 0x4) { ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_SWITCH_5), SW_SHOW); } else { ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_SWITCH_5), SW_HIDE); }
				if (sw[5] || state[13] & 0x1 || state[13] & 0x4) { ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_SWITCH_6), SW_SHOW); } else { ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_SWITCH_6), SW_HIDE); }
				if (sw[6] || state[14] & 0x1 || state[14] & 0x4) { ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_SWITCH_7), SW_SHOW); } else { ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_SWITCH_7), SW_HIDE); }
				if (sw[7] || state[15] & 0x1 || state[15] & 0x4) { ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_SWITCH_8), SW_SHOW); } else { ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_SWITCH_8), SW_HIDE); }
			}
			else
			{
				bool no[8];
				bool nl[8];
				for (int i = 0; i < 8; i++)
				{
					nl[i] = !(WPARAM)mappingData.NlRelease[i];
					no[i] = nl[i] && !(WPARAM)mappingData.NoRelease[i];
				}
				if (state[0]) { ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_NORELEASE_1), SW_SHOW); ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_NLRELEASE_1), SW_SHOW); }
				else { if (no[0]) ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_NORELEASE_1), SW_HIDE); if (nl[0]) ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_NLRELEASE_1), SW_HIDE); }
				if (state[1]) { ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_NORELEASE_2), SW_SHOW); ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_NLRELEASE_2), SW_SHOW); }
				else { if (no[1]) ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_NORELEASE_2), SW_HIDE); if (nl[1]) ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_NLRELEASE_2), SW_HIDE); }
				if (state[2]) { ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_NORELEASE_3), SW_SHOW); ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_NLRELEASE_3), SW_SHOW); }
				else { if (no[2]) ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_NORELEASE_3), SW_HIDE); if (nl[2]) ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_NLRELEASE_3), SW_HIDE); }
				if (state[3]) { ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_NORELEASE_4), SW_SHOW); ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_NLRELEASE_4), SW_SHOW); }
				else { if (no[3]) ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_NORELEASE_4), SW_HIDE); if (nl[3]) ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_NLRELEASE_4), SW_HIDE); }
				if (state[4]) { ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_NORELEASE_5), SW_SHOW); ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_NLRELEASE_5), SW_SHOW); }
				else { if (no[4]) ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_NORELEASE_5), SW_HIDE); if (nl[4]) ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_NLRELEASE_5), SW_HIDE); }
				if (state[5]) { ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_NORELEASE_6), SW_SHOW); ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_NLRELEASE_6), SW_SHOW); }
				else { if (no[5]) ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_NORELEASE_6), SW_HIDE); if (nl[5]) ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_NLRELEASE_6), SW_HIDE); }
				if (state[6]) { ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_NORELEASE_7), SW_SHOW); ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_NLRELEASE_7), SW_SHOW); }
				else { if (no[6]) ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_NORELEASE_7), SW_HIDE); if (nl[6]) ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_NLRELEASE_7), SW_HIDE); }
				if (state[7]) { ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_NORELEASE_8), SW_SHOW); ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_NLRELEASE_8), SW_SHOW); }
				else { if (no[7]) ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_NORELEASE_8), SW_HIDE); if (nl[7]) ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_NLRELEASE_8), SW_HIDE); }
			}
			break;
		}
		default:
			return FALSE;
		}
		break;
	}
	case WM_MOUSEMOVE:
		if (movable)
		{
			RECT win;
			GetWindowRect(m_hWnd, &win);
			if ((GetKeyState(VK_LBUTTON) & 0x800) == 0)
			{
				if (docked == 2)
					::MoveWindow(m_hDlg, win.left + 9, win.top + 31, 474, 287, FALSE);
				ReleaseCapture();
				break;
			}
			GetCursorPos(&p);
			RECT rect;
			GetWindowRect(hWnd, &rect);

			if (abs(win.right - m_x - 6 - p.x) < 40)															//docked right
			{
				docked = 1;
				docked_last = 1;
				::MoveWindow(m_hDlg, win.right - 6, m_y + p.y, 474, 287, FALSE);
			}
			else if ((abs(win.left - m_x + 9 - p.x) < 40) && (p.y > win.top + 20) && (p.y < win.bottom - 20))	//docked center
			{
				docked = 2;
				docked_last = 2;
				::MoveWindow(m_hDlg, win.left + 9, m_y + p.y, 474, 287, FALSE);
			}
			else if (abs(win.left - m_z + 6 - p.x) < 40)														//docked left
			{
				docked = 3;
				docked_last = 3;
				::MoveWindow(m_hDlg, win.left + 6 - 474, m_y + p.y, 474, 287, FALSE);
			}
			else																								//no docked
			{
				docked = 0;
				docked_last = 0;
				::MoveWindow(m_hDlg, m_x + p.x, m_y + p.y, 474, 287, FALSE);
			}
		}
		else
			SetFocus(m_hWnd);
		break;
	case WM_TIMER:
	{
		if (IsIconic(hWnd))
			break;

		if (wParam == 1)
		{
			now = std::chrono::system_clock::now();
			if (now - start < std::chrono::milliseconds(3000))
			{
				danger = true;
				POINT q;
				GetCursorPos(&q);

				TCHAR buf[6];
				swprintf_s(buf, TEXT("%d"), q.x);
				SetWindowText(GetDlgItem(hWnd, IDC_MAPPING_X), buf);
				swprintf_s(buf, TEXT("%d"), q.y);
				SetWindowText(GetDlgItem(hWnd, IDC_MAPPING_Y), buf);
			}
			else if (danger)
			{
				danger = false;
				capturehasbeendone = true;
				Hide();
				Show();
			}
		}
		break;
	}
	case WM_SHOWWINDOW:
		if (wParam == TRUE)
		{
			if (!firsttime)
			{
				GetCursorPos(&p);
				HFONT hFont = CreateFont(12, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
				SendDlgItemMessage(hWnd, IDC_MAPPING_DS_1, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
				SendDlgItemMessage(hWnd, IDC_MAPPING_DS_2, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
				SendDlgItemMessage(hWnd, IDC_MAPPING_DS_3, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
				SendDlgItemMessage(hWnd, IDC_MAPPING_DS_4, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
				SendDlgItemMessage(hWnd, IDC_MAPPING_DS_5, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
				SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_1, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
				SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_2, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
				SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_3, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
				SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_4, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
				SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_5, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
				SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_6, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
				SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_7, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
				SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_8, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
				SendDlgItemMessage(hWnd, IDC_MAPPING_LED, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
				SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_1_START, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
				SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_2_START, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
				SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_3_START, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
				SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_4_START, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
				SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_5_START, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
				SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_6_START, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
				SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_7_START, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
				SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_8_START, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
				SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_1_STOP, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
				SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_2_STOP, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
				SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_3_STOP, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
				SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_4_STOP, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
				SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_5_STOP, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
				SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_6_STOP, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
				SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_7_STOP, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
				SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_8_STOP, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
				SendDlgItemMessage(hWnd, IDC_MAPPING_GRID_X, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
				SendDlgItemMessage(hWnd, IDC_MAPPING_GRID_Y, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
				SendDlgItemMessage(hWnd, IDC_MAPPING_GRID_W, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
				SendDlgItemMessage(hWnd, IDC_MAPPING_GRID_H, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
				SendDlgItemMessage(hWnd, IDC_MAPPING_GRID_NW, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
				SendDlgItemMessage(hWnd, IDC_MAPPING_GRID_NH, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
				SendDlgItemMessage(hWnd, IDC_MAPPING_X, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
				SendDlgItemMessage(hWnd, IDC_MAPPING_Y, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));

				ShowWindow(GetDlgItem(hWnd, IDC_STATIC_PICTURE), SW_SHOW);
				ShowWindow(GetDlgItem(hWnd, IDC_STATIC_HELP), SW_HIDE);
				SendDlgItemMessage(hWnd, IDC_MAPPING_DS_1, CB_RESETCONTENT, 0, 0);
				SendDlgItemMessage(hWnd, IDC_MAPPING_DS_2, CB_RESETCONTENT, 0, 0);
				SendDlgItemMessage(hWnd, IDC_MAPPING_DS_3, CB_RESETCONTENT, 0, 0);
				SendDlgItemMessage(hWnd, IDC_MAPPING_DS_4, CB_RESETCONTENT, 0, 0);
				SendDlgItemMessage(hWnd, IDC_MAPPING_DS_5, CB_RESETCONTENT, 0, 0);
				SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_1, CB_RESETCONTENT, 0, 0);
				SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_2, CB_RESETCONTENT, 0, 0);
				SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_3, CB_RESETCONTENT, 0, 0);
				SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_4, CB_RESETCONTENT, 0, 0);
				SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_5, CB_RESETCONTENT, 0, 0);
				SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_6, CB_RESETCONTENT, 0, 0);
				SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_7, CB_RESETCONTENT, 0, 0);
				SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_8, CB_RESETCONTENT, 0, 0);
				for (int i = 0; i < dsButtonID::button_Count; i++)
				{
					WCHAR* str = dsButton::String((dsButtonID)i);
					if (!(WPARAM)mappingData.Target[0])
						SendDlgItemMessage(hWnd, IDC_MAPPING_DS_1, CB_ADDSTRING, 0, (LPARAM)str);
					if (!(WPARAM)mappingData.Target[1])
						SendDlgItemMessage(hWnd, IDC_MAPPING_DS_2, CB_ADDSTRING, 0, (LPARAM)str);
					if (!(WPARAM)mappingData.Target[2])
						SendDlgItemMessage(hWnd, IDC_MAPPING_DS_3, CB_ADDSTRING, 0, (LPARAM)str);
					if (!(WPARAM)mappingData.Target[3])
						SendDlgItemMessage(hWnd, IDC_MAPPING_DS_4, CB_ADDSTRING, 0, (LPARAM)str);
					if (!(WPARAM)mappingData.Target[4])
						SendDlgItemMessage(hWnd, IDC_MAPPING_DS_5, CB_ADDSTRING, 0, (LPARAM)str);
				}
				for (int i = 0; i < vJoyButtonID::button_Count; i++)
				{
					WCHAR* str = vJoyButton::String((vJoyButtonID)i);
					if ((WPARAM)mappingData.Target[0])
						SendDlgItemMessage(hWnd, IDC_MAPPING_DS_1, CB_ADDSTRING, 0, (LPARAM)str);
					if ((WPARAM)mappingData.Target[1])
						SendDlgItemMessage(hWnd, IDC_MAPPING_DS_2, CB_ADDSTRING, 0, (LPARAM)str);
					if ((WPARAM)mappingData.Target[2])
						SendDlgItemMessage(hWnd, IDC_MAPPING_DS_3, CB_ADDSTRING, 0, (LPARAM)str);
					if ((WPARAM)mappingData.Target[3])
						SendDlgItemMessage(hWnd, IDC_MAPPING_DS_4, CB_ADDSTRING, 0, (LPARAM)str);
					if ((WPARAM)mappingData.Target[4])
						SendDlgItemMessage(hWnd, IDC_MAPPING_DS_5, CB_ADDSTRING, 0, (LPARAM)str);
					if ((WPARAM)!mappingData.MouseAction[0])
						SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_1, CB_ADDSTRING, 0, (LPARAM)str);
					if ((WPARAM)!mappingData.MouseAction[1])
						SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_2, CB_ADDSTRING, 0, (LPARAM)str);
					if ((WPARAM)!mappingData.MouseAction[2])
						SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_3, CB_ADDSTRING, 0, (LPARAM)str);
					if ((WPARAM)!mappingData.MouseAction[3])
						SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_4, CB_ADDSTRING, 0, (LPARAM)str);
					if ((WPARAM)!mappingData.MouseAction[4])
						SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_5, CB_ADDSTRING, 0, (LPARAM)str);
					if ((WPARAM)!mappingData.MouseAction[5])
						SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_6, CB_ADDSTRING, 0, (LPARAM)str);
					if ((WPARAM)!mappingData.MouseAction[6])
						SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_7, CB_ADDSTRING, 0, (LPARAM)str);
					if ((WPARAM)!mappingData.MouseAction[7])
						SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_8, CB_ADDSTRING, 0, (LPARAM)str);
				}
				for (int i = 0; i < Mapping::mouse_Count; i++)
				{
					WCHAR* str = Mapping::MouseString((MouseActionID)i);
					if ((WPARAM)mappingData.MouseAction[0] == 1)
						SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_1, CB_ADDSTRING, 0, (LPARAM)str);
					if ((WPARAM)mappingData.MouseAction[1] == 1)
						SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_2, CB_ADDSTRING, 0, (LPARAM)str);
					if ((WPARAM)mappingData.MouseAction[2] == 1)
						SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_3, CB_ADDSTRING, 0, (LPARAM)str);
					if ((WPARAM)mappingData.MouseAction[3] == 1)
						SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_4, CB_ADDSTRING, 0, (LPARAM)str);
					if ((WPARAM)mappingData.MouseAction[4] == 1)
						SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_5, CB_ADDSTRING, 0, (LPARAM)str);
					if ((WPARAM)mappingData.MouseAction[5] == 1)
						SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_6, CB_ADDSTRING, 0, (LPARAM)str);
					if ((WPARAM)mappingData.MouseAction[6] == 1)
						SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_7, CB_ADDSTRING, 0, (LPARAM)str);
					if ((WPARAM)mappingData.MouseAction[7] == 1)
						SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_8, CB_ADDSTRING, 0, (LPARAM)str);
				}
				for (int i = 0; i < vJoyAxisMoveID::axismove_Count; i++)
				{
					WCHAR* str = vJoyButton::StringAxis((vJoyAxisMoveID)i);
					if ((WPARAM)mappingData.MouseAction[0] == 2)
						SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_1, CB_ADDSTRING, 0, (LPARAM)str);
					if ((WPARAM)mappingData.MouseAction[1] == 2)
						SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_2, CB_ADDSTRING, 0, (LPARAM)str);
					if ((WPARAM)mappingData.MouseAction[2] == 2)
						SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_3, CB_ADDSTRING, 0, (LPARAM)str);
					if ((WPARAM)mappingData.MouseAction[3] == 2)
						SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_4, CB_ADDSTRING, 0, (LPARAM)str);
					if ((WPARAM)mappingData.MouseAction[4] == 2)
						SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_5, CB_ADDSTRING, 0, (LPARAM)str);
					if ((WPARAM)mappingData.MouseAction[5] == 2)
						SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_6, CB_ADDSTRING, 0, (LPARAM)str);
					if ((WPARAM)mappingData.MouseAction[6] == 2)
						SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_7, CB_ADDSTRING, 0, (LPARAM)str);
					if ((WPARAM)mappingData.MouseAction[7] == 2)
						SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_8, CB_ADDSTRING, 0, (LPARAM)str);
				}
				firsttime = true;
			}

			bool no[8];
			bool nl[8];
			for (int i = 0; i < 8; i++)
			{
				nl[i] = (WPARAM)mappingData.NlRelease[i];
				no[i] = nl[i] || (WPARAM)mappingData.NoRelease[i];
			}
			if (no[0]) ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_NORELEASE_1), SW_SHOW); if (nl[0]) ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_NLRELEASE_1), SW_SHOW);
			if (no[1]) ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_NORELEASE_2), SW_SHOW); if (nl[1]) ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_NLRELEASE_2), SW_SHOW);
			if (no[2]) ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_NORELEASE_3), SW_SHOW); if (nl[2]) ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_NLRELEASE_3), SW_SHOW);
			if (no[3]) ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_NORELEASE_4), SW_SHOW); if (nl[3]) ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_NLRELEASE_4), SW_SHOW);
			if (no[4]) ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_NORELEASE_5), SW_SHOW); if (nl[4]) ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_NLRELEASE_5), SW_SHOW);
			if (no[5]) ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_NORELEASE_6), SW_SHOW); if (nl[5]) ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_NLRELEASE_6), SW_SHOW);
			if (no[6]) ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_NORELEASE_7), SW_SHOW); if (nl[6]) ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_NLRELEASE_7), SW_SHOW);
			if (no[7]) ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_NORELEASE_8), SW_SHOW); if (nl[7]) ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_NLRELEASE_8), SW_SHOW);

			if ((WPARAM)mappingData.Overcontrol[0]) ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_OVERCONTROL_1), SW_SHOW);
			if ((WPARAM)mappingData.Overcontrol[1]) ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_OVERCONTROL_2), SW_SHOW);
			if ((WPARAM)mappingData.Overcontrol[2]) ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_OVERCONTROL_3), SW_SHOW);
			if ((WPARAM)mappingData.Overcontrol[3]) ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_OVERCONTROL_4), SW_SHOW);
			if ((WPARAM)mappingData.Overcontrol[4]) ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_OVERCONTROL_5), SW_SHOW);
			if ((WPARAM)mappingData.Overcontrol[5]) ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_OVERCONTROL_6), SW_SHOW);
			if ((WPARAM)mappingData.Overcontrol[6]) ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_OVERCONTROL_7), SW_SHOW);
			if ((WPARAM)mappingData.Overcontrol[7]) ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_OVERCONTROL_8), SW_SHOW);

			if ((WPARAM)mappingData.Switch[0]) ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_SWITCH_1), SW_SHOW);
			if ((WPARAM)mappingData.Switch[1]) ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_SWITCH_2), SW_SHOW);
			if ((WPARAM)mappingData.Switch[2]) ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_SWITCH_3), SW_SHOW);
			if ((WPARAM)mappingData.Switch[3]) ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_SWITCH_4), SW_SHOW);
			if ((WPARAM)mappingData.Switch[4]) ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_SWITCH_5), SW_SHOW);
			if ((WPARAM)mappingData.Switch[5]) ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_SWITCH_6), SW_SHOW);
			if ((WPARAM)mappingData.Switch[6]) ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_SWITCH_7), SW_SHOW);
			if ((WPARAM)mappingData.Switch[7]) ShowWindow(GetDlgItem(hWnd, IDC_MAPPING_SWITCH_8), SW_SHOW);

			SendDlgItemMessage(hWnd, IDC_MAPPING_DS_1, CB_SETCURSEL, (WPARAM)mappingData.dsID[0], 0);
			SendDlgItemMessage(hWnd, IDC_MAPPING_DS_2, CB_SETCURSEL, (WPARAM)mappingData.dsID[1], 0);
			SendDlgItemMessage(hWnd, IDC_MAPPING_DS_3, CB_SETCURSEL, (WPARAM)mappingData.dsID[2], 0);
			SendDlgItemMessage(hWnd, IDC_MAPPING_DS_4, CB_SETCURSEL, (WPARAM)mappingData.dsID[3], 0);
			SendDlgItemMessage(hWnd, IDC_MAPPING_DS_5, CB_SETCURSEL, (WPARAM)mappingData.dsID[4], 0);
			SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_1, CB_SETCURSEL, (WPARAM)mappingData.vjID[0], 0);
			SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_2, CB_SETCURSEL, (WPARAM)mappingData.vjID[1], 0);
			SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_3, CB_SETCURSEL, (WPARAM)mappingData.vjID[2], 0);
			SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_4, CB_SETCURSEL, (WPARAM)mappingData.vjID[3], 0);
			SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_5, CB_SETCURSEL, (WPARAM)mappingData.vjID[4], 0);
			SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_6, CB_SETCURSEL, (WPARAM)mappingData.vjID[5], 0);
			SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_7, CB_SETCURSEL, (WPARAM)mappingData.vjID[6], 0);
			SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_8, CB_SETCURSEL, (WPARAM)mappingData.vjID[7], 0);
			CheckDlgButton(hWnd, IDC_MAPPING_TARGET_1, (WPARAM)mappingData.Target[0]);
			CheckDlgButton(hWnd, IDC_MAPPING_TARGET_2, (WPARAM)mappingData.Target[1]);
			CheckDlgButton(hWnd, IDC_MAPPING_TARGET_3, (WPARAM)mappingData.Target[2]);
			CheckDlgButton(hWnd, IDC_MAPPING_TARGET_4, (WPARAM)mappingData.Target[3]);
			CheckDlgButton(hWnd, IDC_MAPPING_TARGET_5, (WPARAM)mappingData.Target[4]);
			CheckDlgButton(hWnd, IDC_MAPPING_DS_1_DISABLE, (WPARAM)mappingData.dsDisable[0]);
			CheckDlgButton(hWnd, IDC_MAPPING_DS_2_DISABLE, (WPARAM)mappingData.dsDisable[1]);
			CheckDlgButton(hWnd, IDC_MAPPING_DS_3_DISABLE, (WPARAM)mappingData.dsDisable[2]);
			CheckDlgButton(hWnd, IDC_MAPPING_DS_4_DISABLE, (WPARAM)mappingData.dsDisable[3]);
			CheckDlgButton(hWnd, IDC_MAPPING_DS_5_DISABLE, (WPARAM)mappingData.dsDisable[4]);
			if ((WPARAM)mappingData.MouseAction[0])
			{
				CheckDlgButton(hWnd, IDC_MAPPING_VJOY_1_DISABLE, BST_UNCHECKED);
				EnableWindow(GetDlgItem(hWnd, IDC_MAPPING_VJOY_1_DISABLE), false);
			}
			else
			{
				EnableWindow(GetDlgItem(hWnd, IDC_MAPPING_VJOY_1_DISABLE), true);
				CheckDlgButton(hWnd, IDC_MAPPING_VJOY_1_DISABLE, (WPARAM)mappingData.vjDisable[0]);
			}
			if ((WPARAM)mappingData.MouseAction[1])
			{
				CheckDlgButton(hWnd, IDC_MAPPING_VJOY_2_DISABLE, BST_UNCHECKED);
				EnableWindow(GetDlgItem(hWnd, IDC_MAPPING_VJOY_2_DISABLE), false);
			}
			else
			{
				EnableWindow(GetDlgItem(hWnd, IDC_MAPPING_VJOY_2_DISABLE), true);
				CheckDlgButton(hWnd, IDC_MAPPING_VJOY_2_DISABLE, (WPARAM)mappingData.vjDisable[1]);
			}
			if ((WPARAM)mappingData.MouseAction[2])
			{
				CheckDlgButton(hWnd, IDC_MAPPING_VJOY_3_DISABLE, BST_UNCHECKED);
				EnableWindow(GetDlgItem(hWnd, IDC_MAPPING_VJOY_3_DISABLE), false);
			}
			else
			{
				EnableWindow(GetDlgItem(hWnd, IDC_MAPPING_VJOY_3_DISABLE), true);
				CheckDlgButton(hWnd, IDC_MAPPING_VJOY_3_DISABLE, (WPARAM)mappingData.vjDisable[2]);
			}
			if ((WPARAM)mappingData.MouseAction[3])
			{
				CheckDlgButton(hWnd, IDC_MAPPING_VJOY_4_DISABLE, BST_UNCHECKED);
				EnableWindow(GetDlgItem(hWnd, IDC_MAPPING_VJOY_4_DISABLE), false);
			}
			else
			{
				EnableWindow(GetDlgItem(hWnd, IDC_MAPPING_VJOY_4_DISABLE), true);
				CheckDlgButton(hWnd, IDC_MAPPING_VJOY_4_DISABLE, (WPARAM)mappingData.vjDisable[3]);
			}
			if ((WPARAM)mappingData.MouseAction[4])
			{
				CheckDlgButton(hWnd, IDC_MAPPING_VJOY_5_DISABLE, BST_UNCHECKED);
				EnableWindow(GetDlgItem(hWnd, IDC_MAPPING_VJOY_5_DISABLE), false);
			}
			else
			{
				EnableWindow(GetDlgItem(hWnd, IDC_MAPPING_VJOY_5_DISABLE), true);
				CheckDlgButton(hWnd, IDC_MAPPING_VJOY_5_DISABLE, (WPARAM)mappingData.vjDisable[4]);
			}
			if ((WPARAM)mappingData.MouseAction[5])
			{
				CheckDlgButton(hWnd, IDC_MAPPING_VJOY_6_DISABLE, BST_UNCHECKED);
				EnableWindow(GetDlgItem(hWnd, IDC_MAPPING_VJOY_6_DISABLE), false);
			}
			else
			{
				EnableWindow(GetDlgItem(hWnd, IDC_MAPPING_VJOY_6_DISABLE), true);
				CheckDlgButton(hWnd, IDC_MAPPING_VJOY_6_DISABLE, (WPARAM)mappingData.vjDisable[5]);
			}
			if ((WPARAM)mappingData.MouseAction[6])
			{
				CheckDlgButton(hWnd, IDC_MAPPING_VJOY_7_DISABLE, BST_UNCHECKED);
				EnableWindow(GetDlgItem(hWnd, IDC_MAPPING_VJOY_7_DISABLE), false);
			}
			else
			{
				EnableWindow(GetDlgItem(hWnd, IDC_MAPPING_VJOY_7_DISABLE), true);
				CheckDlgButton(hWnd, IDC_MAPPING_VJOY_7_DISABLE, (WPARAM)mappingData.vjDisable[6]);
			}
			if ((WPARAM)mappingData.MouseAction[7])
			{
				CheckDlgButton(hWnd, IDC_MAPPING_VJOY_8_DISABLE, BST_UNCHECKED);
				EnableWindow(GetDlgItem(hWnd, IDC_MAPPING_VJOY_8_DISABLE), false);
			}
			else
			{
				EnableWindow(GetDlgItem(hWnd, IDC_MAPPING_VJOY_8_DISABLE), true);
				CheckDlgButton(hWnd, IDC_MAPPING_VJOY_8_DISABLE, (WPARAM)mappingData.vjDisable[7]);
			}
			CheckDlgButton(hWnd, IDC_MAPPING_DS_2_OR, (WPARAM)mappingData.OrXorNot[0]);
			CheckDlgButton(hWnd, IDC_MAPPING_DS_3_OR, (WPARAM)mappingData.OrXorNot[1]);
			CheckDlgButton(hWnd, IDC_MAPPING_DS_4_NOT, (WPARAM)mappingData.OrXorNot[2]);
			CheckDlgButton(hWnd, IDC_MAPPING_DS_5_NOT, (WPARAM)mappingData.OrXorNot[3]);
			CheckDlgButton(hWnd, IDC_MAPPING_IF_MOUSE, (WPARAM)mappingData.Ifmouse);
			CheckDlgButton(hWnd, IDC_MAPPING_FORCE, (WPARAM)mappingData.Force);
			CheckDlgButton(hWnd, IDC_MAPPING_SHORT, (WPARAM)mappingData.Short);
			CheckDlgButton(hWnd, IDC_MAPPING_DOUBLE, (WPARAM)mappingData.Double);
			CheckDlgButton(hWnd, IDC_MAPPING_LONG, (WPARAM)mappingData.Long);
			SendDlgItemMessage(hWnd, IDC_MAPPING_LED, CB_SETCURSEL, (WPARAM)mappingData.Led, 0);
			CheckDlgButton(hWnd, IDC_MAPPING_MACRO, (WPARAM)mappingData.Macro);
			CheckDlgButton(hWnd, IDC_MAPPING_PAUSE, (WPARAM)mappingData.Pause);
			CheckDlgButton(hWnd, IDC_MAPPING_TRANSITIVITY, (WPARAM)mappingData.Transitivity);
			CheckDlgButton(hWnd, IDC_MAPPING_TOGGLE, (WPARAM)mappingData.Toggle);
			CheckDlgButton(hWnd, IDC_MAPPING_ONRELEASE_1, (WPARAM)mappingData.OnRelease[0]);
			CheckDlgButton(hWnd, IDC_MAPPING_ONRELEASE_2, (WPARAM)mappingData.OnRelease[1]);
			CheckDlgButton(hWnd, IDC_MAPPING_ONRELEASE_3, (WPARAM)mappingData.OnRelease[2]);
			CheckDlgButton(hWnd, IDC_MAPPING_ONRELEASE_4, (WPARAM)mappingData.OnRelease[3]);
			CheckDlgButton(hWnd, IDC_MAPPING_ONRELEASE_5, (WPARAM)mappingData.OnRelease[4]);
			CheckDlgButton(hWnd, IDC_MAPPING_ONRELEASE_6, (WPARAM)mappingData.OnRelease[5]);
			CheckDlgButton(hWnd, IDC_MAPPING_ONRELEASE_7, (WPARAM)mappingData.OnRelease[6]);
			CheckDlgButton(hWnd, IDC_MAPPING_ONRELEASE_8, (WPARAM)mappingData.OnRelease[7]);
			CheckDlgButton(hWnd, IDC_MAPPING_NORELEASE_1, (WPARAM)mappingData.NoRelease[0]);
			CheckDlgButton(hWnd, IDC_MAPPING_NORELEASE_2, (WPARAM)mappingData.NoRelease[1]);
			CheckDlgButton(hWnd, IDC_MAPPING_NORELEASE_3, (WPARAM)mappingData.NoRelease[2]);
			CheckDlgButton(hWnd, IDC_MAPPING_NORELEASE_4, (WPARAM)mappingData.NoRelease[3]);
			CheckDlgButton(hWnd, IDC_MAPPING_NORELEASE_5, (WPARAM)mappingData.NoRelease[4]);
			CheckDlgButton(hWnd, IDC_MAPPING_NORELEASE_6, (WPARAM)mappingData.NoRelease[5]);
			CheckDlgButton(hWnd, IDC_MAPPING_NORELEASE_7, (WPARAM)mappingData.NoRelease[6]);
			CheckDlgButton(hWnd, IDC_MAPPING_NORELEASE_8, (WPARAM)mappingData.NoRelease[7]);
			CheckDlgButton(hWnd, IDC_MAPPING_NLRELEASE_1, (WPARAM)mappingData.NlRelease[0]);
			CheckDlgButton(hWnd, IDC_MAPPING_NLRELEASE_2, (WPARAM)mappingData.NlRelease[1]);
			CheckDlgButton(hWnd, IDC_MAPPING_NLRELEASE_3, (WPARAM)mappingData.NlRelease[2]);
			CheckDlgButton(hWnd, IDC_MAPPING_NLRELEASE_4, (WPARAM)mappingData.NlRelease[3]);
			CheckDlgButton(hWnd, IDC_MAPPING_NLRELEASE_5, (WPARAM)mappingData.NlRelease[4]);
			CheckDlgButton(hWnd, IDC_MAPPING_NLRELEASE_6, (WPARAM)mappingData.NlRelease[5]);
			CheckDlgButton(hWnd, IDC_MAPPING_NLRELEASE_7, (WPARAM)mappingData.NlRelease[6]);
			CheckDlgButton(hWnd, IDC_MAPPING_NLRELEASE_8, (WPARAM)mappingData.NlRelease[7]);
			CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_ACTION_1, (WPARAM)mappingData.MouseAction[0]);
			CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_ACTION_2, (WPARAM)mappingData.MouseAction[1]);
			CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_ACTION_3, (WPARAM)mappingData.MouseAction[2]);
			CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_ACTION_4, (WPARAM)mappingData.MouseAction[3]);
			CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_ACTION_5, (WPARAM)mappingData.MouseAction[4]);
			CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_ACTION_6, (WPARAM)mappingData.MouseAction[5]);
			CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_ACTION_7, (WPARAM)mappingData.MouseAction[6]);
			CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_ACTION_8, (WPARAM)mappingData.MouseAction[7]);
			CheckDlgButton(hWnd, IDC_MAPPING_OVERCONTROL_1, (WPARAM)mappingData.Overcontrol[0]);
			CheckDlgButton(hWnd, IDC_MAPPING_OVERCONTROL_2, (WPARAM)mappingData.Overcontrol[1]);
			CheckDlgButton(hWnd, IDC_MAPPING_OVERCONTROL_3, (WPARAM)mappingData.Overcontrol[2]);
			CheckDlgButton(hWnd, IDC_MAPPING_OVERCONTROL_4, (WPARAM)mappingData.Overcontrol[3]);
			CheckDlgButton(hWnd, IDC_MAPPING_OVERCONTROL_5, (WPARAM)mappingData.Overcontrol[4]);
			CheckDlgButton(hWnd, IDC_MAPPING_OVERCONTROL_6, (WPARAM)mappingData.Overcontrol[5]);
			CheckDlgButton(hWnd, IDC_MAPPING_OVERCONTROL_7, (WPARAM)mappingData.Overcontrol[6]);
			CheckDlgButton(hWnd, IDC_MAPPING_OVERCONTROL_8, (WPARAM)mappingData.Overcontrol[7]);
			CheckDlgButton(hWnd, IDC_MAPPING_SWITCH_1, (WPARAM)mappingData.Switch[0]);
			CheckDlgButton(hWnd, IDC_MAPPING_SWITCH_2, (WPARAM)mappingData.Switch[1]);
			CheckDlgButton(hWnd, IDC_MAPPING_SWITCH_3, (WPARAM)mappingData.Switch[2]);
			CheckDlgButton(hWnd, IDC_MAPPING_SWITCH_4, (WPARAM)mappingData.Switch[3]);
			CheckDlgButton(hWnd, IDC_MAPPING_SWITCH_5, (WPARAM)mappingData.Switch[4]);
			CheckDlgButton(hWnd, IDC_MAPPING_SWITCH_6, (WPARAM)mappingData.Switch[5]);
			CheckDlgButton(hWnd, IDC_MAPPING_SWITCH_7, (WPARAM)mappingData.Switch[6]);
			CheckDlgButton(hWnd, IDC_MAPPING_SWITCH_8, (WPARAM)mappingData.Switch[7]);
			CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_01, ((WPARAM)mappingData.Mouse[0] == 1) ? (((WPARAM)mappingData.Mouse[3]) ? 2 : 1) : 0);
			CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_02, ((WPARAM)mappingData.Mouse[0] == 2) ? (((WPARAM)mappingData.Mouse[3]) ? 2 : 1) : 0);
			CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_03, ((WPARAM)mappingData.Mouse[0] == 3) ? (((WPARAM)mappingData.Mouse[3]) ? 2 : 1) : 0);
			CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_04, ((WPARAM)mappingData.Mouse[0] == 4) ? (((WPARAM)mappingData.Mouse[3]) ? 2 : 1) : 0);
			CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_05, ((WPARAM)mappingData.Mouse[0] == 5) ? (((WPARAM)mappingData.Mouse[3]) ? 2 : 1) : 0);
			CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_11, ((WPARAM)mappingData.Mouse[1] == 1) ? (((WPARAM)mappingData.Mouse[4]) ? 2 : 1) : 0);
			CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_12, ((WPARAM)mappingData.Mouse[1] == 2) ? (((WPARAM)mappingData.Mouse[4]) ? 2 : 1) : 0);
			CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_13, ((WPARAM)mappingData.Mouse[1] == 3) ? (((WPARAM)mappingData.Mouse[4]) ? 2 : 1) : 0);
			CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_14, ((WPARAM)mappingData.Mouse[1] == 4) ? (((WPARAM)mappingData.Mouse[4]) ? 2 : 1) : 0);
			CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_15, ((WPARAM)mappingData.Mouse[1] == 5) ? (((WPARAM)mappingData.Mouse[4]) ? 2 : 1) : 0);
			CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_21, ((WPARAM)mappingData.Mouse[2] == 1) ? (((WPARAM)mappingData.Mouse[5]) ? 2 : 1) : 0);
			CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_22, ((WPARAM)mappingData.Mouse[2] == 2) ? (((WPARAM)mappingData.Mouse[5]) ? 2 : 1) : 0);
			CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_23, ((WPARAM)mappingData.Mouse[2] == 3) ? (((WPARAM)mappingData.Mouse[5]) ? 2 : 1) : 0);
			CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_24, ((WPARAM)mappingData.Mouse[2] == 4) ? (((WPARAM)mappingData.Mouse[5]) ? 2 : 1) : 0);
			CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_25, ((WPARAM)mappingData.Mouse[2] == 5) ? (((WPARAM)mappingData.Mouse[5]) ? 2 : 1) : 0);
			CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_GRID_DEFAULT, (WPARAM)mappingData.Mouse[6]);

			canprint = false;
			TCHAR buf[20];
			swprintf_s(buf, TEXT("%d"), mappingData.Start[0]);
			SetWindowText(GetDlgItem(hWnd, IDC_MAPPING_VJOY_1_START), buf);
			swprintf_s(buf, TEXT("%d"), mappingData.Start[1]);
			SetWindowText(GetDlgItem(hWnd, IDC_MAPPING_VJOY_2_START), buf);
			swprintf_s(buf, TEXT("%d"), mappingData.Start[2]);
			SetWindowText(GetDlgItem(hWnd, IDC_MAPPING_VJOY_3_START), buf);
			swprintf_s(buf, TEXT("%d"), mappingData.Start[3]);
			SetWindowText(GetDlgItem(hWnd, IDC_MAPPING_VJOY_4_START), buf);
			swprintf_s(buf, TEXT("%d"), mappingData.Start[4]);
			SetWindowText(GetDlgItem(hWnd, IDC_MAPPING_VJOY_5_START), buf);
			swprintf_s(buf, TEXT("%d"), mappingData.Start[5]);
			SetWindowText(GetDlgItem(hWnd, IDC_MAPPING_VJOY_6_START), buf);
			swprintf_s(buf, TEXT("%d"), mappingData.Start[6]);
			SetWindowText(GetDlgItem(hWnd, IDC_MAPPING_VJOY_7_START), buf);
			swprintf_s(buf, TEXT("%d"), mappingData.Start[7]);
			SetWindowText(GetDlgItem(hWnd, IDC_MAPPING_VJOY_8_START), buf);
			swprintf_s(buf, TEXT("%d"), mappingData.Stop[0]);
			SetWindowText(GetDlgItem(hWnd, IDC_MAPPING_VJOY_1_STOP), buf);
			swprintf_s(buf, TEXT("%d"), mappingData.Stop[1]);
			SetWindowText(GetDlgItem(hWnd, IDC_MAPPING_VJOY_2_STOP), buf);
			swprintf_s(buf, TEXT("%d"), mappingData.Stop[2]);
			SetWindowText(GetDlgItem(hWnd, IDC_MAPPING_VJOY_3_STOP), buf);
			swprintf_s(buf, TEXT("%d"), mappingData.Stop[3]);
			SetWindowText(GetDlgItem(hWnd, IDC_MAPPING_VJOY_4_STOP), buf);
			swprintf_s(buf, TEXT("%d"), mappingData.Stop[4]);
			SetWindowText(GetDlgItem(hWnd, IDC_MAPPING_VJOY_5_STOP), buf);
			swprintf_s(buf, TEXT("%d"), mappingData.Stop[5]);
			SetWindowText(GetDlgItem(hWnd, IDC_MAPPING_VJOY_6_STOP), buf);
			swprintf_s(buf, TEXT("%d"), mappingData.Stop[6]);
			SetWindowText(GetDlgItem(hWnd, IDC_MAPPING_VJOY_7_STOP), buf);
			swprintf_s(buf, TEXT("%d"), mappingData.Stop[7]);
			SetWindowText(GetDlgItem(hWnd, IDC_MAPPING_VJOY_8_STOP), buf);
			swprintf_s(buf, TEXT("%d"), mappingData.Grid[0]);
			SetWindowText(GetDlgItem(hWnd, IDC_MAPPING_GRID_X), buf);
			swprintf_s(buf, TEXT("%d"), mappingData.Grid[1]);
			SetWindowText(GetDlgItem(hWnd, IDC_MAPPING_GRID_Y), buf);
			swprintf_s(buf, TEXT("%d"), mappingData.Grid[2]);
			SetWindowText(GetDlgItem(hWnd, IDC_MAPPING_GRID_W), buf);
			swprintf_s(buf, TEXT("%d"), mappingData.Grid[3]);
			SetWindowText(GetDlgItem(hWnd, IDC_MAPPING_GRID_H), buf);
			swprintf_s(buf, TEXT("%d"), mappingData.Grid[4]);
			SetWindowText(GetDlgItem(hWnd, IDC_MAPPING_GRID_NW), buf);
			swprintf_s(buf, TEXT("%d"), mappingData.Grid[5]);
			SetWindowText(GetDlgItem(hWnd, IDC_MAPPING_GRID_NH), buf);
			canprint = true;

			if (
				((WPARAM)mappingData.Mouse[0] == 5 && !(WPARAM)mappingData.Mouse[3]) ||
				((WPARAM)mappingData.Mouse[1] == 5 && !(WPARAM)mappingData.Mouse[4]) ||
				((WPARAM)mappingData.Mouse[2] == 5 && !(WPARAM)mappingData.Mouse[5])
				)
			{
				EnableWindow(GetDlgItem(hWnd, IDC_MAPPING_GRID_X), true);
				EnableWindow(GetDlgItem(hWnd, IDC_MAPPING_GRID_Y), true);
				EnableWindow(GetDlgItem(hWnd, IDC_MAPPING_GRID_W), true);
				EnableWindow(GetDlgItem(hWnd, IDC_MAPPING_GRID_H), true);
				EnableWindow(GetDlgItem(hWnd, IDC_MAPPING_GRID_NW), true);
				EnableWindow(GetDlgItem(hWnd, IDC_MAPPING_GRID_NH), true);
			}
			else
			{
				EnableWindow(GetDlgItem(hWnd, IDC_MAPPING_GRID_X), false);
				EnableWindow(GetDlgItem(hWnd, IDC_MAPPING_GRID_Y), false);
				EnableWindow(GetDlgItem(hWnd, IDC_MAPPING_GRID_W), false);
				EnableWindow(GetDlgItem(hWnd, IDC_MAPPING_GRID_H), false);
				EnableWindow(GetDlgItem(hWnd, IDC_MAPPING_GRID_NW), false);
				EnableWindow(GetDlgItem(hWnd, IDC_MAPPING_GRID_NH), false);

				for (int i = 0; i < 8; i++)
				{
					if (mappingData.MouseAction[i] == 1)
					{
						if ((WPARAM)mappingData.vjID[i] == mappingData.MOVE_TO_XY || (WPARAM)mappingData.vjID[i] == mappingData.SAVE_AND_MOVE_TO_XY)
						{
							EnableWindow(GetDlgItem(hWnd, IDC_MAPPING_GRID_X), true);
							EnableWindow(GetDlgItem(hWnd, IDC_MAPPING_GRID_Y), true);
						}
						if ((WPARAM)mappingData.vjID[i] == mappingData.MOVE_TO_WH || (WPARAM)mappingData.vjID[i] == mappingData.SAVE_AND_MOVE_TO_WH)
						{
							EnableWindow(GetDlgItem(hWnd, IDC_MAPPING_GRID_W), true);
							EnableWindow(GetDlgItem(hWnd, IDC_MAPPING_GRID_H), true);
						}
						if ((WPARAM)mappingData.vjID[i] == mappingData.MOVE_TO_NN || (WPARAM)mappingData.vjID[i] == mappingData.SAVE_AND_MOVE_TO_NN)
						{
							EnableWindow(GetDlgItem(hWnd, IDC_MAPPING_GRID_NW), true);
							EnableWindow(GetDlgItem(hWnd, IDC_MAPPING_GRID_NH), true);
						}
					}
				}
			}
		}
		break;
	case WM_INITDIALOG:
	{
		for (int i = 0; i < Mapping::Led_Action_Count; i++)
		{
			WCHAR* str = Mapping::LedString((LedActionID)i);
			SendDlgItemMessage(hWnd, IDC_MAPPING_LED, CB_ADDSTRING, 0, (LPARAM)str);
		}
		break;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			if (mappingData.dsID[0] == 0 && m_mode != 3)
			{
				RECT rect;
				GetWindowRect(m_hWnd, &rect);
				MessageBoxPos(hWnd, I18N.MBOX_NoButtonSelected, I18N.MBOX_ErrTitle, MB_ICONERROR, rect.left + 275, rect.top + 30);
				return TRUE;
			}
			mappingData.Enable = 1;
			if (m_mode == 1)
			{
				if (m_isClonedList)
					SendMessage(m_hWnd, WM_ADDMAPPING, 1, 2);
				else
					SendMessage(m_hWnd, WM_ADDMAPPING, 1, 1);
				Mapping m;
				mDDlg.mappingData = m;
				Hide();
				firsttime = false;
				Show();
			}
			else
			{
				KillTimer(m_hDlg, 1);
				if (m_isClonedList)
					PostMessage(m_hWnd, WM_ADDMAPPING, m_mode, 2);
				else
					PostMessage(m_hWnd, WM_ADDMAPPING, m_mode, 1);
				m_mode = 0;
			}
			break;
		}
		case IDCANCEL:
			KillTimer(m_hDlg, 1);
			PostMessage(m_hWnd, WM_ADDMAPPING, 0, 0);
			m_mode = 0;
			break;
		case IDHELP:
			ShowWindow(GetDlgItem(hWnd, IDC_STATIC_HELP), SW_SHOW);
			ShowWindow(GetDlgItem(hWnd, IDC_STATIC_PICTURE), SW_HIDE);
			break;
		case IDC_CLEAR:
		{
			Mapping m;
			mDDlg.mappingData = m;
			Hide();
			Show();
			break;
		}
		case IDC_MAPPING_DS_1:
			if (HIWORD(wParam) == CBN_SELCHANGE)
			{
				mappingData.dsID[0] = (byte)SendMessage((HWND)lParam, CB_GETCURSEL, 0, 0);
				Modified[Mofified_dsID1] = true;
			}
			break;
		case IDC_MAPPING_DS_2:
			if (HIWORD(wParam) == CBN_SELCHANGE)
			{
				mappingData.dsID[1] = (byte)SendMessage((HWND)lParam, CB_GETCURSEL, 0, 0);
				Modified[Mofified_dsID2] = true;
			}
			break;
		case IDC_MAPPING_DS_3:
			if (HIWORD(wParam) == CBN_SELCHANGE)
			{
				mappingData.dsID[2] = (byte)SendMessage((HWND)lParam, CB_GETCURSEL, 0, 0);
				Modified[Mofified_dsID3] = true;
			}
			break;
		case IDC_MAPPING_DS_4:
			if (HIWORD(wParam) == CBN_SELCHANGE)
			{
				mappingData.dsID[3] = (byte)SendMessage((HWND)lParam, CB_GETCURSEL, 0, 0);
				Modified[Mofified_dsID4] = true;
			}
			break;
		case IDC_MAPPING_DS_5:
			if (HIWORD(wParam) == CBN_SELCHANGE)
			{
				mappingData.dsID[4] = (byte)SendMessage((HWND)lParam, CB_GETCURSEL, 0, 0);
				Modified[Mofified_dsID5] = true;
			}
			break;
		case IDC_MAPPING_VJOY_1:
			if (HIWORD(wParam) == CBN_SELCHANGE)
			{
				mappingData.vjID[0] = (byte)SendMessage((HWND)lParam, CB_GETCURSEL, 0, 0);
				Modified[Mofified_vjID1] = true;
				Hide();
				Show();
			}
			break;
		case IDC_MAPPING_VJOY_2:
			if (HIWORD(wParam) == CBN_SELCHANGE)
			{
				mappingData.vjID[1] = (byte)SendMessage((HWND)lParam, CB_GETCURSEL, 0, 0);
				Modified[Mofified_vjID2] = true;
				Hide();
				Show();
			}
			break;
		case IDC_MAPPING_VJOY_3:
			if (HIWORD(wParam) == CBN_SELCHANGE)
			{
				mappingData.vjID[2] = (byte)SendMessage((HWND)lParam, CB_GETCURSEL, 0, 0);
				Modified[Mofified_vjID3] = true;
				Hide();
				Show();
			}
			break;
		case IDC_MAPPING_VJOY_4:
			if (HIWORD(wParam) == CBN_SELCHANGE)
			{
				mappingData.vjID[3] = (byte)SendMessage((HWND)lParam, CB_GETCURSEL, 0, 0);
				Modified[Mofified_vjID4] = true;
				Hide();
				Show();
			}
			break;
		case IDC_MAPPING_VJOY_5:
			if (HIWORD(wParam) == CBN_SELCHANGE)
			{
				mappingData.vjID[4] = (byte)SendMessage((HWND)lParam, CB_GETCURSEL, 0, 0);
				Modified[Mofified_vjID5] = true;
				Hide();
				Show();
			}
			break;
		case IDC_MAPPING_VJOY_6:
			if (HIWORD(wParam) == CBN_SELCHANGE)
			{
				mappingData.vjID[5] = (byte)SendMessage((HWND)lParam, CB_GETCURSEL, 0, 0);
				Modified[Mofified_vjID6] = true;
				Hide();
				Show();
			}
			break;
		case IDC_MAPPING_VJOY_7:
			if (HIWORD(wParam) == CBN_SELCHANGE)
			{
				mappingData.vjID[6] = (byte)SendMessage((HWND)lParam, CB_GETCURSEL, 0, 0);
				Modified[Mofified_vjID7] = true;
				Hide();
				Show();
			}
			break;
		case IDC_MAPPING_VJOY_8:
			if (HIWORD(wParam) == CBN_SELCHANGE)
			{
				mappingData.vjID[7] = (byte)SendMessage((HWND)lParam, CB_GETCURSEL, 0, 0);
				Modified[Mofified_vjID8] = true;
				Hide();
				Show();
			}
			break;
		case IDC_MAPPING_TARGET_1:
			mappingData.Target[0] = IsDlgButtonChecked(hWnd, LOWORD(wParam));
			SendDlgItemMessage(hWnd, IDC_MAPPING_DS_1, CB_RESETCONTENT, 0, 0);
			mappingData.dsID[0] = dsButtonID::none;
			if (IsDlgButtonChecked(hWnd, LOWORD(wParam)))
			{
				for (int i = 0; i < vJoyButtonID::button_Count; i++)
				{
					WCHAR* str = vJoyButton::String((vJoyButtonID)i);
					SendDlgItemMessage(hWnd, IDC_MAPPING_DS_1, CB_ADDSTRING, 0, (LPARAM)str);
				}
			}
			else
			{
				for (int i = 0; i < dsButtonID::button_Count; i++)
				{
					WCHAR* str = dsButton::String((dsButtonID)i);
					SendDlgItemMessage(hWnd, IDC_MAPPING_DS_1, CB_ADDSTRING, 0, (LPARAM)str);
				}
			}
			Modified[Mofified_Target1] = true;
			Modified[Mofified_dsID1] = true;
			Hide();
			Show();
			break;
		case IDC_MAPPING_TARGET_2:
			mappingData.Target[1] = IsDlgButtonChecked(hWnd, LOWORD(wParam));
			SendDlgItemMessage(hWnd, IDC_MAPPING_DS_2, CB_RESETCONTENT, 0, 0);
			mappingData.dsID[1] = dsButtonID::none;
			if (IsDlgButtonChecked(hWnd, LOWORD(wParam)))
			{
				for (int i = 0; i < vJoyButtonID::button_Count; i++)
				{
					WCHAR* str = vJoyButton::String((vJoyButtonID)i);
					SendDlgItemMessage(hWnd, IDC_MAPPING_DS_2, CB_ADDSTRING, 0, (LPARAM)str);
				}
			}
			else
			{
				for (int i = 0; i < dsButtonID::button_Count; i++)
				{
					WCHAR* str = dsButton::String((dsButtonID)i);
					SendDlgItemMessage(hWnd, IDC_MAPPING_DS_2, CB_ADDSTRING, 0, (LPARAM)str);
				}
			}
			Modified[Mofified_Target2] = true;
			Modified[Mofified_dsID2] = true;
			Hide();
			Show();
			break;
		case IDC_MAPPING_TARGET_3:
			mappingData.Target[2] = IsDlgButtonChecked(hWnd, LOWORD(wParam));
			SendDlgItemMessage(hWnd, IDC_MAPPING_DS_3, CB_RESETCONTENT, 0, 0);
			mappingData.dsID[2] = dsButtonID::none;
			if (IsDlgButtonChecked(hWnd, LOWORD(wParam)))
			{
				for (int i = 0; i < vJoyButtonID::button_Count; i++)
				{
					WCHAR* str = vJoyButton::String((vJoyButtonID)i);
					SendDlgItemMessage(hWnd, IDC_MAPPING_DS_3, CB_ADDSTRING, 0, (LPARAM)str);
				}
			}
			else
			{
				for (int i = 0; i < dsButtonID::button_Count; i++)
				{
					WCHAR* str = dsButton::String((dsButtonID)i);
					SendDlgItemMessage(hWnd, IDC_MAPPING_DS_3, CB_ADDSTRING, 0, (LPARAM)str);
				}
			}
			Modified[Mofified_Target3] = true;
			Modified[Mofified_dsID3] = true;
			Hide();
			Show();
			break;
		case IDC_MAPPING_TARGET_4:
			mappingData.Target[3] = IsDlgButtonChecked(hWnd, LOWORD(wParam));
			SendDlgItemMessage(hWnd, IDC_MAPPING_DS_4, CB_RESETCONTENT, 0, 0);
			mappingData.dsID[3] = dsButtonID::none;
			if (IsDlgButtonChecked(hWnd, LOWORD(wParam)))
			{
				for (int i = 0; i < vJoyButtonID::button_Count; i++)
				{
					WCHAR* str = vJoyButton::String((vJoyButtonID)i);
					SendDlgItemMessage(hWnd, IDC_MAPPING_DS_4, CB_ADDSTRING, 0, (LPARAM)str);
				}
			}
			else
			{
				for (int i = 0; i < dsButtonID::button_Count; i++)
				{
					WCHAR* str = dsButton::String((dsButtonID)i);
					SendDlgItemMessage(hWnd, IDC_MAPPING_DS_4, CB_ADDSTRING, 0, (LPARAM)str);
				}
			}
			Modified[Mofified_Target4] = true;
			Modified[Mofified_dsID4] = true;
			Hide();
			Show();
			break;
		case IDC_MAPPING_TARGET_5:
			mappingData.Target[4] = IsDlgButtonChecked(hWnd, LOWORD(wParam));
			SendDlgItemMessage(hWnd, IDC_MAPPING_DS_5, CB_RESETCONTENT, 0, 0);
			mappingData.dsID[4] = dsButtonID::none;
			if (IsDlgButtonChecked(hWnd, LOWORD(wParam)))
			{
				for (int i = 0; i < vJoyButtonID::button_Count; i++)
				{
					WCHAR* str = vJoyButton::String((vJoyButtonID)i);
					SendDlgItemMessage(hWnd, IDC_MAPPING_DS_5, CB_ADDSTRING, 0, (LPARAM)str);
				}
			}
			else
			{
				for (int i = 0; i < dsButtonID::button_Count; i++)
				{
					WCHAR* str = dsButton::String((dsButtonID)i);
					SendDlgItemMessage(hWnd, IDC_MAPPING_DS_5, CB_ADDSTRING, 0, (LPARAM)str);
				}
			}
			Modified[Mofified_Target5] = true;
			Modified[Mofified_dsID5] = true;
			Hide();
			Show();
			break;
		case IDC_MAPPING_DS_1_DISABLE:mappingData.dsDisable[0] = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_dsDisable1] = true; break;
		case IDC_MAPPING_DS_2_DISABLE:mappingData.dsDisable[1] = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_dsDisable2] = true; break;
		case IDC_MAPPING_DS_3_DISABLE:mappingData.dsDisable[2] = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_dsDisable3] = true; break;
		case IDC_MAPPING_DS_4_DISABLE:mappingData.dsDisable[3] = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_dsDisable4] = true; break;
		case IDC_MAPPING_DS_5_DISABLE:mappingData.dsDisable[4] = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_dsDisable5] = true; break;
		case IDC_MAPPING_VJOY_1_DISABLE:
			if (IsDlgButtonChecked(hWnd, IDC_MAPPING_MOUSE_ACTION_1))
			{
				mappingData.vjDisable[0] = false;
				CheckDlgButton(hWnd, LOWORD(wParam), BST_UNCHECKED);
			}
			else
				mappingData.vjDisable[0] = IsDlgButtonChecked(hWnd, LOWORD(wParam));
			Modified[Mofified_vjDisable1] = true;
			break;
		case IDC_MAPPING_VJOY_2_DISABLE:
			if (IsDlgButtonChecked(hWnd, IDC_MAPPING_MOUSE_ACTION_2))
			{
				mappingData.vjDisable[1] = false;
				CheckDlgButton(hWnd, LOWORD(wParam), BST_UNCHECKED);
			}
			else
				mappingData.vjDisable[1] = IsDlgButtonChecked(hWnd, LOWORD(wParam));
			Modified[Mofified_vjDisable2] = true;
			break;
		case IDC_MAPPING_VJOY_3_DISABLE:
			if (IsDlgButtonChecked(hWnd, IDC_MAPPING_MOUSE_ACTION_3))
			{
				mappingData.vjDisable[2] = false;
				CheckDlgButton(hWnd, LOWORD(wParam), BST_UNCHECKED);
			}
			else
				mappingData.vjDisable[2] = IsDlgButtonChecked(hWnd, LOWORD(wParam));
			Modified[Mofified_vjDisable3] = true;
			break;
		case IDC_MAPPING_VJOY_4_DISABLE:
			if (IsDlgButtonChecked(hWnd, IDC_MAPPING_MOUSE_ACTION_4))
			{
				mappingData.vjDisable[3] = false;
				CheckDlgButton(hWnd, LOWORD(wParam), BST_UNCHECKED);
			}
			else
				mappingData.vjDisable[3] = IsDlgButtonChecked(hWnd, LOWORD(wParam));
			Modified[Mofified_vjDisable4] = true;
			break;
		case IDC_MAPPING_VJOY_5_DISABLE:
			if (IsDlgButtonChecked(hWnd, IDC_MAPPING_MOUSE_ACTION_5))
			{
				mappingData.vjDisable[4] = false;
				CheckDlgButton(hWnd, LOWORD(wParam), BST_UNCHECKED);
			}
			else
				mappingData.vjDisable[4] = IsDlgButtonChecked(hWnd, LOWORD(wParam));
			Modified[Mofified_vjDisable5] = true;
			break;
		case IDC_MAPPING_VJOY_6_DISABLE:
			if (IsDlgButtonChecked(hWnd, IDC_MAPPING_MOUSE_ACTION_6))
			{
				mappingData.vjDisable[5] = false;
				CheckDlgButton(hWnd, LOWORD(wParam), BST_UNCHECKED);
			}
			else
				mappingData.vjDisable[5] = IsDlgButtonChecked(hWnd, LOWORD(wParam));
			Modified[Mofified_vjDisable6] = true;
			break;
		case IDC_MAPPING_VJOY_7_DISABLE:
			if (IsDlgButtonChecked(hWnd, IDC_MAPPING_MOUSE_ACTION_7))
			{
				mappingData.vjDisable[6] = false;
				CheckDlgButton(hWnd, LOWORD(wParam), BST_UNCHECKED);
			}
			else
				mappingData.vjDisable[6] = IsDlgButtonChecked(hWnd, LOWORD(wParam));
			Modified[Mofified_vjDisable7] = true;
			break;
		case IDC_MAPPING_VJOY_8_DISABLE:
			if (IsDlgButtonChecked(hWnd, IDC_MAPPING_MOUSE_ACTION_8))
			{
				mappingData.vjDisable[7] = false;
				CheckDlgButton(hWnd, LOWORD(wParam), BST_UNCHECKED);
			}
			else
				mappingData.vjDisable[7] = IsDlgButtonChecked(hWnd, LOWORD(wParam));
			Modified[Mofified_vjDisable8] = true;
			break;
		case IDC_MAPPING_DS_2_OR:mappingData.OrXorNot[0] = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_OrXorNot1] = true; break;
		case IDC_MAPPING_DS_3_OR:mappingData.OrXorNot[1] = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_OrXorNot2] = true; break;
		case IDC_MAPPING_DS_4_NOT:mappingData.OrXorNot[2] = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_OrXorNot3] = true; break;
		case IDC_MAPPING_DS_5_NOT:mappingData.OrXorNot[3] = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_OrXorNot4] = true; break;
		case IDC_MAPPING_IF_MOUSE:mappingData.Ifmouse = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_Ifmouse] = true; break;
		case IDC_MAPPING_FORCE:mappingData.Force = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_Force] = true; break;
		case IDC_MAPPING_SHORT:mappingData.Short = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_Short] = true; break;
		case IDC_MAPPING_LONG:mappingData.Long = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_Double] = true; break;
		case IDC_MAPPING_DOUBLE:mappingData.Double = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_Long] = true; break;
		case IDC_MAPPING_LED:
			Modified[Mofified_Led] = true;
			if (HIWORD(wParam) == CBN_SELCHANGE)
				mappingData.Led = (int)SendMessage((HWND)lParam, CB_GETCURSEL, 0, 0);
			break;
		case IDC_MAPPING_MACRO:mappingData.Macro = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_Macro] = true; break;
		case IDC_MAPPING_PAUSE:mappingData.Pause = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_Pause] = true; break;
		case IDC_MAPPING_TRANSITIVITY:mappingData.Transitivity = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_Transitivity] = true; break;
		case IDC_MAPPING_TOGGLE:mappingData.Toggle = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_Toggle] = true; break;
		case IDC_MAPPING_ONRELEASE_1:mappingData.OnRelease[0] = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_OnRelease1] = true; break;
		case IDC_MAPPING_ONRELEASE_2:mappingData.OnRelease[1] = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_OnRelease2] = true; break;
		case IDC_MAPPING_ONRELEASE_3:mappingData.OnRelease[2] = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_OnRelease3] = true; break;
		case IDC_MAPPING_ONRELEASE_4:mappingData.OnRelease[3] = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_OnRelease4] = true; break;
		case IDC_MAPPING_ONRELEASE_5:mappingData.OnRelease[4] = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_OnRelease5] = true; break;
		case IDC_MAPPING_ONRELEASE_6:mappingData.OnRelease[5] = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_OnRelease6] = true; break;
		case IDC_MAPPING_ONRELEASE_7:mappingData.OnRelease[6] = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_OnRelease7] = true; break;
		case IDC_MAPPING_ONRELEASE_8:mappingData.OnRelease[7] = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_OnRelease8] = true; break;
		case IDC_MAPPING_NORELEASE_1:mappingData.NoRelease[0] = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_NoRelease1] = true; break;
		case IDC_MAPPING_NORELEASE_2:mappingData.NoRelease[1] = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_NoRelease2] = true; break;
		case IDC_MAPPING_NORELEASE_3:mappingData.NoRelease[2] = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_NoRelease3] = true; break;
		case IDC_MAPPING_NORELEASE_4:mappingData.NoRelease[3] = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_NoRelease4] = true; break;
		case IDC_MAPPING_NORELEASE_5:mappingData.NoRelease[4] = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_NoRelease5] = true; break;
		case IDC_MAPPING_NORELEASE_6:mappingData.NoRelease[5] = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_NoRelease6] = true; break;
		case IDC_MAPPING_NORELEASE_7:mappingData.NoRelease[6] = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_NoRelease7] = true; break;
		case IDC_MAPPING_NORELEASE_8:mappingData.NoRelease[7] = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_NoRelease8] = true; break;
		case IDC_MAPPING_NLRELEASE_1:mappingData.NlRelease[0] = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_NlRelease1] = true; break;
		case IDC_MAPPING_NLRELEASE_2:mappingData.NlRelease[1] = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_NlRelease2] = true; break;
		case IDC_MAPPING_NLRELEASE_3:mappingData.NlRelease[2] = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_NlRelease3] = true; break;
		case IDC_MAPPING_NLRELEASE_4:mappingData.NlRelease[3] = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_NlRelease4] = true; break;
		case IDC_MAPPING_NLRELEASE_5:mappingData.NlRelease[4] = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_NlRelease5] = true; break;
		case IDC_MAPPING_NLRELEASE_6:mappingData.NlRelease[5] = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_NlRelease6] = true; break;
		case IDC_MAPPING_NLRELEASE_7:mappingData.NlRelease[6] = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_NlRelease7] = true; break;
		case IDC_MAPPING_NLRELEASE_8:mappingData.NlRelease[7] = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_NlRelease8] = true; break;
		case IDC_MAPPING_VJOY_1_START:
			WCHAR bufstart1[MAX_PATH];
			GetWindowText((HWND)lParam, bufstart1, MAX_PATH);
			mappingData.Start[0] = max(0, min(9999, _wtoi(bufstart1)));
			if (canprint) Modified[Mofified_Start1] = true;
			break;
		case IDC_MAPPING_VJOY_2_START:
			WCHAR bufstart2[MAX_PATH];
			GetWindowText((HWND)lParam, bufstart2, MAX_PATH);
			mappingData.Start[1] = max(0, min(9999, _wtoi(bufstart2)));
			if (canprint) Modified[Mofified_Start2] = true;
			break;
		case IDC_MAPPING_VJOY_3_START:
			WCHAR bufstart3[MAX_PATH];
			GetWindowText((HWND)lParam, bufstart3, MAX_PATH);
			mappingData.Start[2] = max(0, min(9999, _wtoi(bufstart3)));
			if (canprint) Modified[Mofified_Start3] = true;
			break;
		case IDC_MAPPING_VJOY_4_START:
			WCHAR bufstart4[MAX_PATH];
			GetWindowText((HWND)lParam, bufstart4, MAX_PATH);
			mappingData.Start[3] = max(0, min(9999, _wtoi(bufstart4)));
			if (canprint) Modified[Mofified_Start4] = true;
			break;
		case IDC_MAPPING_VJOY_5_START:
			WCHAR bufstart5[MAX_PATH];
			GetWindowText((HWND)lParam, bufstart5, MAX_PATH);
			mappingData.Start[4] = max(0, min(9999, _wtoi(bufstart5)));
			if (canprint) Modified[Mofified_Start5] = true;
			break;
		case IDC_MAPPING_VJOY_6_START:
			WCHAR bufstart6[MAX_PATH];
			GetWindowText((HWND)lParam, bufstart6, MAX_PATH);
			mappingData.Start[5] = max(0, min(9999, _wtoi(bufstart6)));
			if (canprint) Modified[Mofified_Start6] = true;
			break;
		case IDC_MAPPING_VJOY_7_START:
			WCHAR bufstart7[MAX_PATH];
			GetWindowText((HWND)lParam, bufstart7, MAX_PATH);
			mappingData.Start[6] = max(0, min(9999, _wtoi(bufstart7)));
			if (canprint) Modified[Mofified_Start7] = true;
			break;
		case IDC_MAPPING_VJOY_8_START:
			WCHAR bufstart8[MAX_PATH];
			GetWindowText((HWND)lParam, bufstart8, MAX_PATH);
			mappingData.Start[7] = max(0, min(9999, _wtoi(bufstart8)));
			if (canprint) Modified[Mofified_Start8] = true;
			break;
		case IDC_MAPPING_VJOY_1_STOP:
			WCHAR bufstop1[MAX_PATH];
			GetWindowText((HWND)lParam, bufstop1, MAX_PATH);
			mappingData.Stop[0] = max(0, min(9999, _wtoi(bufstop1)));
			if (canprint) Modified[Mofified_Stop1] = true;
			break;
		case IDC_MAPPING_VJOY_2_STOP:
			WCHAR bufstop2[MAX_PATH];
			GetWindowText((HWND)lParam, bufstop2, MAX_PATH);
			mappingData.Stop[1] = max(0, min(9999, _wtoi(bufstop2)));
			if (canprint) Modified[Mofified_Stop2] = true;
			break;
		case IDC_MAPPING_VJOY_3_STOP:
			WCHAR bufstop3[MAX_PATH];
			GetWindowText((HWND)lParam, bufstop3, MAX_PATH);
			mappingData.Stop[2] = max(0, min(9999, _wtoi(bufstop3)));
			if (canprint) Modified[Mofified_Stop3] = true;
			break;
		case IDC_MAPPING_VJOY_4_STOP:
			WCHAR bufstop4[MAX_PATH];
			GetWindowText((HWND)lParam, bufstop4, MAX_PATH);
			mappingData.Stop[3] = max(0, min(9999, _wtoi(bufstop4)));
			if (canprint) Modified[Mofified_Stop4] = true;
			break;
		case IDC_MAPPING_VJOY_5_STOP:
			WCHAR bufstop5[MAX_PATH];
			GetWindowText((HWND)lParam, bufstop5, MAX_PATH);
			mappingData.Stop[4] = max(0, min(9999, _wtoi(bufstop5)));
			if (canprint) Modified[Mofified_Stop5] = true;
			break;
		case IDC_MAPPING_VJOY_6_STOP:
			WCHAR bufstop6[MAX_PATH];
			GetWindowText((HWND)lParam, bufstop6, MAX_PATH);
			mappingData.Stop[5] = max(0, min(9999, _wtoi(bufstop6)));
			if (canprint) Modified[Mofified_Stop6] = true;
			break;
		case IDC_MAPPING_VJOY_7_STOP:
			WCHAR bufstop7[MAX_PATH];
			GetWindowText((HWND)lParam, bufstop7, MAX_PATH);
			mappingData.Stop[6] = max(0, min(9999, _wtoi(bufstop7)));
			if (canprint) Modified[Mofified_Stop7] = true;
			break;
		case IDC_MAPPING_VJOY_8_STOP:
			WCHAR bufstop8[MAX_PATH];
			GetWindowText((HWND)lParam, bufstop8, MAX_PATH);
			mappingData.Stop[7] = max(0, min(9999, _wtoi(bufstop8)));
			if (canprint) Modified[Mofified_Stop8] = true;
			break;
		case IDC_MAPPING_MOUSE_ACTION_1:
			mappingData.MouseAction[0] = IsDlgButtonChecked(hWnd, LOWORD(wParam));
			SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_1, CB_RESETCONTENT, 0, 0);
			if (IsDlgButtonChecked(hWnd, LOWORD(wParam)) == 1)
			{
				mappingData.vjID[0] = Mapping::mouse_none;
				mappingData.vjDisable[0] = false;
				Modified[Mofified_vjDisable1] = true;
				for (int i = 0; i < Mapping::mouse_Count; i++)
				{
					WCHAR* str = Mapping::MouseString((MouseActionID)i);
					SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_1, CB_ADDSTRING, 0, (LPARAM)str);
				}
			}
			else if (IsDlgButtonChecked(hWnd, LOWORD(wParam)) == 2)
			{
				mappingData.vjID[0] = vJoyAxisMoveID::axismove_none;
				mappingData.vjDisable[0] = false;
				Modified[Mofified_vjDisable1] = true;
				for (int i = 0; i < vJoyAxisMoveID::axismove_Count; i++)
				{
					WCHAR* str = vJoyButton::StringAxis((vJoyAxisMoveID)i);
					SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_1, CB_ADDSTRING, 0, (LPARAM)str);
				}
			}
			else
			{
				mappingData.vjID[0] = vJoyButtonID::none;
				for (int i = 0; i < vJoyButtonID::button_Count; i++)
				{
					WCHAR* str = vJoyButton::String((vJoyButtonID)i);
					SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_1, CB_ADDSTRING, 0, (LPARAM)str);
				}
			}
			Modified[Mofified_MouseAction1] = true;
			Modified[Mofified_vjID1] = true;
			Hide();
			Show();
			break;
		case IDC_MAPPING_MOUSE_ACTION_2:
			mappingData.MouseAction[1] = IsDlgButtonChecked(hWnd, LOWORD(wParam));
			SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_2, CB_RESETCONTENT, 0, 0);
			if (IsDlgButtonChecked(hWnd, LOWORD(wParam)) == 1)
			{
				mappingData.vjID[1] = Mapping::mouse_none;
				mappingData.vjDisable[1] = false;
				Modified[Mofified_vjDisable2] = true;
				for (int i = 0; i < Mapping::mouse_Count; i++)
				{
					WCHAR* str = Mapping::MouseString((MouseActionID)i);
					SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_2, CB_ADDSTRING, 0, (LPARAM)str);
				}
			}
			else if (IsDlgButtonChecked(hWnd, LOWORD(wParam)) == 2)
			{
				mappingData.vjID[1] = vJoyAxisMoveID::axismove_none;
				mappingData.vjDisable[1] = false;
				Modified[Mofified_vjDisable2] = true;
				for (int i = 0; i < vJoyAxisMoveID::axismove_Count; i++)
				{
					WCHAR* str = vJoyButton::StringAxis((vJoyAxisMoveID)i);
					SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_2, CB_ADDSTRING, 0, (LPARAM)str);
				}
			}
			else
			{
				mappingData.vjID[1] = vJoyButtonID::none;
				for (int i = 0; i < vJoyButtonID::button_Count; i++)
				{
					WCHAR* str = vJoyButton::String((vJoyButtonID)i);
					SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_2, CB_ADDSTRING, 0, (LPARAM)str);
				}
			}
			Modified[Mofified_MouseAction2] = true;
			Modified[Mofified_vjID2] = true;
			Hide();
			Show();
			break;
		case IDC_MAPPING_MOUSE_ACTION_3:
			mappingData.MouseAction[2] = IsDlgButtonChecked(hWnd, LOWORD(wParam));
			SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_3, CB_RESETCONTENT, 0, 0);
			if (IsDlgButtonChecked(hWnd, LOWORD(wParam)) == 1)
			{
				mappingData.vjID[2] = Mapping::mouse_none;
				mappingData.vjDisable[2] = false;
				Modified[Mofified_vjDisable3] = true;
				for (int i = 0; i < Mapping::mouse_Count; i++)
				{
					WCHAR* str = Mapping::MouseString((MouseActionID)i);
					SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_3, CB_ADDSTRING, 0, (LPARAM)str);
				}
			}
			else if (IsDlgButtonChecked(hWnd, LOWORD(wParam)) == 2)
			{
				mappingData.vjID[2] = vJoyAxisMoveID::axismove_none;
				mappingData.vjDisable[2] = false;
				Modified[Mofified_vjDisable3] = true;
				for (int i = 0; i < vJoyAxisMoveID::axismove_Count; i++)
				{
					WCHAR* str = vJoyButton::StringAxis((vJoyAxisMoveID)i);
					SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_3, CB_ADDSTRING, 0, (LPARAM)str);
				}
			}
			else
			{
				mappingData.vjID[2] = vJoyButtonID::none;
				for (int i = 0; i < vJoyButtonID::button_Count; i++)
				{
					WCHAR* str = vJoyButton::String((vJoyButtonID)i);
					SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_3, CB_ADDSTRING, 0, (LPARAM)str);
				}
			}
			Modified[Mofified_MouseAction3] = true;
			Modified[Mofified_vjID3] = true;
			Hide();
			Show();
			break;
		case IDC_MAPPING_MOUSE_ACTION_4:
			mappingData.MouseAction[3] = IsDlgButtonChecked(hWnd, LOWORD(wParam));
			SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_4, CB_RESETCONTENT, 0, 0);
			if (IsDlgButtonChecked(hWnd, LOWORD(wParam)) == 1)
			{
				mappingData.vjID[3] = Mapping::mouse_none;
				mappingData.vjDisable[3] = false;
				Modified[Mofified_vjDisable4] = true;
				for (int i = 0; i < Mapping::mouse_Count; i++)
				{
					WCHAR* str = Mapping::MouseString((MouseActionID)i);
					SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_4, CB_ADDSTRING, 0, (LPARAM)str);
				}
			}
			else if (IsDlgButtonChecked(hWnd, LOWORD(wParam)) == 2)
			{
				mappingData.vjID[3] = vJoyAxisMoveID::axismove_none;
				mappingData.vjDisable[3] = false;
				Modified[Mofified_vjDisable4] = true;
				for (int i = 0; i < vJoyAxisMoveID::axismove_Count; i++)
				{
					WCHAR* str = vJoyButton::StringAxis((vJoyAxisMoveID)i);
					SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_4, CB_ADDSTRING, 0, (LPARAM)str);
				}
			}
			else
			{
				mappingData.vjID[3] = vJoyButtonID::none;
				for (int i = 0; i < vJoyButtonID::button_Count; i++)
				{
					WCHAR* str = vJoyButton::String((vJoyButtonID)i);
					SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_4, CB_ADDSTRING, 0, (LPARAM)str);
				}
			}
			Modified[Mofified_MouseAction4] = true;
			Modified[Mofified_vjID4] = true;
			Hide();
			Show();
			break;
		case IDC_MAPPING_MOUSE_ACTION_5:
			mappingData.MouseAction[4] = IsDlgButtonChecked(hWnd, LOWORD(wParam));
			SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_5, CB_RESETCONTENT, 0, 0);
			if (IsDlgButtonChecked(hWnd, LOWORD(wParam)) == 1)
			{
				mappingData.vjID[4] = Mapping::mouse_none;
				mappingData.vjDisable[4] = false;
				Modified[Mofified_vjDisable5] = true;
				for (int i = 0; i < Mapping::mouse_Count; i++)
				{
					WCHAR* str = Mapping::MouseString((MouseActionID)i);
					SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_5, CB_ADDSTRING, 0, (LPARAM)str);
				}
			}
			else if (IsDlgButtonChecked(hWnd, LOWORD(wParam)) == 2)
			{
				mappingData.vjID[4] = vJoyAxisMoveID::axismove_none;
				mappingData.vjDisable[4] = false;
				Modified[Mofified_vjDisable5] = true;
				for (int i = 0; i < vJoyAxisMoveID::axismove_Count; i++)
				{
					WCHAR* str = vJoyButton::StringAxis((vJoyAxisMoveID)i);
					SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_5, CB_ADDSTRING, 0, (LPARAM)str);
				}
			}
			else
			{
				mappingData.vjID[4] = vJoyButtonID::none;
				for (int i = 0; i < vJoyButtonID::button_Count; i++)
				{
					WCHAR* str = vJoyButton::String((vJoyButtonID)i);
					SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_5, CB_ADDSTRING, 0, (LPARAM)str);
				}
			}
			Modified[Mofified_MouseAction5] = true;
			Modified[Mofified_vjID5] = true;
			Hide();
			Show();
			break;
		case IDC_MAPPING_MOUSE_ACTION_6:
			mappingData.MouseAction[5] = IsDlgButtonChecked(hWnd, LOWORD(wParam));
			SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_6, CB_RESETCONTENT, 0, 0);
			if (IsDlgButtonChecked(hWnd, LOWORD(wParam)) == 1)
			{
				mappingData.vjID[5] = Mapping::mouse_none;
				mappingData.vjDisable[5] = false;
				Modified[Mofified_vjDisable6] = true;
				for (int i = 0; i < Mapping::mouse_Count; i++)
				{
					WCHAR* str = Mapping::MouseString((MouseActionID)i);
					SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_6, CB_ADDSTRING, 0, (LPARAM)str);
				}
			}
			else if (IsDlgButtonChecked(hWnd, LOWORD(wParam)) == 2)
			{
				mappingData.vjID[5] = vJoyAxisMoveID::axismove_none;
				mappingData.vjDisable[5] = false;
				Modified[Mofified_vjDisable6] = true;
				for (int i = 0; i < vJoyAxisMoveID::axismove_Count; i++)
				{
					WCHAR* str = vJoyButton::StringAxis((vJoyAxisMoveID)i);
					SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_6, CB_ADDSTRING, 0, (LPARAM)str);
				}
			}
			else
			{
				mappingData.vjID[5] = vJoyButtonID::none;
				for (int i = 0; i < vJoyButtonID::button_Count; i++)
				{
					WCHAR* str = vJoyButton::String((vJoyButtonID)i);
					SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_6, CB_ADDSTRING, 0, (LPARAM)str);
				}
			}
			Modified[Mofified_MouseAction6] = true;
			Modified[Mofified_vjID6] = true;
			Hide();
			Show();
			break;
		case IDC_MAPPING_MOUSE_ACTION_7:
			mappingData.MouseAction[6] = IsDlgButtonChecked(hWnd, LOWORD(wParam));
			SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_7, CB_RESETCONTENT, 0, 0);
			if (IsDlgButtonChecked(hWnd, LOWORD(wParam)) == 1)
			{
				mappingData.vjID[6] = Mapping::mouse_none;
				mappingData.vjDisable[6] = false;
				Modified[Mofified_vjDisable7] = true;
				for (int i = 0; i < Mapping::mouse_Count; i++)
				{
					WCHAR* str = Mapping::MouseString((MouseActionID)i);
					SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_7, CB_ADDSTRING, 0, (LPARAM)str);
				}
			}
			else if (IsDlgButtonChecked(hWnd, LOWORD(wParam)) == 2)
			{
				mappingData.vjID[6] = vJoyAxisMoveID::axismove_none;
				mappingData.vjDisable[6] = false;
				Modified[Mofified_vjDisable7] = true;
				for (int i = 0; i < vJoyAxisMoveID::axismove_Count; i++)
				{
					WCHAR* str = vJoyButton::StringAxis((vJoyAxisMoveID)i);
					SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_7, CB_ADDSTRING, 0, (LPARAM)str);
				}
			}
			else
			{
				mappingData.vjID[6] = vJoyButtonID::none;
				for (int i = 0; i < vJoyButtonID::button_Count; i++)
				{
					WCHAR* str = vJoyButton::String((vJoyButtonID)i);
					SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_7, CB_ADDSTRING, 0, (LPARAM)str);
				}
			}
			Modified[Mofified_MouseAction7] = true;
			Modified[Mofified_vjID7] = true;
			Hide();
			Show();
			break;
		case IDC_MAPPING_MOUSE_ACTION_8:
			mappingData.MouseAction[7] = IsDlgButtonChecked(hWnd, LOWORD(wParam));
			SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_8, CB_RESETCONTENT, 0, 0);
			if (IsDlgButtonChecked(hWnd, LOWORD(wParam)) == 1)
			{
				mappingData.vjID[7] = Mapping::mouse_none;
				mappingData.vjDisable[7] = false;
				Modified[Mofified_vjDisable8] = true;
				for (int i = 0; i < Mapping::mouse_Count; i++)
				{
					WCHAR* str = Mapping::MouseString((MouseActionID)i);
					SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_8, CB_ADDSTRING, 0, (LPARAM)str);
				}
			}
			else if (IsDlgButtonChecked(hWnd, LOWORD(wParam)) == 2)
			{
				mappingData.vjID[7] = vJoyAxisMoveID::axismove_none;
				mappingData.vjDisable[7] = false;
				Modified[Mofified_vjDisable8] = true;
				for (int i = 0; i < vJoyAxisMoveID::axismove_Count; i++)
				{
					WCHAR* str = vJoyButton::StringAxis((vJoyAxisMoveID)i);
					SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_8, CB_ADDSTRING, 0, (LPARAM)str);
				}
			}
			else
			{
				mappingData.vjID[7] = vJoyButtonID::none;
				for (int i = 0; i < vJoyButtonID::button_Count; i++)
				{
					WCHAR* str = vJoyButton::String((vJoyButtonID)i);
					SendDlgItemMessage(hWnd, IDC_MAPPING_VJOY_8, CB_ADDSTRING, 0, (LPARAM)str);
				}
			}
			Modified[Mofified_MouseAction8] = true;
			Modified[Mofified_vjID8] = true;
			Hide();
			Show();
			break;
		case IDC_MAPPING_OVERCONTROL_1:mappingData.Overcontrol[0] = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_Overcontrol1] = true; break;
		case IDC_MAPPING_OVERCONTROL_2:mappingData.Overcontrol[1] = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_Overcontrol2] = true; break;
		case IDC_MAPPING_OVERCONTROL_3:mappingData.Overcontrol[2] = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_Overcontrol3] = true; break;
		case IDC_MAPPING_OVERCONTROL_4:mappingData.Overcontrol[3] = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_Overcontrol4] = true; break;
		case IDC_MAPPING_OVERCONTROL_5:mappingData.Overcontrol[4] = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_Overcontrol5] = true; break;
		case IDC_MAPPING_OVERCONTROL_6:mappingData.Overcontrol[5] = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_Overcontrol6] = true; break;
		case IDC_MAPPING_OVERCONTROL_7:mappingData.Overcontrol[6] = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_Overcontrol7] = true; break;
		case IDC_MAPPING_OVERCONTROL_8:mappingData.Overcontrol[7] = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_Overcontrol8] = true; break;
		case IDC_MAPPING_SWITCH_1:mappingData.Switch[0] = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_Switch1] = true; break;
		case IDC_MAPPING_SWITCH_2:mappingData.Switch[1] = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_Switch2] = true; break;
		case IDC_MAPPING_SWITCH_3:mappingData.Switch[2] = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_Switch3] = true; break;
		case IDC_MAPPING_SWITCH_4:mappingData.Switch[3] = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_Switch4] = true; break;
		case IDC_MAPPING_SWITCH_5:mappingData.Switch[4] = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_Switch5] = true; break;
		case IDC_MAPPING_SWITCH_6:mappingData.Switch[5] = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_Switch6] = true; break;
		case IDC_MAPPING_SWITCH_7:mappingData.Switch[6] = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_Switch7] = true; break;
		case IDC_MAPPING_SWITCH_8:mappingData.Switch[7] = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_Switch8] = true; break;
		case IDC_MAPPING_MOUSE_01:
			if (IsDlgButtonChecked(hWnd, LOWORD(wParam)))
			{
				mappingData.Mouse[0] = 1;
				mappingData.Mouse[3] = (IsDlgButtonChecked(hWnd, LOWORD(wParam)) == 1) ? false : true;
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_02, BST_UNCHECKED);
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_03, BST_UNCHECKED);
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_04, BST_UNCHECKED);
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_05, BST_UNCHECKED);
			}
			else
			{
				mappingData.Mouse[0] = 0;
				mappingData.Mouse[3] = false;
			}
			Modified[Mofified_Mouse1] = true;
			Modified[Mofified_Mouse4] = true;
			Hide();
			Show();
			break;
		case IDC_MAPPING_MOUSE_02:
			if (IsDlgButtonChecked(hWnd, LOWORD(wParam)))
			{
				mappingData.Mouse[0] = 2;
				mappingData.Mouse[3] = (IsDlgButtonChecked(hWnd, LOWORD(wParam)) == 1) ? false : true;
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_01, BST_UNCHECKED);
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_03, BST_UNCHECKED);
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_04, BST_UNCHECKED);
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_05, BST_UNCHECKED);
			}
			else
			{
				mappingData.Mouse[0] = 0;
				mappingData.Mouse[3] = false;
			}
			Modified[Mofified_Mouse1] = true;
			Modified[Mofified_Mouse4] = true;
			Hide();
			Show();
			break;
		case IDC_MAPPING_MOUSE_03:
			if (IsDlgButtonChecked(hWnd, LOWORD(wParam)))
			{
				mappingData.Mouse[0] = 3;
				mappingData.Mouse[3] = (IsDlgButtonChecked(hWnd, LOWORD(wParam)) == 1) ? false : true;
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_01, BST_UNCHECKED);
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_02, BST_UNCHECKED);
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_04, BST_UNCHECKED);
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_05, BST_UNCHECKED);
			}
			else
			{
				mappingData.Mouse[0] = 0;
				mappingData.Mouse[3] = false;
			}
			Modified[Mofified_Mouse1] = true;
			Modified[Mofified_Mouse4] = true;
			Hide();
			Show();
			break;
		case IDC_MAPPING_MOUSE_04:
			if (IsDlgButtonChecked(hWnd, LOWORD(wParam)))
			{
				mappingData.Mouse[0] = 4;
				mappingData.Mouse[3] = (IsDlgButtonChecked(hWnd, LOWORD(wParam)) == 1) ? false : true;
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_01, BST_UNCHECKED);
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_02, BST_UNCHECKED);
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_03, BST_UNCHECKED);
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_05, BST_UNCHECKED);
			}
			else
			{
				mappingData.Mouse[0] = 0;
				mappingData.Mouse[3] = false;
			}
			Modified[Mofified_Mouse1] = true;
			Modified[Mofified_Mouse4] = true;
			Hide();
			Show();
			break;
		case IDC_MAPPING_MOUSE_05:
			if (IsDlgButtonChecked(hWnd, LOWORD(wParam)))
			{
				mappingData.Mouse[0] = 5;
				mappingData.Mouse[3] = (IsDlgButtonChecked(hWnd, LOWORD(wParam)) == 1) ? false : true;
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_01, BST_UNCHECKED);
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_02, BST_UNCHECKED);
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_03, BST_UNCHECKED);
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_04, BST_UNCHECKED);
			}
			else
			{
				mappingData.Mouse[0] = 0;
				mappingData.Mouse[3] = false;
			}
			Modified[Mofified_Mouse1] = true;
			Modified[Mofified_Mouse4] = true;
			Hide();
			Show();
			break;
		case IDC_MAPPING_MOUSE_11:
			if (IsDlgButtonChecked(hWnd, LOWORD(wParam)))
			{
				mappingData.Mouse[1] = 1;
				mappingData.Mouse[4] = (IsDlgButtonChecked(hWnd, LOWORD(wParam)) == 1) ? false : true;
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_12, BST_UNCHECKED);
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_13, BST_UNCHECKED);
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_14, BST_UNCHECKED);
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_15, BST_UNCHECKED);
			}
			else
			{
				mappingData.Mouse[1] = 0;
				mappingData.Mouse[4] = false;
			}
			Modified[Mofified_Mouse2] = true;
			Modified[Mofified_Mouse5] = true;
			Hide();
			Show();
			break;
		case IDC_MAPPING_MOUSE_12:
			if (IsDlgButtonChecked(hWnd, LOWORD(wParam)))
			{
				mappingData.Mouse[1] = 2;
				mappingData.Mouse[4] = (IsDlgButtonChecked(hWnd, LOWORD(wParam)) == 1) ? false : true;
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_11, BST_UNCHECKED);
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_13, BST_UNCHECKED);
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_14, BST_UNCHECKED);
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_15, BST_UNCHECKED);
			}
			else
			{
				mappingData.Mouse[1] = 0;
				mappingData.Mouse[4] = false;
			}
			Modified[Mofified_Mouse2] = true;
			Modified[Mofified_Mouse5] = true;
			Hide();
			Show();
			break;
		case IDC_MAPPING_MOUSE_13:
			if (IsDlgButtonChecked(hWnd, LOWORD(wParam)))
			{
				mappingData.Mouse[1] = 3;
				mappingData.Mouse[4] = (IsDlgButtonChecked(hWnd, LOWORD(wParam)) == 1) ? false : true;
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_11, BST_UNCHECKED);
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_12, BST_UNCHECKED);
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_14, BST_UNCHECKED);
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_15, BST_UNCHECKED);
			}
			else
			{
				mappingData.Mouse[1] = 0;
				mappingData.Mouse[4] = false;
			}
			Modified[Mofified_Mouse2] = true;
			Modified[Mofified_Mouse5] = true;
			Hide();
			Show();
			break;
		case IDC_MAPPING_MOUSE_14:
			if (IsDlgButtonChecked(hWnd, LOWORD(wParam)))
			{
				mappingData.Mouse[1] = 4;
				mappingData.Mouse[4] = (IsDlgButtonChecked(hWnd, LOWORD(wParam)) == 1) ? false : true;
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_11, BST_UNCHECKED);
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_12, BST_UNCHECKED);
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_13, BST_UNCHECKED);
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_15, BST_UNCHECKED);
			}
			else
			{
				mappingData.Mouse[1] = 0;
				mappingData.Mouse[4] = false;
			}
			Modified[Mofified_Mouse2] = true;
			Modified[Mofified_Mouse5] = true;
			Hide();
			Show();
			break;
		case IDC_MAPPING_MOUSE_15:
			if (IsDlgButtonChecked(hWnd, LOWORD(wParam)))
			{
				mappingData.Mouse[1] = 5;
				mappingData.Mouse[4] = (IsDlgButtonChecked(hWnd, LOWORD(wParam)) == 1) ? false : true;
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_11, BST_UNCHECKED);
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_12, BST_UNCHECKED);
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_13, BST_UNCHECKED);
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_14, BST_UNCHECKED);
			}
			else
			{
				mappingData.Mouse[1] = 0;
				mappingData.Mouse[4] = false;
			}
			Modified[Mofified_Mouse2] = true;
			Modified[Mofified_Mouse5] = true;
			Hide();
			Show();
			break;
		case IDC_MAPPING_MOUSE_21:
			if (IsDlgButtonChecked(hWnd, LOWORD(wParam)))
			{
				mappingData.Mouse[2] = 1;
				mappingData.Mouse[5] = (IsDlgButtonChecked(hWnd, LOWORD(wParam)) == 1) ? false : true;
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_22, BST_UNCHECKED);
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_23, BST_UNCHECKED);
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_24, BST_UNCHECKED);
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_25, BST_UNCHECKED);
			}
			else
			{
				mappingData.Mouse[2] = 0;
				mappingData.Mouse[5] = false;
			}
			Modified[Mofified_Mouse3] = true;
			Modified[Mofified_Mouse6] = true;
			Hide();
			Show();
			break;
		case IDC_MAPPING_MOUSE_22:
			if (IsDlgButtonChecked(hWnd, LOWORD(wParam)))
			{
				mappingData.Mouse[2] = 2;
				mappingData.Mouse[5] = (IsDlgButtonChecked(hWnd, LOWORD(wParam)) == 1) ? false : true;
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_21, BST_UNCHECKED);
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_23, BST_UNCHECKED);
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_24, BST_UNCHECKED);
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_25, BST_UNCHECKED);
			}
			else
			{
				mappingData.Mouse[2] = 0;
				mappingData.Mouse[5] = false;
			}
			Modified[Mofified_Mouse3] = true;
			Modified[Mofified_Mouse6] = true;
			Hide();
			Show();
			break;
		case IDC_MAPPING_MOUSE_23:
			if (IsDlgButtonChecked(hWnd, LOWORD(wParam)))
			{
				mappingData.Mouse[2] = 3;
				mappingData.Mouse[5] = (IsDlgButtonChecked(hWnd, LOWORD(wParam)) == 1) ? false : true;
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_21, BST_UNCHECKED);
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_22, BST_UNCHECKED);
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_24, BST_UNCHECKED);
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_25, BST_UNCHECKED);
			}
			else
			{
				mappingData.Mouse[2] = 0;
				mappingData.Mouse[5] = false;
			}
			Modified[Mofified_Mouse3] = true;
			Modified[Mofified_Mouse6] = true;
			Hide();
			Show();
			break;
		case IDC_MAPPING_MOUSE_24:
			if (IsDlgButtonChecked(hWnd, LOWORD(wParam)))
			{
				mappingData.Mouse[2] = 4;
				mappingData.Mouse[5] = (IsDlgButtonChecked(hWnd, LOWORD(wParam)) == 1) ? false : true;
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_21, BST_UNCHECKED);
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_22, BST_UNCHECKED);
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_23, BST_UNCHECKED);
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_25, BST_UNCHECKED);
			}
			else
			{
				mappingData.Mouse[2] = 0;
				mappingData.Mouse[5] = false;
			}
			Modified[Mofified_Mouse3] = true;
			Modified[Mofified_Mouse6] = true;
			Hide();
			Show();
			break;
		case IDC_MAPPING_MOUSE_25:
			if (IsDlgButtonChecked(hWnd, LOWORD(wParam)))
			{
				mappingData.Mouse[2] = 5;
				mappingData.Mouse[5] = (IsDlgButtonChecked(hWnd, LOWORD(wParam)) == 1) ? false : true;
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_21, BST_UNCHECKED);
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_22, BST_UNCHECKED);
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_23, BST_UNCHECKED);
				CheckDlgButton(hWnd, IDC_MAPPING_MOUSE_24, BST_UNCHECKED);
			}
			else
			{
				mappingData.Mouse[2] = 0;
				mappingData.Mouse[5] = false;
			}
			Modified[Mofified_Mouse3] = true;
			Modified[Mofified_Mouse6] = true;
			Hide();
			Show();
			break;
		case IDC_MAPPING_GRID_X:
			switch (HIWORD(wParam))
			{
			case EN_SETFOCUS:
				whohasfocus = 1;
				break;
			case EN_KILLFOCUS:
				whohasfocus = 0;
				break;
			}
			WCHAR buf1[MAX_PATH];
			GetWindowText((HWND)lParam, buf1, MAX_PATH);
			mappingData.Grid[0] = max(-32768, min(32767, _wtoi(buf1)));
			if (canprint) Modified[Mofified_Grid1] = true;
			break;
		case IDC_MAPPING_GRID_Y:
			switch (HIWORD(wParam))
			{
			case EN_SETFOCUS:
				whohasfocus = 1;
				break;
			case EN_KILLFOCUS:
				whohasfocus = 0;
				break;
			}
			WCHAR buf2[MAX_PATH];
			GetWindowText((HWND)lParam, buf2, MAX_PATH);
			mappingData.Grid[1] = max(-32768, min(32767, _wtoi(buf2)));
			if (canprint) Modified[Mofified_Grid2] = true;
			break;
		case IDC_MAPPING_GRID_W:
			switch (HIWORD(wParam))
			{
			case EN_SETFOCUS:
				whohasfocus = 2;
				break;
			case EN_KILLFOCUS:
				whohasfocus = 0;
				break;
			}
			WCHAR buf3[MAX_PATH];
			GetWindowText((HWND)lParam, buf3, MAX_PATH);
			mappingData.Grid[2] = max(-32768, min(32767, _wtoi(buf3)));
			if (canprint) Modified[Mofified_Grid3] = true;
			break;
		case IDC_MAPPING_GRID_H:
			switch (HIWORD(wParam))
			{
			case EN_SETFOCUS:
				whohasfocus = 2;
				break;
			case EN_KILLFOCUS:
				whohasfocus = 0;
				break;
			}
			WCHAR buf4[MAX_PATH];
			GetWindowText((HWND)lParam, buf4, MAX_PATH);
			mappingData.Grid[3] = max(-32768, min(32767, _wtoi(buf4)));
			if (canprint) Modified[Mofified_Grid4] = true;
			break;
		case IDC_MAPPING_GRID_NW:
			switch (HIWORD(wParam))
			{
			case EN_SETFOCUS:
				whohasfocus = 3;
				break;
			case EN_KILLFOCUS:
				whohasfocus = 0;
				break;
			}
			WCHAR buf5[MAX_PATH];
			GetWindowText((HWND)lParam, buf5, MAX_PATH);
			mappingData.Grid[4] = max(-32768, min(32767, _wtoi(buf5)));
			if (canprint) Modified[Mofified_Grid5] = true;
			break;
		case IDC_MAPPING_GRID_NH:
			switch (HIWORD(wParam))
			{
			case EN_SETFOCUS:
				whohasfocus = 3;
				break;
			case EN_KILLFOCUS:
				whohasfocus = 0;
				break;
			}
			WCHAR buf6[MAX_PATH];
			GetWindowText((HWND)lParam, buf6, MAX_PATH);
			mappingData.Grid[5] = max(-32768, min(32767, _wtoi(buf6)));
			if (canprint) Modified[Mofified_Grid6] = true;
			break;
		case IDC_MAPPING_MOUSE_GRID_DEFAULT:
			mappingData.Mouse[6] = IsDlgButtonChecked(hWnd, LOWORD(wParam)); Modified[Mofified_Mouse7] = true; break;
		case IDC_MAPPING_X:
			switch (HIWORD(wParam))
			{
			case STN_CLICKED:
				start = std::chrono::system_clock::now();
				break;
			}
			break;
		case IDC_MAPPING_Y:
			switch (HIWORD(wParam))
			{
			case STN_CLICKED:
				if (capturehasbeendone)
				{
					TCHAR buf[6];
					switch (whohasfocus)
					{
					case 1:
						GetWindowText(GetDlgItem(hWnd, IDC_MAPPING_X), buf, 6);
						SetWindowText(GetDlgItem(hWnd, IDC_MAPPING_GRID_X), buf);
						GetWindowText(GetDlgItem(hWnd, IDC_MAPPING_Y), buf, 6);
						SetWindowText(GetDlgItem(hWnd, IDC_MAPPING_GRID_Y), buf);
						break;
					case 2:
						GetWindowText(GetDlgItem(hWnd, IDC_MAPPING_X), buf, 6);
						SetWindowText(GetDlgItem(hWnd, IDC_MAPPING_GRID_W), buf);
						GetWindowText(GetDlgItem(hWnd, IDC_MAPPING_Y), buf, 6);
						SetWindowText(GetDlgItem(hWnd, IDC_MAPPING_GRID_H), buf);
						break;
					case 3:
						GetWindowText(GetDlgItem(hWnd, IDC_MAPPING_X), buf, 6);
						SetWindowText(GetDlgItem(hWnd, IDC_MAPPING_GRID_NW), buf);
						GetWindowText(GetDlgItem(hWnd, IDC_MAPPING_Y), buf, 6);
						SetWindowText(GetDlgItem(hWnd, IDC_MAPPING_GRID_NH), buf);
						break;
					}
				}
				break;
			}
			break;
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

void MappingDataDlg::Show()
{
	ShowWindow(m_hDlg, SW_SHOW);
}

void MappingDataDlg::Hide()
{
	ShowWindow(m_hDlg, SW_HIDE);
}

BOOL MappingDataDlg::MoveWindow(int x, int y, int w, int h, BOOL r)
{
	return ::MoveWindow(m_hDlg, x, y, w, h, r);
}
