// Win32Project-FileWatch.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Win32Project-FileWatch.h"
#include "FileWatch.h"
//#include "FileTag.h"
#define MAX_LOADSTRING 100

// 全局变量: 
HINSTANCE hInst;                                // 当前实例
WCHAR szProgramName[MAX_LOADSTRING]=L"易元知识管理系统";                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING]=L"YiFileWatch";            // 主窗口类名
WCHAR szPath_ProData[MAX_PATH];
HWND hWndDisplay;
FILEWATCH *pFileWatch;

// 此代码模块中包含的函数的前向声明: 
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

    // TODO: 在此放置代码。
	WCHAR szAllUsersProFilePath[MAX_PATH];
	GetEnvironmentVariable(L"ALLUSERSPROFILE", szAllUsersProFilePath, MAX_PATH);
	wsprintf(szPath_ProData, L"%ls\\%ls", szAllUsersProFilePath, szWindowClass);
	CreateDirectory(szPath_ProData, NULL);
    MyRegisterClass(hInstance);
	
    // 执行应用程序初始化: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32PROJECTFILEWATCH));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32PROJECTFILEWATCH));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WIN32PROJECTFILEWATCH);
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

   HWND hWnd = CreateWindowW(szWindowClass, szProgramName, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 800, 800, nullptr, nullptr, hInstance, nullptr);
   //FileWatch.Start();
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
		hWndDisplay = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | ES_READONLY | ES_MULTILINE | ES_WANTRETURN | WS_VSCROLL, 0, 0, 0, 0, hWnd, NULL, NULL, NULL);
		pFileWatch = new FILEWATCH(hWndDisplay,szPath_ProData);	
		/*
		
		WCHAR *lpszPath = L"E:\\$RECYCLE.BIN";
		ZeroMemory(&sfi, sizeof(SHFILEINFO));
		SHGetFileInfo(lpszPath, 0,&sfi,sizeof(sfi),SHGFI_SYSICONINDEX);*/
		SHFILEINFO sfi;
		wstring wsFileType = L"*.txt";
		SHGetFileInfo(wsFileType.c_str(), FILE_ATTRIBUTE_NORMAL, &sfi, sizeof(sfi), SHGFI_SYSICONINDEX | SHGFI_USEFILEATTRIBUTES);
		WCHAR szMessageStr[1024];
		wsprintf(szMessageStr,L"图标索引%d", sfi.iIcon);
		MessageBox(NULL, szMessageStr, TEXT("错误码"), MB_OK | MB_ICONINFORMATION);

	}
	break;
	case WM_SIZE:
	{
		RECT rc;
		GetClientRect(hWnd, &rc);
		DWORD dwWidth = rc.right - rc.left;
		DWORD dwHeight = rc.bottom - rc.top;		
		DWORD dwSpaceLR = 20;
		DWORD dwSpaceTB = 20;

		DWORD dwEditWidth = dwWidth - dwSpaceLR * 2;
		DWORD dwEditHeight = dwHeight - dwSpaceTB * 2;
		MoveWindow(hWndDisplay, dwSpaceLR, dwSpaceTB, dwEditWidth, dwEditHeight, TRUE);
	}
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
		//pFileWatch->~FILEWATCH();
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
