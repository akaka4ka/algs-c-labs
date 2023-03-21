#include "painting.h"

HWND hWnd = NULL;
HDC hDC = NULL;

HDC hdcMemDC = NULL;
HBITMAP hbmScreen = NULL;
BITMAP bmpScreen;

void DrawInit()
{
	RECT rct;
	hWnd = FindWindow(NULL, L"Task6");
	hDC = GetDC(hWnd);
}

void NewFile()
{
	HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
	RECT rct;
	GetClientRect(hWnd, &rct);

	SelectObject(hDC, hBrush);

	FillRect(hDC, &rct, RGB(255, 255, 255));

	DeleteObject(hBrush);
	ClearBM();
}

void PaintWithBrush(POS pos, COLORREF clr, size_t size)
{
	HBRUSH hBrush = CreateSolidBrush(clr);
	HPEN hBrushRound = CreatePen(PS_SOLID, 1, clr);

	SelectObject(hDC, hBrush);
	SelectObject(hDC, hBrushRound);

	Ellipse(hDC, pos.x - size, pos.y - size, pos.x + size, pos.y + size);

	DeleteObject(hBrush);
	DeleteObject(hBrushRound);
}

void PaintWithPencil(POS pos, COLORREF clr, size_t size)
{
	HPEN hPen = CreatePen(PS_SOLID, size, clr);
	HBRUSH hPenFill = CreateSolidBrush(clr);

	SelectObject(hDC, hPen);
	SelectObject(hDC, hPenFill);

	Rectangle(hDC, pos.x - size, pos.y - size, pos.x + size, pos.y + size);

	DeleteObject(hPen);
	DeleteObject(hPenFill);
}

void Erase(POS pos, size_t size)
{
	HBRUSH hEraser = CreateSolidBrush(RGB(255, 255, 255));
	HPEN hEraserRound = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));

	SelectObject(hDC, hEraser);
	SelectObject(hDC, hEraserRound);

	Ellipse(hDC, pos.x - size, pos.y - size, pos.x + size, pos.y + size);

	DeleteObject(hEraser);
	DeleteObject(hEraserRound);
}

void ResizeWnd(RECT newRct, RECT oldRct)
{
	GetObject(hbmScreen, sizeof(BITMAP), &bmpScreen);

	BitBlt(hDC, 0, 0, bmpScreen.bmWidth, bmpScreen.bmHeight, hdcMemDC, 0, 0, SRCCOPY);
}

void MemorizeWnd(RECT curRct)
{
	hdcMemDC = CreateCompatibleDC(hDC);

	if (!hdcMemDC)
	{
		MessageBox(hWnd, L"CreateCompatibleDC has failed", L"Failed", MB_OK);
		return;
	}

	SetStretchBltMode(hDC, HALFTONE);
	hbmScreen = CreateCompatibleBitmap(hDC, curRct.right - curRct.left, curRct.bottom - curRct.top);
	if (!hbmScreen)
	{
		MessageBox(hWnd, L"CreateCompatibleBitmap Failed", L"Failed", MB_OK);
		return;
	}

	SelectObject(hdcMemDC, hbmScreen);
	if (!BitBlt(hdcMemDC, 0, 0, curRct.right - curRct.left, curRct.bottom - curRct.top, hDC, 0, 0, SRCCOPY))
	{
		MessageBox(hWnd, L"BitBlt has failed", L"Failed", MB_OK);
		return;
	}
}

void ClearBM()
{
	DeleteObject(hbmScreen);
	hbmScreen = NULL;
}

void DrawRelease()
{
	ReleaseDC(hWnd, hDC);
	DeleteDC(hdcMemDC);
	DeleteObject(hbmScreen);
}

HDC GetHDC()
{
	return hDC;
}