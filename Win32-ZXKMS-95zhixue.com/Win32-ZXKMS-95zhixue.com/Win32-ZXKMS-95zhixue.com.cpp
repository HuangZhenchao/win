// Win32-ZXKMS-95zhixue.com.cpp : 定义应用程序的入口点。
//
#include "stdafx.h"
#include "Win32-ZXKMS-95zhixue.com.h"
#include "NW.h"
#include "PKB.h"
#include <Shlwapi.h>
#pragma comment(lib,"shlwapi.lib")



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
	RegWndSide(hInstance);
	
	NW::RegWndContainer(hInstance);
	RegWndSideItem(hInstance);

	PKB::RegWndContainer(hInstance);

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

   hWndMain = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	   WndIni.WndMainX, WndIni.WndMainY, WndIni.WndMainWidth, WndIni.WndMainHeight, nullptr, nullptr, hInstance, nullptr);

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
		//创建侧边栏窗口
		hWndSide = CreateWindowEx(NULL, L"WndSide", L"WndSide", WS_CHILD | WS_VISIBLE,
			0, 0, 50, WndIni.WndMainHeight, hWnd, NULL, hInst, NULL);
		
		//创建hWndSideItem[],hWndContainer[]
		
		hWndSideItem[0] = CreateWindowEx(NULL, L"WndSideItem", NULL, WS_CHILD | WS_VISIBLE,
			0, 0, 50, 50, hWndSide, NULL, hInst, NULL);
		hWndSideItem[1] = CreateWindowEx(NULL, L"WndSideItem", NULL, WS_CHILD | WS_VISIBLE,
			0, 50, 50, 50, hWndSide, NULL, hInst, NULL);
		//hWndContainer[i] = CreateWindowEx(NULL, L"Wnd", NULL, WS_CHILD | WS_VISIBLE,
		//	50+50*i, 0, 50, 50 + 50 * i, hWnd, NULL, hInst, NULL);
		
		hWndContainer[0] = CreateWindowEx(NULL, L"WndContainer1", NULL, WS_CHILD | WS_VISIBLE,
			50 + 50 * 1, 0, 50, 50 + 50 * 1, hWnd, NULL, hInst, NULL);
		
		//ShowWindow(hWndContainer[i], SW_SHOW);
		SetWindowLongPtr(hWndSideItem[0], 0, 1);
		hWndCurrentContainer= hWndContainer[0];
		hWndCurrentItem = hWndSideItem[0];
		//HBRUSH hBrushSelectedItem = CreateSolidBrush(RGB(54, 78, 111));
		//SetClassLong(hWndCurrentItem, GCL_HBRBACKGROUND, (LONG)hBrushSelectedItem);
		/*
		VOID CALLBACK TimerProc(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime)
		{

		}

		SetTimer(hwnd, iTimerID, iMsecInterval, TimerProc);*/
	}
	case WM_SIZE:
		{
			RECT rect;
			GetClientRect(hWnd,&rect);
			widthClient = rect.right - rect.left;  // 获得客户区宽度
			heightClient = rect.bottom - rect.top;  // 获得客户区高度
			MoveWindow(hWndSide,0, 0, widthSide, heightClient,TRUE);
			//MoveWindow(hWndSideItemPKB, 0, 0, 50, 50, TRUE);
			MoveWindow(hWndCurrentContainer, widthSide, 0, widthClient-widthSide, heightClient, TRUE);
			for (DWORD i = 0; i < ItemCount; i++) {
				if (hWndContainer[i] == hWndCurrentContainer) {
					
				}
			}
		}
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            EndPaint(hWnd, &ps);
        }
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
//侧边栏按钮消息处理程序。
LRESULT CALLBACK WndSideItemProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_CREATE:
	{
		SetWindowLongPtr(hWnd, 0, 0);
		//MessageBox(NULL, TEXT("DLL_PROCESS_ATTACH"), TEXT("DLL_PROCESS_ATTACH"), MB_OK | MB_ICONINFORMATION);
	}
	break;
	case WM_SIZE:
	{
		MoveWindow(hWndSideItem[0], 0, 0, 50, 50, TRUE);
		MoveWindow(hWndSideItem[1], 0, 50, 50, 50, TRUE);
	}
	break;
	case WM_LBUTTONDOWN:
	{
		//遍历创建右边容器，以className为区分发送到各自的处理函数，szClassName[]

		//if (GetWindowLongPtr(hWnd, 0) == 0)
		{/*
		 
		 WCHAR tempStr1[10];
		 _itow_s(GetLastError(), tempStr1, 10);
		 MessageBox(hWnd, tempStr1, TEXT("错误码"), MB_OK | MB_ICONINFORMATION);*/
			DestroyWindow(hWndCurrentContainer);
			SetWindowLongPtr(hWndCurrentItem, 0, 0);
			/*
			HBRUSH hBrushSelectedItem = CreateSolidBrush(RGB(42, 51, 60));
			SetClassLong(hWndCurrentItem, GCL_HBRBACKGROUND, (LONG)hBrushSelectedItem);*/
			ItemHoverStatus = 0;
			InvalidateRect(hWndCurrentItem, NULL, TRUE);
			
			hWndCurrentItem = hWnd;
			/*
			hBrushSelectedItem = CreateSolidBrush(RGB(54, 78, 111));
			SetClassLong(hWndCurrentItem, GCL_HBRBACKGROUND, (LONG)hBrushSelectedItem);*/
			InvalidateRect(hWndCurrentItem, NULL, TRUE);
			
			hWndCurrentContainer = CreateWindowEx(NULL, L"WndContainer1", NULL, WS_CHILD | WS_VISIBLE,
				0, 0, 0, 0, hWndMain, NULL, hInst, NULL);
			MoveWindow(hWndCurrentContainer, widthSide, 0, widthClient - widthSide, heightClient, TRUE);
			//ShowWindow(hWndContainer[i], SW_SHOW);
			SetWindowLongPtr(hWndCurrentItem, 0, 1);
			//MoveWindow(hWndContainer[i], 50, 0, 500, 500, TRUE);
		}
	}
	break;
	case WM_MOUSEMOVE:
	{
		if (!bMouseTrack){
			TRACKMOUSEEVENT tme;
			tme.cbSize = sizeof(tme);
			tme.hwndTrack = hWnd;
			tme.dwFlags = TME_LEAVE | TME_HOVER;
			tme.dwHoverTime = 30;
			TrackMouseEvent(&tme);

			bMouseTrack = TRUE;
		}
	}
	break;
	case WM_MOUSELEAVE:
	{
		
		bMouseTrack = FALSE;
		ItemHoverStatus = 0;
		InvalidateRect(hWnd, NULL, TRUE);
	}
	break;
	case WM_MOUSEHOVER:
	{
		ItemHoverStatus = 1;
		InvalidateRect(hWnd, NULL, TRUE);
		//ItemHoverStatus = 0;
	}
	break;
	case WM_PAINT:
	{
		
		// TODO: 在此处添加使用 hdc 的任何绘图代码...
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		if (GetWindowLongPtr(hWnd, 0) == 1) {
			ItemSelectedStatus=1;
		}
		else {
			if (ItemHoverStatus == 1) {
				ItemSelectedStatus = 2;
				//ItemHoverStatus = 0;
			}
			else {
				ItemSelectedStatus = 0;
			}
		}
		/*
		WCHAR tempStr1[10];
		//_itow_s(GetLastError(), tempStr1, 10);
		_itow_s(ItemSelectedStatus, tempStr1, 10);
		MessageBox(hWnd, tempStr1, TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
		*/
		if (ItemSelectedStatus == 0) {
			hBrushItem = CreateSolidBrush(RGB(42, 51, 60));
		}
		if (ItemSelectedStatus == 1) {
			hBrushItem = CreateSolidBrush(RGB(54, 78, 111));
		}
		if (ItemSelectedStatus == 2) {
			hBrushItem = CreateSolidBrush(RGB(202, 81, 0));
		}
		
		RECT rect;
		SetRect(&rect, 0, 0, 50, 50);
		FillRect(hdc, &rect, hBrushItem);
		//Rectangle(hdc,0,0,50,50);
		//InvalidateRect(hWndCurrentItem, NULL, TRUE);
		SelectObject(hdc, hBrushIcon);
		Ellipse(hdc, 10, 10, 40, 40);
		//DeleteObject(hbrush);
		
		EndPaint(hWnd, &ps);
		
	}
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

