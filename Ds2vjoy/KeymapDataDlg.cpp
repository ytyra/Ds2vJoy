﻿#include "stdafx.h"
#include "KeymapDataDlg.h"
#include "Ds2vJoy.h"

KeymapDataDlg::KeymapDataDlg()
	:m_hWnd(0)
	, m_parent(0)
	, m_hDlg(0)
	, m_hList(0)
	, m_hMenu(0)
	, m_hEdit(0)
	, m_defaultInputProc()
	, m_defaultInputProc2()
	, m_defaultListProc()
	, m_editIdx(0)
	, m_editCol(0)
	, m_filter_iwv(false)
	, m_flag_cancel(false)
	, firsttime(false)
	, canprint(false)
	, dlgPage(0)
	, keymapData()
	, fw()
	, alreadydone(false)
	, Modified()
	, m_mode(0)
{
	for (int i = 0; i < Mofified_count; i++) Modified[i] = 0;
}

KeymapDataDlg::~KeymapDataDlg()
{
}

void KeymapDataDlg::Init(HINSTANCE hInst, HWND hWnd)
{
	m_mode = 0;
	m_hDlg = CreateDialogParam(hInst, MAKEINTRESOURCE(IDD_KEYMAP_ADD), hWnd, (DLGPROC)Proc, (LPARAM)this);
	m_hWnd = hWnd;
	Hide();
}

void KeymapDataDlg::Open(HWND parent, int mode)
{
	m_mode = mode;
	dlgPage = 0;
	firsttime = false;
	canprint = false;
	ShowWindow(parent, SW_HIDE);
	ShowWindow(m_hDlg, SW_SHOW);
	PostMessage(m_hWnd, WM_SIZE, 0, -1);
}

/*
INT_PTR KeymapDataDlg::Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	KeymapDataDlg* dlg;
	if (uMsg == WM_INITDIALOG)
	{
		SetWindowLongPtr(hWnd, GWLP_USERDATA, lParam);
		dlg = (KeymapDataDlg*)lParam;
	}
	else
		dlg = (KeymapDataDlg*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if (dlg)
		return dlg->_proc(hWnd, uMsg, wParam, lParam);
	else
		return NULL;
}
*/

INT_PTR CALLBACK KeymapDataDlg::Proc(HWND hWnd, UINT uMsg, WPARAM wparam, LPARAM lparam)
{
	KeymapDataDlg* dlg;

	if (uMsg == WM_INITDIALOG)
	{
		dlg = reinterpret_cast<KeymapDataDlg*>(lparam);
		SetWindowLongPtrW(hWnd, DWLP_USER, lparam);
	}
	else
		dlg = reinterpret_cast<KeymapDataDlg*>(GetWindowLongPtrW(hWnd, DWLP_USER));
	if (dlg)
	{
		INT_PTR result;
		result = dlg->_proc(hWnd, uMsg, wparam, lparam);
		return result;
	}
	return DefWindowProcW(hWnd, uMsg, wparam, lparam);
}

INT_PTR KeymapDataDlg::_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static int moved;
	static RECT rect;
	switch (uMsg)
	{
	case WM_CTLCOLORDLG:
	{
		HDC hdcStatic = (HDC)wParam;
		SetTextColor(hdcStatic, tape.Tx_DLG);
		SetBkMode(hdcStatic, TRANSPARENT);
		SetBkColor(hdcStatic, tape.Bk_DLG);
		return (LRESULT)tape.hB_DLG;
	}
	case WM_CTLCOLORMSGBOX:
	{
		HDC hdcStatic = (HDC)wParam;
		SetTextColor(hdcStatic, tape.Tx_MSGBOX);
		SetBkMode(hdcStatic, TRANSPARENT);
		SetBkColor(hdcStatic, tape.Bk_MSGBOX);
		return (LRESULT)tape.hB_MSGBOX;
	}
	case WM_CTLCOLORSCROLLBAR:
	{
		HDC hdcStatic = (HDC)wParam;
		SetTextColor(hdcStatic, tape.Tx_SCROLLBAR);
		SetBkMode(hdcStatic, TRANSPARENT);
		SetBkColor(hdcStatic, tape.Bk_SCROLLBAR);
		return (LRESULT)tape.hB_SCROLLBAR;
	}
	case WM_CTLCOLORBTN:
	{
		HDC hdcStatic = (HDC)wParam;
		SetTextColor(hdcStatic, tape.Tx_BTN);
		SetBkMode(hdcStatic, TRANSPARENT);
		SetBkColor(hdcStatic, tape.Bk_BTN);
		return (LRESULT)tape.hB_BTN;
	}
	case WM_CTLCOLORSTATIC:
	{
		HDC hdcStatic = (HDC)wParam;
		SetTextColor(hdcStatic, tape.Tx_STATIC);
		SetBkMode(hdcStatic, TRANSPARENT);
		SetBkColor(hdcStatic, tape.Bk_STATIC);
		return (LRESULT)tape.hB_STATIC;
	}
	case WM_CTLCOLOREDIT:
	{
		HDC hdcStatic = (HDC)wParam;
		SetTextColor(hdcStatic, tape.Tx_EDIT);
		SetBkMode(hdcStatic, TRANSPARENT);
		return (LRESULT)tape.hB_EDIT;
	}
	case WM_CTLCOLORLISTBOX:
	{
		HDC hdcStatic = (HDC)wParam;
		SetTextColor(hdcStatic, tape.Tx_LISTBOX);
		SetBkMode(hdcStatic, TRANSPARENT);
		return (LRESULT)tape.hB_LISTBOX;
	}
	case WM_PAINT:
	{
		if (!IsIconic(hWnd))
		{
			PAINTSTRUCT ps;
			HDC hDC = BeginPaint(hWnd, &ps);

			RECT rect;
			GetClientRect(hWnd, &rect);
			FillRect(hDC, &rect, tape.hB_BackGround);

			POINT Pt;
			MoveToEx(hDC, 469, rect.top, &Pt);
			LineTo(hDC, 469, rect.bottom);

			::ReleaseDC(hWnd, hDC);
			EndPaint(hWnd, &ps);
		}
		return FALSE;
	}
	case WM_GETDLGCODE:
		return DLGC_WANTALLKEYS;
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
	{
		if (dlgPage == 0)
		{
			keymapData.GetState();
			SetWindowText(hWnd, keymapData.ValueString(1));
			return TRUE;
		}
		break;
	}
	case WM_RESET_FINDWINDOW:
	{
		if (!alreadydone)
		{
			alreadydone = true;
			SendDlgItemMessage(hWnd, IDC_KEYMAP_WINDOW_LIST, CB_RESETCONTENT, 0, 0);
			int idx = ListView_GetItemCount(m_hList);
			while (--idx >= 0) { ListView_DeleteItem(m_hList, idx); }
		}

		if (m_hEdit != NULL)
			DestroyWindow(m_hEdit);
		m_hEdit = NULL;

		SendMessage(GetDlgItem(m_hDlg, IDC_KEYMAP_IS_W_VISIBLE), BM_SETCHECK, BST_CHECKED, 0);
		m_filter_iwv = true;
		int length = keymapData.findWindow.Size();

		LVITEM item = { 0 };
		item.mask = LVIF_TEXT | LVIF_PARAM;
		item.iSubItem = 0;
		item.lParam = (LPARAM)0;
		for (int i = 0; i < length; i++)
		{
			auto findClass = keymapData.findWindow.Get(i, 0);
			auto findTitle = keymapData.findWindow.Get(i, 1);
			auto findIdx = keymapData.findWindow.Get(i, 2);
			item.iItem = ListView_GetItemCount(m_hList);
			item.pszText = const_cast<WCHAR*>(findClass.c_str());
			ListView_InsertItem(m_hList, &item);
			ListView_SetItemText(m_hList, item.iItem, 1, const_cast<WCHAR*>(findTitle.c_str()));
			ListView_SetItemText(m_hList, item.iItem, 2, const_cast<WCHAR*>(findIdx.c_str()));
		}
		break;
	}
	case WM_SHOWWINDOW:
	{
		if (wParam == TRUE)
		{
			SendDlgItemMessage(hWnd, IDC_KEYMAP_BTN, CB_SETCURSEL, (WPARAM)keymapData.ButtonID, 0);


			SendDlgItemMessage(hWnd, IDC_KEYMAP_INPUT1, CB_SETCURSEL, 0, 0);
			SendDlgItemMessage(hWnd, IDC_KEYMAP_INPUT2, CB_SETCURSEL, 0, 0);
			SendDlgItemMessage(hWnd, IDC_KEYMAP_INPUT3, CB_SETCURSEL, 0, 0);
			HWND hInput = GetDlgItem(hWnd, IDC_KEYMAP_INPUT);

			CheckDlgButton(hWnd, IDC_KEYMAP_NATURAL, keymapData.NaturalTyping);

			SetWindowText(GetDlgItem(hWnd, IDC_KEYMAP_METHOD), L"Method: PostMessage");
			if (keymapData.usePostmessage)
			{
				CheckDlgButton(hWnd, IDC_KEYMAP_POSTMESSAGE, BST_CHECKED);
				EnableWindow(GetDlgItem(hWnd, IDC_KEYMAP_RESTORE), TRUE);
				EnableWindow(GetDlgItem(hWnd, IDC_KEYMAP_MAXIMIZE), TRUE);
				EnableWindow(GetDlgItem(hWnd, IDC_KEYMAP_SHOW), TRUE);
				EnableWindow(GetDlgItem(hWnd, IDC_KEYMAP_ACTIVATING), TRUE);
				EnableWindow(GetDlgItem(hWnd, IDC_KEYMAP_FW_TEXT), TRUE);
				EnableWindow(GetDlgItem(hWnd, IDC_KEYMAP_FINDWINDOW), TRUE);
				CheckDlgButton(hWnd, IDC_KEYMAP_RESTORE, keymapData.WndRestore);
				CheckDlgButton(hWnd, IDC_KEYMAP_MAXIMIZE, keymapData.WndMaximize);
				CheckDlgButton(hWnd, IDC_KEYMAP_SHOW, keymapData.WndShow);
				CheckDlgButton(hWnd, IDC_KEYMAP_ACTIVATING, keymapData.useActivating);
				if (keymapData.useActivating)
					SetWindowText(GetDlgItem(hWnd, IDC_KEYMAP_METHOD), L"Method: Activating");
			}
			else
			{
				CheckDlgButton(hWnd, IDC_KEYMAP_POSTMESSAGE, BST_UNCHECKED);
				EnableWindow(GetDlgItem(hWnd, IDC_KEYMAP_RESTORE), FALSE);
				EnableWindow(GetDlgItem(hWnd, IDC_KEYMAP_MAXIMIZE), FALSE);
				EnableWindow(GetDlgItem(hWnd, IDC_KEYMAP_SHOW), FALSE);
				EnableWindow(GetDlgItem(hWnd, IDC_KEYMAP_ACTIVATING), FALSE);
				EnableWindow(GetDlgItem(hWnd, IDC_KEYMAP_FW_TEXT), FALSE);
				EnableWindow(GetDlgItem(hWnd, IDC_KEYMAP_FINDWINDOW), FALSE);
			}

			canprint = false;
			SetWindowText(GetDlgItem(hWnd, IDC_KEYMAP_INPUT), keymapData.ValueString(1));
			SetWindowText(GetDlgItem(m_hDlg, IDC_KEYMAP_FW_TEXT), keymapData.findWindow.Val().c_str());
			canprint = true;

			CheckDlgButton(hWnd, IDC_KEYMAP_EXTENDEDKEY, keymapData.ExtendedKey);
			CheckDlgButton(hWnd, IDC_KEYMAP_SCANCODE, keymapData.Scancode);

			m_hList = GetDlgItem(m_hDlg, IDC_KEYMAP_FIND_LIST);

			if (!firsttime)
			{
				DWORD dwStyle = ListView_GetExtendedListViewStyle(m_hList);
				dwStyle |= LVS_EX_FULLROWSELECT;
				ListView_SetExtendedListViewStyle(m_hList, dwStyle);
				LVCOLUMN col;

				firsttime = true;
				col.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
				col.fmt = LVCFMT_LEFT;
				col.cx = 100;
				col.pszText = L"Class";
				ListView_InsertColumn(m_hList, 0, &col);
				col.pszText = L"Title";
				col.cx = 200;
				ListView_InsertColumn(m_hList, 1, &col);
				col.pszText = L"Idx";
				col.cx = 100;
				ListView_InsertColumn(m_hList, 2, &col);
			}

			SendMessage(hWnd, WM_RESET_FINDWINDOW, 0, 0);

			SetFocus(GetDlgItem(hWnd, IDC_KEYMAP_INPUT));
			EnableWindow(GetDlgItem(hWnd, IDC_KEYMAP_INPUT), true);

			HFONT hFont = CreateFont(12, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
			SendDlgItemMessage(hWnd, IDC_KEYMAP_INPUT1, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
			SendDlgItemMessage(hWnd, IDC_KEYMAP_INPUT2, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
			SendDlgItemMessage(hWnd, IDC_KEYMAP_INPUT3, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
		}
		break;
	}
	case WM_INITDIALOG:
	{
		SetTimer(hWnd, 1, 10, NULL);
		for (int i = 0; i < vJoyButtonID::button_Count; i++)
		{
			WCHAR* str = vJoyButton::String((vJoyButtonID)i);
			SendDlgItemMessage(hWnd, IDC_KEYMAP_BTN, CB_ADDSTRING, 0, (LPARAM)str);
		}
		for (int i = 0; i < keymapData.keyboard_Count; i++)
		{
			WCHAR* str = keymapData.String((KeyboardID)i);
			SendDlgItemMessage(hWnd, IDC_KEYMAP_INPUT1, CB_ADDSTRING, 0, (LPARAM)str);
			SendDlgItemMessage(hWnd, IDC_KEYMAP_INPUT2, CB_ADDSTRING, 0, (LPARAM)str);
			SendDlgItemMessage(hWnd, IDC_KEYMAP_INPUT3, CB_ADDSTRING, 0, (LPARAM)str);
		}
		return TRUE;
	}
	case WM_TIMER:
		if (wParam == 1)
		{
			if (moved > 0)
			{
				if (moved < 22)
				{
					static const double y470 = 0.046126560401444254082956;
					static const double y20minus470 = 0.177480237348534715557960;

					double tmp1 = (double)moved - 1;
					double tmp2 = ((y20minus470 * tmp1) / 21) + y470;
					double tmp3 = 1 / (tmp2 * tmp2);
					int tmp4 = 492 - (int)tmp3;
					::MoveWindow(m_hDlg, rect.left - tmp4, rect.top, rect.right + tmp4, rect.bottom, TRUE);
					moved++;
				}
				else
				{
					moved = 0;
				}
			}
			if (moved < 0)
			{
				if (moved > -22)
				{
					static const double y470 = 0.046126560401444254082956;
					static const double y20minus470 = 0.177480237348534715557960;

					double tmp1 = -1 - (double)moved;
					double tmp2 = ((y20minus470 * tmp1) / 21) + y470;
					double tmp3 = 1 / (tmp2 * tmp2);
					int tmp4 = 492 - (int)tmp3;
					::MoveWindow(m_hDlg, rect.left - 470 + tmp4, rect.top, rect.right + 470 - tmp4, rect.bottom, TRUE);
					moved--;
				}
				else
				{
					moved = 0;
				}
			}
		}
		break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			if (keymapData.ButtonID == 0 && m_mode != 3)
			{
				RECT rect;
				GetWindowRect(m_hWnd, &rect);
				MessageBoxPos(hWnd, I18N.MBOX_NoButtonSelected, I18N.MBOX_ErrTitle, MB_ICONERROR, rect.left + 275, rect.top + 30);
				return TRUE;
			}
			keymapData.Enable = true;
			int key1 = (int)SendMessage(GetDlgItem(hWnd, IDC_KEYMAP_INPUT1), CB_GETCURSEL, 0, 0);
			int key2 = (int)SendMessage(GetDlgItem(hWnd, IDC_KEYMAP_INPUT2), CB_GETCURSEL, 0, 0);
			int key3 = (int)SendMessage(GetDlgItem(hWnd, IDC_KEYMAP_INPUT3), CB_GETCURSEL, 0, 0);
			if (key1) { Modified[Mofified_vk] = true; keymapData.vk.push_back(keymapData.KeyboardIDtoByte((KeyboardID)key1)); }
			if (key1) { Modified[Mofified_vk] = true; keymapData.vk.push_back(keymapData.KeyboardIDtoByte((KeyboardID)key2)); }
			if (key1) { Modified[Mofified_vk] = true; keymapData.vk.push_back(keymapData.KeyboardIDtoByte((KeyboardID)key3)); }
			PostMessage(m_hWnd, WM_ADDKEYMAP, m_mode, 1);
			m_mode = 0;
			break;
		}
		case IDCANCEL:
			PostMessage(m_hWnd, WM_ADDKEYMAP, 0, 0);
			m_mode = 0;
			break;
		case IDC_CLEAR:
		{
			keymapData.vk.clear();
			keymapData.GetState();
			Modified[Mofified_vk] = true;
			SetWindowText(GetDlgItem(hWnd, IDC_KEYMAP_INPUT), L"");
			break;
		}
		case IDC_KEYMAP_BTN:
			if (HIWORD(wParam) == CBN_SELCHANGE)
			{
				keymapData.ButtonID = (byte)SendMessage((HWND)lParam, CB_GETCURSEL, 0, 0);
				Modified[Mofified_ButtonID] = true;
			}
			break;
		case IDC_KEYMAP_RESTORE:
			keymapData.WndRestore = IsDlgButtonChecked(hWnd, LOWORD(wParam));
			Modified[Mofified_WndRestore] = true;
			break;
		case IDC_KEYMAP_MAXIMIZE:
			keymapData.WndMaximize = IsDlgButtonChecked(hWnd, LOWORD(wParam));
			Modified[Mofified_WndMaximize] = true;
			break;
		case IDC_KEYMAP_SHOW:
			keymapData.WndShow = IsDlgButtonChecked(hWnd, LOWORD(wParam));
			Modified[Mofified_WndShow] = true;
			break;
		case IDC_KEYMAP_NATURAL:
			keymapData.NaturalTyping = IsDlgButtonChecked(hWnd, LOWORD(wParam));
			Modified[Mofified_NaturalTyping] = true;
			break;
		case IDC_KEYMAP_INPUT:
			switch (HIWORD(wParam))
			{
			case EN_CHANGE: {
				keymapData.GetState();
				SetWindowText((HWND)lParam, keymapData.ValueString(1));
				Modified[Mofified_vk] = true;
				break;
			}
			default:
				return FALSE;
			}
			break;
		case IDC_KEYMAP_POSTMESSAGE:
		{
			keymapData.usePostmessage = IsDlgButtonChecked(hWnd, LOWORD(wParam));
			if (keymapData.usePostmessage)
			{
				EnableWindow(GetDlgItem(hWnd, IDC_KEYMAP_RESTORE), TRUE);
				EnableWindow(GetDlgItem(hWnd, IDC_KEYMAP_MAXIMIZE), TRUE);
				EnableWindow(GetDlgItem(hWnd, IDC_KEYMAP_SHOW), TRUE);
				EnableWindow(GetDlgItem(hWnd, IDC_KEYMAP_ACTIVATING), TRUE);
				EnableWindow(GetDlgItem(hWnd, IDC_KEYMAP_FW_TEXT), TRUE);
				EnableWindow(GetDlgItem(hWnd, IDC_KEYMAP_FINDWINDOW), TRUE);
			}
			else
			{
				if (keymapData.WndRestore)
				{
					keymapData.WndRestore = 0;
					CheckDlgButton(hWnd, IDC_KEYMAP_RESTORE, BST_UNCHECKED);
					Modified[Mofified_WndRestore] = true;
				}
				if (keymapData.WndMaximize)
				{
					keymapData.WndMaximize = 0;
					CheckDlgButton(hWnd, IDC_KEYMAP_MAXIMIZE, BST_UNCHECKED);
					Modified[Mofified_WndMaximize] = true;
				}
				if (keymapData.WndShow)
				{
					keymapData.WndShow = 0;
					CheckDlgButton(hWnd, IDC_KEYMAP_SHOW, BST_UNCHECKED);
					Modified[Mofified_WndShow] = true;
				}
				if (keymapData.useActivating)
				{
					keymapData.useActivating = false;
					CheckDlgButton(hWnd, IDC_KEYMAP_ACTIVATING, BST_UNCHECKED);
					SetWindowText(GetDlgItem(hWnd, IDC_KEYMAP_METHOD), L"Method: PostMessage");
				}
				EnableWindow(GetDlgItem(hWnd, IDC_KEYMAP_RESTORE), FALSE);
				EnableWindow(GetDlgItem(hWnd, IDC_KEYMAP_MAXIMIZE), FALSE);
				EnableWindow(GetDlgItem(hWnd, IDC_KEYMAP_SHOW), FALSE);
				EnableWindow(GetDlgItem(hWnd, IDC_KEYMAP_ACTIVATING), FALSE);
				EnableWindow(GetDlgItem(hWnd, IDC_KEYMAP_FW_TEXT), FALSE);
				EnableWindow(GetDlgItem(hWnd, IDC_KEYMAP_FINDWINDOW), FALSE);
			}
			Modified[Mofified_usePostmessage] = true;
			break;
		}
		case IDC_KEYMAP_ACTIVATING:
			keymapData.useActivating = IsDlgButtonChecked(hWnd, LOWORD(wParam));
			if (keymapData.useActivating)
				SetWindowText(GetDlgItem(hWnd, IDC_KEYMAP_METHOD), L"Method: Activating");
			else
				SetWindowText(GetDlgItem(hWnd, IDC_KEYMAP_METHOD), L"Method: PostMessage");
			Modified[Mofified_useActivating] = true;
			break;
		case IDC_KEYMAP_EXTENDEDKEY:
			keymapData.ExtendedKey = IsDlgButtonChecked(hWnd, LOWORD(wParam));
			Modified[Mofified_ExtendedKey] = true;
			break;
		case IDC_KEYMAP_SCANCODE:
			keymapData.Scancode = IsDlgButtonChecked(hWnd, LOWORD(wParam));
			Modified[Mofified_Scancode] = true;
			break;
		case IDC_KEYMAP_FINDWINDOW:
		{
			fw = keymapData.findWindow;
			GetClientRect(m_hWnd, &rect);
			rect.left += 3;
			rect.top += 21;
			rect.right -= 6;
			rect.bottom -= 45;
			dlgPage = 1;
			moved = 1;
			break;
		}
		case IDC_KEYMAP_WINDOW_LIST:
			switch (HIWORD(wParam))
			{
			case CBN_CLOSEUP:
			{
				LRESULT idx = SendMessage((HWND)lParam, CB_GETCURSEL, 0, 0);
				if (idx != CB_ERR)
				{
					HWND hWin = (HWND)SendMessage((HWND)lParam, CB_GETITEMDATA, idx, 0);
					if (hWin != (HWND)CB_ERR && hWin != NULL)
					{
						int iItem = ListView_GetItemCount(m_hList);
						auto classname = Findwindow::GetClass(hWin);
						LVITEM item = { 0 };
						item.mask = LVIF_TEXT | LVIF_PARAM;
						item.iItem = iItem;
						item.pszText = const_cast<LPWSTR>(classname.c_str());
						ListView_InsertItem(m_hList, &item);

						auto titlename = Findwindow::GetText(hWin);
						ListView_SetItemText(m_hList, iItem, 1, const_cast<LPWSTR>(titlename.c_str()));

						int idx = 0;
						HWND hwnd = keymapData.findWindow.Find();
						while (idx == 0)
						{
							HWND child = NULL;

							while ((child = FindWindowEx(hwnd, child, classname.empty() ? NULL : classname.c_str(), titlename.empty() ? NULL : titlename.c_str())) != hWin)
							{
								idx++;
								if (child == NULL)
								{
									idx = -1;
									break;
								}
							}
							if (idx == -1 && !titlename.empty())
							{
								idx = 0;
								titlename.clear();
							}
							else
								break;
						}
						WCHAR findIdx[10] = L"";
						wsprintf(findIdx, L"%d", idx);
						ListView_SetItemText(m_hList, iItem, 2, findIdx);

						keymapData.findWindow.Set(-1, 0, classname);
						keymapData.findWindow.Set(-1, 1, titlename);
						keymapData.findWindow.Set(-1, 2, findIdx);
					}
				}
				break;
			}
			case CBN_DROPDOWN:
				SendMessage((HWND)lParam, CB_RESETCONTENT, 0, 0);
				{
					HWND hwnd = keymapData.findWindow.Find();
					if (hwnd != NULL || keymapData.findWindow.Size() == 0)
					{
						HWND child = NULL;
						int count = 0;
						std::wstring buf;
						while ((child = FindWindowEx(hwnd, child, NULL, NULL)) != NULL)
						{
							if (!m_filter_iwv || IsWindowVisible(child))
							{
								count++;
								buf.clear();
								buf.append(Findwindow::GetClass(child));
								buf.append(L", ");
								buf.append(Findwindow::GetText(child));
								LRESULT idx = SendMessage((HWND)lParam, CB_ADDSTRING, 0, (LPARAM)buf.c_str());
								if (idx != CB_ERR && idx != CB_ERRSPACE)
									SendMessage((HWND)lParam, CB_SETITEMDATA, idx, (LPARAM)child);
							}
						}
						if (count == 0)
						{
							WCHAR buf[100];
							wsprintf(buf, L"HWND:%08x", (int)(size_t)hwnd);
							SendMessage((HWND)lParam, CB_ADDSTRING, 0, (LPARAM)buf);
						}
					}
					else if (hwnd == NULL)
						SendMessage((HWND)lParam, CB_ADDSTRING, 0, (LPARAM)L"Failed FindWindow");
				}
				break;
			}
			break;
		case IDC_CLEAR2:
		{
			int idx = ListView_GetItemCount(m_hList);
			while (--idx >= 0)
				ListView_DeleteItem(m_hList, idx);
			keymapData.findWindow.Clear();
			break;
		}
		case IDC_DELETE:
		{
			int idx = ListView_GetItemCount(m_hList);
			if (idx > 0)
			{
				ListView_DeleteItem(m_hList, --idx);
				keymapData.findWindow.Pop();
			}
			break;
		}
		case IDC_OK2:
			keymapData.findWindow = keymapData.findWindow;
			if (m_hEdit != NULL)
			{
				DestroyWindow(m_hEdit);
				m_hEdit = NULL;
			}
			SetWindowText(GetDlgItem(m_hDlg, IDC_KEYMAP_FW_TEXT), keymapData.findWindow.Val().c_str());
			Modified[Mofified_findWindow] = true;
			dlgPage = 0;
			PostMessage(m_hWnd, WM_SIZE, 0, 0);
			break;
		case IDC_CANCEL2:
			alreadydone = false;
			keymapData.findWindow = fw;
			SendMessage(hWnd, WM_RESET_FINDWINDOW, 0, 0);
			GetClientRect(m_hWnd, &rect);
			rect.left += 3;
			rect.top += 21;
			rect.right -= 6;
			rect.bottom -= 45;
			dlgPage = 0;
			moved = -1;
			break;
		case IDC_KEYMAP_IS_W_VISIBLE:
			m_filter_iwv = IsDlgButtonChecked(hWnd, LOWORD(wParam));
			break;
		}
		break;
	}
	case WM_NOTIFY:
	{
		switch (((LPNMHDR)lParam)->idFrom)
		{
		case IDC_KEYMAP_FIND_LIST:
			switch (((LPNMLISTVIEW)lParam)->hdr.code)
			{
			case NM_DBLCLK:
			{
				if (m_hEdit == NULL)
				{
					LVHITTESTINFO ht;
					GetCursorPos(&ht.pt);
					ScreenToClient(m_hList, &ht.pt);
					if (-1 != ListView_SubItemHitTest(m_hList, &ht))
					{
						RECT rc;
						ListView_GetSubItemRect(m_hList, ht.iItem, ht.iSubItem, LVIR_LABEL, &rc);
						m_hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_WANTRETURN, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, m_hList, 0, GetModuleHandle(NULL), NULL);
						m_editIdx = ht.iItem;
						m_editCol = ht.iSubItem;
						auto buf = keymapData.findWindow.Get(m_editIdx, m_editCol);
						SetWindowText(m_hEdit, buf.c_str());
						HFONT hFont = (HFONT)SendMessage(m_hList, WM_GETFONT, NULL, NULL);
						SendMessage(m_hEdit, WM_SETFONT, (WPARAM)hFont, 0);
						m_flag_cancel = true;
						SetFocus(m_hEdit);
					}
				}
				break;
			}
			case WM_KILLFOCUS:
			case NM_CLICK:
				if (m_hEdit != NULL)
				{
					SendMessage(m_hEdit, WM_KILLFOCUS, 0, 0);
					if (m_flag_cancel)
					{
						auto buf = keymapData.findWindow.GetText(m_hEdit);
						keymapData.findWindow.Set(m_editIdx, m_editCol, std::move(buf));
						buf = keymapData.findWindow.Get(m_editIdx, m_editCol);
						ListView_SetItemText(m_hList, m_editIdx, m_editCol, const_cast<WCHAR*>(buf.c_str()));
					}
					DestroyWindow(m_hEdit);
					m_flag_cancel = false;
					m_hEdit = NULL;
				}
				break;
			case NM_RCLICK:
				if (m_hEdit != NULL)
				{
					DestroyWindow(m_hEdit);
					m_hEdit = NULL;
				}
				break;
			default:
				return FALSE;
			}
			break;
		default:
			return FALSE;
		}
		break;
	}
	default:
		return FALSE;
	}
	return TRUE;
}

void KeymapDataDlg::Show()
{
	ShowWindow(m_hDlg, SW_SHOW);
}

void KeymapDataDlg::Hide()
{
	ShowWindow(m_hDlg, SW_HIDE);
}

BOOL KeymapDataDlg::MoveWindow(int x, int y, int w, int h, BOOL r)
{
	return ::MoveWindow(m_hDlg, x, y, w, h, r);
}
