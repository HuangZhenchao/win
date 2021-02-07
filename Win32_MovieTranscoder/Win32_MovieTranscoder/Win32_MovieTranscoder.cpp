// Win32_MovieTranscoder.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Win32_MovieTranscoder.h"

#define MAX_LOADSTRING 100

// ȫ�ֱ���: 
HINSTANCE hInst;                                // ��ǰʵ��
WCHAR szTitle[MAX_LOADSTRING];                  // �������ı�
WCHAR szWindowClass[MAX_LOADSTRING];            // ����������
HWND hWndDisplay;
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
    LoadStringW(hInstance, IDC_WIN32_MOVIETRANSCODER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ִ��Ӧ�ó����ʼ��: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32_MOVIETRANSCODER));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32_MOVIETRANSCODER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_GRAYTEXT +1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WIN32_MOVIETRANSCODER);
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
	case WM_CREATE: {
		hWndDisplay = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_WANTRETURN | WS_VSCROLL, 20, 20, 400, 100, hWnd, NULL, hInst, NULL);
		CreateWindow(L"Button", L"��ťһ", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			435, 20, 60,30, hWnd, (HMENU)IDB_Start, hInst, NULL);
	}break;
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
			case IDB_Start: 
			{
				clock_t start,end;
				vector<wstring> vFileName;
				wstring wsSourceFolder = L"D:\\��Ƶͬ���ļ���\\�ձ�AV����\\1pon";
				wstring wsTargetFolder = L"E:\\target";
				wstring wsFind = wsSourceFolder;
				wsFind.append(L"\\*");
				WIN32_FIND_DATA FindFileData;
				HANDLE hListFile = FindFirstFile(wsFind.c_str(), &FindFileData);
					// �жϾ�� 
				if (!(hListFile == INVALID_HANDLE_VALUE))
				{
					do {

						if (wcscmp(FindFileData.cFileName, L".") == 0 | wcscmp(FindFileData.cFileName, L"..") == 0) {
							continue;
						}
						vFileName.push_back(FindFileData.cFileName);
						
						//wstring wsFilePath;
						//wsFilePath.append(wsSourceFolder); wsFilePath.append(L"\\"); wsFilePath.append(FindFileData.cFileName);

					} while (FindNextFile(hListFile, &FindFileData));
				}

				for (int i = 0; i < vFileName.size();i++) {
					WCHAR szDisplayString[MAX_PATH + 20];
					wsprintf(szDisplayString, L"%d/%d,%ls...\r\n", i+1, vFileName.size(), vFileName[i].c_str());
					SendMessage(hWndDisplay, EM_SETSEL, -2, -1);					
					SendMessage(hWndDisplay, EM_REPLACESEL, 0, (LPARAM)szDisplayString);
					
					wstring wsSourceFilePath= wsSourceFolder;
					wsSourceFilePath.append(L"\\"); wsSourceFilePath.append(vFileName[i]);
					wstring wsTargetFilePath = wsTargetFolder;
					wsTargetFilePath.append(L"\\"); wsTargetFilePath.append(vFileName[i]);
					ifstream inFile(wsSourceFilePath.c_str(), ios::in | ios::binary);
					ofstream outFile(wsTargetFilePath.c_str(), ios::out | ios::binary);
					char sTemp[1024];
					while (inFile.read((char *)&sTemp, sizeof(sTemp))) { //һֱ�����ļ�����
						int readedBytes = inFile.gcount(); //���ղŶ��˶����ֽ�
						sTemp[0] = ~sTemp[0];
						wsprintf(szDisplayString, L"��ȡ��%#x\r\n", sTemp[0]);
						//MessageBox(NULL, szDisplayString, TEXT("������"), MB_OK | MB_ICONINFORMATION);
						outFile.write((char*)&sTemp, readedBytes);
					}
					inFile.close();
					outFile.close();
				}
			
			}
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
