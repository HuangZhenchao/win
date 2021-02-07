#pragma once

LRESULT CALLBACK    WndMainButtonProc(HWND, UINT, WPARAM, LPARAM);

ATOM RegWndMainButton(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndMainButtonProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = sizeof(long);
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(nullptr, IDC_HAND);
	wcex.hbrBackground = hBrushWndHeader;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"WndMainButton";
	wcex.hIconSm = NULL;

	return RegisterClassExW(&wcex);
}

LRESULT CALLBACK    WndMainButtonProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_CREATE:
		SetWindowLongPtr(hWnd, 0, 0);
		hBitmapPKB = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_PKB));
		GetObject(hBitmapPKB, sizeof(BITMAP), &bitmapPKB);
		hBitmapNW = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_NW));
		GetObject(hBitmapPKB, sizeof(BITMAP), &bitmapNW);
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		if (hWnd == hButtonQuit) {
			if ((GetWindowLongPtr(hWnd, 0)&(1 << 1)) != 0) {
				RECT rect;
				SetRect(&rect, 0, 0, 60, 60);
				FillRect(hdc, &rect, hBrushSelectedItem);
			}
			MoveToEx(hdc, 15, 15, nullptr);
			LineTo(hdc, 26, 26);
			MoveToEx(hdc, 15, 26, nullptr);
			LineTo(hdc, 26, 15);
		}
		if (hWnd == hItemButton[0]) {
			if ((GetWindowLongPtr(hWnd, 0)&(1 << 1)) != 0) {
				RECT rect;
				SetRect(&rect, 0, 0, 60, 60);
				FillRect(hdc, &rect, hBrushSelectedItem);
			}
			if ((GetWindowLongPtr(hWnd, 0)&(1)) != 0) {
				RECT rect;
				SetRect(&rect, 0, 0, 60, 60);
				FillRect(hdc, &rect, hBrushSelectedItem);
			}
			HDC         hdcMem;
			hdcMem = CreateCompatibleDC(hdc);
			SelectObject(hdcMem, hBitmapPKB);


			TransparentBlt(hdc, 14, 14, 32, 32, hdcMem, 0, 0, bitmapPKB.bmWidth, bitmapPKB.bmHeight, RGB(255, 255, 255));
			DeleteDC(hdcMem);
		}
		if (hWnd == hItemButton[1]) {
			if ((GetWindowLongPtr(hWnd, 0)&(1 << 1)) != 0) {
				RECT rect;
				SetRect(&rect, 0, 0, 60, 60);
				FillRect(hdc, &rect, hBrushSelectedItem);
			}
			if ((GetWindowLongPtr(hWnd, 0)&(1)) != 0) {
				RECT rect;
				SetRect(&rect, 0, 0, 60, 60);
				FillRect(hdc, &rect, hBrushSelectedItem);
			}
			HDC         hdcMem;
			hdcMem = CreateCompatibleDC(hdc);
			SelectObject(hdcMem, hBitmapNW);


			TransparentBlt(hdc, 14, 14, 32,32, hdcMem, 0, 0, bitmapNW.bmWidth, bitmapNW.bmHeight, RGB(255, 255, 255));
			DeleteDC(hdcMem);
		}
		EndPaint(hWnd, &ps);
		
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
		SetWindowLongPtr(hWnd, 0, GetWindowLongPtr(hWnd, 0)& 1 );
		InvalidateRect(hWnd, NULL, TRUE);
	}
	break;
	case WM_MOUSEHOVER:
	{

		SetWindowLongPtr(hWnd,0,GetWindowLongPtr(hWnd,0)|1<<1);
		InvalidateRect(hWnd, NULL, TRUE);
		//ItemHoverStatus = 0;
	}
	break;
	case WM_LBUTTONDOWN:
	{
		if (hWnd == hButtonQuit) {
			SendMessage(hWndMain, WM_DESTROY, wParam, lParam);
			break;
		}
		if (hWnd == hButtonMax) {
			if (GetWindowLongPtr(hWnd, 0) == 0) {
				SendMessage(hWndMain, WM_SYSCOMMAND, SC_MAXIMIZE, lParam);
				SetWindowLongPtr(hWnd, 0, 1);
			}
			else {
				SendMessage(hWndMain, WM_SYSCOMMAND, SC_RESTORE, lParam);
				SetWindowLongPtr(hWnd, 0, 0);
			}
			break;
		}
		if (hWnd == hButtonMin) {
			SendMessage(hWndMain, WM_SYSCOMMAND, SC_MINIMIZE, lParam);
			break;
		}
		if ((hWnd == hItemButton[0]) || (hWnd == hItemButton[1])) {

			for (DWORD j = 0; j < dButtonGroupCount; j++) {
				SetWindowLongPtr(hItemButton[j], 0, 0);
				InvalidateRect(hItemButton[j], NULL, TRUE);
			}
			SetWindowLongPtr(hWnd, 0, 1);
			if (hWnd == hItemButton[0]) {
				if (hWndCurrentContainer) {
					DestroyWindow(hWndCurrentContainer);
				}
				hWndCurrentContainer = CreateWindowEx(NULL, L"Wnd_PKB_Container", NULL, WS_CHILD | WS_VISIBLE,
					dWidthBorder + dWidthSide,
					dHeightHeader + dWidthBorder,
					dWidthClient - dWidthSide - dWidthBorder * 2,
					dHeightClient - dHeightHeader - dHeightFooter - dWidthBorder * 2,
					hWndMain, NULL, hInst, NULL);

			}
			if (hWnd == hItemButton[1]) {
				if (hWndCurrentContainer) {
					DestroyWindow(hWndCurrentContainer);
				}
				hWndCurrentContainer = CreateWindowEx(NULL, L"Wnd_NW_Container", NULL, WS_CHILD | WS_VISIBLE,
					dWidthBorder + dWidthSide,
					dHeightHeader + dWidthBorder,
					dWidthClient - dWidthSide - dWidthBorder * 2,
					dHeightClient - dHeightHeader - dHeightFooter - dWidthBorder * 2,
					hWndMain, NULL, hInst, NULL);

			}
		}


	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}