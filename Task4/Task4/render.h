#pragma once

#include "defs.h"
#include "menu.h"
#include "core.h"

#include <time.h>

HDC		GetDrawDC(int varDC);
void	DrawInit();
void	DrawRelease();
void	GameShow(int direct, bool* isGameStarted);
void	MainMenuBgRender(bool reRand);
void	MainMenuRender(size_t btn, bool isLoad);
BTNS	PrepareMainMenuToShow(short* scene, BTN* btns);
void	MainMenuShow(short scene, short* curMenuFocus, bool* isMenuLoad);
void	MainMenuSelect(short scene, short* curFocus, short* prevFocus, int VK);
void	ContainerDraw();
void	BricksRender();
void	PlateRender(int direct);
void	BallRender(bool* isGameStarted);
void	InGameBgRender(bool reRand);
void	LivesRender();
void	BonusesRender();
void	ScoreRender();
