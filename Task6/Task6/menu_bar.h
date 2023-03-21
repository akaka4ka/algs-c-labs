#define _CRT_SECURE_NO_WARNINGS
#pragma once

#include <stdbool.h>
#include "win_api.h"
#include "tools.h"
#include "painting.h"
#include "file_services.h"

enum MenuBarBtns
{
	IDM_FILE_NEW,
	IDM_FILE_OPEN,
	IDM_FILE_SAVE,
	IDM_FILE_QUIT,

	IDM_TOOLS_BRUSH,
	IDM_TOOLS_PENCIL,
	IDM_TOOLS_ERASER
};

void MenuVarsInit(int* tool, COLORREF* curClr, size_t* sizePtr, bool* fileChanged, RECT* wndRct);
void MenuBarInit(HINSTANCE hInstance);
void AddMenus(HWND hWnd);
void MenuSelect(int btnId, HINSTANCE hInst);