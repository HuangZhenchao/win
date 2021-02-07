// Win32-ZXKMS-95zhixue.com.cpp : ����Ӧ�ó������ڵ㡣
//
#include "stdafx.h"
#include "Win32-ZXKMS-95zhixue.com.h"
//#include "NovelWriter.h"
#include <Shlwapi.h>
#pragma comment(lib,"shlwapi.lib")



// �˴���ģ���а����ĺ�����ǰ������: 
BOOL                InitInstance(HINSTANCE, int);
int					WriteWndIni();					//����д��Windows�����ļ�
void LoadDLLFileInDirectory();

//���
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: �ڴ˷��ó�ʼ��ȫ�ֱ����Ĵ��롣

    // ��ʼ��ȫ���ַ���
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WIN32ZXKMS95ZHIXUECOM, szWindowClass, MAX_LOADSTRING);

	//��ʼ��path
	HMODULE hModule = GetModuleHandleW(NULL);
	GetModuleFileName(hModule, szPath, MAX_PATH);
	// setting -> link add Shlwapi.lib   #include <Shlwapi.h>
	PathRemoveFileSpec(szPath);
	//��ʼ�������ļ�·��
	wcscpy_s(IniPath, szPath);
	wcscat_s(IniPath, L"\\wnd.ini");

	//�������ļ�ȡ���ṹ�嵽WndIniRead
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
	RegWndSideItem(hInstance);
	//RegWndCatalog(hInstance);
    // ִ��Ӧ�ó����ʼ��: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }
	
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32ZXKMS95ZHIXUECOM));

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
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND  - ����Ӧ�ó���˵�
//  WM_PAINT    - ����������
//  WM_DESTROY  - �����˳���Ϣ������
//
//
DWORD ItemCount = 0;

LRESULT CALLBACK WndMainProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
	{
		//�������������
		hWndSide = CreateWindowEx(NULL, L"WndSide", L"WndSide", WS_CHILD | WS_VISIBLE,
			0, 0, 50, WndIni.WndMainHeight, hWnd, NULL, hInst, NULL);
		//����DLL�ļ����õ������Item��Ŀ
		LoadDLLFileInDirectory();
		//����hWndSideItem[],hWndContainer[]
		
		for (DWORD i = 0; i <ItemCount;i++) {
			hWndSideItem[i] = CreateWindowEx(NULL, L"WndSideItem", NULL, WS_CHILD | WS_VISIBLE,
				0, 50 * i, 50, 50, hWndSide, NULL, hInst, NULL);
			
			//hWndContainer[i] = CreateWindowEx(NULL, L"Wnd", NULL, WS_CHILD | WS_VISIBLE,
			//	50+50*i, 0, 50, 50 + 50 * i, hWnd, NULL, hInst, NULL);
		}
		SetWindowLongPtr(hWndSideItem[0], 0, 1);
		//hWndCurrentContainer = hWndContainer[0];
		hWndContainer[0] = CreateWindowEx(NULL, L"Wnd", NULL, WS_CHILD | WS_VISIBLE,
				0, 0, 0, 0, hWnd, NULL, hInst, NULL);
		hWndCurrentContainer = hWndContainer[0];
		hWndCurrentItem = hWndSideItem[0];
		//MoveWindow(hWndCurrentContainer, widthSide, 0, widthClient - widthSide, heightClient, TRUE);
		dllfnCreateWndContainer[0](hInst, hWndCurrentContainer, 0);
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
			widthClient = rect.right - rect.left;  // ��ÿͻ������
			heightClient = rect.bottom - rect.top;  // ��ÿͻ����߶�
			MoveWindow(hWndSide,0, 0, widthSide, heightClient,TRUE);
			//MoveWindow(hWndSideItemPKB, 0, 0, 50, 50, TRUE);
			
			MoveWindow(hWndCurrentContainer, widthSide, 0, widthClient-widthSide, heightClient, TRUE);
			for (DWORD i = 0; i < ItemCount; i++) {
				if (hWndContainer[i] == hWndCurrentContainer) {
					dllfnReSize[i](hWndCurrentContainer);
					WCHAR tempStr1[10];
					_itow_s(GetLastError(), tempStr1, 10);
					MessageBox(hWnd, tempStr1, TEXT("��"), MB_OK | MB_ICONINFORMATION);
				}
			}
		}
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: �ڴ˴����ʹ�� hdc ���κλ�ͼ����...
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

//�������ť��Ϣ�������
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

	}
	break;
	case WM_LBUTTONDOWN:
	{
		for (DWORD i = 0; i < ItemCount; i++) 
		{
			if (hWndSideItem[i] == hWnd) 
			{
				
				if (GetWindowLongPtr(hWndSideItem[i], 0)==1) 
				{
					continue;
				}
				else 
				{	
					DestroyWindow(hWndCurrentContainer);
					SetWindowLongPtr(hWndCurrentItem, 0, 0);
					hWndCurrentItem = hWndSideItem[i];

					hWndContainer[i] = CreateWindowEx(NULL, L"Wnd", NULL, WS_CHILD | WS_VISIBLE,
						0, 0, 0, 0, hWndMain, NULL, hInst, NULL);
					hWndCurrentContainer = hWndContainer[i];
					WCHAR tempStr1[10];
					_itow_s(GetLastError(), tempStr1, 10);
					MessageBox(hWnd, tempStr1, TEXT("������"), MB_OK | MB_ICONINFORMATION);
					//hWndCurrentContainer = hWndContainer[i];
					MoveWindow(hWndCurrentContainer, widthSide, 0, widthClient - widthSide, heightClient, TRUE);
					dllfnCreateWndContainer[i](hInst, hWndCurrentContainer,i);
					//ShowWindow(hWndContainer[i], SW_SHOW);
					SetWindowLongPtr(hWndSideItem[i], 0, 1);
					//MoveWindow(hWndContainer[i], 50, 0, 500, 500, TRUE);
					
				}
			}
			
			
		}
	}
	break;
	case WM_PAINT:
	{
		for (DWORD i = 0; i < ItemCount; i++) {
			if (hWndSideItem[i] == hWnd) {
				
				// TODO: �ڴ˴����ʹ�� hdc ���κλ�ͼ����...
				dllfnPaintItemIcon[i](hWnd);
				
			}
		}
	}
	break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void LoadDLLFileInDirectory()
{
	//FreeLibrary(hDll);

	WIN32_FIND_DATA FindFileData;
	WCHAR szDLLFilePath[MAX_PATH];
	// ���������Ŀ¼���ļ���·�����ַ�����ʹ��ͨ���"*" 
	lstrcpy(szDLLFilePath, szPath);
	//TODO:�������޸�DLL�ļ��У���������exe·����
	// ע�͵Ĵ���������ڲ��������ԡ�.txt����β���ļ� 
	lstrcat(szDLLFilePath, L"\\*.dll");
	// lstrcat(szFilePath, L"\\*");

	// ���ҵ�һ���ļ�/Ŀ¼����ò��Ҿ�� 
	HANDLE hListFile = FindFirstFile(szDLLFilePath, &FindFileData);
	// �жϾ�� 
	if (hListFile == INVALID_HANDLE_VALUE)
	{
		wchar_t tempStr[10];
		_itow_s(GetLastError(), tempStr, 10);
		MessageBox(NULL, TEXT("������"), tempStr, MB_OK | MB_ICONINFORMATION);
	}
	else
	{
		do {
			WCHAR szDLLFileName[MAX_PATH];
			lstrcpy(szDLLFileName, szPath);
			lstrcat(szDLLFileName, L"\\");
			lstrcat(szDLLFileName, FindFileData.cFileName);
			// ��ӡ�ļ�����Ŀ¼�� 
			WCHAR DebugOutputField[] = L"����";
			WCHAR DebugOutputInfo[] = L"���Զ�ȡ�ļ�";
			WCHAR DebugOutputFilePath[] = L"D:\\DebugOutputFile.ini";
			WritePrivateProfileString(_T("Debug"), DebugOutputField, szDLLFileName, DebugOutputFilePath);

			hDll[ItemCount] = LoadLibrary(szDLLFileName);
			dllfnPaintItemIcon[ItemCount] = (DLLFNPAINTITEMICON)GetProcAddress(hDll[ItemCount], "fnInitDll");
			dllfnCreateWndContainer[ItemCount] = (DLLFNCREATEWNDCONTAINER)GetProcAddress(hDll[ItemCount], "fnCreateWndContainer");
			dllfnReSize[ItemCount]=(DLLFNRESIZE)GetProcAddress(hDll[ItemCount], "fnReSize");
			ItemCount = ItemCount + 1;
		} while (FindNextFile(hListFile, &FindFileData));
	}
}


int WriteWndIni() 
{
	RECT rect;
	GetWindowRect(hWndMain, &rect);
	int widthWndMain = rect.right - rect.left;  // �����������
	int heightWndMain = rect.bottom - rect.top;  // ���������߶�
	WndIni.WndMainX = rect.left;
	WndIni.WndMainY = rect.top;
	WndIni.WndMainWidth = widthWndMain;
	WndIni.WndMainHeight = heightWndMain;
	return WritePrivateProfileStruct(_T("Windows"), _T("Struct"), &WndIni, sizeof(WndIni), IniPath);
	
}

