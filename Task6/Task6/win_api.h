#pragma once

#define MAX_LOADSTRING 100
#define MAIN_TIMER 0

#pragma region Framework

// »сключите редко используемые компоненты из заголовков Windows
#define WIN32_LEAN_AND_MEAN
// ‘айлы заголовков Windows
#include <windows.h>
// ‘айлы заголовков среды выполнени€ C
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#pragma endregion

#pragma region Resources

#define IDS_APP_TITLE			103

#define IDR_MAINFRAME			128
#define IDD_TASK6_DIALOG	102
#define IDD_ABOUTBOX			103
#define IDM_ABOUT				104
#define IDM_EXIT				105
#define IDI_TASK6			107
#define IDI_SMALL				108
#define IDC_TASK6			109
#define IDC_MYICON				2
#ifndef IDC_STATIC
#define IDC_STATIC				-1
#endif

#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS

#define _APS_NO_MFC					130
#define _APS_NEXT_RESOURCE_VALUE	129
#define _APS_NEXT_COMMAND_VALUE		32771
#define _APS_NEXT_CONTROL_VALUE		1000
#define _APS_NEXT_SYMED_VALUE		110
#endif
#endif

#pragma endregion

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
BOOL	CALLBACK	DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

//MAKEINTRESOURCEW(IDC_TASK6) Ч lpszMenuName