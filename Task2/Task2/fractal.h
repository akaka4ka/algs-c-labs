#pragma once

#include "resource.h"
#include <math.h>
#include <stdlib.h>
#define MEMORYERR 1
#define INITPOINTS 2
#define PENWIDTH 3
#define SELECTED 2
#define READYTOSELECT 1
#define NOTSELECTED 0

typedef struct{
	double x;
	double y;
} DOT;

DOT* MemoryInit();
DOT* IterUp(DOT* dots, size_t iter, size_t lastIter, int* lastDotsCount, int* dotsCount, double* length,
			double* sin, double* cos, int* side);
void GeomHandler(DOT* dots, int dotNumber, double* length, double* sin, double* cos);
void FigureHandler(DOT* dots, int dotNumber, double length, double sin, double cos, int* side);
