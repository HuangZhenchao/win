#pragma once
namespace PKB {
HWND NW_hWndCatalog;
HWND NW_hWndCatalog2;
DWORD ItemSelectedStatus = 0;
DWORD ItemHoverStatus;
WCHAR szClassNameWndSideItem[50] = L"WndSideItem1";
WCHAR szClassNameWndView[50] = L"WndView";
WCHAR szClassNameWndContainer[50] = L"WndContainer1";

HBRUSH hBrushBgContainer = CreateSolidBrush(RGB(221, 0, 27));


LRESULT CALLBACK WndContainerProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK    WndSideItemProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndViewProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_CREATE:
	{
		/*
		WCHAR tempStr1[10];
		_itow_s(GetLastError(), tempStr1, 10);
		MessageBox(hWnd, tempStr1, TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
		*/
		HWND NW_hWndCatalog2 = CreateWindowEx(NULL, L"Wnd", NULL, WS_CHILD | WS_VISIBLE,
			5, 0, 65, 30, hWnd, NULL, hInst, NULL);
		HWND NW_hWndCatalog3 = CreateWindowEx(NULL, L"Wnd", NULL, WS_CHILD | WS_VISIBLE,
			75, 0, 65, 30, hWnd, NULL, hInst, NULL);
		SetClassLong(NW_hWndCatalog2, GCL_HBRBACKGROUND, (LONG)CreateSolidBrush(RGB(239, 239, 239)));
	}
	case WM_SIZE:
	{

	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		RECT rect;
		GetClientRect(hWnd, &rect);
		FillRect(hdc, &rect, CreateSolidBrush(RGB(239, 239, 239)));

		EndPaint(hWnd, &ps);
	}
	break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
ATOM RegWndSideItem(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndSideItemProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = sizeof(long);
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(nullptr, IDC_HAND);
	wcex.hbrBackground = hBrushDefault;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szClassNameWndSideItem;
	wcex.hIconSm = NULL;

	return RegisterClassExW(&wcex);
}
ATOM RegWndView(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndViewProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(nullptr, IDC_HAND);
	wcex.hbrBackground = hBrushDefault;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szClassNameWndView;
	wcex.hInstance = hInstance;
	wcex.hIconSm = NULL;

	return RegisterClassExW(&wcex);
}
ATOM RegWndContainer(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndContainerProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(nullptr, IDC_HAND);
	wcex.hbrBackground = hBrushDefault;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szClassNameWndContainer;
	wcex.hIconSm = NULL;

	return RegisterClassExW(&wcex);
}

//侧边栏按钮消息处理程序。
//侧边栏按钮消息处理程序。
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
		MoveWindow(hWnd, 0, 0, 50, 50, TRUE);
	}
	break;
	case WM_LBUTTONDOWN:
	{
		//遍历创建右边容器，以className为区分发送到各自的处理函数，szClassName[]

		//if (GetWindowLongPtr(hWnd, 0) == 0)
		{
			if (hWndCurrentItem) {
				SetWindowLongPtr(hWndCurrentItem, 0, 0);
				InvalidateRect(hWndCurrentItem, NULL, TRUE);

				DestroyWindow(hWndCurrentContainer);
			}
			hWndCurrentItem = hWnd;
			
			InvalidateRect(hWndCurrentItem, NULL, TRUE);

			hWndCurrentContainer = CreateWindowEx(NULL, L"WndContainer1", NULL, WS_CHILD | WS_VISIBLE,
				0, 0, 0, 0, hWndMain, NULL, hInst, NULL);
			MoveWindow(hWndCurrentContainer, widthSide, 0, widthClient - widthSide, heightClient, TRUE);
			//ShowWindow(hWndContainer[i], SW_SHOW);
			SetWindowLongPtr(hWndCurrentItem, 0, 1);
			//MoveWindow(hWndContainer[i], 50, 0, 500, 500, TRUE);
		}
	}
	break;
	case WM_MOUSEMOVE:
	{
		if (!bMouseTrack) {
			TRACKMOUSEEVENT tme;
			tme.cbSize = sizeof(tme);
			tme.hwndTrack = hWnd;
			tme.dwFlags = TME_LEAVE | TME_HOVER;
			tme.dwHoverTime = 30;
			TrackMouseEvent(&tme);

			bMouseTrack = TRUE;
		}
	}
	break;
	case WM_MOUSELEAVE:
	{

		bMouseTrack = FALSE;
		ItemHoverStatus = 0;
		InvalidateRect(hWnd, NULL, TRUE);
	}
	break;
	case WM_MOUSEHOVER:
	{
		ItemHoverStatus = 1;
		InvalidateRect(hWnd, NULL, TRUE);
		//ItemHoverStatus = 0;
	}
	break;
	case WM_PAINT:
	{

		// TODO: 在此处添加使用 hdc 的任何绘图代码...
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		if (GetWindowLongPtr(hWnd, 0) == 1) {
			ItemSelectedStatus = 1;
		}
		else {
			if (ItemHoverStatus == 1) {
				ItemSelectedStatus = 2;
				//ItemHoverStatus = 0;
			}
			else {
				ItemSelectedStatus = 0;
			}
		}
		/*
		WCHAR tempStr1[10];
		//_itow_s(GetLastError(), tempStr1, 10);
		_itow_s(ItemSelectedStatus, tempStr1, 10);
		MessageBox(hWnd, tempStr1, TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
		*/
		if (ItemSelectedStatus == 0) {
			hBrushItem = CreateSolidBrush(RGB(42, 51, 60));
		}
		if (ItemSelectedStatus == 1) {
			hBrushItem = CreateSolidBrush(RGB(54, 78, 111));
		}
		if (ItemSelectedStatus == 2) {
			hBrushItem = CreateSolidBrush(RGB(202, 81, 0));
		}

		RECT rect;
		SetRect(&rect, 0, 0, 50, 50);
		FillRect(hdc, &rect, hBrushItem);
		//Rectangle(hdc,0,0,50,50);
		//InvalidateRect(hWndCurrentItem, NULL, TRUE);
		SelectObject(hdc, hBrushIcon);
		Ellipse(hdc, 10, 10, 40, 40);
		//DeleteObject(hbrush);

		EndPaint(hWnd, &ps);
	}
	break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}



LRESULT CALLBACK WndContainerProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		RegWndView(hInst);
		
		NW_hWndCatalog = CreateWindowEx(NULL, szClassNameWndView, NULL, WS_CHILD | WS_VISIBLE,
			0, 0,0, 0, hWnd, NULL, hInst, NULL);
		//SetClassLong(NW_hWndCatalog, GCL_HBRBACKGROUND, (LONG)CreateSolidBrush(RGB(54, 78, 111)));
		//NW_hWndCatalog2 = CreateWindowEx(NULL, szClassNameWndView, NULL, WS_CHILD | WS_VISIBLE,
		//	0, 0, 0, 0, hWnd, NULL, hInst, NULL);
		//SetClassLong(NW_hWndCatalog2, GCL_HBRBACKGROUND, (LONG)CreateSolidBrush(RGB(221, 0, 27)));
	}
	case WM_SIZE:
	{
		RECT rect;
		GetClientRect(hWnd, &rect);
		DWORD widthWndContainer = rect.right - rect.left;  // 获得客户区宽度
		DWORD heightWndContainer = rect.bottom - rect.top;  // 获得客户区高度
		MoveWindow(NW_hWndCatalog, 0, 0, 150, heightWndContainer, TRUE);
		//MoveWindow(NW_hWndCatalog2, 150, 0, 150, heightWndContainer, TRUE);
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
}
