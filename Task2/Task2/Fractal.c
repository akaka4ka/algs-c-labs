#include "fractal.h"

DOT* MemoryInit()
{
    DOT* dots = (DOT*)malloc(INITPOINTS * sizeof(DOT));
    if (dots == NULL)
    {
        return NULL;
    }

    return dots;
}

DOT* IterUp(DOT* dots, size_t iter, size_t lastIter, int* lastDotsCount, int* dotsCount, double* length,
            double* sin, double* cos, int* side)
{
    DOT* _memoryCheck = NULL;
    if (iter > lastIter)
    {
        *lastDotsCount = *dotsCount;

        *dotsCount = *dotsCount + (int)pow(3.0, ((double)iter - 1)) * 2;
        _memoryCheck = (DOT*)realloc(dots, *dotsCount * sizeof(DOT));
        if (_memoryCheck == NULL)
        {
            return MEMORYERR;
        }
        else
        {
            dots = _memoryCheck;
        }

        for (int i = (*lastDotsCount - 1); i >= 0; i--)
        {
            dots[i * 3].x = dots[i].x;
            dots[i * 3].y = dots[i].y;
        }


        for (int i = 0; i < (*dotsCount - 1); i++)
        {
            if ((i % 3) == 0)
            {
                GeomHandler(dots, i, length, sin, cos);
                FigureHandler(dots, i, *length, *sin, *cos, side);
            }
        }
    }

	return dots;
}

void GeomHandler(DOT* dots, int dotNumber, double* length, double* sin, double* cos)
{
    *length = sqrt(pow((dots[dotNumber + 3].x - dots[dotNumber].x), 2.0) + pow((dots[dotNumber + 3].y - dots[dotNumber].y), 2.0));
    *sin = (dots[dotNumber + 3].y - dots[dotNumber].y) / *length;
    *cos = (dots[dotNumber + 3].x - dots[dotNumber].x) / *length;

	return;
}

void FigureHandler(DOT* dots, int dotNumber, double length, double sin, double cos, int* side)
{
    double _ax, _ay, _bx, _cx, _cy;
    _ax = (dots[dotNumber].y + dots[dotNumber].x * cos / sin) / (sin + pow(cos, 2) / sin);
    _bx = (dots[dotNumber + 3].y + dots[dotNumber + 3].x * cos / sin) / (sin + pow(cos, 2) / sin);
    _ay = (_ax * cos - dots[dotNumber].x) / sin;
    _cx = (_ax + _bx) / 2;

    if (*side == 1)
    {
        _cy = _ay - length * sqrt(3.0) / 4;
        *side = 0;
    }
    else
    {
        _cy = _ay + length * sqrt(3.0) / 4;
        *side = 1;
    }

    dots[dotNumber + 1].x = (_cx - length / 4) * cos - _cy * sin;
    dots[dotNumber + 1].y = (_cx - length / 4) * sin + _cy * cos;
    dots[dotNumber + 2].x = (_cx + length / 4) * cos - _cy * sin;
    dots[dotNumber + 2].y = (_cx + length / 4) * sin + _cy * cos;

	return;
}
