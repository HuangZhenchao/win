// Win32test2.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include <windowsx.h>

LRESULT CALLBACK WndProc(HWND handleforwindow1, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK windowprocessforwindow2(HWND handleforwindow2, UINT message, WPARAM wParam, LPARAM lParam);

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32PROJECT2));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WIN32PROJECT1);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

bool window1closed = false;
bool window2closed = false;

//child hwnd 小子窗口，非弹出式子窗口，是CreateWindowEx设置了parent
HWND cHwnd[5]; //0 左窗口 1 右窗口或右上窗口  2 竖分隔符窗口 spiltter 3 右下窗口  4 横分隔符窗口 spiltter
			   //main hwnd
HWND mHwnd;




int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nShowCmd)
{
	bool endprogram = false;

	//create window 1



	HWND handleforwindow1 = CreateWindowEx(NULL,
		windowclassforwindow1.lpszClassName,
		L"Parent Window",
		WS_OVERLAPPEDWINDOW,
		200,
		200,
		640,
		480,
		NULL,
		NULL,
		hInst,
		NULL                /* No Window Creation data */
	);

	if (!handleforwindow1)
	{
		int nResult = GetLastError();

		MessageBox(NULL,
			L"Window creation failed",
			L"Window Creation Failed",
			MB_ICONERROR);
	}

	ShowWindow(handleforwindow1, nShowCmd);
	mHwnd = handleforwindow1;


	// create window 2

	WNDCLASSEX windowclassforwindow2;
	ZeroMemory(&windowclassforwindow2, sizeof(WNDCLASSEX));
	windowclassforwindow2.cbClsExtra = NULL;
	windowclassforwindow2.cbSize = sizeof(WNDCLASSEX);
	windowclassforwindow2.cbWndExtra = NULL;
	windowclassforwindow2.hbrBackground = (HBRUSH)COLOR_WINDOW;
	windowclassforwindow2.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowclassforwindow2.hIcon = NULL;
	windowclassforwindow2.hIconSm = NULL;
	windowclassforwindow2.hInstance = hInst;
	windowclassforwindow2.lpfnWndProc = (WNDPROC)windowprocessforwindow2;
	windowclassforwindow2.lpszClassName = L"window class2";
	windowclassforwindow2.lpszMenuName = NULL;
	windowclassforwindow2.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&windowclassforwindow2))
	{
		int nResult = GetLastError();
		MessageBox(NULL,
			L"Window class creation failed for window 2",
			L"Window Class Failed",
			MB_ICONERROR);
	}

	HWND handleforwindow2 = CreateWindowEx(NULL,
		windowclassforwindow2.lpszClassName,
		L"Child Window",
		WS_CHILD
		,
		0,
		0,
		195,
		480,
		handleforwindow1,
		NULL,
		hInst,
		NULL);

	if (!handleforwindow2)
	{
		int nResult = GetLastError();

		MessageBox(NULL,
			L"Window creation failed",
			L"Window Creation Failed",
			MB_ICONERROR);
	}

	ShowWindow(handleforwindow2, nShowCmd);
	cHwnd[0] = handleforwindow2;

	// create window 3

	HWND handleforwindow3 = CreateWindowEx(NULL,
		windowclassforwindow2.lpszClassName,
		L"Child Window",
		WS_CHILD
		,
		200,
		0,
		440,
		275,
		handleforwindow1,
		NULL,
		hInst,
		NULL);

	if (!handleforwindow3)
	{
		int nResult = GetLastError();

		MessageBox(NULL,
			L"Window creation failed",
			L"Window Creation Failed",
			MB_ICONERROR);
	}

	ShowWindow(handleforwindow3, nShowCmd);
	cHwnd[1] = handleforwindow3;

	// create window 4 spiltter

	WNDCLASSEX windowclassforwindow4;
	ZeroMemory(&windowclassforwindow4, sizeof(WNDCLASSEX));
	windowclassforwindow4.cbClsExtra = NULL;
	windowclassforwindow4.cbSize = sizeof(WNDCLASSEX);
	windowclassforwindow4.cbWndExtra = NULL;
	windowclassforwindow4.hbrBackground = (HBRUSH)COLOR_WINDOW;
	windowclassforwindow4.hCursor = LoadCursor(NULL, IDC_SIZEWE);
	windowclassforwindow4.hIcon = NULL;
	windowclassforwindow4.hIconSm = NULL;
	windowclassforwindow4.hInstance = hInst;
	windowclassforwindow4.lpfnWndProc = (WNDPROC)windowprocessforwindow2;
	windowclassforwindow4.lpszClassName = L"window class4";
	windowclassforwindow4.lpszMenuName = NULL;
	windowclassforwindow4.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&windowclassforwindow4))
	{
		int nResult = GetLastError();
		MessageBox(NULL,
			L"Window class creation failed for window 2",
			L"Window Class Failed",
			MB_ICONERROR);
	}

	HWND handleforwindow4 = CreateWindowEx(NULL,
		windowclassforwindow4.lpszClassName,
		L"Child Window",
		WS_CHILD \
		| WS_BORDER
		,
		195,
		0,
		3,
		480,
		handleforwindow1,
		NULL,
		hInst,
		NULL);

	if (!handleforwindow4)
	{
		int nResult = GetLastError();

		MessageBox(NULL,
			L"Window creation failed",
			L"Window Creation Failed",
			MB_ICONERROR);
	}

	ShowWindow(handleforwindow4, nShowCmd);
	cHwnd[2] = handleforwindow4;

	// create window 5 有窗口下面窗口

	HWND handleforwindow5 = CreateWindowEx(NULL,
		windowclassforwindow2.lpszClassName,
		L"Child Window",
		WS_CHILD
		,
		200,
		280,
		440,
		200,
		handleforwindow1,
		NULL,
		hInst,
		NULL);

	if (!handleforwindow5)
	{
		int nResult = GetLastError();

		MessageBox(NULL,
			L"Window creation failed",
			L"Window Creation Failed",
			MB_ICONERROR);
	}
	ShowWindow(handleforwindow5, nShowCmd);
	cHwnd[3] = handleforwindow5;

	// create window 6 spiltter 右窗口spiltter

	WNDCLASSEX windowclassforwindow6;
	ZeroMemory(&windowclassforwindow6, sizeof(WNDCLASSEX));
	windowclassforwindow6.cbClsExtra = NULL;
	windowclassforwindow6.cbSize = sizeof(WNDCLASSEX);
	windowclassforwindow6.cbWndExtra = NULL;
	windowclassforwindow6.hbrBackground = (HBRUSH)COLOR_WINDOW;
	windowclassforwindow6.hCursor = LoadCursor(NULL, IDC_SIZENS);
	windowclassforwindow6.hIcon = NULL;
	windowclassforwindow6.hIconSm = NULL;
	windowclassforwindow6.hInstance = hInst;
	windowclassforwindow6.lpfnWndProc = (WNDPROC)windowprocessforwindow2;
	windowclassforwindow6.lpszClassName = L"window class6";
	windowclassforwindow6.lpszMenuName = NULL;
	windowclassforwindow6.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&windowclassforwindow6))
	{
		int nResult = GetLastError();
		MessageBox(NULL,
			L"Window class creation failed for window 2",
			L"Window Class Failed",
			MB_ICONERROR);
	}

	HWND handleforwindow6 = CreateWindowEx(NULL,
		windowclassforwindow6.lpszClassName,
		L"Child Window",
		WS_CHILD \
		| WS_BORDER
		,
		200,
		275,
		480,
		1,
		handleforwindow1,
		NULL,
		hInst,
		NULL);

	if (!handleforwindow6)
	{
		int nResult = GetLastError();

		MessageBox(NULL,
			L"Window creation failed",
			L"Window Creation Failed",
			MB_ICONERROR);
	}

	ShowWindow(handleforwindow6, nShowCmd);
	cHwnd[4] = handleforwindow6;

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (endprogram == false) {
		if (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (window1closed == true && window2closed == true) {
			endprogram = true;
		}
	}

	return 0;
}

LRESULT CALLBACK windowprocessforwindow1(HWND handleforwindow, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY: {

		window1closed = true;
		return 0;
	}
					 break;
	}

	return DefWindowProc(handleforwindow, msg, wParam, lParam);
}

//子窗口处理
LRESULT CALLBACK windowprocessforwindow2(HWND handleforwindow, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//鼠标拖动spiltter时，相对于主窗口的x
	int spos, border;
	//鼠标左键单击spiltter时，在spiltter内部的偏移长度
	int posinspiltter = 0;
	static BOOL	bSplitterMoving;

	RECT rectMain, rectSpiltterH, rectRightUp;

	switch (msg)
	{
	case WM_DESTROY: {


		window2closed = true;
		return 0;
	}
	case WM_LBUTTONDOWN:

		if (handleforwindow == cHwnd[0])
		{
			MessageBox(NULL, TEXT("1鼠标左键点击"), TEXT("Win32_Mouse"), MB_OK);
		}
		else if (handleforwindow == cHwnd[1])
		{
			MessageBox(NULL, TEXT("2鼠标左键点击"), TEXT("Win32_Mouse"), MB_OK);
		}
		else if (handleforwindow == cHwnd[3])
		{
			MessageBox(NULL, TEXT("3鼠标左键点击"), TEXT("Win32_Mouse"), MB_OK);
		}
		else if (handleforwindow == cHwnd[2])
		{
			//MessageBox(NULL, TEXT("3鼠标左键点击"), TEXT("Win32_Mouse"), MB_OK);
			bSplitterMoving = TRUE;
			//抓住左窗口，WM_MOUSEMOVE得到的就是鼠标相对于左窗口移动距离
			SetCapture(cHwnd[0]);
			posinspiltter = GET_X_LPARAM(lParam);
			return 0;
		}
		else if (handleforwindow == cHwnd[4])
		{
			//MessageBox(NULL, TEXT("3鼠标左键点击"), TEXT("Win32_Mouse"), MB_OK);
			bSplitterMoving = TRUE;
			//抓住右上窗口
			SetCapture(cHwnd[1]);
			posinspiltter = GET_Y_LPARAM(lParam);
			return 0;
		}

		break;
	case WM_LBUTTONUP:
		ReleaseCapture();
		bSplitterMoving = FALSE;
		return 0;
	case WM_MOUSEMOVE:
		if ((wParam == MK_LBUTTON) & bSplitterMoving &(handleforwindow == cHwnd[0]))
		{
			spos = GET_X_LPARAM(lParam);
			//spos- posinspiltter  鼠标的相对于主窗口的x - 在spiltter内的偏移 就是左窗口的宽度
			MoveWindow(cHwnd[0], 0, 0, spos - posinspiltter, 480, TRUE); //左窗口
																		 //spos+(5- posinspiltter)  5- posinspiltter是点击位置到spiltter有边框的距离， 鼠标的相对于主窗口的x + 点击位置到spiltter有边框的距离就是 有窗口的x起始位置
																		 //640-(spos + (5 - posinspiltter))  640是主窗口宽度 - 有窗口的起始位置，就是宽度

			MoveWindow(cHwnd[2], spos, 0, 5, 480, TRUE); //spiltter

			GetWindowRect(mHwnd, &rectMain);
			GetWindowRect(cHwnd[1], &rectRightUp);

			MoveWindow(cHwnd[1], spos + (5 - posinspiltter), 0, 640 - (spos + (5 - posinspiltter)), rectRightUp.bottom - rectMain.top - 31, TRUE); //右窗口

			MoveWindow(cHwnd[3], spos + (5 - posinspiltter), rectRightUp.bottom - rectMain.top - 31 + 5, 640 - (spos + (5 - posinspiltter)), 480 - (rectRightUp.bottom - rectMain.top - 31 + 5), TRUE); //右下
			MoveWindow(cHwnd[4], spos + (5 - posinspiltter), rectRightUp.bottom - rectMain.top - 31, 640 - (spos + (5 - posinspiltter)), 5, TRUE); //spiltter
		}
		else if ((wParam == MK_LBUTTON) &bSplitterMoving &(handleforwindow == cHwnd[1]))
		{
			border = GetSystemMetrics(SM_CXBORDER);
			spos = GET_Y_LPARAM(lParam);

			GetWindowRect(mHwnd, &rectMain);
			GetWindowRect(cHwnd[2], &rectSpiltterH);
			GetWindowRect(cHwnd[1], &rectRightUp);

			MoveWindow(cHwnd[1], rectRightUp.left - rectMain.left - 8, 0, rectRightUp.right - rectRightUp.left, spos - posinspiltter, TRUE); //右上
			MoveWindow(cHwnd[3], rectRightUp.left - rectMain.left - 8, spos + (5 - posinspiltter), rectRightUp.right - rectRightUp.left, 480 - (spos + (5 - posinspiltter)), TRUE); //右下
			MoveWindow(cHwnd[4], rectRightUp.left - rectMain.left - 8, spos - posinspiltter, rectRightUp.right - rectRightUp.left, 5, TRUE); //spiltter

		}
		return 0;
	}

	return DefWindowProc(handleforwindow, msg, wParam, lParam);
}