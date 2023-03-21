#include <stdbool.h>
#include "win_api.h"
#include "menu_bar.h"
#include "tools.h"
#include "painting.h"

RECT wndRct;
int curSize = 0;
COLORREF curClr = 0;
int curTool = NaT;
bool paint = false;
bool fileChanged = false;

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TASK6, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    MenuBarInit(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TASK6));

    MSG msg;

    DrawInit();

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

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TASK6));
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = 0;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance;

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_SIZE:
    {
        RECT oldRct = wndRct;
        if (oldRct.right == 0)
        {
            wndRct.left = 0;
            wndRct.top = 0;
            wndRct.right = LOWORD(lParam);
            wndRct.bottom = HIWORD(lParam);
            break;
        }
        ResizeWnd(wndRct, oldRct);
    } break;

    case WM_CREATE:
    {
        AddMenus(hWnd);
        MenuVarsInit(&curTool, &curClr, &curSize, &fileChanged, &wndRct);
    } break;

    case WM_COMMAND:
    {
        MenuSelect(LOWORD(wParam), hInst);

        return DefWindowProc(hWnd, message, wParam, lParam);
    } break;

    case WM_MOUSEMOVE:
    {
        if (paint == true)
        {
            
            POS pos;

                pos.x = LOWORD(lParam);
                pos.y = HIWORD(lParam);

            if (curTool == BRUSH)
            {
                PaintWithBrush(pos, curClr, curSize);
            }

            if (curTool == PENCIL)
            {
                PaintWithPencil(pos, curClr, curSize);
            }

            if (curTool == ERASER)
            {
                Erase(pos, curSize);
            }
        }
    } break;

    case WM_LBUTTONDOWN:
    {
        paint = true;
        fileChanged = true;
    } break;

    case WM_LBUTTONUP:
    {
        paint = false;
        MemorizeWnd(wndRct);
    } break;

    case WM_DESTROY:
    {
        PostQuitMessage(0);
        DrawRelease();
    } break;

    default: return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
