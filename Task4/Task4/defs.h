#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include "framework.h"
#include "resource.h"
#include "varsmanaging.h"
#include <math.h>
#include <stdbool.h>
#include <float.h>

#pragma region macros

	#define SetYSpeedByVector(a, b) sqrt(pow((double)a, 2.0) - pow((double)b, 2.0))
	#define GetDiffFromMenu()	curMenuFocus - MAIN_MENU_BTNS

#pragma endregion

#pragma region defines

	#pragma region Draw

		#define BRUSHES_NUM			9
		#define BG_BRUSH			0
		#define STAR_BRUSH			1
		#define NORMAL_BTN_BRUSH	2
		#define FOCUS_BTN_BRUSH		3
		#define PLATE_BRUSH			4
		#define	BALL_BRUSH			5
		#define HP_UP_BRUSH			6
		#define CLIP_BRUSH			7
		#define LINE_BRUSH			8

	#pragma endregion

	#define GET_VRDC				0
	#define	GET_DC					1

	#define	GET						0
	#define SET						1

	#define GET_BALL				0
	#define GET_PLATE				1
	#define GET_CONT				2
	#define GET_RECT				3
	#define GET_BRICKS				4
	#define GET_BRICKS_NUM			5
	#define GET_SCORE				6

	#define SET_RCT					0
	#define SET_DIFF				1

	#define NaI						-1
	#define BTN_STYLE				WS_CHILD | WS_VISIBLE | BS_OWNERDRAW
	#define BTN_WIDTH				120
	#define BTN_HEIGHT				30
	#define BTNS_MARGIN				20
	#define MAIN_MENU_BTNS			3
	#define DIFF_BTNS				3

	#define CONT_MARGIN				46
	#define BRICKS_IN_ROW			12	
	#define BRICK_WIDTH				64
	#define BRICK_HEIGHT			20

	#define HEART_SIZE				64

	#define CONTAINER_MAX_WIDTH		860
	#define CONTAINER_MARGIN		5

	#define DEF_PLATE_SPEED			4
	#define DEF_LIVES				3
	#define DEF_WIDTH				120
	#define DEF_HEIGHT				15

	#define DEF_BALL_MAX_SPEED		3
	#define DEF_BALL_MIN_SPEED		1.5
	#define DEF_BALL_RADIUS			10

	#define ILVLBRICKS				36
	#define IILVLBRICKS				60
	#define IIILVLBRICKS			84

	#define IBRICKTYPE				1
	#define IIBRICKTYPE				2

	#define	SCORE					100
	#define HIT_SCORE				50

	#define MAX_RENDER_BONUSES		5
	#define	HP_UP_COLOR				RGB(255, 0, 0)
	#define CLIP_COLOR				RGB(0, 255, 0)
	#define BONUS_RADIUS			6

	#define EASY					0
	#define MEDIUM					1
	#define	HARD					2

	#define MOVE_LEFT				-1
	#define MOVE_RIGHT				1

	enum Lvls
	{
		ILVL = 1,
		IILVL,
		IIILVL
	};

	enum Scenes
	{
		MAIN_MENU,
		DIFFICULTY_CHOICE,
		GAME,
		IN_GAME_MENU
	};

	enum MainMenuBtns
	{
		START,
		DIFFICULTY,
		QUIT,
		Easy,
		Medium,
		Hard
	};

	enum Bonuses
	{
		HealthUp = 1,
		Clip
	};

#pragma endregion

#pragma region types

	typedef struct
	{
		double x;
		double y;
	} POS;

	typedef struct
	{
		double x;
		double y;
	} SPEED;

	typedef struct
	{
		wchar_t* text;
		POS pos;
		RECT rct;
		bool isFocused;
	} BTN, *BTNS;

	typedef struct
	{
		short type;
		short bonus;
		short lives;
		bool isDestroyed;
		POS pos;
		SIZE size;
		RECT rct;
	} BRICK, *BRICKS;

	typedef struct
	{
		bool cliped;
		short bonus;
		short lives;
		SPEED speed;
		POS pos;
		SIZE size;
		RECT rct;
	} PLATFORM, *PPLATFORM;

	typedef struct
	{
		double radius;
		SPEED speed;
		POS pos;
	} BALL, *PBALL;

	typedef struct
	{
		short type;
		short grav;
		POS pos;
		COLORREF clr;
	} BONUS, *BONUSES;

#pragma endregion
