#include "arcanoid.h"

#define MAX_LOADSTRING 100

RECT        clientRct;
RECT        gameContainer;
PBALL       mainBall;
PPLATFORM   plate;
BRICKS      bricksPtr = NULL;
BTNS        mainMenuBtns = NULL;
short       scene = 0;
short       lvl = 1;
short       curMenuFocus = NaI;
short       prevMenuFocus = NaI;
short       diff = 0;
int         bricksNum = 0;
int         direction;
bool        isMenuLoad = false;
bool        isGameStarted = false;

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
    LoadStringW(hInstance, IDC_TASK4, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    DrawInit();
    HDC _hDC = GetDrawDC(GET_DC);
    HDC _memDC = GetDrawDC(GET_VRDC);

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TASK4));

    MSG msg;

    while (TRUE)
    {
        if (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT) break;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            if (!isMenuLoad)
            {
                mainMenuBtns = PrepareMainMenuToShow(&scene, mainMenuBtns);
                isMenuLoad = true;
            }

            if (scene == MAIN_MENU || scene == DIFFICULTY_CHOICE)
            {
                MainMenuShow(scene, &curMenuFocus, &isMenuLoad);
            }

            if (scene == GAME)
            {
                GameShow(direction, &isGameStarted);
            }

            BitBlt(_hDC, 0, 0, clientRct.right, clientRct.bottom, _memDC, 0, 0, SRCCOPY);
        }
    }

    DrawRelease();
    GameClose();

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TASK4));
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

   SetWindowLong(hWnd, GWL_STYLE, WS_POPUP);
   SetWindowLong(hWnd, GWL_EXSTYLE, WS_EX_TOPMOST);
   ShowWindow(hWnd, SW_SHOWMAXIMIZED);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    switch (message)
    {
    case WM_COMMAND:
    {
        
    } break;
    
    case WM_SIZE:
    {
        GetWindowRect(hWnd, &clientRct);
        SetGameVars(SET_RCT, &clientRct);
    }break;

    case WM_KEYDOWN:
    {
        switch (wParam)
        {
        case VK_ESCAPE:
        {
            //DrawRelease();
            DestroyWindow(hWnd);
        } break;

        case VK_RETURN:
        {
            if (scene == MAIN_MENU)
            {
                switch (curMenuFocus)
                {
                case START:
                {
                    if (mainMenuBtns != NULL)
                    {
                        free(mainMenuBtns);
                        isMenuLoad = false;
                    }
                    scene = GAME;
                    if (diff == 0)
                    {
                        diff = EASY;
                        SetGameVars(SET_DIFF, &diff);
                    }
                    bricksPtr = GameCore();
                    plate = GetGameVars(GET_PLATE);
                    mainBall = GetGameVars(GET_BALL);
                    gameContainer = *((RECT*)GetGameVars(GET_CONT));
                    mainMenuBtns[curMenuFocus].isFocused = false;
                } break;

                case DIFFICULTY:
                {
                    scene = DIFFICULTY_CHOICE;
                    mainMenuBtns[curMenuFocus].isFocused = false;
                    curMenuFocus = NaI;
                    prevMenuFocus = NaI;
                } break;

                case QUIT:
                {
                    mainMenuBtns[curMenuFocus].isFocused = false;
                    if (mainMenuBtns != NULL)
                    {
                        free(mainMenuBtns);
                    }
                    PostQuitMessage(0);
                } break;
                }
            }

            if (scene == DIFFICULTY_CHOICE)
            {
                if (curMenuFocus != NaI)
                {
                    mainMenuBtns[curMenuFocus].isFocused = false;
                    diff = GetDiffFromMenu();
                    SetGameVars(SET_DIFF, &diff);
                    scene = MAIN_MENU;
                    curMenuFocus = NaI;
                    prevMenuFocus = NaI;
                }
            }
        } break;

        case VK_UP:
        {
            switch (scene)
            {
            case MAIN_MENU:
            {
                if (isMenuLoad)
                {
                    MainMenuSelect(scene, &curMenuFocus, &prevMenuFocus, VK_UP);
                }
            } break;

            case DIFFICULTY_CHOICE:
            {
                MainMenuSelect(scene, &curMenuFocus, &prevMenuFocus, VK_UP);
            } break;

            case GAME:
            {
                if (!isGameStarted || plate->cliped)
                {
                    isGameStarted = true;
                    plate->cliped = false;
                    plate->bonus = 0;

                    if (plate->speed.x != 0)
                    {
                        mainBall->speed.x = plate->speed.x / abs(plate->speed.x) * DEF_BALL_MAX_SPEED / 2;
                    }
                    else
                    {
                        mainBall->speed.x = 0;
                    }
                    
                    mainBall->speed.y = -sqrt(pow((double)DEF_BALL_MAX_SPEED, 2.0) - pow(mainBall->speed.x, 2.0));
                }
            } break;

            }
        } break;

        case VK_DOWN:
        {
            if (isMenuLoad)
            {
                MainMenuSelect(scene, &curMenuFocus, &prevMenuFocus, VK_DOWN);
            }
        } break;

        case VK_LEFT:
        {
            if (scene == GAME)
            {
                if (InContainerCheck(plate, gameContainer, MOVE_LEFT))
                {
                    plate->speed.x = MOVE_LEFT * DEF_PLATE_SPEED;
                    direction = MOVE_LEFT;
                }
                else
                {
                    direction = 0;
                }

            }
        } break;

        case VK_RIGHT:
        {
            if (scene == GAME)
            {
                if (InContainerCheck(plate, gameContainer, MOVE_RIGHT))
                {
                    plate->speed.x = MOVE_RIGHT * DEF_PLATE_SPEED;
                    direction = MOVE_RIGHT;
                }
                else
                {
                    direction = 0;
                }
            }
        } break;
        }
    } break;

    case WM_KEYUP:
    {
        if (scene == GAME)
        {
            switch (wParam)
            {
            case VK_LEFT:
            {
                plate->speed.x = 0;
                direction = 0;
            } break;

            case VK_RIGHT:
            {
                plate->speed.x = 0;
                direction = 0;
            } break;
            }
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
