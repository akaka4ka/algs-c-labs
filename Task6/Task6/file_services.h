#define _CRT_SECURE_NO_WARNINGS
#pragma once

#include "win_api.h"

int CaptureAnImage(HWND hWnd, HDC hdcWindow, LPCTSTR fname);
int LoadFromFile(HWND hWnd, HDC hdcWindow, LPCTSTR fname);