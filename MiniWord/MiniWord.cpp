//MiniWord.cpp: 定义应用程序的入口点。
//
///2E80～33FFh：中日韩符号区
///3400～4DFFh：中日韩认同表意文字扩充A区，总计收容6582个中日韩汉字。
///4E00～9FFFh：中日韩认同表意文字区，总计收容20902个中日韩汉字。
///FF01～FF5Eh:
///TODO LIST:
///
///		4. FILE
///		5. CTRL+S/X/C/V/D/O/A/F
///			S:保存
///			X:剪切、删除行
///			C:复制
///			V:粘贴
///			D:复制行
///			O:打开文件
///			A:全选
///			F:查找
///		6. MOUSE CURSOR
///

#include "stdafx.h"
#include "MiniWord.h"

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
HDC hdc;
PAINTSTRUCT ps;
HBITMAP hCaret;

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int i;

//Window
static int xClientMax;
static int nCharX, nCharY;						//字符长宽
static int nWindowX, nWindowY;					//页面长宽
static int nWindowCharX, nWindowCharY;			//页面最大字符
static int scrFirstLine, scrLastLine;			//输出篇幅
// Carets
class Caret crt;

static UINT uOldBlink;
int x, y;										//字符位置
TEXTMETRIC tm;									//字体信息

//Scroll
SCROLLINFO si;
static int xScrPos, yScrPos;
#define ScrCrtPosX ((crt.CaretPosX) - (xScrPos))
#define ScrCrtPosY ((crt.CaretPosY) - (yScrPos))

Article Ar;
Line tmpL, p;

#define MAXPAUSE 100
WCHAR pause[MAXPAUSE];

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 在此放置代码。

	// 初始化全局字符串
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_MINIWORD, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MINIWORD));

	MSG msg;

	// 主消息循环:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MINIWORD));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MINIWORD);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 将实例句柄存储在全局变量中

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW |
				WS_HSCROLL | WS_VSCROLL,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:

		hdc = GetDC(hWnd);
		SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
		GetTextMetrics(hdc, &tm);
		ReleaseDC(hWnd, hdc);

		nCharX = tm.tmAveCharWidth;
		nCharY = tm.tmHeight;

		for (int i = 0; i < MAXPAUSE; i++) {
			pause[i] = L' ';
		}

		return 0;

	case WM_SETFOCUS:

		CreateCaret(hWnd, (HBITMAP)NULL, 1, nCharY);
		SetCaretPos(ScrCrtPosX * nCharX, ScrCrtPosY * nCharY);
		ShowCaret(hWnd);

		return 0;

	case WM_SIZE:

		nWindowY = HIWORD(lParam);
		nWindowX = LOWORD(lParam);
		nWindowCharX = nWindowX / nCharX;
		nWindowCharY = nWindowY / nCharY;
		SetCaretPos(ScrCrtPosX * nCharX, ScrCrtPosY * nCharY);

		si.cbSize = sizeof(si);
		si.fMask = SIF_RANGE | SIF_PAGE;
		si.nMin = 0;
		si.nMax = Ar.LineNum() - 1;
		si.nPage = nWindowY / nCharY;
		SetScrollInfo(hWnd, SB_VERT, &si, TRUE);

		si.cbSize = sizeof(si);
		si.fMask = SIF_RANGE | SIF_PAGE;
		si.nMin = 0;
		si.nMax = 2 + Ar.MaxWidth();
		si.nPage = nWindowX / nCharX;
		SetScrollInfo(hWnd, SB_HORZ, &si, TRUE);

		return 0;

	case WM_HSCROLL:
		// Get all the vertial scroll bar information.
		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;

		// Save the position for comparison later on.
		GetScrollInfo(hWnd, SB_HORZ, &si);
		xScrPos = si.nPos;
		switch (LOWORD(wParam))
		{
			// User clicked the left arrow.
		case SB_LINELEFT:
			si.nPos -= 1;
			break;

			// User clicked the right arrow.
		case SB_LINERIGHT:
			si.nPos += 1;
			break;

			// User clicked the scroll bar shaft left of the scroll box.
		case SB_PAGELEFT:
			si.nPos -= si.nPage;
			break;

			// User clicked the scroll bar shaft right of the scroll box.
		case SB_PAGERIGHT:
			si.nPos += si.nPage;
			break;

			// User dragged the scroll box.
		case SB_THUMBTRACK:
			si.nPos = si.nTrackPos;
			break;

		default:
			break;
		}

		// Set the position and then retrieve it.  Due to adjustments
		// by Windows it may not be the same as the value set.
		si.fMask = SIF_POS;
		SetScrollInfo(hWnd, SB_HORZ, &si, TRUE);
		GetScrollInfo(hWnd, SB_HORZ, &si);

		// If the position has changed, scroll the window.
		if (si.nPos != xScrPos)
		{
			ScrollWindow(hWnd, nCharX * (xScrPos - si.nPos), 0, NULL, NULL);
			UpdateWindow(hWnd);
		}

		return 0;

	case WM_VSCROLL:
		// Get all the vertial scroll bar information.
		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;
		GetScrollInfo(hWnd, SB_VERT, &si);

		// Save the position for comparison later on.
		yScrPos = si.nPos;
		switch (LOWORD(wParam))
		{

			// User clicked the HOME keyboard key.
		case SB_TOP:
			si.nPos = si.nMin;
			break;

			// User clicked the END keyboard key.
		case SB_BOTTOM:
			si.nPos = si.nMax;
			break;

			// User clicked the top arrow.
		case SB_LINEUP:
			si.nPos -= 1;
			break;

			// User clicked the bottom arrow.
		case SB_LINEDOWN:
			si.nPos += 1;
			break;

			// User clicked the scroll bar shaft above the scroll box.
		case SB_PAGEUP:
			si.nPos -= si.nPage;
			break;

			// User clicked the scroll bar shaft below the scroll box.
		case SB_PAGEDOWN:
			si.nPos += si.nPage;
			break;

			// User dragged the scroll box.
		case SB_THUMBTRACK:
			si.nPos = si.nTrackPos;
			break;

		default:
			break;
		}

		// Set the position and then retrieve it.  Due to adjustments
		// by Windows it may not be the same as the value set.
		si.fMask = SIF_POS;
		SetScrollInfo(hWnd, SB_VERT, &si, TRUE);
		GetScrollInfo(hWnd, SB_VERT, &si);

		// If the position has changed, scroll window and update it.
		if (si.nPos != yScrPos)
		{
			ScrollWindow(hWnd, 0, nCharY * (yScrPos - si.nPos), NULL, NULL);
			UpdateWindow(hWnd);
		}

		return 0;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_HOME:
			tmpL = Ar.GetLine(crt.CaretPosY);
			crt.MvHome(tmpL);
			break;

		case VK_END:
			tmpL = Ar.GetLine(crt.CaretPosY);
			crt.MvEnd(tmpL);
			break;

		case VK_LEFT:
			tmpL = Ar.GetLine(crt.CaretPosY);
			crt.MvLeft(tmpL);
			break;

		case VK_RIGHT:
			tmpL = Ar.GetLine(crt.CaretPosY);
			crt.MvRight(tmpL);
			break;

		case VK_UP:
			tmpL = Ar.GetLine(crt.CaretPosY);
			crt.MvUp(tmpL);
			break;

		case VK_DOWN:
			tmpL = Ar.GetLine(crt.CaretPosY);
			crt.MvDown(tmpL);
			break;

		case VK_DELETE:
			tmpL = Ar.GetLine(crt.CaretPosY);
			WCHAR x = crt.CtrDelete(tmpL);
			if (x == 0)
				break;
			InvalidateRect(hWnd, NULL, TRUE);
			/*			HideCaret(hWnd);
						hdc = GetDC(hWnd);
						SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));

						TextOut(hdc, crt.CaretPosX * nCharX, crt.CaretPosY * nCharY,
							tmpL->lnBuff.ArrayPos(RG), tmpL->lnBuff.GetSize(RG));
						if(0x2E80 <= x && x <= 0x9FFF)
							TextOut(hdc, tmpL->lnBuff.CharWidth() * nCharX, crt.CaretPosY * nCharY, pause, 2);
						else
							TextOut(hdc, tmpL->lnBuff.CharWidth() * nCharX, crt.CaretPosY * nCharY, pause, 1);

						ReleaseDC(hWnd, hdc);
						ShowCaret(hWnd);
			*/
						//Delete
			break;
		}
		SetCaretPos(ScrCrtPosX * nCharX, ScrCrtPosY * nCharY);

		return 0;

	case WM_CHAR:
		switch (wParam)
		{
		case 0x08:
			tmpL = Ar.GetLine(crt.CaretPosY);
			if (!tmpL->lnBuff.IsEmpty(LF)) {
				crt.MvLeft(tmpL);
				SendMessage(hWnd, WM_KEYDOWN, VK_DELETE, 1L);
			}
			else {
				if (!tmpL->IsFirstL()) {

					while (!tmpL->lnBuff.IsEmpty(RG)) {
						tmpL->lnBuff.RGtoLF();
					}
					while (!tmpL->lnBuff.IsEmpty(LF)) {
						tmpL->prevL->lnBuff.Push(tmpL->lnBuff.PopandTop(LF), RG);
					}

					crt.CaretPosY -= 1;
					crt.CaretPosX = tmpL->prevL->lnBuff.CharWidth(LF);
					InvalidateRect(hWnd, NULL, TRUE);

					Ar.Remove(tmpL);
				}
			}
			//Process a backspace.
			break;
		case 0x09:

			do
			{
				SendMessage(hWnd, WM_CHAR, ' ', 1L);
			} while (crt.CaretPosX % 4 != 0);
			//Process a tab.

			break;

		case 0x0D:
			tmpL = Ar.GetLine(crt.CaretPosY);

			crt.CtrEnter(tmpL);
			Ar.IncLineN();
			InvalidateRect(hWnd, NULL, TRUE);

			//Process a return.

			break;

		default:
			tmpL = Ar.GetLine(crt.CaretPosY);
			tmpL->lnBuff.Push((WCHAR)wParam, LF);
			if (0x2E80 <= (WCHAR)wParam && (WCHAR)wParam <= 0x9FFF) {
				crt.CaretPosX += 2;
			}
			else
				crt.CaretPosX += 1;
			InvalidateRect(hWnd, NULL, TRUE);
						//Hide the caret.
			/*			HideCaret(hWnd);
						hdc = GetDC(hWnd);
						SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));

						TextOut(hdc, crt.CaretPosX * nCharX, crt.CaretPosY * nCharY,
							tmpL->lnBuff.ArrayPos(LF) + tmpL->lnBuff.GetSize(LF) - 1, 1);

						if (0x4E00 <= (WCHAR)wParam && (WCHAR)wParam <= 0x9FBB) {
							crt.CaretPosX += 2;
						}
						else
							crt.CaretPosX += 1;


						TextOut(hdc, crt.CaretPosX * nCharX, crt.CaretPosY * nCharY,
							tmpL->lnBuff.ArrayPos(RG), tmpL->lnBuff.GetSize(RG));

						ReleaseDC(hWnd, hdc);

						ShowCaret(hWnd);
*/
		}

		SetCaretPos(ScrCrtPosX* nCharX, ScrCrtPosY * nCharY);

		if (ScrCrtPosX * nCharX > nWindowX || ScrCrtPosY * nCharY > nWindowY || ScrCrtPosX <= 0 || ScrCrtPosY <= 0) {
			si.cbSize = sizeof(si);
			si.fMask = SIF_RANGE | SIF_PAGE;
			si.nMin = 0;
			si.nMax = Ar.LineNum() - 1;
			si.nPage = nWindowY / nCharY;
			SetScrollInfo(hWnd, SB_VERT, &si, TRUE);

			si.cbSize = sizeof(si);
			si.fMask = SIF_RANGE | SIF_PAGE;
			si.nMin = 0;
			si.nMax = 2 + Ar.MaxWidth();
			si.nPage = nWindowX / nCharX;
			SetScrollInfo(hWnd, SB_HORZ, &si, TRUE);
		}
		return 0;

	case WM_KILLFOCUS:

		DestroyCaret();

		return 0;

	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 分析菜单选择:
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
	}
	return 0;

	case WM_PAINT:
	{
		HideCaret(hWnd);
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		si.cbSize = sizeof(si);
		si.fMask = SIF_POS;
		GetScrollInfo(hWnd, SB_VERT, &si);
		yScrPos = si.nPos;

		GetScrollInfo(hWnd, SB_HORZ, &si);
		xScrPos = si.nPos;

		scrFirstLine = max(0, yScrPos + ps.rcPaint.top / nCharY);
		scrLastLine = min(Ar.LineNum() - 1, yScrPos + ps.rcPaint.bottom / nCharY);

		SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));

		tmpL = Ar.GetLine(scrFirstLine);
		for (i = scrFirstLine; i <= scrLastLine; tmpL = tmpL->nextL, i++)
		{
			x = nCharX * (-xScrPos);
			y = nCharY * (-yScrPos);

			TextOut(hdc, x, (i)* nCharY + y, tmpL->lnBuff.ArrayPos(LF),
				tmpL->lnBuff.GetSize(LF));
			TextOut(hdc, x + tmpL->lnBuff.CharWidth(LF) * nCharX,
				(i)* nCharY + y, tmpL->lnBuff.ArrayPos(RG), tmpL->lnBuff.GetSize(RG));
		}

		EndPaint(hWnd, &ps);
		ShowCaret(hWnd);

		return 0;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
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
