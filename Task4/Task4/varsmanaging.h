#pragma once

#include "defs.h"

int			LevelManage(int mode, int val);
HANDLE		GetGameVars(int var);
void		SetGameVars(int var, HANDLE val);
HANDLE		GetBtns();