#include "menu.h"

FILE* SysOpen()
{
	FILE* filePtr = fopen("menu.txt", "r");
	if (filePtr == NULL)
	{
		WriteFile(STD_OUTPUT_HANDLE, "Failure to open the menu-file...", strlen("Failure to open the menu-file..."),
				  NULL, NULL);
		return NULL;
	}

	return filePtr;
}

BTNS* SysClose(BTNS* btns, FILE* filePtr, int btnsNum)
{
	HWND hWnd = FindWindow(NULL, L"Task3");
	fclose(filePtr);
	for (int i = 0; i < btnsNum; i++)
	{
		DestroyWindow(btns[i].hBtnWnd);
	}
	if (btns != NULL)
	{
		free(btns);
	}
	InvalidateRect(hWnd, NULL, TRUE);
	UpdateWindow(hWnd);

	return NULL;
}

BTNS* SystemInitialise(FILE* filePtr, size_t* btnsNum)
{
	BTNS* btns = NULL;
	HBRUSH hWndBrush;
	COLORREF _clr = 0x0;
	size_t _clrR = 0;
	size_t _clrG = 0;
	size_t _clrB = 0;
	size_t i = 0;
	size_t j = 0;
	char* strPtr = NULL;
	char* str = NULL;
	char c = 0;
	_Bool menuFlag = FALSE;
	
	str = (char*)calloc(BUFFSZ, sizeof(char));
	if (str == NULL)
	{
		WriteFile(STD_OUTPUT_HANDLE, "Ran out of memory...", strlen("Ran out of memory..."), NULL, NULL);
		return NULL;
	}

	while ((c = fgetc(filePtr)) != EOF)
	{
		if (c == '\n')
		{
			if (!menuFlag && (strstr(str, MENU_BGN) != NULL))
			{
				menuFlag = TRUE;
			}

			if (menuFlag && (strstr(str, MENU_END) != NULL))
			{
				menuFlag = FALSE;
				break;
			}

			if (menuFlag && (strstr(str, WND_BGN) != NULL))
			{
				memset(str, 0, 50);
				j = 0;

				while (strstr(str, WND_END) == NULL)
				{
					c = fgetc(filePtr);
					if (c == '\n')
					{
						HWND hWnd = FindWindow(NULL, L"Task3");

						if (strstr(str, POS))
						{
							int _wndX = 0;
							int _wndY = 0;

							strPtr = strchr(str, '(');
							sscanf(strPtr + 1, "%d", &_wndX);
							
							strPtr = strchr(str, ',');
							sscanf(strPtr + 1, "%d", &_wndY);

							SetWindowPos(hWnd, NULL, _wndX, _wndY, 250, 350, NULL);
						}

						if (strstr(str, CLR))
						{
							strPtr = strchr(str, '(');
							sscanf(strPtr + 1, "%u", &_clrR);

							strPtr = strchr(str, ',');
							sscanf(strPtr + 1, "%u", &_clrG);

							strPtr = strrchr(str, ',');
							sscanf(strPtr + 1, "%u", &_clrB);

							_clr = RGB(_clrR, _clrG, _clrB);
							hWndBrush = CreateSolidBrush(_clr);

							SetClassLongPtr(hWnd, GCLP_HBRBACKGROUND, hWndBrush);
							InvalidateRect(hWnd, NULL, TRUE);
							UpdateWindow(hWnd);
							DeleteObject(hWndBrush);
						}
					}
					else
					{
						str[j] = c;
						j++;
						continue;
					}

					j = 0;
					memset(str, 0, 50);
				}
			}

			if (menuFlag && (strstr(str, BTN_BGN) != NULL))
			{
				(*btnsNum)++;
				BTNS* _memoryCheck = NULL;

				_memoryCheck = (BTNS*)realloc(btns, *btnsNum * sizeof(BTNS));
				if (_memoryCheck == NULL)
				{
					free(btns);
					WriteFile(STD_OUTPUT_HANDLE, "Ran out of memory...", strlen("Ran out of memory..."), NULL, NULL);
					return NULL;
				}
				else
				{
					btns = _memoryCheck;
				}

				memset(str, 0, 50);
				j = 0;

				while (strstr(str, BTN_END) == NULL)
				{
					size_t nameLength = 0;

					c = fgetc(filePtr);
					if (c == '\n')
					{
						if (strstr(str, POS))
						{
							strPtr = strchr(str, '(');
							sscanf(strPtr + 1, "%d", &btns[*btnsNum - 1].posX);

							strPtr = strchr(str, ',');
							sscanf(strPtr + 1, "%d", &btns[*btnsNum - 1].posY);
						}

						if (strstr(str, CLR))
						{
							strPtr = strchr(str, '(');
							sscanf(strPtr + 1, "%u", &_clrR);

							strPtr = strchr(str, ',');
							sscanf(strPtr + 1, "%u", &_clrG);

							strPtr = strrchr(str, ',');
							sscanf(strPtr + 1, "%u", &_clrB);

							btns[*btnsNum - 1].color = RGB(_clrR, _clrG, _clrB);
						}

						if (strstr(str, HIGHLIGHTCLR))
						{
							strPtr = strchr(str, '(');
							sscanf(strPtr + 1, "%u", &_clrR);

							strPtr = strchr(str, ',');
							sscanf(strPtr + 1, "%u", &_clrG);

							strPtr = strrchr(str, ',');
							sscanf(strPtr + 1, "%u", &_clrB);

							btns[*btnsNum - 1].highLightColor = RGB(_clrR, _clrG, _clrB);
						}

						if (strstr(str, NAME))
						{
							strPtr = strchr(str, '\"');
							nameLength = strchr(strPtr + 1, '\"') - strPtr;

							btns[*btnsNum - 1].name = (wchar_t*)calloc(nameLength, sizeof(wchar_t));
							if (btns[*btnsNum - 1].name == NULL)
							{
								WriteFile(STD_OUTPUT_HANDLE, "Ran out of memory...", strlen("Ran out of memory..."),
										  NULL, NULL);
								return NULL;
							}

							for (int k = 0; k < nameLength - 1; k++)
							{
								btns[*btnsNum - 1].name[k] = *(strPtr + 1 + k);
							}
						}
					}
					else
					{
						str[j] = c;
						j++;
						continue;
					}

					j = 0;
					memset(str, 0, 50);
				}
			}
		}
		else
		{
			str[j] = c;
			j++;
			continue;
		}

		j = 0;
		memset(str, 0, 50);
	}
	return btns;
}

int SysUpdate(BTNS* btns, LPDRAWITEMSTRUCT draw, HWND hWnd, int curFocus, int prevFocus, _Bool isload, WPARAM wParam)
{
	HBRUSH hNormalBrush, hFocusBrush;
	wchar_t btnText[255];
	static size_t redrawNum = 0;
	memset(btnText, 0, 255);

	if (redrawNum == 0)
	{
		if (draw->CtlType == ODT_BUTTON)
		{
			hNormalBrush = CreateSolidBrush(btns[curFocus].color);
			hFocusBrush = CreateSolidBrush(btns[curFocus].highLightColor);

			if (btns[curFocus].focused)
			{
				COLORREF txtClr = btns[curFocus].highLightColor;
				FillRect(draw->hDC, &draw->rcItem, hFocusBrush);
				SetTextColor(draw->hDC, RGB(255 - GetRValue(txtClr), 255 - GetGValue(txtClr), 255 - GetRValue(txtClr)));
			}
			else
			{
				COLORREF txtClr = btns[curFocus].color;
				FillRect(draw->hDC, &draw->rcItem, hNormalBrush);
				SetTextColor(draw->hDC, RGB(255 - GetRValue(txtClr), 255 - GetGValue(txtClr), 255 - GetRValue(txtClr)));
			}

			SetBkMode(draw->hDC, TRANSPARENT);
			GetDlgItemText(hWnd, curFocus, (LPWSTR)&btnText, 255);

			DrawText(draw->hDC, (LPCWSTR)&btnText, wcslen(btnText), &draw->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			ReleaseDC(draw->hwndItem, draw->hDC);
			DeleteObject(hNormalBrush);
			DeleteObject(hFocusBrush);
		}
		if (isload && prevFocus != NaI)
		{
			redrawNum++;
		}
	}
	else
	{
		if (draw->CtlType == ODT_BUTTON)
		{
			hNormalBrush = CreateSolidBrush(btns[prevFocus].color);
			hFocusBrush = CreateSolidBrush(btns[prevFocus].highLightColor);

			if (btns[prevFocus].focused)
			{
				COLORREF txtClr = btns[prevFocus].highLightColor;
				FillRect(draw->hDC, &draw->rcItem, hFocusBrush);
				SetTextColor(draw->hDC, RGB(255 - GetRValue(txtClr), 255 - GetGValue(txtClr), 255 - GetRValue(txtClr)));
			}
			else
			{
				COLORREF txtClr = btns[prevFocus].color;
				FillRect(draw->hDC, &draw->rcItem, hNormalBrush);
				SetTextColor(draw->hDC, RGB(255 - GetRValue(txtClr), 255 - GetGValue(txtClr), 255 - GetRValue(txtClr)));
			}

			SetBkMode(draw->hDC, TRANSPARENT);
			GetDlgItemText(hWnd, prevFocus, (LPWSTR)&btnText, 255);

			DrawText(draw->hDC, (LPCWSTR)&btnText, wcslen(btnText), &draw->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			ReleaseDC(draw->hwndItem, draw->hDC);
			DeleteObject(hNormalBrush);
			DeleteObject(hFocusBrush);
		}
		redrawNum = 0;
	}

	return 0;
}

BTNS* SysRelease(BTNS* btns, int btnsNum)
{
	if (btns != NULL)
	{
		HWND hWnd = FindWindow(NULL, L"Task3");
		for (int i = 0; i < btnsNum; i++)
		{
			DestroyWindow(btns[i].hBtnWnd);
		}
		free(btns);
		InvalidateRect(hWnd, NULL, TRUE);
		UpdateWindow(hWnd);
	}

	return NULL;
}