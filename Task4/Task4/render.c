#include "render.h"

size_t		bricksNumber = 0;

HWND		hWnd;
HDC			hDC;
HDC			memDC;
HDC			virtualDC;
HBRUSH*		hBrushes = NULL;
HPEN		hEdgePen;
HFONT		hMenuTextFont;
HBITMAP*	hBricksBM;
HBITMAP*	hLivesBM;
HBITMAP		memBM;

RECT		rct;
RECT		cont;

PPLATFORM	plate = NULL;
PBALL		mainBall = NULL;
BRICKS		bricks;
BONUSES		bonuses;

HDC GetDrawDC(int varDC)
{
	switch (varDC)
	{
	case GET_VRDC:
	{
		return memDC;
	} break;

	case GET_DC:
	{
		return hDC;
	} break;
	}
	
	PostQuitMessage(0);
	return;
}

void DrawInit()
{
	hWnd = FindWindow(NULL, L"Task4");
	hDC = GetWindowDC(hWnd);
	memDC = CreateCompatibleDC(hDC);
	memBM = CreateCompatibleBitmap(hDC, rct.right, rct.bottom);
	SelectObject(memDC, memBM);

	hMenuTextFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, 0, 0, DEFAULT_CHARSET, OUT_DEVICE_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH, L"Lucida Console");

	hEdgePen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));

	hBrushes = (HBRUSH*)calloc(BRUSHES_NUM, sizeof(HBRUSH));
	if (hBrushes == NULL)
	{
		PostQuitMessage(0);
		return;
	}

	hBrushes[BG_BRUSH] = CreateSolidBrush(RGB(0, 0, 0));
	hBrushes[STAR_BRUSH] = CreateSolidBrush(RGB(255, 255, 255));
	hBrushes[NORMAL_BTN_BRUSH] = CreateSolidBrush(RGB(0, 0, 0));
	hBrushes[FOCUS_BTN_BRUSH] = CreateSolidBrush(RGB(255, 255, 255));
	hBrushes[PLATE_BRUSH] = CreateSolidBrush(RGB(255, 255, 255));
	hBrushes[BALL_BRUSH] = CreateSolidBrush(RGB(0, 0, 255));
	hBrushes[HP_UP_BRUSH] = CreateSolidBrush(HP_UP_COLOR);
	hBrushes[CLIP_BRUSH] = CreateSolidBrush(CLIP_COLOR);
	hBrushes[LINE_BRUSH] = CreateSolidBrush(RGB(255, 0, 0));

	hBricksBM = (HBITMAP*)calloc(3, sizeof(HBITMAP));
	if (hBricksBM == NULL)
	{
		PostQuitMessage(0);
		return;
	}

	for (size_t i = 0; i < 3; i++)
	{
		hBricksBM[i] = CreateCompatibleBitmap(memDC, BRICK_WIDTH, BRICK_HEIGHT);
	}
	hBricksBM[0] = (HBITMAP)LoadImage(NULL, L"./textures/ArkEzPlat.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hBricksBM[1] = (HBITMAP)LoadImage(NULL, L"./textures/ArkMedPlatHalf.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hBricksBM[2] = (HBITMAP)LoadImage(NULL, L"./textures/ArkMedPlatFull.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	hLivesBM = (HBITMAP*)calloc(2, sizeof(HBITMAP));
	if (hLivesBM == NULL)
	{
		PostQuitMessage(0);
		return;
	}

	for (size_t i = 0; i < 2; i++)
	{
		hLivesBM[i] = CreateCompatibleBitmap(memDC, HEART_SIZE, HEART_SIZE);
	}
	hLivesBM[0] = (HBITMAP)LoadImage(NULL, L"./textures/EmptyHeart.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hLivesBM[1] = (HBITMAP)LoadImage(NULL, L"./textures/Heart.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	virtualDC = CreateCompatibleDC(memDC);
}

void DrawRelease()
{
	for (size_t brush = 0; brush < BRUSHES_NUM; brush++)
	{
		DeleteObject(hBrushes[brush]);
	}
	DeleteObject(hEdgePen);
	DeleteObject(hMenuTextFont);

	for (size_t i = 0; i < 3; i++)
	{
		DeleteObject(hBricksBM[i]);
	}
	for (size_t i = 0; i < 2; i++)
	{
		DeleteObject(hLivesBM[i]);
	}

	DeleteDC(virtualDC);

	DeleteObject(memBM);

	free(hBrushes);
	free(hBricksBM);
	free(hLivesBM);
}

void GameShow(int direct, bool* isGameStarted)
{
	#pragma region VarsCheck

	if (!rct.right || !rct.bottom)
	{
		rct = *((RECT*)GetGameVars(GET_RECT));
	}

	if (!cont.left || !cont.right || !cont.bottom)
	{
		cont = *((RECT*)GetGameVars(GET_CONT));
	}

	if (plate == NULL)
	{
		plate = GetGameVars(GET_PLATE);
	}

	if (mainBall == NULL)
	{
		mainBall = GetGameVars(GET_BALL);
	}

	if (bricks == NULL || !(*isGameStarted))
	{
		bricks = GetGameVars(GET_BRICKS);
	}

	if (bricksNumber == 0 || !(*isGameStarted))
	{
		bricksNumber = *((size_t*)GetGameVars(GET_BRICKS_NUM));
	}

	#pragma endregion

	NewLevel(isGameStarted);
	InGameBgRender(TRUE);
	ContainerDraw();
	PlateRender(direct);
	BallRender(isGameStarted);
	BricksRender();
	LivesRender();
	BonusesRender();
	ScoreRender();
}

void MainMenuBgRender(bool reRand)
{
	POS starPos;

	FillRect(memDC, &rct, hBrushes[BG_BRUSH]);

	if (reRand)
	{
		srand(time(NULL));
	}
	SelectObject(memDC, hBrushes[STAR_BRUSH]);
	for (size_t i = 0; i < 100; i++)
	{
		starPos.x = rand() % rct.right;
		starPos.y = rand() % rct.bottom;
		Ellipse(memDC, starPos.x - 2, starPos.y - 2, starPos.x + 2, starPos.y + 2);
	}
}

void MainMenuRender(size_t btn, bool isLoad)
{
	BTNS btns = (BTNS)GetBtns();

	if (btns[btn].isFocused)
	{
		COLORREF txtClr = RGB(255, 255, 255);
		FillRect(memDC, &btns[btn].rct, hBrushes[FOCUS_BTN_BRUSH]);
		SetTextColor(memDC, RGB(255 - GetRValue(txtClr), 255 - GetGValue(txtClr), 255 - GetRValue(txtClr)));
	}
	else
	{
		COLORREF txtClr = RGB(0, 0, 0);
		FillRect(memDC, &btns[btn].rct, hBrushes[NORMAL_BTN_BRUSH]);
		SetTextColor(memDC, RGB(255 - GetRValue(txtClr), 255 - GetGValue(txtClr), 255 - GetRValue(txtClr)));
	}

	SetBkMode(memDC, TRANSPARENT);

	SelectObject(memDC, hMenuTextFont);
	DrawText(memDC, (LPCWSTR)btns[btn].text, wcslen(btns[btn].text), &btns[btn].rct, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

BTNS PrepareMainMenuToShow(short* scene, BTN* btns)
{
	if (!(*scene))
	{
		*scene = MAIN_MENU;
		btns = MainMenuInit(rct);
	}

	return btns;
}

void MainMenuShow(short scene, short* curMenuFocus, bool* isMenuLoad)
{
	MainMenuBgRender(TRUE);

	switch (scene)
	{
	case MAIN_MENU:
	{
		for (size_t i = 0; i < MAIN_MENU_BTNS; i++)
		{
			MainMenuRender(i, isMenuLoad);
		}
	} break;

	case DIFFICULTY_CHOICE:
	{
		for (size_t i = 3; i < MAIN_MENU_BTNS + DIFF_BTNS; i++)
		{
			MainMenuRender(i, isMenuLoad);
		}
	} break;
	}



	if (isMenuLoad != NULL)
	{
		if (!(*isMenuLoad))
		{
			*curMenuFocus = NaI;
		}

		*isMenuLoad = TRUE;
	}
}

void MainMenuSelect(short scene, short* curFocus, short* prevFocus, int VK)
{
	BTNS btns = (BTNS)GetBtns();

	#pragma region FocusSelection

	if (*curFocus == NaI)
	{
		*prevFocus = NaI;
		switch (VK)
		{
		case VK_UP:
		{
			if (scene == MAIN_MENU)
			{
				*curFocus = MAIN_MENU_BTNS - 1;
			}
			else
			{
				*curFocus = MAIN_MENU_BTNS + DIFF_BTNS - 1;
			}
		} break;

		case VK_DOWN:
		{
			if (scene == MAIN_MENU)
			{
				*curFocus = 0;
			}
			else
			{
				*curFocus = 3;
			}
		}
		}
		btns[*curFocus].isFocused = TRUE;
		return;
	}

	if (VK == VK_UP)
	{
		switch (scene)
		{
		case MAIN_MENU:
		{
			if (*curFocus == 0)
			{
				*prevFocus = *curFocus;
				*curFocus = MAIN_MENU_BTNS - 1;
			}
			else
			{
				*prevFocus = *curFocus;
				(*curFocus)--;
			}
		} break;

		case DIFFICULTY_CHOICE:
		{
			if (*curFocus == 3)
			{
				*prevFocus = *curFocus;
				*curFocus = MAIN_MENU_BTNS + DIFF_BTNS - 1;
			}
			else
			{
				*prevFocus = *curFocus;
				(*curFocus)--;
			}
		} break;
		}
		
	}

	if (VK == VK_DOWN)
	{
		switch (scene)
		{
		case MAIN_MENU:
		{
			if (*curFocus == MAIN_MENU_BTNS - 1)
			{
				*prevFocus = *curFocus;
				*curFocus = 0;
			}
			else
			{
				*prevFocus = *curFocus;
				(*curFocus)++;
			}
		} break;

		case DIFFICULTY_CHOICE:
		{
			if (*curFocus == MAIN_MENU_BTNS + DIFF_BTNS - 1)
			{
				*prevFocus = *curFocus;
				*curFocus = 3;
			}
			else
			{
				*prevFocus = *curFocus;
				(*curFocus)++;
			}
		} break;
		}
		
	}

	#pragma endregion

	btns[*curFocus].isFocused = TRUE;
	btns[*prevFocus].isFocused = FALSE;
}

void ContainerDraw()
{
	SelectObject(memDC, hEdgePen);
	MoveToEx(memDC, cont.left, cont.top, NULL);
	LineTo(memDC, cont.left, cont.bottom);

	MoveToEx(memDC, cont.right, cont.top, NULL);
	LineTo(memDC, cont.right, cont.bottom);
}

void BricksRender()
{
	for (size_t i = 0; i < bricksNumber; i++)
	{
		if (!bricks[i].isDestroyed)
		{
			switch (bricks[i].type)
			{
			case IBRICKTYPE:
			{
				SelectObject(virtualDC, hBricksBM[0]);
			} break;

			case IIBRICKTYPE:
			{
				switch (bricks[i].lives)
				{
				case 1:
				{
					SelectObject(virtualDC, hBricksBM[1]);
				} break;

				case 2:
				{
					SelectObject(virtualDC, hBricksBM[2]);
				} break;
				}
			} break;
			}

			BitBlt(memDC, bricks[i].rct.left, bricks[i].rct.top, BRICK_WIDTH, BRICK_HEIGHT, virtualDC, 0, 0, SRCCOPY);
		}
	}
}

void PlateRender(int direct)
{
	if (!InContainerCheck(direct))
	{
		plate->speed.x = 0;
	}

		plate->rct.left += plate->speed.x;
		plate->rct.right += plate->speed.x;

	FillRect(memDC, &(plate->rct), hBrushes[PLATE_BRUSH]);
}

void BallRender(bool* isGameStarted)
{		
		if (!(*isGameStarted) || plate->cliped)
		{
			mainBall->pos.x += plate->speed.x;
		}
		else
		{
			ContainerReflect();
			BrickReflect();
			PlateReflect();
			TopReflect();
			BallFall(isGameStarted);

			mainBall->pos.x += mainBall->speed.x;
			mainBall->pos.y += mainBall->speed.y;
		}
		
		
	SelectObject(memDC, hBrushes[BALL_BRUSH]);
	Ellipse(memDC, mainBall->pos.x - mainBall->radius, mainBall->pos.y - mainBall->radius,
			mainBall->pos.x + mainBall->radius, mainBall->pos.y + mainBall->radius);

			/*SelectObject(memDC, hBrushes[LINE_BRUSH]);
			MoveToEx(memDC, mainBall->pos.x, mainBall->pos.y, NULL);
			LineTo(memDC, mainBall->pos.x + mainBall->speed.x * 1000, mainBall->pos.y + mainBall->speed.y * 1000);*/
}

void InGameBgRender(bool reRand)
{	
	FillRect(memDC, &rct, hBrushes[BG_BRUSH]);

	POS starPos;

	SelectObject(memDC, hBrushes[STAR_BRUSH]);

	if (reRand)
	{
		srand(time(NULL));
	}
	for (size_t i = 0; i < 100; i++)
	{
		if (i % 2)
		{
			starPos.x = rand() % cont.left;
			starPos.y = rand() % cont.bottom;
		}
		else
		{
			starPos.x = cont.right + rand() % (rct.right - cont.right);
			starPos.y = rand() % cont.bottom;
		}

		Ellipse(memDC, starPos.x - 1, starPos.y - 1, starPos.x + 1, starPos.y + 1);
	}
}

void LivesRender()
{
	size_t i = 0;
	size_t j = 0;

	for (i = 0; i < plate->lives / 2; i++)
	{
		SelectObject(virtualDC, hLivesBM[1]);
		BitBlt(memDC, 20 + i * 64 + i * 10, 20, HEART_SIZE, HEART_SIZE, virtualDC, 0, 0, SRCCOPY);
	}

	for (j = 0; j < plate->lives % 2; j++)
	{
		SelectObject(virtualDC, hLivesBM[0]);
		BitBlt(memDC, 20 + i * 64 + i * 10, 20, HEART_SIZE, HEART_SIZE, virtualDC, 0, 0, SRCCOPY);
	}
}

void BonusesRender()
{
	for (size_t i = 0; i < MAX_RENDER_BONUSES; i++)
	{
		if (bonuses[i].type != 0)
		{
			if (BonusFall(i))
			{
				continue;
			}
			if (BonusPickUp(i))
			{
				continue;
			}
			switch (bonuses[i].type)
			{
			case HealthUp:
			{
				SelectObject(memDC, hBrushes[HP_UP_BRUSH]);
			} break;

			case Clip:
			{
				SelectObject(memDC, hBrushes[CLIP_BRUSH]);
			} break;
			}

			Ellipse(memDC, bonuses[i].pos.x - BONUS_RADIUS, bonuses[i].pos.y - BONUS_RADIUS,
					bonuses[i].pos.x + BONUS_RADIUS, bonuses[i].pos.y + BONUS_RADIUS);

			bonuses[i].pos.y += bonuses[i].grav;
		}
	}
}

void ScoreRender()
{
	int score = *((int*)GetGameVars(GET_SCORE));
	if (score > 0)
	{
		score = score;
	}
	WCHAR text[50] = { 0 };
	RECT scoreRct;
	scoreRct.left = cont.right + 20;
	scoreRct.top = 20;
	scoreRct.right = scoreRct.left + 120;
	scoreRct.bottom = 40;

	_stprintf(text, L"Score: %d", score);

	SetBkMode(memDC, TRANSPARENT);

	SelectObject(memDC, hMenuTextFont);
	DrawText(memDC, (LPCWSTR)text, wcslen(text), &scoreRct, DT_SINGLELINE);
}
