#pragma once
//#include "NW_WndCatalog.h"
#include "WindowWithScroll.h"

LRESULT CALLBACK    WndProc_NW_Container(HWND, UINT, WPARAM, LPARAM);

ATOM RegWnd_NW_Container(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc_NW_Container;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = hBrushDefault;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"Wnd_NW_Container";
	wcex.hIconSm = NULL;

	return RegisterClassExW(&wcex);
}

LRESULT CALLBACK WndProc_NW_Container(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		//RegWnd_NW_Catalog(hInst);
		
		//hWnd_NW_Catalog = CreateWindowEx(NULL, L"Wnd_NW_Catalog", NULL, WS_CHILD | WS_VISIBLE,
		//	0, 0,0, 0, hWnd, NULL, hInst, NULL);
		wstring szProjectPath = L"E:\\OneDrive - elcass\\novel\\逍遥道医\\正文";
		wstring szTempStr = L"E:\\OneDrive - elcass\\novel\\逍遥道医\\正文\\第一卷 潜龙出渊\\逍遥道医 英文版的一些探索\\逍遥道医 英文释义.txt";
		SetDirectory(0, szProjectPath, szTempStr);
		hWnd_NW_NovelChapterList = CreateWindowWithScroll(0,hInst, hWnd, 0, 150, 200, 200);
		//SetDirectory(1, szProjectPath, szTempStr);
		hWnd_NW_NovelChapterList1 = CreateWindowWithScroll(1, hInst, hWnd, 200, 150, 200, 200);
		//SetClassLong(NW_hWndCatalog, GCL_HBRBACKGROUND, (LONG)CreateSolidBrush(RGB(54, 78, 111)));
		//NW_hWndCatalog2 = CreateWindowEx(NULL, szClassNameWndView, NULL, WS_CHILD | WS_VISIBLE,
		//	0, 0, 0, 0, hWnd, NULL, hInst, NULL);
		//SetClassLong(NW_hWndCatalog2, GCL_HBRBACKGROUND, (LONG)CreateSolidBrush(RGB(221, 0, 27)));
	}
	case WM_SIZE:
	{
		
		RECT rect;
		GetClientRect(hWnd, &rect);
		DWORD dWidthWndContainer = rect.right - rect.left;  // 获得客户区宽度
		DWORD dHeightWndContainer = rect.bottom - rect.top;  // 获得客户区高度
		MoveWindow(hWnd_NW_NovelChapterList, 0, 175, 200, dHeightWndContainer - 125 - 25 - 25, TRUE);
		MoveWindow(hWnd_NW_NovelChapterList1, 200, 175, 200, dHeightWndContainer - 125 - 25 - 25, TRUE);
		/*
		MoveWindow(hWnd_Button_NW_Import, 0, 0, 200, 25, TRUE);
		MoveWindow(hWnd_NW_NovelItem, 0, 25, 200,125, TRUE);
		MoveWindow(hWnd_NW_NovelChapterList, 0, 150, 200, dHeightWndContainer - 125-25-25, TRUE);
		//MoveWindow(NW_hWndCatalog2, 150, 0, 150, heightWndContainer, TRUE);
		*/
	}
	case WM_PAINT:
	{
		
	}
	//break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
