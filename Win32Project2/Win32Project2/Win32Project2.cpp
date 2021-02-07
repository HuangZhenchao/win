#include "stdafx.h"
#include "Win32Project2.h"
#include "global.h"
#include "base/function.h"
#include "Control/TreeViewControl.h"
//#include "Control/ScrollPanelControl.h"
#include "Control/LabelControl.h"
#include "OLE/OLEDragDrop.h"
#define MAX_LOADSTRING 100

// ȫ�ֱ���: 
HINSTANCE hInst;                                // ��ǰʵ��
WCHAR szTitle[MAX_LOADSTRING];                  // �������ı�
WCHAR szWindowClass[MAX_LOADSTRING];            // ����������
SCROLLPANELCONTROLINFO *scrollPanelControlInfo;

TREEVIEWINFO *tvInfo;
HWND hWndTreeView;
HWND hWndLabel;
UINT ID_TIMER;
// �˴���ģ���а����ĺ�����ǰ������: 
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

    // TODO: �ڴ˷��ô��롣
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
	lstrcpy(lfList.lfFaceName, _T("����"));
    // ��ʼ��ȫ���ַ���
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WIN32PROJECT2, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ִ��Ӧ�ó����ʼ��: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32PROJECT2));

    MSG msg;

    // ����Ϣѭ��: 
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
			if (msg.message == WM_MOUSEWHEEL)
			{			
				POINT pos;
				GetCursorPos(&pos);
				msg.hwnd = WindowFromPoint(pos); //Ҫ��TranslateMessage ����ǰ��
			}
			/*
			if (msg.message == WM_LBUTTONDOWN) {
				ID_TIMER=SetTimer(msg.hwnd, 1, 1000, TimerProc);
			}
			if (msg.message == WM_MOUSEMOVE) {				
				if (bDragStatus) {
					
					//�˴������ڳ��ν���ʱ��һ�����ھ����ÿ���������бȽϣ���ͬʱ��������һ���裬�ɱ����ظ�������������
					if (IdDataBinding.count(GetWindowLongPtr(msg.hwnd, GWLP_ID)) == 1) {
						PWNDDATA* pDropWndData= (PWNDDATA *)IdDataBinding[GetWindowLongPtr(msg.hwnd, GWLP_ID)];
						PWNDDATA* pDragWndData = (PWNDDATA *)IdDataBinding[dwDragWndID];
						if (pDropWndData->dwDragType&pDragWndData->dwDropType) {
							SetCursor(LoadCursor(NULL, IDC_ARROW));
						}
						//����Ҫ����·���жϣ����ܰѸ�Ŀ¼�ƶ�����Ŀ¼
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
	//MessageBox(NULL, L"��갴�¼�ʱ", TEXT("������"), MB_OK | MB_ICONINFORMATION); 
	if (IdDataBinding.count(GetWindowLongPtr(hwnd, GWLP_ID)) == 1&& (((PWNDDATA *)IdDataBinding[GetWindowLongPtr(hwnd, GWLP_ID)])->dwDragType>0)) {
		//MessageBox(NULL, L"�ô���������ק", TEXT("������"), MB_OK | MB_ICONINFORMATION);
		//����ק״̬��Ϊ1
		bDragStatus = TRUE;
		dwDragWndID = GetWindowLongPtr(hwnd, GWLP_ID);
	}*/
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32PROJECT2));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
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
   tvInfo->wsRootNodePath= L"D:\\ͼƬ2�����鵵\\jilebox-photo\\9";


   PWNDDATA *pTVData = new PWNDDATA();
   pTVData->pCtrlInfo = tvInfo;
   pTVData->pExtInfo = nullptr;

   
   hWndTreeView = CreateWindowEx(NULL, L"MyTreeViewControl", NULL, WS_CHILD | WS_VISIBLE,
	   0, 0, 200, 600, hWnd, (HMENU)BindIdData(pTVData), hInstance, NULL);


   CreateWindow(_T("static"), _T("��ϲ��ʲô��ɫ\0AAAAAAAAAAAA:"), WS_CHILD | WS_VISIBLE|SS_CENTER|SS_CENTERIMAGE, 215, 115, 150, 120, hWnd, (HMENU)5006, hInst, nullptr);

   vector<wstring> sections;
   sections.push_back(L"����һ������β���Label�ؼ����ַ�����");
   sections.push_back(L"���ǵ�һ�ε����ݣ������");
   sections.push_back(L"���ǵڶ��ε����ݣ������");
   sections.push_back(L"���ǵ����ε����ݣ������");
   sections.push_back(L"���ǵ��Ķε����ݣ������");
   sections.push_back(L"���ǵ���ε����ݣ������");

   LABELCONTROLINFO *LabelControlInfo = new LABELCONTROLINFO();

   LabelControlInfo->wsText = L"����һ������β���Label�ؼ����ַ�����";
   LabelControlInfo->sections = sections;
   LabelControlInfo->dwCS = LS_HSCROLL | LS_VSCROLL | LS_HLEFT | LS_VUPPER;
   //LS_INVARWIDTH |LS_INVARHEIGHT |LS_HLEFT |LS_VUPPER	//Ĭ�ϣ���0
   //LS_HSCROLL | LS_VSCROLL | LS_HLEFT | LS_VUPPER

   LabelControlInfo->lfText = lfList;

   PWNDDATA *pWndData = new PWNDDATA();
   pWndData->pCtrlInfo = LabelControlInfo;
   pWndData->pExtInfo = nullptr;

   hWndLabel=CreateWindowEx(NULL, L"ControlTransMessage", NULL, WS_CHILD | WS_VISIBLE,
	   215, 250, 240, 80, hWnd, (HMENU)BindIdData(pWndData), hInstance, NULL);

   //MessageBox(NULL, LabelControlInfo->GetDebugString().c_str(), TEXT("������"), MB_OK | MB_ICONINFORMATION);
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
	case WM_CUSTOM_TVN:
	{		
		switch (wParam)
		{
		case TVN_SETROOT:
		{
			NODEDATA *NodeData=new NODEDATA();
			NodeData->wsNodeName = L"ALLcrollPanelControl1234";
			NodeData->wsNodePath = L"D:\\��Ƶͬ���ļ���\\��������Ƶ";
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
			//MessageBox(NULL, pNode->data.wsNodeName.c_str(), TEXT("������"), MB_OK | MB_ICONINFORMATION);
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
				//MessageBox(NULL, NodeData->wsNodeName.c_str(), TEXT("������"), MB_OK | MB_ICONINFORMATION);
				//��ȡ���ݺ�ѭ��������
				//�ٻ�ȡ�ӽڵ��Ƿ�������
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
				//�ڲ����ID�����ID�����ݹ���	
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
			MessageBox(NULL, L"��ק���˴���", TEXT("������"), MB_OK | MB_ICONINFORMATION);
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

		//MessageBox(NULL, tempStr1, TEXT("������"), MB_OK | MB_ICONINFORMATION);
		MoveWindow(hWndLabel, 215, 250, wParam, lParam, TRUE);
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
