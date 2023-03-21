#include "menu.h"

#define MAX_LOADSTRING 100

BTNS* btns = NULL;
FILE* filePtr = NULL;
size_t btnsNum = 0;
size_t i = 0;   
size_t curFocus = 0;
size_t prevFocus = 0;
char pressedBtnId[20] = { 0 };
_Bool isload = FALSE;
_Bool isReleased = TRUE;

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

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

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TASK3, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    AllocConsole();

    if (!(filePtr = SysOpen()))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TASK3));

    MSG msg;

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TASK3));
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TASK3);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance;

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
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
    case WM_DRAWITEM:
    {
        LPDRAWITEMSTRUCT draw = (LPDRAWITEMSTRUCT)lParam;

        SysUpdate(btns, draw, hWnd, curFocus, prevFocus, isload, wParam);

        return TRUE;
    } break;

    case WM_KEYDOWN:
    {
        switch (wParam)
        {
        case 0x52:
        {
            if (isReleased && (isload != FALSE))
            {
                isReleased = FALSE;
            }
            else if (isload != FALSE)
            {
                btns = SysRelease(btns, btnsNum);
                isReleased = TRUE;
                isload = NULL;
            }
        } break;

        case VK_ESCAPE:
        {
            if (btns == NULL)
            {
                DestroyWindow(hWnd);
            }
            if (btns != NULL)
            {
                btns = SysClose(btns, filePtr, btnsNum);
                isload = FALSE;
                DestroyWindow(hWnd);
            }
        } break;

        case VK_UP:
        {
            if (isload)
            {
                if (curFocus == NaI)
                {
                    prevFocus = NaI;
                    curFocus = 0;
                    btns[curFocus].focused = TRUE;
                    InvalidateRect(btns[curFocus].hBtnWnd, NULL, TRUE);
                    UpdateWindow(btns[curFocus].hBtnWnd);
                    break;
                }
                if (curFocus == 0)
                {
                    prevFocus = curFocus;
                    curFocus = btnsNum - 1;
                }
                else
                {
                    prevFocus = curFocus;
                    curFocus--;
                }
                btns[curFocus].focused = TRUE;
                btns[prevFocus].focused = FALSE;
                InvalidateRect(btns[curFocus].hBtnWnd, NULL, TRUE);
                UpdateWindow(btns[curFocus].hBtnWnd);
                InvalidateRect(btns[prevFocus].hBtnWnd, NULL, TRUE);
                UpdateWindow(btns[prevFocus].hBtnWnd);
            }
        } break;

        case VK_DOWN:
        {
            if (isload)
            {
                if (curFocus == NaI)
                {
                    prevFocus = NaI;
                    curFocus = btnsNum - 1;
                    btns[curFocus].focused = TRUE;
                    InvalidateRect(btns[curFocus].hBtnWnd, NULL, TRUE);
                    UpdateWindow(btns[curFocus].hBtnWnd);
                    break;
                }
                if (curFocus == btnsNum - 1)
                {
                    prevFocus = curFocus;
                    curFocus = 0;
                }
                else
                {
                    prevFocus = curFocus;
                    curFocus++;
                }
                btns[curFocus].focused = TRUE;
                btns[prevFocus].focused = FALSE;
                InvalidateRect(btns[curFocus].hBtnWnd, NULL, TRUE);
                UpdateWindow(btns[curFocus].hBtnWnd);
                InvalidateRect(btns[prevFocus].hBtnWnd, NULL, TRUE);
                UpdateWindow(btns[prevFocus].hBtnWnd);
            }
        } break;
        
        case VK_RETURN:
        {
            if (!isload && isReleased)
            {
                if (btns == NULL)
                {
                    btnsNum = 0;
                    curFocus = 0;
                    fseek(filePtr, 0, SEEK_SET);
                    btns = SystemInitialise(filePtr, &btnsNum);
                }
                while (curFocus < btnsNum)
                {
                    btns[curFocus].focused = FALSE;
                    btns[curFocus].hBtnWnd = CreateWindow(L"BUTTON", btns[curFocus].name,
                                                          BTN_STYLE, btns[curFocus].posX, btns[curFocus].posY,
                                                          50, 50, hWnd, curFocus, hInst, NULL);
                    InvalidateRect(btns[curFocus].hBtnWnd, NULL, TRUE);
                    UpdateWindow(btns[curFocus].hBtnWnd);
                    curFocus++;
                }
                curFocus = NaI;
                isload = TRUE;
                isReleased = FALSE;
                break;
            }
            if ((btns != NULL) && (isReleased == FALSE)) {
                memset(pressedBtnId, 0, 20);
                if (curFocus == NaI)
                {
                    sprintf(pressedBtnId, "%s", "NaI");
                }
                else
                {
                    sprintf(pressedBtnId, "%d", curFocus);
                }
                WriteFile(STD_OUTPUT_HANDLE, "Pressed button with ID: ", strlen("Pressed button with ID: "), NULL, NULL);
                WriteFile(STD_OUTPUT_HANDLE, (LPCWSTR)pressedBtnId, strlen(pressedBtnId), NULL, NULL);
                WriteFile(STD_OUTPUT_HANDLE, "\n", 1, NULL, NULL);
            }
        } break;

        default:
            break;
        }
    } break;

    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
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
        
        EndPaint(hWnd, &ps);
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
