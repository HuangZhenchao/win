#include "stdafx.h"
#include "Win32Project2.h"
#include "global.h"
#include "base/function.h"
#include "Control/TreeViewControl.h"
//#include "Control/ScrollPanelControl.h"
#include "Control/LabelControl.h"
#include "OLE/OLEDragDrop.h"
#define MAX_LOADSTRING 100

// 全局变量: 
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
SCROLLPANELCONTROLINFO *scrollPanelControlInfo;

TREEVIEWINFO *tvInfo;
HWND hWndTreeView;
HWND hWndLabel;
UINT ID_TIMER;
// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
VOID CALLBACK TimerProc(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime);
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此放置代码。
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
    LoadStringW(hInstance, IDC_WIN32PROJECT2, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32PROJECT2));

    MSG msg;

    // 主消息循环: 
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
			if (msg.message == WM_MOUSEWHEEL)
			{			
				POINT pos;
				GetCursorPos(&pos);
				msg.hwnd = WindowFromPoint(pos); //要在TranslateMessage 函数前面
			}
			/*
			if (msg.message == WM_LBUTTONDOWN) {
				ID_TIMER=SetTimer(msg.hwnd, 1, 1000, TimerProc);
			}
			if (msg.message == WM_MOUSEMOVE) {				
				if (bDragStatus) {
					
					//此处可以在初次进入时加一个窗口句柄，每次与他进行比较，相同时不进入下一步骤，可避免重复计算和设置鼠标
					if (IdDataBinding.count(GetWindowLongPtr(msg.hwnd, GWLP_ID)) == 1) {
						PWNDDATA* pDropWndData= (PWNDDATA *)IdDataBinding[GetWindowLongPtr(msg.hwnd, GWLP_ID)];
						PWNDDATA* pDragWndData = (PWNDDATA *)IdDataBinding[dwDragWndID];
						if (pDropWndData->dwDragType&pDragWndData->dwDropType) {
							SetCursor(LoadCursor(NULL, IDC_ARROW));
						}
						//还有要进行路径判断，不能把父目录移动到子目录
						else { SetCursor(LoadCursor(NULL, IDC_NO)); }
					}
					else { SetCursor(LoadCursor(NULL, IDC_NO)); }
					//SetCursor(LoadCursor(NULL, IDC_ARROW));
				}
			}
			if (msg.message == WM_LBUTTONUP) {
				KillTimer(msg.hwnd, ID_TIMER);
				if (bDragStatus) {
					dwDropWndID = GetWindowLongPtr(msg.hwnd, GWLP_ID);
					PostMessage(hWndMain, WM_CUSTOM_TVN, TVN_REPATH, NULL);
				}
				bDragStatus = FALSE;
			}
			*/
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

VOID CALLBACK TimerProc(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime) {
	KillTimer(hwnd, ID_TIMER);
	
	/*
	//if(GetWindowLongPtr(hwnd, GWLP_HWNDPARENT)==NULL|GetWindowLongPtr(hwnd, GWLP_ID)==NULL){ 
	//MessageBox(NULL, L"鼠标按下计时", TEXT("错误码"), MB_OK | MB_ICONINFORMATION); 
	if (IdDataBinding.count(GetWindowLongPtr(hwnd, GWLP_ID)) == 1&& (((PWNDDATA *)IdDataBinding[GetWindowLongPtr(hwnd, GWLP_ID)])->dwDragType>0)) {
		//MessageBox(NULL, L"该窗口允许拖拽", TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
		//把拖拽状态设为1
		bDragStatus = TRUE;
		dwDragWndID = GetWindowLongPtr(hwnd, GWLP_ID);
	}*/
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32PROJECT2));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
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

   HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
   hWndMain = hWnd;
   CtrlInit();
   LW_RegisterWnd(hInstance);
   SPW_RegisterWnd(hInstance);
   TVW_RegisterWnd(hInstance);

   tvInfo = new TREEVIEWINFO();
   tvInfo->dIndent = 20;
   tvInfo->dwDataSource = 1;
   tvInfo->wsRootNodePath= L"D:\\图片2：待归档\\jilebox-photo\\9";


   PWNDDATA *pTVData = new PWNDDATA();
   pTVData->pCtrlInfo = tvInfo;
   pTVData->pExtInfo = nullptr;

   
   hWndTreeView = CreateWindowEx(NULL, L"MyTreeViewControl", NULL, WS_CHILD | WS_VISIBLE,
	   0, 0, 200, 600, hWnd, (HMENU)BindIdData(pTVData), hInstance, NULL);


   CreateWindow(_T("static"), _T("你喜欢什么颜色\0AAAAAAAAAAAA:"), WS_CHILD | WS_VISIBLE|SS_CENTER|SS_CENTERIMAGE, 215, 115, 150, 120, hWnd, (HMENU)5006, hInst, nullptr);

   vector<wstring> sections;
   sections.push_back(L"这里一共有五段测试Label控件的字符串：");
   sections.push_back(L"这是第一段的内容，共五段");
   sections.push_back(L"这是第二段的内容，共五段");
   sections.push_back(L"这是第三段的内容，共五段");
   sections.push_back(L"这是第四段的内容，共五段");
   sections.push_back(L"这是第五段的内容，共五段");

   LABELCONTROLINFO *LabelControlInfo = new LABELCONTROLINFO();

   LabelControlInfo->wsText = L"这里一共有五段测试Label控件的字符串：";
   LabelControlInfo->sections = sections;
   LabelControlInfo->dwCS = LS_HSCROLL | LS_VSCROLL | LS_HLEFT | LS_VUPPER;
   //LS_INVARWIDTH |LS_INVARHEIGHT |LS_HLEFT |LS_VUPPER	//默认，即0
   //LS_HSCROLL | LS_VSCROLL | LS_HLEFT | LS_VUPPER

   LabelControlInfo->lfText = lfList;

   PWNDDATA *pWndData = new PWNDDATA();
   pWndData->pCtrlInfo = LabelControlInfo;
   pWndData->pExtInfo = nullptr;

   hWndLabel=CreateWindowEx(NULL, L"ControlTransMessage", NULL, WS_CHILD | WS_VISIBLE,
	   215, 250, 240, 80, hWnd, (HMENU)BindIdData(pWndData), hInstance, NULL);

   //MessageBox(NULL, LabelControlInfo->GetDebugString().c_str(), TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
   HDC hdc = GetDC(hWndMain);

   TextOut(hdc, 500, 0,
	   LabelControlInfo->GetDebugString().c_str(),
	   wcslen(LabelControlInfo->GetDebugString().c_str()));


   ReleaseDC(hWndMain, hdc);

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
	case WM_CUSTOM_TVN:
	{		
		switch (wParam)
		{
		case TVN_SETROOT:
		{
			NODEDATA *NodeData=new NODEDATA();
			NodeData->wsNodeName = L"ALLcrollPanelControl1234";
			NodeData->wsNodePath = L"D:\\视频同步文件夹\\国产短视频";
			NodeData->dwNodeDepth = 0;
			NodeData->dwExpanded = 0;

			NodeData->dwNodeType = 1;
			NodeData->dwHasChildren = 1;
			NodeData->dwLoadChildrenDone = 1;
			
			return BindIdData(NodeData);
		}
		break;
		case TVN_SETDATA:
		{
			PWNDDATA *pWndData = ((PWNDDATA *)IdDataBinding[(DWORD)lParam]);
			TreeNode<NODEDATA *> *pNode = (TreeNode<NODEDATA *> *)pWndData->pExtInfo;
			//MessageBox(NULL, pNode->data.wsNodeName.c_str(), TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
			vector<WIN32_FIND_DATA> ChildNodeList;
			
			vector<NODEDATA *> *pVNodeData= new vector<NODEDATA *>;
			FINDFILEPARAM *FindFileParam = new FINDFILEPARAM();
			FindFileParam->dwFileFloderFlag = 0x00000011;
			FindFileParam->szFileDirectory = pNode->data->wsNodePath;

			ChildNodeList = FileSystem::FindFile(FindFileParam);
			for (vector<WIN32_FIND_DATA>::iterator iter = ChildNodeList.begin(); iter != ChildNodeList.end(); iter++)
			{
				NODEDATA *NodeData = new NODEDATA();
				NodeData->wsNodeName = iter->cFileName;
				//MessageBox(NULL, NodeData->wsNodeName.c_str(), TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
				//获取数据后循环插入树
				//再获取子节点是否有子项
				wstring temStr = pNode->data->wsNodePath;
				temStr.append(L"\\");
				temStr.append(iter->cFileName);

				if (iter->dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) {
					NodeData->dwNodeType = 1;
					NodeData->dwIconType = 1;
					NodeData->dwHasChildren = 1;

				}

				NodeData->wsNodePath = temStr;
				NodeData->dwNodeDepth = pNode->data->dwNodeDepth + 1;
				
				pVNodeData->push_back(NodeData);				
				//内部组件ID，组件ID与数据关联	
			}
			return BindIdData(pVNodeData);
		}
		case TVN_SIZE:
		{
			
			//PostMessage(scrollPanelControl->ScrollPanelControlInfo->hWndControl, WM_SIZE, NULL, NULL);
		}
		break;	
		case TVN_REPATH:
		{
			MessageBox(NULL, L"拖拽到此窗口", TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
		}
		break;
		default:
			break;
		}
		
		
	}
	break;
	case LN_SIZED:
	{
		wchar_t tempStr1[32];
		_itow_s(wParam, tempStr1, 32);

		//MessageBox(NULL, tempStr1, TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
		MoveWindow(hWndLabel, 215, 250, wParam, lParam, TRUE);
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
