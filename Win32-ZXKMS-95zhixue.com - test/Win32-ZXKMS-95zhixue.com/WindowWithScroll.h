#pragma once
#include <windows.h>
//#include "FileTree.h"
#include "TreeViewControl.h"
class HSCROLLINFO {
public:
	DWORD height = 5;
	DWORD cxSBMax=0;
	DWORD cxSBPos = 0;
	DWORD cxSBBar = 50;
	DWORD cxPos = 0;
	DWORD cxContent=0;
	DWORD cxWnd = 0;
	int xPointDown = 0;
	int yPointDown = 0;
};
class VSCROLLINFO {
public:
	DWORD width = 5;
	DWORD cySBMax = 0;
	DWORD cySBPos = 0;
	DWORD cySBBar = 50;
	DWORD cyPos = 0;
	DWORD cyContent = 0;
	DWORD cyWnd = 0;
	int xPointDown = 0;
	int yPointDown = 0;
};

DWORD dragState = 0;


class HandlerOfScrollCtrl {
public:
	HINSTANCE hInstance;
	HWND hWnd_Control = NULL;
	HWND hWnd_Container = NULL;
	HWND hWnd_Content = NULL;
	HWND hWnd_HScroll = NULL;
	HWND hWnd_VScroll = NULL;
	HWND hWnd_HScrollBar = NULL;
	HWND hWnd_VScrollBar = NULL;

	HSCROLLINFO Hsi;
	VSCROLLINFO Vsi;
	//void RegisterClass(HINSTANCE hInst);
	//HWND CreateWindowWithScroll(HINSTANCE hInstance, HWND hWndParent, int x, int y, int width, int height);

};
#include "TreeView.h"
//vector<vector<HandlerOfScrollCtrl> > vvHandlerOfScrollCtrl;
vector<HandlerOfScrollCtrl> vHandlerOfScrollCtrl(5);
INT vID = 0;
LRESULT CALLBACK Proc_WndWithScroll_Control(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK Proc_WndWithScroll_ScrollBar(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void RegisterClass(HINSTANCE hInst) {
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = DefWindowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInst;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = CreateSolidBrush(RGB(239, 239, 239));
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"WndDef";
	wcex.hIconSm = NULL;
	if (!GetClassInfoEx(hInst, wcex.lpszClassName,&wcex)) {		
		RegisterClassExW(&wcex);
	}

	wcex.lpfnWndProc = Proc_WndWithScroll_Control;
	wcex.lpszClassName = L"WndWithScroll_Control";
	if (!GetClassInfoEx(hInst, wcex.lpszClassName, &wcex)) {
		RegisterClassExW(&wcex);		
	}


	wcex.hbrBackground = CreateSolidBrush(RGB(194, 195, 201));
	wcex.lpfnWndProc = Proc_WndWithScroll_ScrollBar;
	wcex.lpszClassName = L"WndWithScroll_ScrollBar";
	if (!GetClassInfoEx(hInst, wcex.lpszClassName, &wcex)) {
		RegisterClassExW(&wcex);
	}
}
HWND CreateWindowWithScroll(INT ID, HINSTANCE hInstance, HWND hWndParent, int x, int y, int width, int height)
{
	RegisterClass(hInstance);
	vID = ID;
	//HWND hWnd_Control = 
	//vHandlerOfScrollCtrl.push_back(hScrollCtrl);
	return CreateWindowEx(NULL, L"WndWithScroll_Control", NULL, WS_CHILD | WS_VISIBLE,
		x, y, width, height, hWndParent, NULL, hInstance, NULL);
}
// 消息处理。需要后续默认处理则需要返回0；停止该消息后续处理，则返回1
/*
void HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		DWORD CurrentOrder;
		for (int i = 0; i < vHandlerOfScrollCtrl.size(); i++) {
			if (hWnd == vHandlerOfScrollCtrl[i].hWnd_Control) {
				CurrentOrder = i;
			}
		}
		if (CurrentOrder==0) {
			vector<int> contentSize=TreeViewAsContent(vHandlerOfScrollCtrl[0]);
			vHandlerOfScrollCtrl[0].Hsi.cxContent = contentSize[0];
			vHandlerOfScrollCtrl[0].Vsi.cyContent = contentSize[1];

		}
	}
	break; 
	case WM_SIZE:
	{
		DWORD CurrentOrder;
		for (int i = 0; i < vHandlerOfScrollCtrl.size(); i++) {
			if (hWnd == vHandlerOfScrollCtrl[i].hWnd_Control) {
				CurrentOrder = i;
			}
		}
		if (CurrentOrder == 0) {
			TreeViewItemSize(vHandlerOfScrollCtrl[0]);
		}
	}
	break;
	default:		
		break;
	}

}
*/
// 原始窗口过程
LRESULT CALLBACK Proc_WndWithScroll_Control(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
	if (message==WM_NCCREATE) {
		HandlerOfScrollCtrl hScrollCtrl;
		hScrollCtrl.hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
		hScrollCtrl.hWnd_Control = hWnd;
		
		vHandlerOfScrollCtrl[vID]=hScrollCtrl;
	}
	DWORD CurrentOrder=0;
	for (int i = 0; i < vHandlerOfScrollCtrl.size(); i++) {
		if (hWnd == vHandlerOfScrollCtrl[i].hWnd_Control) {
			CurrentOrder=i;
		}
	}
	switch (message)
	{
	case WM_CREATE:
	{				
		//HandlerOfScrollCtrl vHandlerOfScrollCtrl[i];
		
		
		vHandlerOfScrollCtrl[CurrentOrder].hWnd_Container = CreateWindowEx(NULL, L"WndDef", NULL, WS_CHILD | WS_VISIBLE,
			0, 0, 0, 0, hWnd, NULL, vHandlerOfScrollCtrl[CurrentOrder].hInstance, NULL);
		
		vHandlerOfScrollCtrl[CurrentOrder].hWnd_Content = CreateWindowEx(NULL, L"WndDef", NULL, WS_CHILD | WS_VISIBLE,
			0, 0, 0, 0, vHandlerOfScrollCtrl[CurrentOrder].hWnd_Container, NULL, vHandlerOfScrollCtrl[CurrentOrder].hInstance, NULL);

		vHandlerOfScrollCtrl[CurrentOrder].hWnd_HScroll = CreateWindowEx(NULL, L"WndDef", NULL, WS_CHILD | WS_VISIBLE,
			0, 0, 0, 0, hWnd, NULL, vHandlerOfScrollCtrl[CurrentOrder].hInstance, NULL);
		vHandlerOfScrollCtrl[CurrentOrder].hWnd_HScrollBar = CreateWindowEx(NULL, L"WndWithScroll_ScrollBar", NULL, WS_CHILD | WS_VISIBLE,
			0, 0, 0, 0, vHandlerOfScrollCtrl[CurrentOrder].hWnd_HScroll, NULL, vHandlerOfScrollCtrl[CurrentOrder].hInstance, NULL);
		vHandlerOfScrollCtrl[CurrentOrder].hWnd_VScroll = CreateWindowEx(NULL, L"WndDef", NULL, WS_CHILD | WS_VISIBLE,
			0, 0, 0, 0, hWnd, NULL, vHandlerOfScrollCtrl[CurrentOrder].hInstance, NULL);
		vHandlerOfScrollCtrl[CurrentOrder].hWnd_VScrollBar = CreateWindowEx(NULL, L"WndWithScroll_ScrollBar", NULL, WS_CHILD | WS_VISIBLE,
			0, 0, 0, 0, vHandlerOfScrollCtrl[CurrentOrder].hWnd_VScroll, NULL, vHandlerOfScrollCtrl[CurrentOrder].hInstance, NULL);

		//HandleMessage(hWnd, message, wParam, lParam);
		
		if (CurrentOrder == 0) {

			vector<int> contentSize = TreeViewAsContent(0,vHandlerOfScrollCtrl[0]);
			vHandlerOfScrollCtrl[0].Hsi.cxContent = contentSize[0];
			vHandlerOfScrollCtrl[0].Vsi.cyContent = contentSize[1];

		}
		if (CurrentOrder == 1) {
			vector<int> contentSize = TreeViewAsContent(1,vHandlerOfScrollCtrl[1]);
			vHandlerOfScrollCtrl[1].Hsi.cxContent = contentSize[0];
			vHandlerOfScrollCtrl[1].Vsi.cyContent = contentSize[1];

		}
	}
	break;
	case WM_SIZE: 
	{		

		RECT rcClient;
		GetWindowRect(hWnd, &rcClient);

		//TODO:此处因为减法运算可能出现负数，应进行判断
		vHandlerOfScrollCtrl[CurrentOrder].Hsi.cxWnd = rcClient.right - rcClient.left;//包含滚动栏在内的宽度
		vHandlerOfScrollCtrl[CurrentOrder].Vsi.cyWnd = rcClient.bottom - rcClient.top;//包含滚动栏在内的高度
		vHandlerOfScrollCtrl[CurrentOrder].Hsi.cxSBMax = vHandlerOfScrollCtrl[CurrentOrder].Hsi.cxWnd - vHandlerOfScrollCtrl[CurrentOrder].Vsi.width;//水平滚动栏总宽度
		vHandlerOfScrollCtrl[CurrentOrder].Vsi.cySBMax = vHandlerOfScrollCtrl[CurrentOrder].Vsi.cyWnd - vHandlerOfScrollCtrl[CurrentOrder].Hsi.height;//水平滚动栏总高度
		//vHandlerOfScrollCtrl[CurrentOrder].Hsi.cxContent = vHandlerOfScrollCtrl[CurrentOrder].Hsi.cxWnd - vHandlerOfScrollCtrl[CurrentOrder].Vsi.width;//内容默认最小宽度，即容器宽度
		//vHandlerOfScrollCtrl[CurrentOrder].Vsi.cyContent = vHandlerOfScrollCtrl[CurrentOrder].Vsi.cyWnd - vHandlerOfScrollCtrl[CurrentOrder].Hsi.height;//内容默认最小高度，即容器高度
		if (vHandlerOfScrollCtrl[CurrentOrder].Hsi.cxContent < vHandlerOfScrollCtrl[CurrentOrder].Hsi.cxSBMax) {
			vHandlerOfScrollCtrl[CurrentOrder].Hsi.cxContent = vHandlerOfScrollCtrl[CurrentOrder].Hsi.cxSBMax;//内容宽于容器，取最宽
		}
		if (vHandlerOfScrollCtrl[CurrentOrder].Vsi.cyContent < vHandlerOfScrollCtrl[CurrentOrder].Vsi.cySBMax) {//(ftChapter.ItemHeight + 1)*vHWNDItem.size()
			vHandlerOfScrollCtrl[CurrentOrder].Vsi.cyContent = vHandlerOfScrollCtrl[CurrentOrder].Vsi.cySBMax; //内容高于容器，取最高
		}
		HDC hdc = GetDC(hWndMain);
		RECT rc;
		SetRect(&rc, 200, 20, 150, 20);
		wchar_t tempStr1[10];
		_itow_s((LONG)vHandlerOfScrollCtrl[CurrentOrder].Hsi.cxContent, tempStr1, 10);
		TextOut(hdc, 200, 0,
			tempStr1,
			wcslen(tempStr1));
		
		

		_itow_s((LONG)vHandlerOfScrollCtrl[CurrentOrder].Vsi.cyContent, tempStr1, 10);
		TextOut(hdc, 200, 10,
			tempStr1,
			wcslen(tempStr1));

		
		ReleaseDC(hWndMain, hdc);
		//按比值计算滚动条长度或高度
		vHandlerOfScrollCtrl[CurrentOrder].Hsi.cxSBBar = (vHandlerOfScrollCtrl[CurrentOrder].Hsi.cxSBMax*vHandlerOfScrollCtrl[CurrentOrder].Hsi.cxSBMax) / vHandlerOfScrollCtrl[CurrentOrder].Hsi.cxContent;
		vHandlerOfScrollCtrl[CurrentOrder].Vsi.cySBBar = (vHandlerOfScrollCtrl[CurrentOrder].Vsi.cySBMax*vHandlerOfScrollCtrl[CurrentOrder].Vsi.cySBMax) / vHandlerOfScrollCtrl[CurrentOrder].Vsi.cyContent;
		//各窗口位置
		MoveWindow(vHandlerOfScrollCtrl[CurrentOrder].hWnd_Container,
			0, 0, vHandlerOfScrollCtrl[CurrentOrder].Hsi.cxSBMax, vHandlerOfScrollCtrl[CurrentOrder].Vsi.cySBMax, TRUE);
		MoveWindow(vHandlerOfScrollCtrl[CurrentOrder].hWnd_Content,
			0, 0, vHandlerOfScrollCtrl[CurrentOrder].Hsi.cxContent, vHandlerOfScrollCtrl[CurrentOrder].Vsi.cyContent, TRUE);
		MoveWindow(vHandlerOfScrollCtrl[CurrentOrder].hWnd_HScroll,
			0, vHandlerOfScrollCtrl[CurrentOrder].Vsi.cySBMax, vHandlerOfScrollCtrl[CurrentOrder].Hsi.cxSBMax, vHandlerOfScrollCtrl[CurrentOrder].Hsi.height, TRUE);//水平
		MoveWindow(vHandlerOfScrollCtrl[CurrentOrder].hWnd_VScroll,
			vHandlerOfScrollCtrl[CurrentOrder].Hsi.cxSBMax, 0, vHandlerOfScrollCtrl[CurrentOrder].Vsi.width, vHandlerOfScrollCtrl[CurrentOrder].Vsi.cySBMax, TRUE);//垂直
		
		MoveWindow(vHandlerOfScrollCtrl[CurrentOrder].hWnd_HScrollBar, vHandlerOfScrollCtrl[CurrentOrder].Hsi.cxSBPos, 0, vHandlerOfScrollCtrl[CurrentOrder].Hsi.cxSBBar, vHandlerOfScrollCtrl[CurrentOrder].Hsi.height, TRUE);
		MoveWindow(vHandlerOfScrollCtrl[CurrentOrder].hWnd_VScrollBar, 0, vHandlerOfScrollCtrl[CurrentOrder].Vsi.cySBPos, vHandlerOfScrollCtrl[CurrentOrder].Vsi.width, vHandlerOfScrollCtrl[CurrentOrder].Vsi.cySBBar, TRUE);
		
		
		
		//HandleMessage(hWnd, message, wParam, lParam);
		if (CurrentOrder == 0) {
			TreeViewItemSize(0,vHandlerOfScrollCtrl[0]);
		}
		if (CurrentOrder == 1) {
			TreeViewItemSize(1, vHandlerOfScrollCtrl[1]);
		}
	}
	break;
	case WM_USER + 2:
	{
		int x = (short)LOWORD(lParam);
		//x是点击滚动条后，鼠标移动到的位置，xPointDown是点击时的点，与x比较确定移动方向，再计算移动距离
		if (x>vHandlerOfScrollCtrl[CurrentOrder].Hsi.xPointDown) {
			//x>vHandlerOfScrollCtrl[i].Hsi.xPointDown,distance是正数
			int distance = x - vHandlerOfScrollCtrl[CurrentOrder].Hsi.xPointDown;
			vHandlerOfScrollCtrl[CurrentOrder].Hsi.cxSBPos = vHandlerOfScrollCtrl[CurrentOrder].Hsi.cxSBPos + distance;
			if (vHandlerOfScrollCtrl[CurrentOrder].Hsi.cxSBPos>vHandlerOfScrollCtrl[CurrentOrder].Hsi.cxSBMax - vHandlerOfScrollCtrl[CurrentOrder].Hsi.cxSBBar) {
				vHandlerOfScrollCtrl[CurrentOrder].Hsi.cxSBPos = vHandlerOfScrollCtrl[CurrentOrder].Hsi.cxSBMax - vHandlerOfScrollCtrl[CurrentOrder].Hsi.cxSBBar;
			}
		}
		else {
			int distance = vHandlerOfScrollCtrl[CurrentOrder].Hsi.xPointDown - x;
			if (vHandlerOfScrollCtrl[CurrentOrder].Hsi.cxSBPos>distance) {
				vHandlerOfScrollCtrl[CurrentOrder].Hsi.cxSBPos = vHandlerOfScrollCtrl[CurrentOrder].Hsi.cxSBPos - distance;
			}
			else {
				vHandlerOfScrollCtrl[CurrentOrder].Hsi.cxSBPos = 0;
			}

		}
		
		MoveWindow(vHandlerOfScrollCtrl[CurrentOrder].hWnd_HScrollBar, vHandlerOfScrollCtrl[CurrentOrder].Hsi.cxSBPos, 0, vHandlerOfScrollCtrl[CurrentOrder].Hsi.cxSBBar, vHandlerOfScrollCtrl[CurrentOrder].Hsi.height, TRUE);
		vHandlerOfScrollCtrl[CurrentOrder].Hsi.cxPos = vHandlerOfScrollCtrl[CurrentOrder].Hsi.cxSBPos*vHandlerOfScrollCtrl[CurrentOrder].Hsi.cxContent / vHandlerOfScrollCtrl[CurrentOrder].Hsi.cxSBMax;
		MoveWindow(vHandlerOfScrollCtrl[CurrentOrder].hWnd_Content, -vHandlerOfScrollCtrl[CurrentOrder].Hsi.cxPos, -vHandlerOfScrollCtrl[CurrentOrder].Vsi.cyPos, vHandlerOfScrollCtrl[CurrentOrder].Hsi.cxContent, vHandlerOfScrollCtrl[CurrentOrder].Vsi.cyContent, TRUE);

	}
	break;
	case WM_USER + 3:
	{
		int y = (short)HIWORD(lParam);
		if (y>vHandlerOfScrollCtrl[CurrentOrder].Vsi.yPointDown) {
			int distance = y - vHandlerOfScrollCtrl[CurrentOrder].Vsi.yPointDown;
			vHandlerOfScrollCtrl[CurrentOrder].Vsi.cySBPos = vHandlerOfScrollCtrl[CurrentOrder].Vsi.cySBPos + distance;
			if (vHandlerOfScrollCtrl[CurrentOrder].Vsi.cySBPos>vHandlerOfScrollCtrl[CurrentOrder].Vsi.cySBMax - vHandlerOfScrollCtrl[CurrentOrder].Vsi.cySBBar) {
				vHandlerOfScrollCtrl[CurrentOrder].Vsi.cySBPos = vHandlerOfScrollCtrl[CurrentOrder].Vsi.cySBMax - vHandlerOfScrollCtrl[CurrentOrder].Vsi.cySBBar;
			}
		}
		else {
			int distance = vHandlerOfScrollCtrl[CurrentOrder].Vsi.yPointDown - y;
			if (vHandlerOfScrollCtrl[CurrentOrder].Vsi.cySBPos>distance) {
				vHandlerOfScrollCtrl[CurrentOrder].Vsi.cySBPos = vHandlerOfScrollCtrl[CurrentOrder].Vsi.cySBPos - distance;
			}
			else {
				vHandlerOfScrollCtrl[CurrentOrder].Vsi.cySBPos = 0;
			}

		}

		MoveWindow(vHandlerOfScrollCtrl[CurrentOrder].hWnd_VScrollBar, 0, vHandlerOfScrollCtrl[CurrentOrder].Vsi.cySBPos, vHandlerOfScrollCtrl[CurrentOrder].Vsi.width, vHandlerOfScrollCtrl[CurrentOrder].Vsi.cySBBar, TRUE);
		vHandlerOfScrollCtrl[CurrentOrder].Vsi.cyPos = vHandlerOfScrollCtrl[CurrentOrder].Vsi.cySBPos*vHandlerOfScrollCtrl[CurrentOrder].Vsi.cyContent / vHandlerOfScrollCtrl[CurrentOrder].Vsi.cySBMax;

		
		MoveWindow(vHandlerOfScrollCtrl[CurrentOrder].hWnd_Content, -vHandlerOfScrollCtrl[CurrentOrder].Hsi.cxPos, -vHandlerOfScrollCtrl[CurrentOrder].Vsi.cyPos, vHandlerOfScrollCtrl[CurrentOrder].Hsi.cxContent, vHandlerOfScrollCtrl[CurrentOrder].Vsi.cyContent, TRUE);

	}
	break;
	default:		
		return DefWindowProc(hWnd, message, wParam, lParam);	
	break;
	}
	return 0;	
}

// 原始窗口过程
LRESULT CALLBACK Proc_WndWithScroll_ScrollBar(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
		if (message == WM_NCCREATE) {
			for (int i = 0; i < vHandlerOfScrollCtrl.size(); i++) {
				if (((LPCREATESTRUCT)lParam)->hwndParent== vHandlerOfScrollCtrl[i].hWnd_HScroll) {
					vHandlerOfScrollCtrl[i].hWnd_HScrollBar = hWnd;
				}
				if (((LPCREATESTRUCT)lParam)->hwndParent == vHandlerOfScrollCtrl[i].hWnd_VScroll) 
				{
					vHandlerOfScrollCtrl[i].hWnd_VScrollBar = hWnd;
				}
			}
		}
		DWORD CurrentOrder=0;
		for (int i = 0; i < vHandlerOfScrollCtrl.size(); i++) {
			if (hWnd == vHandlerOfScrollCtrl[i].hWnd_HScrollBar | hWnd == vHandlerOfScrollCtrl[i].hWnd_VScrollBar) {
				CurrentOrder=i;
			}
		}
		switch (message)
		{
		case WM_CREATE:
		{

		}
		break;
		case WM_LBUTTONDOWN:
		{
			SetCapture(hWnd);
			dragState = 1;
			vHandlerOfScrollCtrl[CurrentOrder].Vsi.xPointDown = vHandlerOfScrollCtrl[CurrentOrder].Hsi.xPointDown = (short)LOWORD(lParam);
			vHandlerOfScrollCtrl[CurrentOrder].Vsi.yPointDown = vHandlerOfScrollCtrl[CurrentOrder].Hsi.yPointDown = (short)HIWORD(lParam);

		}
		break;
		case WM_MOUSEMOVE:
		{

			if (dragState == 1) {

				if (hWnd == vHandlerOfScrollCtrl[CurrentOrder].hWnd_HScrollBar) {

					PostMessage(vHandlerOfScrollCtrl[CurrentOrder].hWnd_Control, WM_USER + 2, wParam, lParam);
				}
				if (hWnd == vHandlerOfScrollCtrl[CurrentOrder].hWnd_VScrollBar) {

					PostMessage(vHandlerOfScrollCtrl[CurrentOrder].hWnd_Control, WM_USER + 3, wParam, lParam);
				}

			}

		}
		break;
		case WM_LBUTTONUP:
			if (dragState == 1) {
				ReleaseCapture();   // 释放鼠标锁定
				dragState = 0;          // 清除状态
										//InvalidateRect(hwnd, NULL, false);
				vHandlerOfScrollCtrl[CurrentOrder].Hsi.xPointDown = 0;
			}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;

}