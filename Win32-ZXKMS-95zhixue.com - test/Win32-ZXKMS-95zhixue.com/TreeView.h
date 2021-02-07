#pragma once
#include "FileTree.h"
class TVINFO
{
public:
	FileTree ft;
	vector<HWND> vHWNDItem;
	wstring wsProjectPath=L"E:\\OneDrive - elcass\\MyProject";
	wstring wsTempStr=L"";
};

vector<TVINFO> vTVInfo(5);

LRESULT CALLBACK  Proc_WndTreeViewItem(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void SetDirectory(INT tvID, wstring wsProjectPath, wstring wsTempStr) {
	vTVInfo[tvID].wsProjectPath = wsProjectPath;
	vTVInfo[tvID].wsTempStr = wsTempStr;
}

HWND CreateWindowTVItem(HINSTANCE hInstance, HWND hWndParent, int x, int y, int width, int height)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = Proc_WndTreeViewItem;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = sizeof(LONG);
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = CreateSolidBrush(RGB(248, 242, 220));
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"MyTreeViewItem";
	wcex.hIconSm = NULL;

	if (!GetClassInfoEx(hInstance, wcex.lpszClassName, &wcex)) {
		RegisterClassExW(&wcex);
	}

	return  CreateWindowEx(NULL, L"MyTreeViewItem", NULL, WS_CHILD | WS_VISIBLE,
		x, y, width, height, hWndParent, NULL, hInstance, NULL);

	//vHandlerOfScrollCtrl.push_back(hScrollCtrl);
	//return hWnd_TVItem;
}

vector<int> TreeViewAsContent(INT tvID,HandlerOfScrollCtrl hScrollCtrl) {
	
	vTVInfo[tvID].ft.InitFileTree(vTVInfo[tvID].wsProjectPath, vTVInfo[tvID].wsTempStr);
	
	vTVInfo[tvID].ft.ClearVTreeNode();
	vTVInfo[tvID].ft.SetLOGFONT(lfList);
	vTVInfo[tvID].ft.TreeToVector(vTVInfo[tvID].ft.tree.pRoot);
	vector<HWND>().swap(vTVInfo[tvID].vHWNDItem);
	HDC hdc = GetDC(hScrollCtrl.hWnd_Content);
	HFONT hFont = CreateFontIndirect(&lfList);
	SelectObject(hdc, hFont);
	
	for (int i = 0; i < vTVInfo[tvID].ft.vTreeNode.size(); i++) {
		vTVInfo[tvID].vHWNDItem.push_back(CreateWindowTVItem(hScrollCtrl.hInstance, hScrollCtrl.hWnd_Content,0,0,0,0));
		SetWindowLongPtr(vTVInfo[tvID].vHWNDItem[i], 0, (tvID <<16)|i);
		
		SIZE sizeTest;
		GetTextExtentPoint32(hdc, vTVInfo[tvID].ft.vTreeNode[i]->data.FileData.cFileName, wcslen(vTVInfo[tvID].ft.vTreeNode[i]->data.FileData.cFileName), &sizeTest);
		DWORD ItemWidth = vTVInfo[tvID].ft.dIndent*(vTVInfo[tvID].ft.vTreeNode[i]->data.depth - 1) + sizeTest.cx;//sizeTest.cx
		if (vTVInfo[tvID].ft.MaxItemWidth<ItemWidth) {
			vTVInfo[tvID].ft.MaxItemWidth = ItemWidth;
		}
		vTVInfo[tvID].ft.ItemHeight = sizeTest.cy;
	}
	ReleaseDC(hScrollCtrl.hWnd_Content, hdc);

	vector<int> contentSize;
	contentSize.push_back(vTVInfo[tvID].ft.MaxItemWidth+20);
	contentSize.push_back((vTVInfo[tvID].ft.ItemHeight + 1)*vTVInfo[tvID].vHWNDItem.size());
	return contentSize;
}

void TreeViewItemSize(INT tvID, HandlerOfScrollCtrl hScrollCtrl) {
	for (int i = 0; i < vTVInfo[tvID].vHWNDItem.size(); i++) {
		MoveWindow(vTVInfo[tvID].vHWNDItem[i], 0, i*(vTVInfo[tvID].ft.ItemHeight + 1), hScrollCtrl.Hsi.cxContent, vTVInfo[tvID].ft.ItemHeight, TRUE);
	}
}

LRESULT CALLBACK  Proc_WndTreeViewItem(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_CREATE:
	{

	}
	break;
	case WM_PAINT:
	{

		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此处添加使用 hdc 的任何绘图代码...

		RECT rc;
		SetRect(&rc, 0, 0, 150, 20);
		HFONT hFont = CreateFontIndirect(&lfList);
		HFONT hOldFont = (HFONT)SelectObject(hdc, hFont); //把新字体选定为设备环境的当前字体，并返回之前的字体

		int tvID = HIWORD(GetWindowLongPtr(hWnd, 0));
		int id = LOWORD(GetWindowLongPtr(hWnd, 0));
		

		TextOut(hdc, vTVInfo[tvID].ft.dIndent*(vTVInfo[tvID].ft.vTreeNode[id]->data.depth), 0,
			vTVInfo[tvID].ft.vTreeNode[id]->data.FileData.cFileName,
			wcslen(vTVInfo[tvID].ft.vTreeNode[id]->data.FileData.cFileName));

		EndPaint(hWnd, &ps);
	}
	break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}