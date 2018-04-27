//MiniWord.cpp: 定义应用程序的入口点。
//
///4E00-9FBB：汉字区段
///TODO LIST:
///
///		3. SCROLL (SCREENPOSX, SCREENPOSY)
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

// Carets
class Caret crt;
int nCharX, nCharY;								//字符长宽
int nWindowX, nWindowY;							//页面长宽
int nWindowCharsX, nWindowCharsY;				//页面可容纳字符数
static UINT uOldBlink;
int x, y;										//字符位置
TEXTMETRIC tm;									//字体信息

//Scroll
SCROLLINFO si;
static int xPos, yPos;

Article Ar;
Line tmpL, p;

#define MAXPAUSE 100
wchar_t pause[MAXPAUSE];

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

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL,
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
		SetCaretPos(crt.CaretPosX, crt.CaretPosY);
		ShowCaret(hWnd);

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

		case VK_PRIOR:

			break;

		case VK_NEXT:
			//Page Down
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
			wchar_t x = crt.CtrDelete(tmpL);
			if (x == 0)
				break;
			HideCaret(hWnd);
			hdc = GetDC(hWnd);
			SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));

			TextOut(hdc, crt.CaretPosX * nCharX, crt.CaretPosY * nCharY,
				tmpL->lnBuff.ArrayPos(RG), tmpL->lnBuff.GetSize(RG));
			if(0x4E00 <= x && x <= 0x9FBB)
				TextOut(hdc, tmpL->lnBuff.CharWidth() * nCharX, crt.CaretPosY * nCharY, pause, 2);
			else
				TextOut(hdc, tmpL->lnBuff.CharWidth() * nCharX, crt.CaretPosY * nCharY, pause, 1);

			ReleaseDC(hWnd, hdc);
			ShowCaret(hWnd);

			//Delete
			break;
		}

		SetCaretPos(crt.CaretPosX * nCharX, crt.CaretPosY * nCharY);

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


					HideCaret(hWnd);
					hdc = GetDC(hWnd);
					SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));

					TextOut(hdc, 0, crt.CaretPosY * nCharY, pause, tmpL->lnBuff.CharWidth(RG));

					while (!tmpL->lnBuff.IsEmpty(RG)) {
						tmpL->lnBuff.RGtoLF();
					}
					while (!tmpL->lnBuff.IsEmpty(LF)) {
						tmpL->prevL->lnBuff.Push(tmpL->lnBuff.PopandTop(LF), RG);
					}

					crt.CaretPosY--;
					crt.CaretPosX = tmpL->prevL->lnBuff.CharWidth(LF);
					TextOut(hdc, tmpL->prevL->lnBuff.CharWidth(LF) * nCharX, crt.CaretPosY * nCharY,
						tmpL->prevL->lnBuff.ArrayPos(RG),
						tmpL->prevL->lnBuff.GetSize(RG));

					ReleaseDC(hWnd, hdc);
					ShowCaret(hWnd);

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

			HideCaret(hWnd);
			hdc = GetDC(hWnd);
			SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));

			TextOut(hdc, tmpL->lnBuff.CharWidth(LF) * nCharX, crt.CaretPosY * nCharY, pause, tmpL->lnBuff.CharWidth(RG));

			crt.CtrEnter(tmpL);
			Ar.IncLineN();

			p = tmpL->nextL;
			y = crt.CaretPosY;

			while (p->nextL->nextL != nullptr) {
				TextOut(hdc, 0, (y++) * nCharY, pause, 100);
				p = p->nextL;
			}

			y = crt.CaretPosY;

			while (tmpL->nextL->nextL != nullptr) {
				tmpL = tmpL->nextL;
				TextOut(hdc, 0, (y) * nCharY, tmpL->lnBuff.ArrayPos(LF),
					tmpL->lnBuff.GetSize(LF));
				TextOut(hdc, tmpL->lnBuff.CharWidth(LF) * nCharX,
					(y++) * nCharY, tmpL->lnBuff.ArrayPos(RG),
					tmpL->lnBuff.GetSize(RG));
			}

			ReleaseDC(hWnd, hdc);
			ShowCaret(hWnd);

			//Process an carriage return.

			break;

		default:
			tmpL = Ar.GetLine(crt.CaretPosY);
			tmpL->lnBuff.Push((wchar_t)wParam, LF);

			//Hide the caret.
			HideCaret(hWnd);
			hdc = GetDC(hWnd);
			SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));

			TextOut(hdc, crt.CaretPosX * nCharX, crt.CaretPosY * nCharY,
				tmpL->lnBuff.ArrayPos(LF) + tmpL->lnBuff.GetSize(LF) - 1, 1);


			if (0x4E00 <= (wchar_t)wParam && (wchar_t)wParam <= 0x9FBB) {
				crt.CaretPosX += 2;
			}
			else
				crt.CaretPosX += 1;

			TextOut(hdc, crt.CaretPosX * nCharX, crt.CaretPosY * nCharY,
				tmpL->lnBuff.ArrayPos(RG), tmpL->lnBuff.GetSize(RG));

			ReleaseDC(hWnd, hdc);

			ShowCaret(hWnd);

		}

		SetCaretPos(crt.CaretPosX * nCharX, crt.CaretPosY * nCharY);
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
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            EndPaint(hWnd, &ps);
        }
        break;
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
