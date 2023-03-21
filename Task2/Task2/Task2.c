#include "framework.h"
#include "fractal.h"
#include <stdio.h>

#define MAX_LOADSTRING 100

size_t curIter = 0;
size_t lastIter = 0;
int isSideReadyToSelect = 0;
int dot = 0;
int dotCount = 2;
int lastIterDots = 0;
int lineSide = 0;
int sideSelectionDotX = 0;
int sideSelectionDotY = 0;
double lineLength = 0;
double lineSin = 0;
double lineCos = 0;
DOT* dots = NULL;

HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    dots = MemoryInit();
    if (dots == NULL)
    {
        return 1;
    }

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TASK2, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TASK2));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TASK2));
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TASK2);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   SetWindowLong(hWnd, GWL_STYLE, WS_POPUP);
   SetWindowLong(hWnd, GWL_EXSTYLE, WS_EX_TOPMOST);
   ShowWindow(hWnd, SW_SHOWMAXIMIZED);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Разобрать выбор в меню:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            free(dots);
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    } break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            
            HPEN hPen = CreatePen(PS_SOLID, PENWIDTH, RGB(139, 0, 255));
            SelectObject(hdc, hPen);

            if (dot > 1)
            {
                MoveToEx(hdc, dots[0].x, dots[0].y, NULL);

                if (dot == 2 && isSideReadyToSelect == READYTOSELECT)
                {
                    LineTo(hdc, dots[1].x, dots[1].y);
                    MoveToEx(hdc, dots[1].x, dots[1].y, NULL);
                    LineTo(hdc, sideSelectionDotX, sideSelectionDotY);
                    MoveToEx(hdc, sideSelectionDotX, sideSelectionDotY, NULL);
                    LineTo(hdc, dots[0].x, dots[0].y);
                }

                if (curIter >= lastIter)
                {
                    for (int i = 1; i < dotCount; i++)
                    {
                        LineTo(hdc, dots[i].x, dots[i].y);
                    }
                }

                if (curIter < lastIter)
                {
                    for (int i = 0; i < dotCount; i++)
                    {
                        if (i % (int)pow(3.0, (lastIter - curIter)) == 0) LineTo(hdc, dots[i].x, dots[i].y);
                    }
                }
            }
            else
            {
                MoveToEx(hdc, dots[0].x, dots[0].y, NULL);
                LineTo(hdc, dots[dot].x, dots[dot].y);
            }

            EndPaint(hWnd, &ps);
        } break;
    case WM_KEYDOWN:
    {
        switch (wParam)
        {
        case VK_ESCAPE:
        {
            free(dots);
            DestroyWindow(hWnd);
        } break;
        case VK_LEFT:
        {
            if (curIter > 0)
            {
                if (curIter > lastIter)
                {
                    lastIter = curIter;
                }
                curIter = curIter - 1;
                InvalidateRect(hWnd, NULL, TRUE);
            }
        } break;
        case VK_RIGHT:
        {
            if (curIter > lastIter)
            {
                lastIter = curIter;
            }
            curIter = curIter + 1;

            dots = IterUp(dots, curIter, lastIter, &lastIterDots, &dotCount, &lineLength, &lineSin,
                          &lineCos, &lineSide);
            InvalidateRect(hWnd, NULL, TRUE);
        } break;
        default:
            break;
        }
    } break;
    case WM_LBUTTONDOWN:
    {
        if ((dot == 2) && (isSideReadyToSelect == READYTOSELECT))
        {
            double y = dots[0].y + (LOWORD(lParam) - dots[0].x) * (dots[1].y - dots[0].y) / (dots[1].x - dots[0].x);
            if (dots[0].x < dots[1].x)
            {
                if (HIWORD(lParam) < y)
                {
                    lineSide = 1;
                }
                else
                {
                    lineSide = 0;
                }
            }
            else
            {
                if (HIWORD(lParam) < y)
                {
                    lineSide = 0;
                }
                else
                {
                    lineSide = 1;
                }
            }
            isSideReadyToSelect = SELECTED;
            InvalidateRect(hWnd, NULL, TRUE);
        }

        if (dot < 2)
        {
            dots[dot].x = LOWORD(lParam);
            dots[dot].y = HIWORD(lParam);
            dot = dot + 1;

            if (dot > 1)
            {
                dots = IterUp(dots, curIter, lastIter, &lastIterDots, &dotCount, &lineLength, &lineSin,
                              &lineCos, &lineSide);
                InvalidateRect(hWnd, NULL, FALSE);
            }
        }

        if ((dot == 2) && (isSideReadyToSelect == NOTSELECTED))
        {
            isSideReadyToSelect = READYTOSELECT;
        }
        //y = dots[0].y + (LOWORD(lParam) - dots[0].x) * (dots[1].y - dots[0].y) / (dots[1].x - dots[0].x);
        
    } break;
    case WM_MOUSEMOVE:
    {
        if (dot < 2)
        {
            dots[dot].x = LOWORD(lParam);
            dots[dot].y = HIWORD(lParam);
            InvalidateRect(hWnd, NULL, TRUE);
        }

        if ((dot == 2) && (isSideReadyToSelect == READYTOSELECT))
        {
            lineLength = sqrt(pow((dots[1].x - dots[0].x), 2.0) + pow((dots[1].y - dots[0].y), 2.0));
            lineSin = (dots[1].y - dots[0].y) / lineLength;
            lineCos = (dots[1].x - dots[0].x) / lineLength;

            double y = dots[0].y + (LOWORD(lParam) - dots[0].x) * (dots[1].y - dots[0].y) / (dots[1].x - dots[0].x);
            double _ax, _ay, _bx, _cx, _cy;
            _ax = (dots[0].y + dots[0].x * lineCos / lineSin) / (lineSin + pow(lineCos, 2) / lineSin);
            _bx = (dots[1].y + dots[1].x * lineCos / lineSin) / (lineSin + pow(lineCos, 2) / lineSin);
            _ay = (_ax * lineCos - dots[0].x) / lineSin;
            _cx = (_ax + _bx) / 2;
            if (dots[0].x < dots[1].x)
            {
                if (HIWORD(lParam) > y)
                {
                    _cy = _ay + lineLength * sqrt(3.0) / 2;
                }
                else
                {
                    _cy = _ay - lineLength * sqrt(3.0) / 2;
                }
            }
            else
            {
                if (HIWORD(lParam) < y)
                {
                    _cy = _ay - lineLength * sqrt(3.0) / 2;
                }
                else
                {
                    _cy = _ay + lineLength * sqrt(3.0) / 2;
                }
            }

            sideSelectionDotX = _cx * lineCos - _cy * lineSin;
            sideSelectionDotY = _cx * lineSin + _cy * lineCos;

            InvalidateRect(hWnd, NULL, TRUE);
        }
    } break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
