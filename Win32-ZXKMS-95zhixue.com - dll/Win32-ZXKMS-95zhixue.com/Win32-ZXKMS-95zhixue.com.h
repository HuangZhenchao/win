#pragma once

#include "resource.h"

/*������ļ�������ȫ�ֱ���
**�Լ�����ע�ᴰ���ຯ��
*/
#define MAX_LOADSTRING 100
WCHAR szTitle[MAX_LOADSTRING];                  // �������ı�
WCHAR szWindowClass[MAX_LOADSTRING];            // ����������

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

WCHAR szPath[MAX_PATH];							//���г����ļ���dll���ڵ�λ��
WCHAR IniPath[MAX_PATH];

// ȫ�ֱ���: 
HINSTANCE hInst;							// ��ǰʵ��


//������ر���
//������
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

//����ص�������ǰ������
LRESULT CALLBACK    WndSideItemProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    WndMainProc(HWND, UINT, WPARAM, LPARAM);
//LRESULT CALLBACK    WndSideProc(HWND, UINT, WPARAM, LPARAM);
//INT_PTR CALLBACK    DlgProc(HWND, UINT, WPARAM, LPARAM);
//LRESULT CALLBACK    WndCatalogProc(HWND, UINT, WPARAM, LPARAM);
//
//  ����: RegWndMain()
//
//  Ŀ��: ע���������ࡣ
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
//  ����: RegWndSide()
//
//  Ŀ��: ע�����������ࡣ
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