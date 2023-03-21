#include "menu_bar.h"
#include "resource.h"

HINSTANCE hInstPtr;

HMENU hMenubar;
HMENU hFileMenu;
HMENU hToolsMenu;

RECT* wndRectPtr;
size_t* curSizePtr;
COLORREF* curClrPtr;
int* curToolPtr;
bool* fileChangedPtr;

void MenuVarsInit(int* tool, COLORREF* curClr, size_t* sizePtr, bool* fileChanged, RECT* wndRct)
{
    wndRectPtr = wndRct;
    curSizePtr = sizePtr;
    curClrPtr = curClr;
    curToolPtr = tool;
    fileChangedPtr = fileChanged;
}

void MenuBarInit(HINSTANCE hInstance)
{
    WNDCLASSW wc = { 0 };
    wc.lpszClassName = L"Simple menu";
    wc.hInstance = hInstance;
    wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
    wc.lpfnWndProc = WndProc;
    wc.hCursor = LoadCursor(0, IDC_ARROW);

    RegisterClassW(&wc);
}

void AddMenus(HWND hWnd)
{
    if (hMenubar == NULL)
    {
        hMenubar = CreateMenu();
        if (hFileMenu == NULL)
        {
            hFileMenu = CreateMenu();
        }
        if (hToolsMenu == NULL)
        {
            hToolsMenu = CreateMenu();
        }
    }

    if (hMenubar == NULL || hFileMenu == NULL || hToolsMenu == NULL)
    {
        PostQuitMessage(0);
        return;
    }

    AppendMenuW(hFileMenu, MF_STRING, IDM_FILE_NEW, L"&New");
    AppendMenuW(hFileMenu, MF_STRING, IDM_FILE_OPEN, L"&Open");
    AppendMenuW(hFileMenu, MF_STRING, IDM_FILE_SAVE, L"&Save");
    AppendMenuW(hFileMenu, MF_SEPARATOR, 0, NULL);
    AppendMenuW(hFileMenu, MF_STRING, IDM_FILE_QUIT, L"&Quit");

    AppendMenuW(hToolsMenu, MF_STRING, IDM_TOOLS_BRUSH, L"&Brush");
    AppendMenuW(hToolsMenu, MF_STRING, IDM_TOOLS_PENCIL, L"&Pencil");
    AppendMenuW(hToolsMenu, MF_STRING, IDM_TOOLS_ERASER, L"&Eraser");

    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hFileMenu, L"&File");
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hToolsMenu, L"&Tools");

    SetMenu(hWnd, hMenubar);
}

void MenuSelect(int btnId, HINSTANCE hInst)
{
    hInstPtr = hInst;

    switch (btnId)
    {
    case IDM_TOOLS_BRUSH:
    {
        *curToolPtr = BRUSH;

        DialogBox(hInstPtr, MAKEINTRESOURCE(IDD_SIZE_DIALOG), NULL, (DLGPROC)DlgMain);

        MENUITEMINFO curBtnInfo = { 0 };
        curBtnInfo.cbSize = sizeof(MENUITEMINFO);
        curBtnInfo.fMask = MIIM_STATE;
        GetMenuItemInfo(hToolsMenu, IDM_TOOLS_BRUSH, FALSE, &curBtnInfo);
        curBtnInfo.fState = MF_CHECKED;
        SetMenuItemInfo(hToolsMenu, IDM_TOOLS_BRUSH, FALSE, &curBtnInfo);

        MENUITEMINFO pencilBtnInfo = { 0 };
        pencilBtnInfo.cbSize = sizeof(MENUITEMINFO);
        pencilBtnInfo.fMask = MIIM_STATE;
        GetMenuItemInfo(hToolsMenu, IDM_TOOLS_PENCIL, FALSE, &pencilBtnInfo);
        pencilBtnInfo.fState = MF_UNCHECKED;
        SetMenuItemInfo(hToolsMenu, IDM_TOOLS_PENCIL, FALSE, &pencilBtnInfo);

        MENUITEMINFO eraserBtnInfo = { 0 };
        eraserBtnInfo.cbSize = sizeof(MENUITEMINFO);
        eraserBtnInfo.fMask = MIIM_STATE;
        GetMenuItemInfo(hToolsMenu, IDM_TOOLS_ERASER, FALSE, &eraserBtnInfo);
        eraserBtnInfo.fState = MF_UNCHECKED;
        SetMenuItemInfo(hToolsMenu, IDM_TOOLS_ERASER, FALSE, &eraserBtnInfo);
    } break;

    case IDM_TOOLS_PENCIL:
    {
        *curToolPtr = PENCIL;

        DialogBox(hInstPtr, MAKEINTRESOURCE(IDD_SIZE_DIALOG), NULL, (DLGPROC)DlgMain);

        MENUITEMINFO curBtnInfo = { 0 };
        curBtnInfo.cbSize = sizeof(MENUITEMINFO);
        curBtnInfo.fMask = MIIM_STATE;
        GetMenuItemInfo(hToolsMenu, IDM_TOOLS_PENCIL, FALSE, &curBtnInfo);
        curBtnInfo.fState = MF_CHECKED;
        SetMenuItemInfo(hToolsMenu, IDM_TOOLS_PENCIL, FALSE, &curBtnInfo);

        MENUITEMINFO brushBtnInfo = { 0 };
        brushBtnInfo.cbSize = sizeof(MENUITEMINFO);
        brushBtnInfo.fMask = MIIM_STATE;
        GetMenuItemInfo(hToolsMenu, IDM_TOOLS_BRUSH, FALSE, &brushBtnInfo);
        brushBtnInfo.fState = MF_UNCHECKED;
        SetMenuItemInfo(hToolsMenu, IDM_TOOLS_BRUSH, FALSE, &brushBtnInfo);

        MENUITEMINFO eraserBtnInfo = { 0 };
        eraserBtnInfo.cbSize = sizeof(MENUITEMINFO);
        eraserBtnInfo.fMask = MIIM_STATE;
        GetMenuItemInfo(hToolsMenu, IDM_TOOLS_ERASER, FALSE, &eraserBtnInfo);
        eraserBtnInfo.fState = MF_UNCHECKED;
        SetMenuItemInfo(hToolsMenu, IDM_TOOLS_ERASER, FALSE, &eraserBtnInfo);
    } break;

    case IDM_TOOLS_ERASER:
    {
        *curToolPtr = ERASER;

        DialogBox(hInstPtr, MAKEINTRESOURCE(IDD_SIZE_DIALOG), NULL, (DLGPROC)DlgMain);

        MENUITEMINFO curBtnInfo = { 0 };
        curBtnInfo.cbSize = sizeof(MENUITEMINFO);
        curBtnInfo.fMask = MIIM_STATE;
        GetMenuItemInfo(hToolsMenu, IDM_TOOLS_ERASER, FALSE, &curBtnInfo);
        curBtnInfo.fState = MF_CHECKED;
        SetMenuItemInfo(hToolsMenu, IDM_TOOLS_ERASER, FALSE, &curBtnInfo);

        MENUITEMINFO brushBtnInfo = { 0 };
        brushBtnInfo.cbSize = sizeof(MENUITEMINFO);
        brushBtnInfo.fMask = MIIM_STATE;
        GetMenuItemInfo(hToolsMenu, IDM_TOOLS_BRUSH, FALSE, &brushBtnInfo);
        brushBtnInfo.fState = MF_UNCHECKED;
        SetMenuItemInfo(hToolsMenu, IDM_TOOLS_BRUSH, FALSE, &brushBtnInfo);

        MENUITEMINFO pencilBtnInfo = { 0 };
        pencilBtnInfo.cbSize = sizeof(MENUITEMINFO);
        pencilBtnInfo.fMask = MIIM_STATE;
        GetMenuItemInfo(hToolsMenu, IDM_TOOLS_PENCIL, FALSE, &pencilBtnInfo);
        pencilBtnInfo.fState = MF_UNCHECKED;
        SetMenuItemInfo(hToolsMenu, IDM_TOOLS_PENCIL, FALSE, &pencilBtnInfo);
    } break;

    case IDM_FILE_NEW:
    {
        if (*fileChangedPtr == true)
        {
            DialogBox(hInstPtr, MAKEINTRESOURCE(IDD_SAVE_ASK), NULL, (DLGPROC)DlgMain);
        }
    } break;

    case IDM_FILE_SAVE:
    {
        DialogBox(hInstPtr, MAKEINTRESOURCE(IDD_SAVE_ASK), NULL, (DLGPROC)DlgMain);
    } break;

    case IDM_FILE_OPEN:
    {
        if (*fileChangedPtr == true)
        {
            DialogBox(hInstPtr, MAKEINTRESOURCE(IDD_SAVE_ASK), NULL, (DLGPROC)DlgMain);
        }
        DialogBox(hInstPtr, MAKEINTRESOURCE(IDD_OPEN_DIALOG), NULL, (DLGPROC)DlgMain);
    } break;

    case IDM_FILE_QUIT:
    {
        if (*fileChangedPtr == true)
        {
            DialogBox(hInstPtr, MAKEINTRESOURCE(IDD_SAVE_ASK), NULL, (DLGPROC)DlgMain);
        }
        PostQuitMessage(0);
        return;
    } break;

    default: break;
    }
}

BOOL CALLBACK DlgMain(HWND hWndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_INITDIALOG:
    {
        return TRUE;
    } break;

    /*case WM_CLOSE:
    {
        EndDialog(hWndDlg, 0);
        return TRUE;
    } break;*/

    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case IDM_SET_SIZE:
        {
            *curSizePtr = GetDlgItemInt(hWndDlg, IDC_SIZE_SEL, NULL, FALSE);
            if (*curSizePtr == 0)
            {
                SetDlgItemText(hWndDlg, IDC_SIZE_SEL, L"incorrect size...");
                break;
            }

            if (*curToolPtr == BRUSH)
            {
                if (*curSizePtr > 48)
                {
                    SetDlgItemText(hWndDlg, IDC_SIZE_SEL, L"size<=48");
                    break;
                }
            }

            if (*curToolPtr == PENCIL)
            {
                if (*curSizePtr > 5)
                {
                    SetDlgItemText(hWndDlg, IDC_SIZE_SEL, L"size<=4");
                    break;
                }
            }

            EndDialog(hWndDlg, 0);
            if (*curToolPtr == ERASER)
            {
                return TRUE;
            }
            DialogBox(hInstPtr, MAKEINTRESOURCE(IDD_CLR_DIALOG), NULL, (DLGPROC)DlgMain);
        } break;

        case ID_SET_CLR:
        {
            int r = GetDlgItemInt(hWndDlg, IDC_R_SEL, NULL, FALSE);;
            int g = GetDlgItemInt(hWndDlg, IDC_G_SEL, NULL, FALSE);;
            int b = GetDlgItemInt(hWndDlg, IDC_B_SEL, NULL, FALSE);;
            if (r == 0 || r > 255)
            {
                SetDlgItemText(hWndDlg, IDC_R_SEL, L"r <= 255");
                break;
            }
            if (g == 0 || g > 255)
            {
                SetDlgItemText(hWndDlg, IDC_G_SEL, L"g <= 255");
                break;
            }
            if (b == 0 || b > 255)
            {
                SetDlgItemText(hWndDlg, IDC_B_SEL, L"b <= 255");
                break;
            }

            *curClrPtr = RGB(r, g, b);
            EndDialog(hWndDlg, 0);
        } break;

        case IDM_NO_SAVE:
        {
            EndDialog(hWndDlg, 0);
            *fileChangedPtr = false;
            *curToolPtr = NaT;
            *curClrPtr = 0;
            *curSizePtr = 0;
            NewFile();
            return TRUE;
        } break;

        case IDM_YES_SAVE:
        {
            CHAR fileNameChar[51] = { 0 };
            WCHAR fileName[51] = { 0 };
            GetDlgItemTextA(hWndDlg, IDC_FILE_SAVE_PATH, fileNameChar, 50);
            for (size_t i = 0; i < strlen(fileNameChar); i++)
            {
                fileName[i] = fileNameChar[i];
            }
            CaptureAnImage(FindWindow(NULL, L"Task6"), GetHDC(), (LPCTSTR)wcscat(fileName, L".bmp"));
            EndDialog(hWndDlg, 0);
            *fileChangedPtr = false;
            *curToolPtr = NaT;
            *curClrPtr = 0;
            *curSizePtr = 0;
            NewFile();
        } break;

        case IDM_OPEN:
        {
            CHAR fileNameChar[256] = { 0 };
            WCHAR fileName[256] = { 0 };
            GetDlgItemTextA(hWndDlg, IDC_FILE_NAME_OPEN, fileNameChar, 256);
            for (size_t i = 0; i < strlen(fileNameChar); i++)
            {
                fileName[i] = fileNameChar[i];
            }
            LoadFromFile(FindWindow(NULL, L"Task6"), GetHDC(), (LPCTSTR)wcscat(fileName, L".bmp"));
            EndDialog(hWndDlg, 0);
            *fileChangedPtr = false;
            *curToolPtr = NaT;
            *curClrPtr = 0;
            *curSizePtr = 0;
        } break;

        default: break;
        }
        return TRUE;
    } break;

    default: break;
    }
    return FALSE;
}