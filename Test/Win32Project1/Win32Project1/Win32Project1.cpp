// Win32Project1.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Win32Project1.h"

#define MAX_LOADSTRING 100

// 全局变量: 
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

HWND hwndMain;
HWND hwndEdit;
HWND hwndPreview;
HWND hwndTitleimg;
static int CLIENT_W = 800;//主窗口客户区宽
static int CLIENT_H = 600;//主窗口客户区高
static RECT cliRect = { 0,0,CLIENT_W,CLIENT_H };//主窗口客户区矩形位置
static int spX = 550;//水平分割条初始位置
static int spY = 150;//垂直分隔条初始位置
static int spBar = 5;//分隔条的厚度
static RECT rectSpVer = { spX,0,spX + spBar,CLIENT_H };//垂直分隔条矩形位置
static RECT rectSpHor = { spX + spBar,spY,CLIENT_W,spY + spBar };//水平分隔条矩形位置
bool fDragVer = false; bool fMoveVer = false;//垂直分隔条是否按下/拖动
bool fDragHor = false; bool fMoveHor = false;//水平分隔条是否按下/拖动
POINT pold;//注意 保存旧的鼠标POINT位置 这是一个要点 并没有WM_PAINT重画，所以我们要消除之前绘制的阴影，利用PATINVERT方式重新绘制一次

		   //全局函数

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);//窗口过程函数
void InitHwndChild(HWND hwnd);//初始化三个子窗口所用 在WM_CREATE处被调用
void SizeWindowContents(int nWidth, int nHeight);//调整三个子窗口size所用 在WM_SIZE处调用 参数为客户区宽高
void Sp_LBDown(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);//分隔条区域鼠标按下 WM_LBUTTONDOWN处调用
void Sp_LBUp(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);//分隔条区域鼠标释放 WM_LBUTTONUP处调用
void Sp_MouseMove(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);//分隔条区域鼠标移动 WM_MOUSEMOVE处调用
void DrawSpBar(LPRECT lpRect);//绘制模拟分隔条的阴影 参数为要绘制的分隔条矩形位置
LPRECT GetVerSpBar();//根据鼠标位置求得新的垂直分割矩形 用于绘制分割条时
LPRECT GetHorSpBar();//根据鼠标位置求得新的水平分割矩形 用于绘制分隔条时

void SetWndCenter(HWND hwnd);//根据客户区大小设置主窗口大小并居中

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
    LoadStringW(hInstance, IDC_WIN32PROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32PROJECT1));

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

    return (int) msg.wParam;
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

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32PROJECT1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WIN32PROJECT1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

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
		{
			hwndEdit = CreateWindowEx(WS_EX_CLIENTEDGE,
				TEXT("STATIC"), TEXT("hwndEdit"),
				WS_VISIBLE | WS_CHILD,
				0, 0, 0, 0, hWnd, 0, hInst, 0);
			//SetWindowLongPtr(hwndEdit,GWLP_WNDPROC,(LONG)hwndEditProc);
			hwndPreview = CreateWindowEx(WS_EX_CLIENTEDGE,
				TEXT("STATIC"), TEXT("hwndPreview"),
				WS_VISIBLE | WS_CHILD,
				0, 0, 0, 0, hWnd, 0, hInst, 0);
			hwndTitleimg = CreateWindowEx(WS_EX_CLIENTEDGE,
				TEXT("STATIC"), TEXT("hwndTitleimg"),
				WS_VISIBLE | WS_CHILD,
				0, 0, 0, 0, hWnd, 0, hInst, 0);
		}
		break;
	case WM_SIZE:
		SizeWindowContents(LOWORD(lParam), HIWORD(lParam));//set three child window positions
		break;
	case WM_LBUTTONDOWN:
		Sp_LBDown(hWnd, message, wParam, lParam);
		break;
	case WM_LBUTTONUP:
		Sp_LBUp(hWnd, message, wParam, lParam);
		break;
	case WM_MOUSEMOVE:
		Sp_MouseMove(hWnd, message, wParam, lParam);
		break;
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
        break;
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
        break;
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

void SizeWindowContents(int nWidth, int nHeight)
{
	MoveWindow(hwndEdit, 0, 0, spX, nHeight, true);
	MoveWindow(hwndPreview, spX + spBar, 0, nWidth - spX - spBar, spY, true);
	MoveWindow(hwndTitleimg, spX + spBar, spY + spBar, nWidth - spX - spBar, nWidth - spY - spBar, true);
}
void Sp_LBDown(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	SetCapture(hwndMain);
	POINT p;
	p.x = LOWORD(lParam); p.y = HIWORD(lParam);
	if ((fDragVer = PtInRect(GetVerSpBar(), p))) { DrawSpBar(GetVerSpBar()); pold.x = spX; pold.y = spY; return; }
	if ((fDragHor = PtInRect(GetHorSpBar(), p))) { DrawSpBar(GetHorSpBar()); pold.x = spX; pold.y = spY; return; }
	//if(fDragVer||fDragVer) MessageBox(hwnd,"asd","asd",MB_OK);
}
void Sp_LBUp(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	POINT p; p.x = LOWORD(lParam); p.y = HIWORD(lParam);
	RECT rect; GetClientRect(hwnd, &rect);
	if (fDragVer)
	{
		DrawSpBar(GetVerSpBar()); fDragVer = fMoveVer = false;
		SizeWindowContents(rect.right, rect.bottom); ReleaseCapture(); return;
	}
	if (fDragHor)
	{
		DrawSpBar(GetHorSpBar()); fDragHor = fMoveHor = false;
		SizeWindowContents(rect.right, rect.bottom); ReleaseCapture(); return;
	}
}
void Sp_MouseMove(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	POINT p;
	p.x = LOWORD(lParam); p.y = HIWORD(lParam);
	if (wParam & MK_LBUTTON)//only handle mouse LeftButton drag and moved
	{
		if (fDragVer)
		{
			fMoveVer = true;
			spX = pold.x; DrawSpBar(GetVerSpBar());
			spX = p.x; DrawSpBar(GetVerSpBar());
			pold = p; return;
		}
		if (fDragHor)
		{
			fMoveHor = true;
			spY = pold.y; DrawSpBar(GetHorSpBar());
			spY = p.y; DrawSpBar(GetHorSpBar());
			pold = p; return;
		}
	}
}
void DrawSpBar(LPRECT lpRect)
{
	//InvalidateRect(hwndMain,NULL,false);
	HDC hdc;
	hdc = GetDC(hwndMain);
	static WORD _dotPatternBmp[8] =
	{
		0x00aa, 0x0055, 0x00aa, 0x0055,
		0x00aa, 0x0055, 0x00aa, 0x0055
	};

	HBITMAP hbm;
	HBRUSH  hbr, hbrushOld;

	hbm = CreateBitmap(8, 8, 1, 1, _dotPatternBmp);
	hbr = CreatePatternBrush(hbm);

	SetBrushOrgEx(hdc, lpRect->left, lpRect->top, 0);
	hbrushOld = (HBRUSH)SelectObject(hdc, hbr);

	PatBlt(hdc, lpRect->left, lpRect->top, lpRect->right - lpRect->left, lpRect->bottom - lpRect->top, PATINVERT);

	SelectObject(hdc, hbrushOld);

	DeleteObject(hbr);
	DeleteObject(hbm);
	ReleaseDC(hwndMain, hdc);

}
LPRECT GetVerSpBar()
{
	rectSpVer.left = spX;
	rectSpVer.top = 0;
	rectSpVer.right = spX + spBar;
	rectSpVer.bottom = CLIENT_H;
	return &rectSpVer;
}
LPRECT GetHorSpBar()
{
	rectSpHor.left = spX + spBar;
	rectSpHor.top = spY;
	rectSpHor.right = CLIENT_W;
	rectSpHor.bottom = spY + spBar;
	return &rectSpHor;
}

void SetWndCenter(HWND hwnd)
{
	AdjustWindowRect(&cliRect, WS_OVERLAPPEDWINDOW, false);
	int WIN_W = cliRect.right - cliRect.left;
	int WIN_H = cliRect.bottom - cliRect.top;
	cliRect.left = (GetSystemMetrics(SM_CXSCREEN) - WIN_W) / 2;
	cliRect.top = (GetSystemMetrics(SM_CYSCREEN) - WIN_H) / 2;
	cliRect.right = cliRect.left + WIN_W;
	cliRect.bottom = cliRect.top + WIN_H;
	SetWindowPos(hwnd, HWND_TOPMOST, cliRect.left, cliRect.top, WIN_W, WIN_H, SWP_SHOWWINDOW);
}