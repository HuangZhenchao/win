#pragma once
namespace NW{
HWND NW_hWndCatalog;

LRESULT CALLBACK WndContainerProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

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
	}
	case WM_SIZE:
	{

	}
	break;
	case WM_PAINT:
	{

	}
	//break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
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
	wcex.hbrBackground = CreateSolidBrush(RGB(37, 37, 38));
	//wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	//wcex.hbrBackground = (HBRUSH)NULL_BRUSH;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"WndCatalog1";
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
	wcex.hbrBackground = CreateSolidBrush(RGB(221, 0, 27));
	//wcex.hbrBackground = (HBRUSH)NULL_BRUSH;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"WndContainer1";
	wcex.hIconSm = NULL;

	return RegisterClassExW(&wcex);
}

//侧边栏按钮消息处理程序。




LRESULT CALLBACK WndContainerProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		RegWndView(hInst);
		
		NW_hWndCatalog = CreateWindowEx(NULL, L"WndCatalog1", NULL, WS_CHILD | WS_VISIBLE,
			0, 0,0, 0, hWnd, NULL, hInst, NULL);
	}
	case WM_SIZE:
	{
		RECT rect;
		GetClientRect(hWnd, &rect);
		DWORD widthWndContainer = rect.right - rect.left;  // 获得客户区宽度
		DWORD heightWndContainer = rect.bottom - rect.top;  // 获得客户区高度
		MoveWindow(NW_hWndCatalog, 0, 0, 150, heightWndContainer, TRUE);
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