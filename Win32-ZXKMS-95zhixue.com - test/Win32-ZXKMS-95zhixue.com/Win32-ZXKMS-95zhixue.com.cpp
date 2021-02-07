// Win32-ZXKMS-95zhixue.com.cpp : 定义应用程序的入口点。
//
#include "stdafx.h"
#include "Win32-ZXKMS-95zhixue.com.h"

#include "MainButtonProc.h"

#include "NW.h"
#include "PKB.h"
#include <Shlwapi.h>
#pragma comment(lib,"shlwapi.lib")
#pragma comment(lib,"Msimg32.lib")
#include "Commdlg.h"
// 此代码模块中包含的函数的前向声明: 
BOOL                InitInstance(HINSTANCE, int);
int					WriteWndIni();					//用来写入Windows配置文件

//入口
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此放置初始化全局变量的代码。
	lfList.lfHeight = 16;
	lfList.lfWidth = 0;
	lfList.lfEscapement = 0;
	lfList.lfOrientation = 0;
	lfList.lfWeight = 5;
	lfList.lfItalic = 0;
	lfList.lfUnderline = 0;
	lfList.lfStrikeOut = 0;
	lfList.lfCharSet = DEFAULT_CHARSET;
	lfList.lfOutPrecision = 0;
	lfList.lfClipPrecision = 0;
	lfList.lfQuality = 0;
	lfList.lfPitchAndFamily = 0;
	lstrcpy(lfList.lfFaceName, _T("楷体"));

	
    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WIN32ZXKMS95ZHIXUECOM, szWindowClass, MAX_LOADSTRING);
	
	//初始化path
	HMODULE hModule = GetModuleHandleW(NULL);
	GetModuleFileName(hModule, szPath, MAX_PATH);
	// setting -> link add Shlwapi.lib   #include <Shlwapi.h>
	PathRemoveFileSpec(szPath);
	//初始化配置文件路径
	wcscpy_s(IniPath, szPath);
	wcscat_s(IniPath, L"\\wnd.ini");

	//从配置文件取出结构体到WndIniRead
	if (!GetPrivateProfileStruct(_T("Windows"), _T("Struct"), &WndIni, sizeof(WndIni), IniPath))
	{
		WndIni.WndMainX = CW_USEDEFAULT;
		WndIni.WndMainY = CW_USEDEFAULT;
		WndIni.WndMainWidth = CW_USEDEFAULT;
		WndIni.WndMainHeight = CW_USEDEFAULT;
	}

	RegWnd(hInstance);
	RegWndMain(hInstance);
	//RegWndHeader(hInstance);

	//RegWndFooter(hInstance);
	RegWndMainButton(hInst);

	RegWnd_NW_Container(hInstance);
	RegWnd_PKB_Container(hInstance);

	//RegWndCatalog(hInstance);
    // 执行应用程序初始化: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32ZXKMS95ZHIXUECOM));
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

   //hWndMain = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_MINIMIZEBOX&~WS_CAPTION,
	//   WndIni.WndMainX, WndIni.WndMainY, WndIni.WndMainWidth, WndIni.WndMainHeight, nullptr, nullptr, hInstance, nullptr);//&~WS_THICKFRAME
   hWndMain = CreateWindowEx(NULL,szWindowClass, szTitle, WS_POPUP | WS_MINIMIZEBOX,
	   WndIni.WndMainX, WndIni.WndMainY, WndIni.WndMainWidth, WndIni.WndMainHeight, NULL, NULL, hInstance, NULL);
   
   if (!hWndMain)
   {
      return FALSE;
   }

   ShowWindow(hWndMain, nCmdShow);
   UpdateWindow(hWndMain);

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


LRESULT CALLBACK WndMainProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
	switch (message)
	{
		
	case WM_CREATE:
	{
		SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE)&~WS_CAPTION);//&~WS_THICKFRAME
		SetWindowPos(hWnd, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_DRAWFRAME);

		hButtonQuit = CreateWindowEx(NULL, L"WndMainButton", NULL, WS_CHILD | WS_VISIBLE,
			0, 0, 0, 0, hWnd, NULL, hInst, NULL);
		hButtonMax = CreateWindowEx(NULL, L"WndMainButton", NULL, WS_CHILD | WS_VISIBLE,
			0, 0, 0, 0, hWnd, NULL, hInst, NULL);
		hButtonMin = CreateWindowEx(NULL, L"WndMainButton", NULL, WS_CHILD | WS_VISIBLE,
			0, 0, 0, 0, hWnd, NULL, hInst, NULL);
		//创建侧边栏窗口
		hItemButton[1] = hButtonNW = CreateWindowEx(NULL, L"WndMainButton", NULL, WS_CHILD | WS_VISIBLE,
			0, 0, 0, 0, hWnd, NULL, hInst, NULL);

		hItemButton[0] = hButtonPKB = CreateWindowEx(NULL, L"WndMainButton", NULL, WS_CHILD | WS_VISIBLE,
			0, 0, 0, 0, hWnd, NULL, hInst, NULL);
		//hWndCurrentContainer = CreateWindowEx(NULL, NW::szClassNameWndContainer,NULL,  WS_CHILD | WS_VISIBLE,
		//	0,0,0,0,hWnd, NULL, hInst, NULL);
		
		hBitmapLogo = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		GetObject(hBitmapLogo, sizeof(BITMAP), &bitmaplogo);
		PostMessage(hItemButton[0], WM_LBUTTONDOWN, wParam, lParam);
	}
	break;
	case WM_SIZE:
	{
		
		RECT rect;
		GetClientRect(hWnd, &rect);
		dWidthClient = rect.right - rect.left;  // 获得客户区宽度
		dHeightClient = rect.bottom - rect.top;  // 获得客户区高度
		dWidthHeader = dWidthClient - dWidthBorder * 2;

		MoveWindow(hButtonQuit, dWidthClient - dHeightHeader- dWidthBorder, dWidthBorder, dHeightHeader, dHeightHeader, TRUE);
		MoveWindow(hButtonMax, dWidthClient - dHeightHeader * 2- dWidthBorder, dWidthBorder, dHeightHeader, dHeightHeader, TRUE);
		MoveWindow(hButtonMin, dWidthClient - dHeightHeader * 3- dWidthBorder, dWidthBorder, dHeightHeader, dHeightHeader, TRUE);
		
		
		MoveWindow(hItemButton[0], dWidthBorder, dHeightHeader+ dWidthBorder, 60, 60, TRUE);
		MoveWindow(hItemButton[1], dWidthBorder, dHeightHeader+ dWidthBorder +60, 60, 60, TRUE);

		MoveWindow(hWndCurrentContainer,
			dWidthBorder+ dWidthSide,
			dHeightHeader + dWidthBorder,
			dWidthClient - dWidthSide- dWidthBorder * 2,
			dHeightClient - dHeightHeader - dHeightFooter - dWidthBorder * 2, TRUE);
				
		//SendMessage(hWndCurrentContainer,WM_Size);
		MoveWindow(hWndFooter, dWidthBorder, dHeightClient - dHeightFooter - dWidthBorder, dWidthClient - dWidthBorder * 2, dHeightFooter, TRUE);
		
	}
	break;
	case WM_GETMINMAXINFO:
		MINMAXINFO *mminfo;
		mminfo = (PMINMAXINFO)lParam;

		mminfo->ptMinTrackSize.x = 1500;
		mminfo->ptMinTrackSize.y = 1000;
		break;
	case WM_MOUSEMOVE:
	{
		POINT pt;
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);
		//ScreenToClient(hWnd, &pt);
		//ClientToScreen(hWnd, &pt);
		RECT rcClient;
		GetClientRect(hWnd, &rcClient);

		if (pt.x < dWidthBorder)
		{
			if (pt.y < dWidthBorder)
			{
				SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENWSE)));
			}
			else if (pt.y < rcClient.bottom - dWidthBorder)
			{
				SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE)));
			}
			else if (pt.y < rcClient.bottom) {
				SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENESW)));
			}
		}
		else if (pt.x < rcClient.right - dWidthBorder)
		{
			if (pt.y < dWidthBorder)
			{ 
				SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENS))); 
			}
			
			else if (pt.y > rcClient.bottom- dWidthBorder)
			{
				SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENS)));
			}
			else{
				SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
			}
		}
		else if (pt.x < rcClient.right) 
		{
			if (pt.y < dWidthBorder)
			{
				SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENESW)));
			}
			else if (pt.y < rcClient.bottom - dWidthBorder)
			{
				SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE)));
			}
			else if (pt.y < rcClient.bottom) 
			{
				SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENWSE)));
			}
		}
	}
	return TRUE;
	
	case WM_LBUTTONDOWN:
	{
		POINT pt;
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);
		//ScreenToClient(hWnd, &pt);
		//ClientToScreen(hWnd, &pt);
		RECT rcClient;
		GetClientRect(hWnd, &rcClient);

		if (pt.x < dWidthBorder)
		{
			if (pt.y < dWidthBorder)
			{
				SendMessage(hWnd, WM_NCLBUTTONDOWN, HTTOPLEFT, MAKELPARAM(pt.x, pt.y));
			}
			else if (pt.y < rcClient.bottom - dWidthBorder)
			{
				SendMessage(hWnd, WM_NCLBUTTONDOWN, HTLEFT, MAKELPARAM(pt.x, pt.y));
				
			}
			else if (pt.y < rcClient.bottom) {
				SendMessage(hWnd, WM_NCLBUTTONDOWN, HTBOTTOMLEFT, MAKELPARAM(pt.x, pt.y));
			}
		}
		else if (pt.x < rcClient.right - dWidthBorder)
		{
			if (pt.y < dWidthBorder)
			{
				SendMessage(hWnd, WM_NCLBUTTONDOWN, HTTOP, MAKELPARAM(pt.x, pt.y));
			}
			
			else if (pt.y > rcClient.bottom- dWidthBorder)
			{
				SendMessage(hWnd, WM_NCLBUTTONDOWN, HTBOTTOM, MAKELPARAM(pt.x, pt.y));
			}
			else {
				SendMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
			}
		}
		else if (pt.x < rcClient.right)
		{
			if (pt.y < dWidthBorder)
			{
				SendMessage(hWnd, WM_NCLBUTTONDOWN, HTTOPRIGHT, MAKELPARAM(pt.x, pt.y));
			}
			else if (pt.y < rcClient.bottom - dWidthBorder)
			{
				SendMessage(hWnd, WM_NCLBUTTONDOWN, HTRIGHT, MAKELPARAM(pt.x, pt.y));
			}
			else if (pt.y < rcClient.bottom)
			{
				SendMessage(hWnd, WM_NCLBUTTONDOWN, HTBOTTOMRIGHT, MAKELPARAM(pt.x, pt.y));
			}
		}	
	}
	return TRUE;
	case WM_MOUSEHOVER:
	{
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		
		HDC         hdcMem;
		hdcMem = CreateCompatibleDC(hdc);
		SelectObject(hdcMem, hBitmapLogo);


		TransparentBlt(hdc, 5, 0, 50, 50, hdcMem, 0, 0, bitmaplogo.bmWidth, bitmaplogo.bmHeight, RGB(255, 255, 255));
		DeleteDC(hdcMem);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_SETCURSOR:              //拦截了WM_SETCURSOR消息
		break;
		
    case WM_DESTROY:
		WriteWndIni();
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

int WriteWndIni() 
{
	RECT rect;
	GetWindowRect(hWndMain, &rect);
	int widthWndMain = rect.right - rect.left;  // 获得主窗体宽度
	int heightWndMain = rect.bottom - rect.top;  // 获得主窗体高度
	WndIni.WndMainX = rect.left;
	WndIni.WndMainY = rect.top;
	WndIni.WndMainWidth = widthWndMain;
	WndIni.WndMainHeight = heightWndMain;
	return WritePrivateProfileStruct(_T("Windows"), _T("Struct"), &WndIni, sizeof(WndIni), IniPath);
	
}

/*
VOID CALLBACK TimerProc(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime)
{

}

SetTimer(hwnd, iTimerID, iMsecInterval, TimerProc);*/