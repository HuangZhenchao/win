// NovelWriter.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "NovelWriter.h"

namespace  NovelWriter {
	WNDCLASSEXW wcex;
	HINSTANCE hInst;
	WCHAR szClassName[100] = L"WndCatalog";
	HWND hWndCatalog;
	LRESULT CALLBACK WndCatalogProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		UNREFERENCED_PARAMETER(lParam);
		switch (message)
		{
		case WM_CREATE:
		{
			WCHAR tempStr1[10];
			_itow_s(GetLastError(), tempStr1, 10);
			MessageBox(hWnd, tempStr1, TEXT("������"), MB_OK | MB_ICONINFORMATION);
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
	ATOM RegWndCatalog(HINSTANCE hInstance)
	{
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = NovelWriter::WndCatalogProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hIcon = NULL;
		wcex.hCursor = LoadCursor(nullptr, IDC_HAND);
		wcex.hbrBackground = CreateSolidBrush(RGB(37, 37, 38));
		//wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		//wcex.hbrBackground = (HBRUSH)NULL_BRUSH;
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = szClassName;
		wcex.hInstance = hInstance;
		wcex.hIconSm = NULL;

		return RegisterClassExW(&wcex);
	}
}
// ���ǵ���������һ��ʾ����
void fnInitDll(HWND hWnd)
{

	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	//if (hWndSideItem == hWndSideItemPKB) {
	HBRUSH hbrush = CreateSolidBrush(RGB(198, 6, 36));
	//HPEN hpen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
	//SelectObject(hdc, brush);
	SelectObject(hdc, hbrush);
	//SelectObject(hdc, GetStockObject(NULL_BRUSH));
	Ellipse(hdc, 10, 10, 40, 40);
	
	EndPaint(hWnd, &ps);
	//DeleteObject(hpen);
	//}
	
}

void fnCreateWndContainer(HINSTANCE hInstance, HWND hWnd, DWORD i)
{
	using namespace NovelWriter;
	
	//ȫ�ֱ�����ֵ
	WCHAR tempStr[10];
	_itow_s(i, tempStr, 10);
	lstrcat(szClassName, tempStr);

	hInst = hInstance;
	//MoveWindow(hWnd, 50, 0, 500+50*i, 500, TRUE);
	if (!GetClassInfoEx(hInst, szClassName, &wcex))
	{
		RegWndCatalog(hInst);
	}
	RECT rect;
	GetClientRect(hWnd, &rect);
	DWORD widthWndContainer = rect.right - rect.left;  // ��ÿͻ������
	DWORD heightWndContainer = rect.bottom - rect.top;  // ��ÿͻ����߶�
	hWndCatalog=CreateWindowEx(NULL, szClassName, NULL, WS_CHILD | WS_VISIBLE,
		0, 0, 150+50*i, heightWndContainer, hWnd, (HMENU)i, hInst, NULL);
	
	/*
	WCHAR IniPath[MAX_PATH];
	//��ʼ��path
	HMODULE hModule = GetModuleHandleW(NULL);
	GetModuleFileName(hModule, IniPath, MAX_PATH);
	// setting -> link add Shlwapi.lib   #include <Shlwapi.h>
	PathRemoveFileSpec(IniPath);
	WCHAR szDLLFilePath[MAX_PATH];
	// ���������Ŀ¼���ļ���·�����ַ�����ʹ��ͨ���"*" 
	lstrcpy(szDLLFilePath, IniPath);
	//MessageBox(hWnd, szDLLFilePath, TEXT("������"), MB_OK | MB_ICONINFORMATION);*/
}
void fnReSize(HWND hWndContainer) {
	RECT rect;
	GetClientRect(hWndContainer, &rect);
	DWORD widthWndContainer = rect.right - rect.left;  // ��ÿͻ������
	DWORD heightWndContainer = rect.bottom - rect.top;  // ��ÿͻ����߶�
	
	MoveWindow(NovelWriter::hWndCatalog, 0, 0, 150, heightWndContainer, TRUE);
}