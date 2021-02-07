// Win32Project-FileWatch.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Win32Project-FileWatch.h"
#include "FileWatch.h"
#define MAX_LOADSTRING 100

// ȫ�ֱ���: 
HINSTANCE hInst;                                // ��ǰʵ��
WCHAR szTitle[MAX_LOADSTRING];                  // �������ı�
WCHAR szWindowClass[MAX_LOADSTRING];            // ����������
FILEWATCH *pFileWatch;
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
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WIN32PROJECTFILEWATCH, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
	
    // ִ��Ӧ�ó����ʼ��: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32PROJECTFILEWATCH));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32PROJECTFILEWATCH));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WIN32PROJECTFILEWATCH);
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

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
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
		
		pFileWatch = new FILEWATCH();	
		INT iDriverNum = pFileWatch->vDriver.size();
		HWND *hWndEdit = new HWND[iDriverNum];
		for (INT i = 0; i < iDriverNum;i++) {
			hWndEdit[i] = CreateWindow(L"edit",NULL,WS_CHILD|WS_VISIBLE| ES_READONLY| ES_MULTILINE | ES_WANTRETURN|WS_VSCROLL,0,0,0,0,hWnd,NULL,NULL,NULL);
		}
		pFileWatch->hWnd = hWndEdit;
		pFileWatch->Start();
		
		/*
		SHFILEINFO sfi;
		WCHAR *lpszPath = L"E:\\$RECYCLE.BIN";
		ZeroMemory(&sfi, sizeof(SHFILEINFO));
		SHGetFileInfo(lpszPath, 0,&sfi,sizeof(sfi),SHGFI_SYSICONINDEX);*/
		WCHAR szMessageStr[1024];
		wsprintf(szMessageStr,L"ͼ������%d", 0x0001L);
		//MessageBox(NULL, szMessageStr, TEXT("������"), MB_OK | MB_ICONINFORMATION);
		
	}
	break;
	case WM_SIZE:
	{
		RECT rc;
		GetClientRect(hWnd, &rc);
		DWORD dwWidth = rc.right - rc.left;
		DWORD dwHeight = rc.bottom - rc.top;
		DWORD n = pFileWatch->vDriver.size();
		
		DWORD dwSpaceLR = 20;
		DWORD dwSpaceTB = 20;
		DWORD dwSpaceBetween = 10;
		//DWORD dwEditWidth = (dwWidth- dwSpaceLR*2- dwSpaceBetween*(n-1))/n;
		//DWORD dwEditHeight = dwHeight- dwSpaceTB*2;
		DWORD dwEditWidth = dwWidth - dwSpaceLR * 2;
		DWORD dwEditHeight = (dwHeight - dwSpaceTB * 2-dwSpaceBetween*(n-1))/n;
		for (DWORD i = 0; i < n; i++) {
			//DWORD dwEditLeft = dwSpaceLR + dwEditWidth*i + dwSpaceBetween*i;
			//MoveWindow(pFileWatch->hWnd[i], dwEditLeft, dwSpaceTB, dwEditWidth, dwEditHeight,TRUE);

			DWORD dwEditTop = dwSpaceTB + dwEditHeight*i + dwSpaceBetween*i;
			MoveWindow(pFileWatch->hWnd[i], dwSpaceLR, dwEditTop, dwEditWidth, dwEditHeight, TRUE);
		}
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
    case WM_DESTROY:
		//pFileWatch->~FILEWATCH();
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
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
