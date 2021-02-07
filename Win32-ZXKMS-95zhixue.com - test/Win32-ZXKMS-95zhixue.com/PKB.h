#pragma once

#include "PKB_WndCatalog.h"
HWND hWnd_PKB_Catalog;
HWND hWnd_PKB_Catalog2;

LRESULT CALLBACK WndProc_PKB_Container(HWND, UINT, WPARAM, LPARAM);

ATOM RegWnd_PKB_Container(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc_PKB_Container;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(nullptr, IDC_HAND);
	wcex.hbrBackground = hBrushDefault;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"Wnd_PKB_Container";
	wcex.hIconSm = NULL;

	return RegisterClassExW(&wcex);
}

	
	LRESULT CALLBACK WndProc_PKB_Container(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_CREATE:
			{
			RegWnd_PKB_Catalog(hInst);

			hWnd_PKB_Catalog = CreateWindowEx(NULL, L"Wnd_PKB_Catalog", NULL, WS_CHILD | WS_VISIBLE,
					0, 0, 0, 0, hWnd, NULL, hInst, NULL);
			//SetClassLong(NW_hWndCatalog, GCL_HBRBACKGROUND, (LONG)CreateSolidBrush(RGB(221, 0, 27)));
			}
		case WM_SIZE:
			{
				
				RECT rect;
				GetClientRect(hWnd, &rect);
				DWORD widthWndContainer = rect.right - rect.left;  // 获得客户区宽度
				DWORD heightWndContainer = rect.bottom - rect.top;  // 获得客户区高度
				MoveWindow(hWnd_PKB_Catalog, 0, 0, 150, heightWndContainer, TRUE);
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
	
