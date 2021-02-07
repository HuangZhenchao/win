// Win32_MovieTranscoder.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Win32_MovieTranscoder.h"

#define MAX_LOADSTRING 100

// 全局变量: 
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
HWND hWndDisplay;
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
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WIN32_MOVIETRANSCODER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32_MOVIETRANSCODER));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32_MOVIETRANSCODER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_GRAYTEXT +1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WIN32_MOVIETRANSCODER);
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
	case WM_CREATE: {
		hWndDisplay = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_WANTRETURN | WS_VSCROLL, 20, 20, 400, 100, hWnd, NULL, hInst, NULL);
		CreateWindow(L"Button", L"按钮一", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			435, 20, 60,30, hWnd, (HMENU)IDB_Start, hInst, NULL);
	}break;
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
			case IDB_Start: 
			{
				clock_t start,end;
				vector<wstring> vFileName;
				wstring wsSourceFolder = L"D:\\视频同步文件夹\\日本AV无码\\1pon";
				wstring wsTargetFolder = L"E:\\target";
				wstring wsFind = wsSourceFolder;
				wsFind.append(L"\\*");
				WIN32_FIND_DATA FindFileData;
				HANDLE hListFile = FindFirstFile(wsFind.c_str(), &FindFileData);
					// 判断句柄 
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
					while (inFile.read((char *)&sTemp, sizeof(sTemp))) { //一直读到文件结束
						int readedBytes = inFile.gcount(); //看刚才读了多少字节
						sTemp[0] = ~sTemp[0];
						wsprintf(szDisplayString, L"读取了%#x\r\n", sTemp[0]);
						//MessageBox(NULL, szDisplayString, TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
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
