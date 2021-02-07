#pragma once

#include "resource.h"

/*在这个文件里声明全局变量
**以及定义注册窗体类函数
*/
#define MAX_LOADSTRING 100
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

struct WNDINI {
	int WndMainX;
	int WndMainY;
	int WndMainWidth;
	int WndMainHeight;
};

DWORD widthClient;
DWORD heightClient;
DWORD widthSide=50;

WNDINI WndIni = { 0 };

WCHAR szPath[MAX_PATH];							//运行程序文件或dll所在的位置
WCHAR IniPath[MAX_PATH];

// 全局变量: 
HINSTANCE hInst;							// 当前实例


//窗体相关变量
//窗体句柄
HWND hWndMain;
HWND hWndSide;
HWND hWndCurrentItem;
HWND hWndCurrentContainer;
const DWORD MaxItemNum = 10;
HWND hWndSideItem[MaxItemNum];
HWND hWndContainer[MaxItemNum];

HINSTANCE hDll[MaxItemNum];
typedef void(*DLLFNPAINTITEMICON) (HWND);
typedef void(*DLLFNCREATEWNDCONTAINER) (HINSTANCE,HWND, DWORD);
typedef void(*DLLFNRESIZE) (HWND);
DLLFNPAINTITEMICON  dllfnPaintItemIcon[MaxItemNum];
DLLFNCREATEWNDCONTAINER dllfnCreateWndContainer[MaxItemNum];
DLLFNRESIZE dllfnReSize[MaxItemNum];
HWND hWndVice;

//窗体回调函数的前向声明
LRESULT CALLBACK    WndSideItemProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    WndMainProc(HWND, UINT, WPARAM, LPARAM);
//LRESULT CALLBACK    WndSideProc(HWND, UINT, WPARAM, LPARAM);
//INT_PTR CALLBACK    DlgProc(HWND, UINT, WPARAM, LPARAM);
//LRESULT CALLBACK    WndCatalogProc(HWND, UINT, WPARAM, LPARAM);
//
//  函数: RegWndMain()
//
//  目的: 注册主窗口类。
//
ATOM RegWndMain(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndMainProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32ZXKMS95ZHIXUECOM));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	//wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WIN32ZXKMS95ZHIXUECOM);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}
//
//  函数: RegWndSide()
//
//  目的: 注册侧边栏窗口类。
//
ATOM RegWndSide(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = DefWindowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = CreateSolidBrush(RGB(42, 51, 60));
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"WndSide";
	wcex.hIconSm = NULL;

	return RegisterClassExW(&wcex);
}

ATOM RegWnd(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = DefWindowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = CreateSolidBrush(RGB(221, 0, 27));
	//wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"Wnd";
	wcex.hIconSm = NULL;

	return RegisterClassExW(&wcex);
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
	wcex.hbrBackground = CreateSolidBrush(RGB(42, 51, 60));
	//wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"WndSideItem";
	wcex.hIconSm = NULL;

	return RegisterClassExW(&wcex);
}


/*
ATOM RegWndCatalog(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndCatalogProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(nullptr, IDC_HAND);
	wcex.hbrBackground = CreateSolidBrush(RGB(37, 37, 38));
	//wcex.hbrBackground = (HBRUSH)NULL_BRUSH;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"WndCatalog";
	wcex.hIconSm = NULL;

	return RegisterClassExW(&wcex);
}
*/

ATOM RegWndFileList(HINSTANCE hInstance);
ATOM RegWndSpiltter(HINSTANCE hInstance);
ATOM RegWndEdit(HINSTANCE hInstance);