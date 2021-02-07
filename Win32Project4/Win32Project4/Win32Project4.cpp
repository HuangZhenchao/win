// Win32Project4.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include "Win32Project4.h"
#include "MyTreeView.h"
#include "LabelPanel.h"
#include "MyListView.h"
#include "TreeAndList.h"
#define MAX_LOADSTRING 100

// ȫ�ֱ���: 
HINSTANCE hInst;                                // ��ǰʵ��
WCHAR szProgramName[MAX_LOADSTRING]=L"��Ԫ֪ʶ����ϵͳ";                  // �������ı�
WCHAR szWindowClass[MAX_LOADSTRING]=L"YiKMS";            // ����������
WCHAR szPath_ProData[MAX_PATH];
wstring wsDBPath = L"C:\\ProgramData\\��Ԫ֪ʶ����ϵͳ\\FileInfo.db";

HWND hWndMain;

HWND hFolderTreeView;
HWND hTagTreeView;
HWND hMyListView;
TREEANDLIST *pTreeAndList;

// �˴���ģ���а����ĺ�����ǰ������: 
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

    // TODO: �ڴ˷��ô��롣

    // ��ʼ��ȫ���ַ���
    //LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    //LoadStringW(hInstance, IDC_WIN32PROJECT4, szWindowClass, MAX_LOADSTRING);
	WCHAR szAllUsersProFilePath[MAX_PATH];
	GetEnvironmentVariable(L"ALLUSERSPROFILE", szAllUsersProFilePath, MAX_PATH);
	wsprintf(szPath_ProData, L"%ls\\%ls", szAllUsersProFilePath, szProgramName);
    MyRegisterClass(hInstance);
	
	
    // ִ��Ӧ�ó����ʼ��: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32PROJECT4));

    MSG msg;

    // ����Ϣѭ��: 
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
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
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
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����
   OleInitialize(NULL);
   
   InitCommonControls();

   hWndMain = CreateWindowW(szWindowClass, szProgramName, WS_OVERLAPPEDWINDOW,
	   CW_USEDEFAULT, 0, 1000, 800, nullptr, nullptr, hInst, nullptr);
   //MessageBox(NULL, L"yangshi", TEXT("������"), MB_OK | MB_ICONINFORMATION);
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
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND  - ����Ӧ�ó���˵�
//  WM_PAINT    - ����������
//  WM_DESTROY  - �����˳���Ϣ������
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
		wsprintf(szDisplayString, L"������ɣ���ʱ%dms\r\n", end - start);
		MessageBox(NULL, szDisplayString, TEXT("��ʾ"), MB_OK | MB_ICONINFORMATION);

		wstring wstr = L"Hello #A#2018, Bye ##2017";
		wsmatch result;
		wregex pattern(L"#\\w{0,2}#((?!#).)*");	//ƥ���ĸ�����

									//����������
		wstring::const_iterator iterStart = wstr.begin();
		wstring::const_iterator iterEnd = wstr.end();
		wstring temp;
		while (regex_search(iterStart, iterEnd, result, pattern))
		{
			temp = result[0];
			//MessageBox(NULL, temp.c_str(), TEXT("������"), MB_OK | MB_ICONINFORMATION);
			iterStart = result[0].second;	//����������ʼλ��,����ʣ�µ��ַ���
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
            // �����˵�ѡ��: 
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
            // TODO: �ڴ˴����ʹ�� hdc ���κλ�ͼ����...
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

// �����ڡ������Ϣ�������
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
