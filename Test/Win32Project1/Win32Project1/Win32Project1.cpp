// Win32Project1.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Win32Project1.h"

#define MAX_LOADSTRING 100

// ȫ�ֱ���: 
HINSTANCE hInst;                                // ��ǰʵ��
WCHAR szTitle[MAX_LOADSTRING];                  // �������ı�
WCHAR szWindowClass[MAX_LOADSTRING];            // ����������

// �˴���ģ���а����ĺ�����ǰ������: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

HWND hwndMain;
HWND hwndEdit;
HWND hwndPreview;
HWND hwndTitleimg;
static int CLIENT_W = 800;//�����ڿͻ�����
static int CLIENT_H = 600;//�����ڿͻ�����
static RECT cliRect = { 0,0,CLIENT_W,CLIENT_H };//�����ڿͻ�������λ��
static int spX = 550;//ˮƽ�ָ�����ʼλ��
static int spY = 150;//��ֱ�ָ�����ʼλ��
static int spBar = 5;//�ָ����ĺ��
static RECT rectSpVer = { spX,0,spX + spBar,CLIENT_H };//��ֱ�ָ�������λ��
static RECT rectSpHor = { spX + spBar,spY,CLIENT_W,spY + spBar };//ˮƽ�ָ�������λ��
bool fDragVer = false; bool fMoveVer = false;//��ֱ�ָ����Ƿ���/�϶�
bool fDragHor = false; bool fMoveHor = false;//ˮƽ�ָ����Ƿ���/�϶�
POINT pold;//ע�� ����ɵ����POINTλ�� ����һ��Ҫ�� ��û��WM_PAINT�ػ�����������Ҫ����֮ǰ���Ƶ���Ӱ������PATINVERT��ʽ���»���һ��

		   //ȫ�ֺ���

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);//���ڹ��̺���
void InitHwndChild(HWND hwnd);//��ʼ�������Ӵ������� ��WM_CREATE��������
void SizeWindowContents(int nWidth, int nHeight);//���������Ӵ���size���� ��WM_SIZE������ ����Ϊ�ͻ������
void Sp_LBDown(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);//�ָ���������갴�� WM_LBUTTONDOWN������
void Sp_LBUp(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);//�ָ�����������ͷ� WM_LBUTTONUP������
void Sp_MouseMove(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);//�ָ�����������ƶ� WM_MOUSEMOVE������
void DrawSpBar(LPRECT lpRect);//����ģ��ָ�������Ӱ ����ΪҪ���Ƶķָ�������λ��
LPRECT GetVerSpBar();//�������λ������µĴ�ֱ�ָ���� ���ڻ��Ʒָ���ʱ
LPRECT GetHorSpBar();//�������λ������µ�ˮƽ�ָ���� ���ڻ��Ʒָ���ʱ

void SetWndCenter(HWND hwnd);//���ݿͻ�����С���������ڴ�С������

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
    LoadStringW(hInstance, IDC_WIN32PROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ִ��Ӧ�ó����ʼ��: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32PROJECT1));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32PROJECT1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WIN32PROJECT1);
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