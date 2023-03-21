#pragma once

#include "win_api.h"
#include "tools.h"

void DrawInit();
void NewFile();
void PaintWithBrush(POS pos, COLORREF clr, size_t size);
void PaintWithPencil(POS pos, COLORREF clr, size_t size);
void Erase(POS pos, size_t size);
void ResizeWnd(RECT newRct, RECT oldRct);
void MemorizeWnd(RECT curRct);
void ClearBM();
void DrawRelease();
HDC GetHDC();