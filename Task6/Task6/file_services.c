#include "file_services.h"

int CaptureAnImage(HWND hWnd, HDC hdcWindow, LPCTSTR fname)
{
	HDC hdcMemDC = NULL;
	HBITMAP hbmScreen = NULL;
	BITMAP bmpScreen;
	hdcMemDC = CreateCompatibleDC(hdcWindow);

	if (!hdcMemDC)
	{
		MessageBox(hWnd, L"CreateCompatibleDC has failed", L"Failed", MB_OK);
		return;
	}
	RECT rcClient;
	GetClientRect(hWnd, &rcClient);

	SetStretchBltMode(hdcWindow, HALFTONE);

	hbmScreen = CreateCompatibleBitmap(hdcWindow, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);

	if (!hbmScreen)
	{
		MessageBox(hWnd, L"CreateCompatibleBitmap Failed", L"Failed", MB_OK);
		return;
	}
	SelectObject(hdcMemDC, hbmScreen);
	if (!BitBlt(hdcMemDC,
		0, 0,
		rcClient.right - rcClient.left, rcClient.bottom - rcClient.top,
		hdcWindow,
		0, 0,
		SRCCOPY))
	{
		MessageBox(hWnd, L"BitBlt has failed", L"Failed", MB_OK);
		return;
	}
	GetObject(hbmScreen, sizeof(BITMAP), &bmpScreen);

	BITMAPFILEHEADER   bmfHeader;
	BITMAPINFOHEADER   bi;

	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = bmpScreen.bmWidth;
	bi.biHeight = bmpScreen.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = 32;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	DWORD dwBmpSize = ((bmpScreen.bmWidth * bi.biBitCount + 31) / 32) * 4 * bmpScreen.bmHeight;
	HANDLE hDIB = GlobalAlloc(GHND, dwBmpSize);
	char* lpbitmap = (char*)GlobalLock(hDIB);
	GetDIBits(hdcWindow, hbmScreen, 0,
		(UINT)bmpScreen.bmHeight,
		lpbitmap,
		(BITMAPINFO*)&bi, DIB_RGB_COLORS);
	HANDLE hFile = CreateFile(fname,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD dwSizeofDIB = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);
	bmfHeader.bfSize = dwSizeofDIB;
	bmfHeader.bfType = 0x4D42; //BM   
	DWORD dwBytesWritten = 0;
	WriteFile(hFile, (LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
	WriteFile(hFile, (LPSTR)&bi, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
	WriteFile(hFile, (LPSTR)lpbitmap, dwBmpSize, &dwBytesWritten, NULL);
	GlobalUnlock(hDIB);
	GlobalFree(hDIB);
	CloseHandle(hFile);
	DeleteObject(hbmScreen);
	DeleteObject(hdcMemDC);
	return 0;
}

int LoadFromFile(HWND hWnd, HDC hdcWindow, LPCTSTR fname)
{
	HDC virtualDC = CreateCompatibleDC(hdcWindow);
	HBITMAP memBM = CreateCompatibleBitmap(hdcWindow, 0, 0);
	memBM = (HBITMAP)LoadImage(NULL, (LPCWSTR)fname, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	BITMAP bm;
	GetObject(memBM, sizeof(BITMAP), &bm);

	SelectObject(virtualDC, memBM);

	SetWindowPos(hWnd, NULL, 200, 200, bm.bmWidth, bm.bmHeight, NULL);

	BitBlt(hdcWindow, 0, 0, bm.bmWidth, bm.bmHeight, virtualDC, 0, 0, SRCCOPY);

	DeleteObject(memBM);
	DeleteDC(virtualDC);
}