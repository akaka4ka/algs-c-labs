#include "menu.h"

BTNS btns = NULL;

HANDLE GetBtns()
{
	return btns;
}

BTNS MainMenuInit(RECT rct)
{
	btns = (BTNS)malloc((MAIN_MENU_BTNS + DIFF_BTNS) * sizeof(BTN));
	if (btns == NULL)
	{
		return NULL;
	}

	btns[0].text = (wchar_t*)calloc(sizeof(L"Start Game"), sizeof(wchar_t));
	btns[1].text = (wchar_t*)calloc(sizeof(L"Difficulty"), sizeof(wchar_t));
	btns[2].text = (wchar_t*)calloc(sizeof(L"Quit"), sizeof(wchar_t));
	btns[3].text = (wchar_t*)calloc(sizeof(L"Easy"), sizeof(wchar_t));
	btns[4].text = (wchar_t*)calloc(sizeof(L"Medium"), sizeof(wchar_t));
	btns[5].text = (wchar_t*)calloc(sizeof(L"Hard"), sizeof(wchar_t));
	if (!btns[0].text || !btns[1].text || !btns[2].text)
	{
		return NULL;
	}

	MainMenuTextFill(btns);

	for (size_t i = 0; i < MAIN_MENU_BTNS + DIFF_BTNS; i++)
	{
		btns[i].isFocused = false;
		btns[i].pos.x = (rct.right - BTN_WIDTH) / 2;
		btns[i].pos.y = ((rct.bottom) / 2.5) + (i % 3) * (BTN_HEIGHT + BTNS_MARGIN);
		btns[i].rct.left = btns[i].pos.x;
		btns[i].rct.top = btns[i].pos.y;
		btns[i].rct.right = btns[i].pos.x + BTN_WIDTH;
		btns[i].rct.bottom = btns[i].pos.y + BTN_HEIGHT;
	}

	return btns;
}

void MainMenuTextFill(BTNS btns)
{
	for (size_t i = 0; i < MAIN_MENU_BTNS + DIFF_BTNS; i++)
	{
		switch (i)
		{
		case START:
		{
			wchar_t text[] = L"Start Game";
			for (size_t j = 0; j < strlen("Start Game"); j++)
			{
				btns[i].text[j] = text[j];
			}
		} break;

		case DIFFICULTY:
		{
			wchar_t text[] = L"Difficulty";
			for (size_t j = 0; j < strlen("Difficulty"); j++)
			{
				btns[i].text[j] = text[j];
			}
		} break;

		case QUIT:
		{
			wchar_t text[] = L"Quit";
			for (size_t j = 0; j < strlen("Quit"); j++)
			{
				btns[i].text[j] = text[j];
			}
		} break;

		case Easy:
		{
			wchar_t text[] = L"Easy";
			for (size_t j = 0; j < strlen("Easy"); j++)
			{
				btns[i].text[j] = text[j];
			}
		} break;

		case Medium:
		{
			wchar_t text[] = L"Medium";
			for (size_t j = 0; j < strlen("Medium"); j++)
			{
				btns[i].text[j] = text[j];
			}
		} break;

		case Hard:
		{
			wchar_t text[] = L"Hard";
			for (size_t j = 0; j < strlen("Hard"); j++)
			{
				btns[i].text[j] = text[j];
			}
		} break;
		}
	}
}
