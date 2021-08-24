﻿#pragma once
#include "Mapping.h"

class MappingDlg
{
public:
	MappingDlg();
	~MappingDlg();

	void Init(HINSTANCE hInst, HWND hWnd);
	void Init2(HINSTANCE hInst, HWND hWnd);
	bool isCloned();
	void setCloned(bool cloned);
	void SetTab(int tab);
	int GetTab();
	void addMappingDlgBack();
	void editMappingDlgBack(int idx);
	void Show();
	void Hide();
	BOOL MoveWindow(int, int, int, int, BOOL);

	bool moving;

private:
	static INT_PTR CALLBACK Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	INT_PTR CALLBACK _proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void redrawMenu(int ntabs, bool menu = false);
	void redrawListReminder();
	void load();
	void save(bool closeeditor = true);
	void addMappingDlg();
	void editMappingDlg();
	void deleteMappingDlg();
	void duplicateMappingDlg();
	void moveMappingDlg(int tab);
	int insertMapping(int idx, Mapping* m);
	void BeginDrag(int idx);
	void DragMove(int x, int y);
	void EndDrag(int x, int y);
	void setInsertMark(int idx);

	HWND m_hWnd;
	HWND m_hDlg;
	HWND m_hList;
	HMENU m_hMenu;
	HMENU m_hMenu2;

	int m_Tab;
	int m_TabsID[13];
	int m_ReminderId[32];
	int m_Randcolor[32];
	bool m_active;
	bool m_flag_drag;
	int m_insrtpos;
	bool m_isClonedList;
	bool m_isCloned;
};

extern MappingDlg mDlg;
