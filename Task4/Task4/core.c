#include "core.h"

short		difficulty = 0;
int			level = 1;
int			score = 0;
size_t		bricksNum;
size_t		destroyedBricksNum = 0;

RECT		cont;
RECT		rct;
PPLATFORM	plate;
PBALL		mainBall;
BRICKS		bricks = NULL;
BONUSES		bonuses = NULL;

int LevelManage(int mode, int val)
{
	switch (mode)
	{
	case GET:
	{
		return level;
	} break;

	case SET:
	{
		level = val;
	} break;
	}

	return NaI;
}

HANDLE GetGameVars(int var)
{
	switch (var)
	{
	case GET_BALL:
	{
		return mainBall;
	} break;

	case GET_PLATE:
	{
		return plate;
	} break;

	case GET_CONT:
	{
		return &cont;
	} break;

	case GET_RECT:
	{
		return &rct;
	} break;

	case GET_BRICKS:
	{
		return bricks;
	} break;

	case GET_BRICKS_NUM:
	{
		return &bricksNum;
	} break;

	case GET_SCORE:
	{
		return &score;
	} break;
	}
}

void SetGameVars(int var, HANDLE val)
{
	switch (var)
	{
	case SET_RCT:
	{
		rct = *((RECT*)val);
	} break;

	case SET_DIFF:
	{
		difficulty = *((short*)val);
	} break;
	}
}

BRICKS GameCore()
{	
	BRICKS _memCheck = NULL;

	if (plate == NULL)
	{
		plate = (PPLATFORM)calloc(1, sizeof(PLATFORM));
	}
	if (mainBall == NULL)
	{
		mainBall = (PBALL)calloc(1, sizeof(BALL));
	}
	if (bonuses == NULL)
	{
		bonuses = (BONUSES)calloc(MAX_RENDER_BONUSES, sizeof(BONUS));
	}

	if ((plate == NULL) || (mainBall == NULL))
	{
		PostQuitMessage(0);
		return NULL;
	}

	if (bonuses == NULL)
	{
		PostQuitMessage(0);
		return NULL;
	}

	#pragma region Container_Plate_Ball_Init

		cont.left = (rct.right - CONTAINER_MAX_WIDTH) / 2;
		cont.top = rct.top;
		cont.right = (rct.right + CONTAINER_MAX_WIDTH) / 2;
		cont.bottom = rct.bottom;

		plate->cliped = false;
		plate->bonus = 0;
		plate->lives = DEF_LIVES - difficulty;
		plate->speed.x = 0;
		plate->speed.y = 0;
		plate->pos.x = (rct.right - DEF_WIDTH) / 2;
		plate->pos.y = rct.bottom - DEF_HEIGHT - BTNS_MARGIN;
		plate->size.cx = DEF_WIDTH;
		plate->size.cy = DEF_HEIGHT;
		plate->rct.left = (rct.right - DEF_WIDTH) / 2;
		plate->rct.top = rct.bottom - DEF_HEIGHT - BTNS_MARGIN;
		plate->rct.right = (rct.right + DEF_WIDTH) / 2;
		plate->rct.bottom = rct.bottom - BTNS_MARGIN;

		mainBall->radius = DEF_BALL_RADIUS;
		mainBall->speed.x = 0;
		mainBall->speed.y = 0;
		mainBall->pos.x = rct.right / 2;
		mainBall->pos.y = rct.bottom - DEF_HEIGHT - BTNS_MARGIN - DEF_BALL_RADIUS;

	#pragma endregion

	switch (level)
	{
	case ILVL:
	{
		bricksNum = ILVLBRICKS;
	} break;

	case IILVL:
	{
		bricksNum = IILVLBRICKS;
	} break;

	case IIILVL:
	{
		bricksNum = IIILVLBRICKS;
	} break;
	}

	_memCheck = (BRICKS)realloc(bricks, bricksNum * sizeof(BRICK));
	if (_memCheck == NULL)
	{
		PostQuitMessage(0);
		return;
	}
	else
	{
		bricks = _memCheck;
	}

	for (size_t i = 0, lines = 0; i < bricksNum; i++)
	{
		if (!(i % BRICKS_IN_ROW))
		{
			lines++;
		}
		switch (difficulty)
		{
		case EASY:
		{
			if (!(rand() % 5))
			{
				bricks[i].bonus = 1 + rand() % 2;
			}
			else
			{
				bricks[i].bonus = 0;
			}

			if (!(rand() % 4))
			{
				bricks[i].type = 2;
			}
			else
			{
				bricks[i].type = 1;
			}
		} break;

		case MEDIUM:
		{
			if (!(rand() % 7))
			{
				bricks[i].bonus = 1 + rand() % 2;
			}
			else
			{
				bricks[i].bonus = 0;
			}

			if (!(rand() % 2))
			{
				bricks[i].type = 2;
			}
			else
			{
				bricks[i].type = 1;
			}
		} break;

		case HARD:
		{
			if (!(rand() % 13))
			{
				bricks[i].bonus = 1 + rand() % 2;
			}
			else
			{
				bricks[i].bonus = 0;
			}

			bricks[i].type = 2;
		} break;
		}
		bricks[i].lives = bricks[i].type;
		bricks[i].isDestroyed = false;
		bricks[i].pos.x = CONT_MARGIN + cont.left + BRICK_WIDTH * (i % BRICKS_IN_ROW);
		bricks[i].pos.y = cont.top + BTNS_MARGIN * (lines + 1);
		bricks[i].size.cx = BRICK_WIDTH;
		bricks[i].size.cy = BRICK_HEIGHT;
		bricks[i].rct.left = bricks[i].pos.x;
		bricks[i].rct.right = bricks[i].pos.x + BRICK_WIDTH;
		bricks[i].rct.top = bricks[i].pos.y;
		bricks[i].rct.bottom = bricks[i].pos.y + BRICK_HEIGHT;
	}
	
	return bricks;
}

bool InContainerCheck(int direction)
{
	switch (direction)
	{
	case MOVE_LEFT:
	{
		if (plate->rct.left >= (cont.left + direction * plate->speed.x))
		{
			return true;
		}

		return false;
	} break;

	case MOVE_RIGHT:
	{
		if (plate->rct.right <= (cont.right - plate->speed.x))
		{
			return true;
		}

		return false;
	} break;
	}
}

void ContainerReflect()
{
	if ((mainBall->pos.x + mainBall->radius) >= (cont.right - mainBall->speed.x / 2))
	{
		mainBall->speed.x = -fabs(mainBall->speed.x);
	}

	if ((mainBall->pos.x - mainBall->radius) <= (cont.left + mainBall->speed.x / 2))
	{
		mainBall->speed.x = fabs(mainBall->speed.x);
	}
}

void BrickReflect()
{
	double yEdge = 0.0;
	double x = 0.0;
	double y = 0.0;
	double k = 0.0;
	double b = 0.0;

	for (size_t i = 0; i < bricksNum; i++)
	{
		if (!bricks[i].isDestroyed)
		{
			#pragma region FromBot
			
			if ((mainBall->speed.y < 0) && (fabs(mainBall->pos.y - bricks[i].rct.bottom) < 0.5 * fabs(mainBall->speed.y)))
			{
				if (mainBall->speed.x != 0)
				{
					k = mainBall->speed.y / mainBall->speed.x;
				}
				else
				{
					k = 100.0;
				}
				b = mainBall->pos.y - k * mainBall->pos.x;
				y = bricks[i].rct.bottom;

				x = (y - b) / k;

				if (x > bricks[i].rct.left && x < bricks[i].rct.right)
				{
					mainBall->speed.y = SetYSpeedByVector(DEF_BALL_MAX_SPEED, mainBall->speed.x);
					bricks[i].lives--;

					if (bricks[i].lives == 0)
					{
						bricks[i].isDestroyed = true;
						score = score + bricks[i].type * SCORE;
						destroyedBricksNum++;
						BonusAdd(i);
					}
					else
					{
						score = score + HIT_SCORE;
					}

					return;
				}

				#pragma region FromCorners

				if ((mainBall->pos.x - mainBall->radius <= bricks[i].rct.right) && (mainBall->pos.x >= bricks[i].rct.right))
				{
					double kReflect;
					if ((mainBall->pos.x - bricks[i].rct.right) != 0)
					{
						kReflect = (bricks[i].rct.bottom - mainBall->pos.y) / (mainBall->pos.x - bricks[i].rct.right);
					}
					else
					{
						kReflect = -1;
					}
					yEdge = mainBall->radius * sqrt(1 + k * k);
					x = bricks[i].rct.right;
					y = k * x + b;

					if (k == 100.0)
					{
						if ((bricks[i].rct.right < bricks[i + 1].rct.right) && !bricks[i + 1].isDestroyed)
						{
							mainBall->speed.y = -mainBall->speed.y;
						}
						else
						{
							mainBall->speed.x = DEF_BALL_MAX_SPEED / sqrt(1 + kReflect * kReflect);
							mainBall->speed.y = SetYSpeedByVector(DEF_BALL_MAX_SPEED, mainBall->speed.x);
						}

						bricks[i].lives--;
						if (bricks[i].lives == 0)
						{
							bricks[i].isDestroyed = true;
							score = score + bricks[i].type * SCORE;
							destroyedBricksNum++;
							BonusAdd(i);
						}
						else
						{
							score = score + HIT_SCORE;
						}

						return;
					}

					if (y <= bricks[i].rct.top + yEdge)
					{
						if ((bricks[i].rct.right < bricks[i + 1].rct.right) && !bricks[i + 1].isDestroyed)
						{
							mainBall->speed.y = mainBall->speed.y;

							bricks[i].lives--;
							if (bricks[i].lives == 0)
							{
								bricks[i].isDestroyed = true;
								score = score + bricks[i].type * SCORE;
								destroyedBricksNum++;
								BonusAdd(i);
							}
							else
							{
								score = score + HIT_SCORE;
							}

							return;
						}
						else
						{
							if (mainBall->speed.x < 0)
							{
								mainBall->speed.x = -mainBall->speed.x * 1 / sqrt(1 + pow(((k - kReflect) / (1 + k * kReflect)), 2.0));
							}
							else
							{
								mainBall->speed.x = mainBall->speed.x * 1 / sqrt(1 + pow(((k - kReflect) / (1 + k * kReflect)), 2.0));
							}

							if (fabs(mainBall->speed.x) >= DEF_BALL_MAX_SPEED)
							{
								mainBall->speed.x = mainBall->speed.x / fabs(mainBall->speed.x) * (DEF_BALL_MAX_SPEED - DEF_BALL_MIN_SPEED);
							}
							mainBall->speed.y = SetYSpeedByVector(DEF_BALL_MAX_SPEED, mainBall->speed.x);

							bricks[i].lives--;
							if (bricks[i].lives == 0)
							{
								bricks[i].isDestroyed = true;
								score = score + bricks[i].type * SCORE;
								destroyedBricksNum++;
								BonusAdd(i);
							}
							else
							{
								score = score + HIT_SCORE;
							}

							return;
						}
					}
					else
					{
						if (mainBall->pos.x - mainBall->radius / 2 <= bricks[i].rct.right)
						{
							mainBall->speed.x = -mainBall->speed.x;
							if (fabs(mainBall->speed.x) >= DEF_BALL_MAX_SPEED)
							{
								mainBall->speed.x = mainBall->speed.x / fabs(mainBall->speed.x) * (DEF_BALL_MAX_SPEED - DEF_BALL_MIN_SPEED);
							}
							mainBall->speed.y = SetYSpeedByVector(DEF_BALL_MAX_SPEED, mainBall->speed.x);

							bricks[i].lives--;
							if (bricks[i].lives == 0)
							{
								bricks[i].isDestroyed = true;
								score = score + bricks[i].type * SCORE;
								destroyedBricksNum++;
								BonusAdd(i);
							}
							else
							{
								score = score + HIT_SCORE;
							}

							return;
						}
					}
				}

				if ((mainBall->pos.x <= bricks[i].rct.left) && (mainBall->pos.x + mainBall->radius >= bricks[i].rct.left))
				{
					double kReflect;
					if ((mainBall->pos.x - bricks[i].rct.left) != 0)
					{
						kReflect = (bricks[i].rct.bottom - mainBall->pos.y) / (mainBall->pos.x - bricks[i].rct.right);
					}
					else
					{
						kReflect = -1;
					}
					yEdge = mainBall->radius * sqrt(1 + k * k);
					x = bricks[i].rct.left;
					y = k * x + b;

					if (k == 100.0)
					{
						if ((bricks[i].rct.left < bricks[i - 1].rct.left) && !bricks[i - 1].isDestroyed)
						{
							mainBall->speed.y = -mainBall->speed.y;
						}
						else
						{
							mainBall->speed.x = -DEF_BALL_MAX_SPEED / sqrt(1 + kReflect * kReflect);
							mainBall->speed.y = SetYSpeedByVector(DEF_BALL_MAX_SPEED, mainBall->speed.x);
						}

						bricks[i].lives--;
						if (bricks[i].lives == 0)
						{
							bricks[i].isDestroyed = true;
							score = score + bricks[i].type * SCORE;
							destroyedBricksNum++;
							BonusAdd(i);
						}
						else
						{
							score = score + HIT_SCORE;
						}

						return;
					}

					if (y <= bricks[i].rct.top + yEdge)
					{
						if ((bricks[i].rct.left > bricks[i - 1].rct.left) && !bricks[i - 1].isDestroyed)
						{
							mainBall->speed.y = -mainBall->speed.y;

							bricks[i].lives--;
							if (bricks[i].lives == 0)
							{
								bricks[i].isDestroyed = true;
								score = score + bricks[i].type * SCORE;
								destroyedBricksNum++;
								BonusAdd(i);
							}
							else
							{
								score = score + HIT_SCORE;
							}

							return;
						}
						else
						{
							if (mainBall->speed.x > 0)
							{
								mainBall->speed.x = -mainBall->speed.x * 1 / sqrt(1 + pow(((k - kReflect) / (1 + k * kReflect)), 2.0));
							}
							else
							{
								mainBall->speed.x = mainBall->speed.x * 1 / sqrt(1 + pow(((k - kReflect) / (1 + k * kReflect)), 2.0));
							}

							if (mainBall->speed.x >= DEF_BALL_MAX_SPEED)
							{
								mainBall->speed.x = mainBall->speed.x / fabs(mainBall->speed.x) * (DEF_BALL_MAX_SPEED - DEF_BALL_MIN_SPEED);
							}
							mainBall->speed.y = -SetYSpeedByVector(DEF_BALL_MAX_SPEED, mainBall->speed.x);

							bricks[i].lives--;
							if (bricks[i].lives == 0)
							{
								bricks[i].isDestroyed = true;
								score = score + bricks[i].type * SCORE;
								destroyedBricksNum++;
								BonusAdd(i);
							}
							else
							{
								score = score + HIT_SCORE;
							}

							return;
						}
					}
					else
					{
						if (mainBall->pos.x + mainBall->radius / 2 >= bricks[i].rct.left)
						{
							mainBall->speed.x = -mainBall->speed.x;
							if (fabs(mainBall->speed.x) >= DEF_BALL_MAX_SPEED)
							{
								mainBall->speed.x = mainBall->speed.x / fabs(mainBall->speed.x) * (DEF_BALL_MAX_SPEED - DEF_BALL_MIN_SPEED);
							}
							mainBall->speed.y = SetYSpeedByVector(DEF_BALL_MAX_SPEED, mainBall->speed.x);

							bricks[i].lives--;
							if (bricks[i].lives == 0)
							{
								bricks[i].isDestroyed = true;
								score = score + bricks[i].type * SCORE;
								destroyedBricksNum++;
								BonusAdd(i);
							}
							else
							{
								score = score + HIT_SCORE;
							}

							return;
						}
					}
				}
				#pragma endregion
			}

			#pragma endregion

			#pragma region FromTop

			if ((mainBall->speed.y > 0) && (fabs(bricks[i].rct.top - mainBall->pos.y) < 0.5 * mainBall->speed.y))
			{
				if (mainBall->speed.x != 0)
				{
					k = mainBall->speed.y / mainBall->speed.x;
				}
				else
				{
					k = 100.0;
				}
				b = mainBall->pos.y - k * mainBall->pos.x;
				y = bricks[i].rct.top;

				x = (y - b) / k;

				if (x > bricks[i].rct.left && x < bricks[i].rct.right)
				{
					mainBall->speed.y = -SetYSpeedByVector(DEF_BALL_MAX_SPEED, mainBall->speed.x);
					
					bricks[i].lives--;
					if (bricks[i].lives == 0)
					{
						bricks[i].isDestroyed = true;
						score = score + bricks[i].type * SCORE;
						destroyedBricksNum++;
						BonusAdd(i);
					}
					else
					{
						score = score + HIT_SCORE;
					}

					return;
				}

				#pragma region FromCorners

				if ((mainBall->pos.x - mainBall->radius <= bricks[i].rct.right) && (mainBall->pos.x >= bricks[i].rct.right))
				{
					double kReflect;
					kReflect = (bricks[i].rct.top - mainBall->pos.y) / (mainBall->pos.x - bricks[i].rct.right);
					yEdge = mainBall->radius * sqrt(1 + k * k);
					x = bricks[i].rct.right;
					y = k * x + b;

					if (y <= bricks[i].rct.top + yEdge)
					{
						if ((bricks[i].rct.right < bricks[i + 1].rct.right) && !bricks[i + 1].isDestroyed)
						{
							mainBall->speed.y = -mainBall->speed.y;

							bricks[i].lives--;
							if (bricks[i].lives == 0)
							{
								bricks[i].isDestroyed = true;
								score = score + bricks[i].type * SCORE;
								destroyedBricksNum++;
								BonusAdd(i);
							}
							else
							{
								score = score + HIT_SCORE;
							}

							return;
						}
						else
						{
							if (mainBall->speed.x < 0)
							{
								mainBall->speed.x = -mainBall->speed.x * 1 / sqrt(1 + pow(((k - kReflect) / (1 + k * kReflect)), 2.0));
							}
							else
							{
								mainBall->speed.x = mainBall->speed.x * 1 / sqrt(1 + pow(((k - kReflect) / (1 + k * kReflect)), 2.0));
							}

							if (fabs(mainBall->speed.x) >= DEF_BALL_MAX_SPEED)
							{
								mainBall->speed.x = mainBall->speed.x / fabs(mainBall->speed.x) * (DEF_BALL_MAX_SPEED - DEF_BALL_MIN_SPEED);
							}
							mainBall->speed.y = -SetYSpeedByVector(DEF_BALL_MAX_SPEED, mainBall->speed.x);

							bricks[i].lives--;
							if (bricks[i].lives == 0)
							{
								bricks[i].isDestroyed = true;
								score = score + bricks[i].type * SCORE;
								destroyedBricksNum++;
								BonusAdd(i);
							}
							else
							{
								score = score + HIT_SCORE;
							}

							return;
						}
					}
					else
					{
						if (mainBall->pos.x - mainBall->radius / 2 <= bricks[i].rct.right)
						{
							mainBall->speed.x = -mainBall->speed.x;
							if (fabs(mainBall->speed.x) >= DEF_BALL_MAX_SPEED)
							{
								mainBall->speed.x = mainBall->speed.x / fabs(mainBall->speed.x) * (DEF_BALL_MAX_SPEED - DEF_BALL_MIN_SPEED);
							}
							mainBall->speed.y = SetYSpeedByVector(DEF_BALL_MAX_SPEED, mainBall->speed.x);

							bricks[i].lives--;
							if (bricks[i].lives == 0)
							{
								bricks[i].isDestroyed = true;
								score = score + bricks[i].type * SCORE;
								destroyedBricksNum++;
								BonusAdd(i);
							}
							else
							{
								score = score + HIT_SCORE;
							}

							return;
						}
					}
				}

				if ((mainBall->pos.x <= bricks[i].rct.left) && (mainBall->pos.x + mainBall->radius >= bricks[i].rct.left))
				{
					double kReflect;
					kReflect = (bricks[i].rct.top - mainBall->pos.y) / (bricks[i].rct.left - mainBall->pos.x);
					yEdge = mainBall->radius * sqrt(1 + k * k);
					x = bricks[i].rct.left;
					y = k * x + b;

					if (y <= bricks[i].rct.top + yEdge)
					{
						if ((bricks[i].rct.left > bricks[i - 1].rct.left) && !bricks[i - 1].isDestroyed)
						{
							mainBall->speed.y = -mainBall->speed.y;

							bricks[i].lives--;
							if (bricks[i].lives == 0)
							{
								bricks[i].isDestroyed = true;
								score = score + bricks[i].type * SCORE;
								destroyedBricksNum++;
								BonusAdd(i);
							}
							else
							{
								score = score + HIT_SCORE;
							}

							return;
						}
						else
						{
							if (mainBall->speed.x > 0)
							{
								mainBall->speed.x = -mainBall->speed.x * 1 / sqrt(1 + pow(((k - kReflect) / (1 + k * kReflect)), 2.0));
							}
							else
							{
								mainBall->speed.x = mainBall->speed.x * 1 / sqrt(1 + pow(((k - kReflect) / (1 + k * kReflect)), 2.0));
							}

							if (mainBall->speed.x >= DEF_BALL_MAX_SPEED)
							{
								mainBall->speed.x = mainBall->speed.x / fabs(mainBall->speed.x) * (DEF_BALL_MAX_SPEED - DEF_BALL_MIN_SPEED);
							}
							mainBall->speed.y = -SetYSpeedByVector(DEF_BALL_MAX_SPEED, mainBall->speed.x);

							bricks[i].lives--;
							if (bricks[i].lives == 0)
							{
								bricks[i].isDestroyed = true;
								score = score + bricks[i].type * SCORE;
								destroyedBricksNum++;
								BonusAdd(i);
							}
							else
							{
								score = score + HIT_SCORE;
							}

							return;
						}
					}
					else
					{
						if (mainBall->pos.x + mainBall->radius / 2 >= bricks[i].rct.left)
						{
							mainBall->speed.x = -mainBall->speed.x;
							if (fabs(mainBall->speed.x) >= DEF_BALL_MAX_SPEED)
							{
								mainBall->speed.x = mainBall->speed.x / fabs(mainBall->speed.x) * (DEF_BALL_MAX_SPEED - DEF_BALL_MIN_SPEED);
							}
							mainBall->speed.y = SetYSpeedByVector(DEF_BALL_MAX_SPEED, mainBall->speed.x);

							bricks[i].lives--;
							if (bricks[i].lives == 0)
							{
								bricks[i].isDestroyed = true;
								score = score + bricks[i].type * SCORE;
								destroyedBricksNum++;
								BonusAdd(i);
							}
							else
							{
								score = score + HIT_SCORE;
							}

							return;
						}
					}
				}

				#pragma endregion
			}

			#pragma endregion
		}
	}
}

void PlateReflect()
{
	if ((mainBall->speed.y > 0) && (plate->rct.top - mainBall->pos.y < 0.5 * mainBall->speed.y))
	{
		#pragma region FromTopSurf

		double yEdge = 0.0;
		double x = 0.0;
		double y = 0.0;
		double k = 0.0;
		double b = 0.0;

		if (mainBall->speed.x != 0)
		{
			k = mainBall->speed.y / mainBall->speed.x;
		}
		else
		{
			k = 100.0;
		}
		b = mainBall->pos.y - k * mainBall->pos.x;
		y = plate->rct.top;

		x = (y - b) / k;

		if ((x > plate->rct.left) && (x < plate->rct.right))
		{
			if (plate->bonus == Clip)
			{
				mainBall->speed.x = 0;
				mainBall->speed.y = 0;
				mainBall->pos.y = plate->rct.top - mainBall->radius;
				plate->cliped = true;
			}
			else
			{
				mainBall->speed.x += 0.3 * plate->speed.x;

				if (fabs(mainBall->speed.x) >= DEF_BALL_MAX_SPEED - 0.5)
				{
					mainBall->speed.x = mainBall->speed.x / fabs(mainBall->speed.x) * (DEF_BALL_MAX_SPEED - DEF_BALL_MIN_SPEED);
				}

				mainBall->speed.y = -SetYSpeedByVector(DEF_BALL_MAX_SPEED, mainBall->speed.x);
			}

			return;
		}

		if (k == 100.0 && ((x - mainBall->radius > plate->rct.right) || (x + mainBall->radius > plate->rct.left)))
		{
			return;
		}

		#pragma endregion

		#pragma region FromCorners

		if ((mainBall->pos.x - mainBall->radius <= plate->rct.right) && (mainBall->pos.x >= plate->rct.right))
		{
			double kReflect;
			kReflect = (plate->rct.top - mainBall->pos.y) / (mainBall->pos.x - plate->rct.right);
			yEdge = mainBall->radius * sqrt(1 + k * k);
			x = plate->rct.right;
			y = k * x + b;

			if (y <= plate->rct.top + yEdge)
			{
				if (mainBall->speed.x < 0)
				{
					mainBall->speed.x = -mainBall->speed.x * 1 / sqrt(1 + pow(((k - kReflect) / (1 + k * kReflect)), 2.0)) + 0.3 * plate->speed.x;
				}
				else
				{
					mainBall->speed.x = mainBall->speed.x * 1 / sqrt(1 + pow(((k - kReflect) / (1 + k * kReflect)), 2.0)) + 0.3 * plate->speed.x;
				}

				if (fabs(mainBall->speed.x) >= DEF_BALL_MAX_SPEED)
				{
					mainBall->speed.x = mainBall->speed.x / fabs(mainBall->speed.x) * (DEF_BALL_MAX_SPEED - DEF_BALL_MIN_SPEED);
				}
				mainBall->speed.y = -SetYSpeedByVector(DEF_BALL_MAX_SPEED, mainBall->speed.x);
			}
			else
			{
				if (mainBall->pos.x - mainBall->radius / 2 <= plate->rct.right)
				{
					mainBall->speed.x = -mainBall->speed.x + 0.3 * plate->speed.x;
					if (fabs(mainBall->speed.x) >= DEF_BALL_MAX_SPEED)
					{
						mainBall->speed.x = mainBall->speed.x / fabs(mainBall->speed.x) * (DEF_BALL_MAX_SPEED - DEF_BALL_MIN_SPEED);
					}
					mainBall->speed.y = SetYSpeedByVector(DEF_BALL_MAX_SPEED, mainBall->speed.x);
				}
			}
		}

		if ((mainBall->pos.x <= plate->rct.left) && (mainBall->pos.x + mainBall->radius >= plate->rct.left))
		{
			double kReflect;
			kReflect = (plate->rct.top - mainBall->pos.y) / (plate->rct.left - mainBall->pos.x);
			yEdge = mainBall->radius * sqrt(1 + k * k);
			x = plate->rct.left;
			y = k * x + b;

			if (y <= plate->rct.top + yEdge)
			{
				if (mainBall->speed.x > 0)
				{
					mainBall->speed.x = -mainBall->speed.x * 1 / sqrt(1 + pow(((k - kReflect) / (1 + k * kReflect)), 2.0)) + 0.3 * plate->speed.x;
				}
				else
				{
					mainBall->speed.x = mainBall->speed.x * 1 / sqrt(1 + pow(((k - kReflect) / (1 + k * kReflect)), 2.0)) + 0.3 * plate->speed.x;
				}

				if (mainBall->speed.x >= DEF_BALL_MAX_SPEED)
				{
					mainBall->speed.x = mainBall->speed.x / fabs(mainBall->speed.x) * (DEF_BALL_MAX_SPEED - DEF_BALL_MIN_SPEED);
				}
				mainBall->speed.y = -SetYSpeedByVector(DEF_BALL_MAX_SPEED, mainBall->speed.x);
			}
			else
			{
				if (mainBall->pos.x + mainBall->radius / 2 >= plate->rct.left)
				{
					mainBall->speed.x = -mainBall->speed.x + 0.3 * plate->speed.x;
					if (fabs(mainBall->speed.x) >= DEF_BALL_MAX_SPEED)
					{
						mainBall->speed.x = mainBall->speed.x / fabs(mainBall->speed.x) * (DEF_BALL_MAX_SPEED - DEF_BALL_MIN_SPEED);
					}
					mainBall->speed.y = SetYSpeedByVector(DEF_BALL_MAX_SPEED, mainBall->speed.x);
				}
			}
		}

		#pragma endregion
	}
}

void TopReflect()
{
	if ((mainBall->pos.y - mainBall->radius - cont.top) <= fabs(mainBall->speed.y))
	{
		mainBall->speed.y = fabs(mainBall->speed.y);
	}
}

void BallFall(bool* isGameStarted)
{
	if ((rct.bottom - mainBall->pos.y) <= mainBall->radius)
	{
		plate->lives = plate->lives - 1;
		if (plate->lives == 0)
		{
			PostQuitMessage(0);
			return;
		}
		plate->speed.x = 0;
		plate->speed.y = 0;
		plate->pos.x = (rct.right - DEF_WIDTH) / 2;
		plate->pos.y = rct.bottom - DEF_HEIGHT - BTNS_MARGIN;
		plate->rct.left = (rct.right - DEF_WIDTH) / 2;
		plate->rct.top = rct.bottom - DEF_HEIGHT - BTNS_MARGIN;
		plate->rct.right = (rct.right + DEF_WIDTH) / 2;
		plate->rct.bottom = rct.bottom - BTNS_MARGIN;

		mainBall->speed.x = 0;
		mainBall->speed.y = 0;
		mainBall->pos.x = rct.right / 2;
		mainBall->pos.y = rct.bottom - DEF_HEIGHT - BTNS_MARGIN - DEF_BALL_RADIUS;

		*isGameStarted = false;
	}
}

void BonusAdd(int brick)
{
	if (bricks[brick].bonus != 0)
	{
		for (size_t i = 0; i < MAX_RENDER_BONUSES; i++)
		{
			if (bonuses[i].type == 0)
			{
				bonuses[i].type = bricks[brick].bonus;
				bonuses[i].grav = 2;
				bonuses[i].pos.x = bricks[brick].pos.x + BRICK_WIDTH / 2;
				bonuses[i].pos.y = bricks[brick].pos.y + BRICK_HEIGHT / 2;
				if (bonuses[i].type == HealthUp)
				{
					bonuses[i].clr = HP_UP_COLOR;
				}
				if (bonuses[i].type == Clip)
				{
					bonuses[i].clr = CLIP_COLOR;
				}
				return;
			}
		}
	}
}

bool BonusFall(int i)
{
	if ((rct.bottom - bonuses[i].pos.y) <= BONUS_RADIUS)
	{
		bonuses[i].clr = 0;
		bonuses[i].grav = 0;
		bonuses[i].pos.x = 0;
		bonuses[i].pos.y = 0;
		bonuses[i].type = 0;

		return true;
	}

	return false;
}

bool BonusPickUp(int i)
{
	if ((plate->rct.top - bonuses[i].pos.y) <= BONUS_RADIUS)
	{
		if ((plate->rct.left <= bonuses[i].pos.x) && (plate->rct.right >= bonuses[i].pos.x))
		{
			if (bonuses[i].type == HealthUp)
			{
				plate->lives = plate->lives + 1;
				if (plate->lives >= 9)
				{
					plate->lives = 8;
				}
			}

			if (bonuses[i].type == Clip)
			{
				plate->bonus = Clip;
			}

			bonuses[i].clr = 0;
			bonuses[i].grav = 0;
			bonuses[i].pos.x = 0;
			bonuses[i].pos.y = 0;
			bonuses[i].type = 0;
			return true;
		}
	}

	return false;
}

void GameClose()
{
	free(bonuses);
	free(plate);
	free(mainBall);
	free(bricks);
}

void NewLevel(bool* isGameStarted)
{
	if (destroyedBricksNum >= bricksNum)
	{
		if (level == IIILVL)
		{
			PostQuitMessage(0);
		}
		else
		{
			level++;
			GameCore();
			for (size_t i = 0; i < MAX_RENDER_BONUSES; i++)
			{
				bonuses[i].clr = 0;
				bonuses[i].grav = 0;
				bonuses[i].pos.x = 0;
				bonuses[i].pos.y = 0;
				bonuses[i].type = 0;
			}
			*isGameStarted = false;
			destroyedBricksNum = 0;
		}
	}
}
