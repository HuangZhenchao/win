// Win32Project4.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"

#include "Win32Project4.h"
#include "MyTreeView.h"
#include "LabelPanel.h"
#include "MyListView.h"
#include "TreeAndList.h"
#define MAX_LOADSTRING 100

// 全局变量: 
HINSTANCE hInst;                                // 当前实例
WCHAR szProgramName[MAX_LOADSTRING]=L"易元知识管理系统";                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING]=L"YiKMS";            // 主窗口类名
WCHAR szPath_ProData[MAX_PATH];
wstring wsDBPath = L"C:\\ProgramData\\易元知识管理系统\\FileInfo.db";

HWND hWndMain;

HWND hFolderTreeView;
HWND hTagTreeView;
HWND hMyListView;
TREEANDLIST *pTreeAndList;

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

    // 初始化全局字符串
    //LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    //LoadStringW(hInstance, IDC_WIN32PROJECT4, szWindowClass, MAX_LOADSTRING);
	WCHAR szAllUsersProFilePath[MAX_PATH];
	GetEnvironmentVariable(L"ALLUSERSPROFILE", szAllUsersProFilePath, MAX_PATH);
	wsprintf(szPath_ProData, L"%ls\\%ls", szAllUsersProFilePath, szProgramName);
    MyRegisterClass(hInstance);
	
	
    // 执行应用程序初始化: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32PROJECT4));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32PROJECT4));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WIN32PROJECT4);
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
   OleInitialize(NULL);
   
   InitCommonControls();

   hWndMain = CreateWindowW(szWindowClass, szProgramName, WS_OVERLAPPEDWINDOW,
	   CW_USEDEFAULT, 0, 1000, 800, nullptr, nullptr, hInst, nullptr);
   //MessageBox(NULL, L"yangshi", TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
   //pYTreeView->ExpandLastPath();
   //LabelPanel *pLabelPanel = new LabelPanel(hInstance);
   //HWND hLabelPanel = CreateWindow(pLabelPanel->lpszClassName, NULL, WS_CHILD | WS_VISIBLE, 200, 20, 200, 580, hWndMain, (HMENU)pLabelPanel->GetObjectID(), hInst, NULL);

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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
	{
		hFolderTreeView = CreateWindow(L"SysTreeView32", NULL, WS_CHILD | WS_VISIBLE | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_FULLROWSELECT| TVS_SHOWSELALWAYS
			, 0, 0, 0, 0, hWnd, NULL, hInst, NULL);

		hTagTreeView = CreateWindow(L"SysTreeView32", NULL, WS_CHILD | WS_VISIBLE | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_FULLROWSELECT| TVS_SHOWSELALWAYS
			, 0, 0, 0, 0, hWnd, NULL, hInst, NULL);
		hMyListView = CreateWindow(L"SysListView32", NULL, WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SHAREIMAGELISTS,
			0, 0, 0, 0, hWnd, NULL, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		SendMessage(hMyListView, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT| LVS_EX_AUTOSIZECOLUMNS, LVS_EX_FULLROWSELECT| LVS_EX_AUTOSIZECOLUMNS);
		//SendMessage(hMyListView, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_AUTOSIZECOLUMNS, LVS_EX_AUTOSIZECOLUMNS);
		

		//pTreeAndList = new TREEANDLIST(hFolderTreeView, hTagTreeView, hMyListView,wsDBPath);
		

		clock_t start, end;
		start = clock();
		//WaitForMultipleObjects(THREADCOUNT, hEventLoadDone, TRUE, INFINITE);
		for (DWORD i = 1; i < 20000;i++) {
			TVINSERTSTRUCT tvInsert;
			tvInsert.hParent = NULL;
			tvInsert.hInsertAfter = TVI_SORT;

			tvInsert.item.mask = TVIF_TEXT | TCIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_CHILDREN | TVIF_PARAM;
			WCHAR index[20];
			wsprintf(index, L"%dms\r\n", i);
			tvInsert.item.pszText = (LPWSTR)index;// sfi.szDisplayName;
			tvInsert.item.iImage = 0;
			tvInsert.item.iSelectedImage = 0;
			tvInsert.item.cChildren = 0;
			tvInsert.item.lParam = (LPARAM)new FOLDERTREEITEMDATA();
			((FOLDERTREEITEMDATA*)tvInsert.item.lParam)->wsPath = (LPWSTR)index;

			HTREEITEM hNewItem = (HTREEITEM)SendMessage(hFolderTreeView, TVM_INSERTITEM, 0, (LPARAM)&tvInsert);
		}
		end = clock();
		WCHAR szDisplayString[1024];
		wsprintf(szDisplayString, L"加载完成，耗时%dms\r\n", end - start);
		MessageBox(NULL, szDisplayString, TEXT("提示"), MB_OK | MB_ICONINFORMATION);

		wstring wstr = L"Hello #A#2018, Bye ##2017";
		wsmatch result;
		wregex pattern(L"#\\w{0,2}#((?!#).)*");	//匹配四个数字

									//迭代器声明
		wstring::const_iterator iterStart = wstr.begin();
		wstring::const_iterator iterEnd = wstr.end();
		wstring temp;
		while (regex_search(iterStart, iterEnd, result, pattern))
		{
			temp = result[0];
			//MessageBox(NULL, temp.c_str(), TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
			iterStart = result[0].second;	//更新搜索起始位置,搜索剩下的字符串
		}

	}
	break;
	case WM_SIZE:
	{
		RECT rc;
		GetClientRect(hWnd, &rc);
		DWORD dwClientWidth = rc.right - rc.left;
		DWORD dwClientHeight = rc.bottom - rc.top-10;
		MoveWindow(hFolderTreeView, 0, 0, 250, dwClientHeight*2/3,TRUE);
		MoveWindow(hTagTreeView, 0, dwClientHeight * 2 / 3, 250, dwClientHeight/ 3, TRUE);
		MoveWindow(hMyListView, 250, 0, dwClientWidth-250, dwClientHeight, TRUE);
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
	case WM_NOTIFY:
	{
		NMHDR *pHdr = (NMHDR *)lParam;
		if (pHdr->hwndFrom == hFolderTreeView) {
			pTreeAndList->FolderTreeNotifyHandler(pHdr);
		}
		if (pHdr->hwndFrom == hMyListView) {
			pTreeAndList->ListNotifyHandler(pHdr);
		}
		if (pHdr->hwndFrom == hTagTreeView) {
			pTreeAndList->TagTreeNotifyHandler(pHdr);
		}
	}
	break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
		break;
    }
	return DefWindowProc(hWnd, message, wParam, lParam);
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
