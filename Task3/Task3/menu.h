#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include "resource.h"
#include "framework.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

#define NaI -1
#define BUFFSZ 50
#define MENU_WRDS 10
#define MENU_BGN "MenuBegin"
#define MENU_END "MenuEnd"
#define WND_BGN "WindowBegin"
#define WND_END "WindowEnd"
#define BTN_BGN "ButtonBegin"
#define BTN_END "ButtonEnd"
#define POS "pos="
#define CLR "color="
#define HIGHLIGHTCLR "higlightColor="
#define NAME "name="
#define BTN_STYLE WS_CHILD | WS_VISIBLE | BS_OWNERDRAW

typedef struct {
	_Bool focused;
	HWND hBtnWnd;
	wchar_t* name;
	int posX;
	int posY;
	COLORREF color;
	COLORREF highLightColor;
} BTNS;

FILE* SysOpen();
BTNS* SysClose(BTNS* btns, FILE* filePtr, int btnsNum);
BTNS* SystemInitialise(FILE* filePtr, size_t* btnsNum);
BTNS* SysRelease(BTNS* btns, int btnsNum);
int SysUpdate(BTNS* btns, LPDRAWITEMSTRUCT draw, HWND hWnd, int curFocus, int nPrev, _Bool isload, WPARAM wParam);