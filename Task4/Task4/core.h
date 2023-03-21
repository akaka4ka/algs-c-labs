#pragma once

#include "defs.h"

BRICKS		GameCore();
bool		InContainerCheck(int direction);
void		ContainerReflect();
void		BrickReflect();
void		PlateReflect();
void		TopReflect();
void		BallFall(bool* isGameStarted);
void		BonusAdd(int brick);
bool		BonusFall(int i);
bool		BonusPickUp(int i);
void		GameClose();
void		NewLevel(bool* isGameStarted);