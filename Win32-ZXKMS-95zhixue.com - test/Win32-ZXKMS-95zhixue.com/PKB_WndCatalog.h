#pragma once
LRESULT CALLBACK    WndViewPKBProc(HWND, UINT, WPARAM, LPARAM);

ATOM RegWnd_PKB_Catalog(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndViewPKBProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(nullptr, IDC_HAND);
	wcex.hbrBackground = hBrushDefault;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"Wnd_PKB_Catalog";
	wcex.hInstance = hInstance;
	wcex.hIconSm = NULL;

	return RegisterClassExW(&wcex);
}
LRESULT CALLBACK WndViewPKBProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_CREATE:
	{
		/*
		WCHAR tempStr1[10];
		_itow_s(GetLastError(), tempStr1, 10);
		MessageBox(hWnd, tempStr1, TEXT("´íÎóÂë"), MB_OK | MB_ICONINFORMATION);
		*/
		HWND NW_hWndCatalog2 = CreateWindowEx(NULL, L"Wnd", NULL, WS_CHILD | WS_VISIBLE,
			5, 0, 65, 30, hWnd, NULL, hInst, NULL);
		HWND NW_hWndCatalog3 = CreateWindowEx(NULL, L"Wnd", NULL, WS_CHILD | WS_VISIBLE,
			75, 0, 65, 30, hWnd, NULL, hInst, NULL);
		//SetClassLong(NW_hWndCatalog2, GCL_HBRBACKGROUND, (LONG)CreateSolidBrush(RGB(239, 239, 239)));
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