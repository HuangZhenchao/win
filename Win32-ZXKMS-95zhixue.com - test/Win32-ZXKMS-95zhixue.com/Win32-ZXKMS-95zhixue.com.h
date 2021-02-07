#pragma once

#include "resource.h"
#include <vector>
#include <string>
using namespace std;
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
LOGFONT lfList;
/*
struct LinkedListNode
{
	HWND hwnd;
	struct LinkedListNode *next;
};
struct LinkedListNode *head;
struct LinkedListNode *tail, *newNode;
*/
DWORD dWidthBorder =2;
DWORD dWidthClient;
DWORD dHeightClient;
DWORD dWidthHeader;
DWORD dHeightHeader =50;

DWORD dWidthSide = 60;
DWORD dHeightFooter = 20;
WNDINI WndIni = { 0 };

WCHAR szPath[MAX_PATH];							//���г����ļ���dll���ڵ�λ��
WCHAR IniPath[MAX_PATH];

// ȫ�ֱ���: 
HINSTANCE hInst;							// ��ǰʵ��


//������ر���
//������
HWND hWndMain;
HWND hWndHeader;
HWND hWndSide;
HWND hWndFooter;
HWND hWndCurrentItem;
HWND hWndCurrentContainer;
const DWORD ItemCount = 2;
const DWORD dButtonGroupCount = 2;
HWND hItemButton[dButtonGroupCount];
HWND hWndItem[ItemCount];
HWND hWndContainer[ItemCount];
BOOL bMouseTrack = FALSE;

HWND hButtonNW;
HWND hButtonPKB;
HWND hButtonQuit;
HWND hButtonMax;
HWND hButtonMin;

//��ˢ
HBRUSH hBrushDefault = (HBRUSH)(COLOR_WINDOW + 1);
HBRUSH hBrushWndHeader = CreateSolidBrush(RGB(233, 207, 174));
//HBRUSH hBrushWndHeader = CreateSolidBrush(RGB(230, 238, 245));
HBRUSH hBrushSelectedItem = CreateSolidBrush(RGB(248, 242, 220));
//HBRUSH hBrushSelectedItem=CreateSolidBrush(RGB(207, 214, 229));  
HBRUSH hBrushItem=CreateSolidBrush(RGB(42, 51, 60));//��ɫ
HBRUSH hBrushHoverItem;
HBRUSH hBrushIcon = CreateSolidBrush(RGB(54, 78, 111));
HBRUSH hBrushGray = CreateSolidBrush(RGB(239, 239, 239));
HBRUSH hBrushCyan = CreateSolidBrush(RGB(54, 78, 111));
HBRUSH hBrushBgContainer = CreateSolidBrush(RGB(221, 0, 27));
HBRUSH hBrush1 = CreateSolidBrush(RGB(255,241, 222));
HBRUSH hBrush2 = CreateSolidBrush(RGB(255, 0, 0));
//����ص�������ǰ������
HBITMAP hBitmapLogo;
BITMAP         bitmaplogo;

HBITMAP hBitmapPKB;
BITMAP         bitmapPKB;

HBITMAP hBitmapNW;
BITMAP         bitmapNW;


HWND hWnd_NW_NovelChapterList;
HWND hWnd_NW_NovelChapterList1;
LRESULT CALLBACK    WndMainProc(HWND, UINT, WPARAM, LPARAM);
//LRESULT CALLBACK    WndHeaderProc(HWND, UINT, WPARAM, LPARAM);
//LRESULT CALLBACK    WndFooterProc(HWND, UINT, WPARAM, LPARAM);
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
	wcex.hCursor = LoadCursor(nullptr, IDC_HAND);
	wcex.hbrBackground = hBrushWndHeader;
	//wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WIN32ZXKMS95ZHIXUECOM);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
	wcex.hbrBackground = hBrushGray;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"Wnd";
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
ATOM RegWndHeader(HINSTANCE);
ATOM RegWndFooter(HINSTANCE);
ATOM RegWndHeaderButton(HINSTANCE);