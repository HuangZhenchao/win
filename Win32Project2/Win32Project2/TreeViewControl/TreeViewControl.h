#pragma once
#include <map>
#include "TreeViewData.h"
class TREEVIEWSTYLE {
public:
	HINSTANCE hInstance;
	HWND hWndParent;

	DWORD dIndent;
};


class TREEVIEWCONTROL {
private:
	DWORD m_id=0;
	static int static_m_id;
	static BOOL bWndRegistered;
	static map<DWORD, TREEVIEWCONTROL*> m_map;
public:
	TREEVIEWDATA *TreeViewData;
	//vector<TreeNode<NODEDATA> *> vTreeNode;
	TREEVIEWSTYLE *TVStyle;
	//void SetCur();
	vector<HWND> vHWNDItem;
	HWND hWndControl;
	DWORD MaxWidth=0;
	DWORD TotalHeight;
	DWORD ItemHeight;
	TREEVIEWCONTROL(TREEVIEWDATA *);

	void RegisterWnd();
	HWND CreateTreeViewControl(TREEVIEWSTYLE*);
	void CreateTreeViewItem();
	void TraverseTVItem(TreeNode<NODEDATA> *);
	static void CreateTVItem(TreeNode<NODEDATA> *, TREEVIEWCONTROL *);

	static LRESULT CALLBACK  Proc_WndTreeViewControl(HWND, UINT, WPARAM, LPARAM);
	static LRESULT CALLBACK  Proc_WndTreeViewItem(HWND, UINT, WPARAM, LPARAM);
	static LRESULT CALLBACK  Proc_WndTreeViewItemButton(HWND, UINT, WPARAM, LPARAM);
};

int TREEVIEWCONTROL::static_m_id;
map<DWORD, TREEVIEWCONTROL*> TREEVIEWCONTROL::m_map;
BOOL TREEVIEWCONTROL::bWndRegistered;

TREEVIEWCONTROL::TREEVIEWCONTROL(TREEVIEWDATA *treeViewData)
{
	TreeViewData = treeViewData;
	m_id = m_map.size() + 1;
	m_map.insert(map<DWORD, TREEVIEWCONTROL*>::value_type(m_id, this));
	wchar_t tempStr1[32];
	_itow_s((LONG)this, tempStr1, 32);
	//MessageBox(NULL, tempStr1, TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
	//vTreeNode = TreeViewData.vTreeNode;
}

void TREEVIEWCONTROL::RegisterWnd() 
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style =  CS_DBLCLKS;
	wcex.lpfnWndProc = Proc_WndTreeViewControl;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = sizeof(LONG);
	wcex.hInstance = TVStyle->hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"MyTreeViewControl";
	wcex.hIconSm = NULL;

	if (!GetClassInfoEx(TVStyle->hInstance, wcex.lpszClassName, &wcex)) {
		RegisterClassExW(&wcex);
	}
	wcex.style = CS_DBLCLKS;
	wcex.lpfnWndProc = Proc_WndTreeViewItem;
	wcex.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wcex.lpszClassName = L"MyTreeViewItem";
	if (!GetClassInfoEx(TVStyle->hInstance, wcex.lpszClassName, &wcex)) {
		RegisterClassExW(&wcex);
	}
	bWndRegistered = TRUE;
}

void TREEVIEWCONTROL::TraverseTVItem(TreeNode<NODEDATA> *pNode) {
	if (pNode == nullptr) {
		return;
	}	
	MessageBox(NULL, pNode->data.wsNodePath.c_str(), TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
	if (pNode->data.dwExpanded) {
		TraverseTVItem(pNode->pFirstChild);
	}
	TraverseTVItem(pNode->pNextBrother);
}

HWND TREEVIEWCONTROL::CreateTreeViewControl(TREEVIEWSTYLE *tvStyle)
{
	TVStyle = tvStyle;
	if (!bWndRegistered) { RegisterWnd(); }
	DWORD dControlID = (1<<28) | (m_id<<16);
	hWndControl= CreateWindowEx(NULL, L"MyTreeViewControl", NULL, WS_CHILD | WS_VISIBLE| WS_CLIPCHILDREN,
		0, 0, 0, 0, tvStyle->hWndParent, (HMENU)dControlID, tvStyle->hInstance, NULL);
	TreeViewData->hWndControl = hWndControl;
	TreeViewData->hInstance = tvStyle->hInstance;
	//CreateTreeViewItem();
	TreeViewData->InitTree();
	return hWndControl;
}
void TREEVIEWCONTROL::CreateTreeViewItem() {

	vector<HWND>().swap(vHWNDItem);
	 
	HDC hdc = GetDC(hWndControl);
	HFONT hFont = CreateFontIndirect(&lfList);
	SelectObject(hdc, hFont);

	for (DWORD i = 0; i < TreeViewData->vTreeNode.size(); i++) {
		DWORD dItemID = (1 << 28) | (m_id << 16) | (1 << 12) | i;

		vHWNDItem.push_back(CreateWindowEx(NULL, L"MyTreeViewItem", NULL, WS_CHILD | WS_VISIBLE| WS_CLIPCHILDREN,
			0, 0, 0, 0, hWndControl, (HMENU)dItemID, TVStyle->hInstance, NULL));

		SIZE sizeTest;
		GetTextExtentPoint32(hdc, TreeViewData->vTreeNode[i]->data.wsNodeName.c_str(), wcslen(TreeViewData->vTreeNode[i]->data.wsNodeName.c_str()), &sizeTest);
		DWORD ItemWidth = TVStyle->dIndent*(TreeViewData->vTreeNode[i]->data.dwNodeDepth) + sizeTest.cx;//sizeTest.cx
		if (MaxWidth<ItemWidth) {
			MaxWidth = ItemWidth;
		}
		ItemHeight = sizeTest.cy;
	}
	ReleaseDC(hWndControl, hdc);
	TotalHeight = TreeViewData->vTreeNode.size()*(ItemHeight + 1);
	MoveWindow(hWndControl, 0, 0, MaxWidth, TotalHeight, TRUE);
}

LRESULT CALLBACK  TREEVIEWCONTROL::Proc_WndTreeViewControl(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_CREATE:
	{

	}
	break;
	case WM_ERASEBKGND:
		return TRUE;
	case WM_SIZE: 
	{
		DWORD dControlID = GetWindowLongPtr(hWnd,GWLP_ID);
		DWORD map_id= (dControlID<<4)>>20;
		TREEVIEWCONTROL *pThis = m_map[map_id];

		if ((pThis->vHWNDItem).empty()) {
			
			break;
		}
		//DWORD index = 0;
		//for (vector<HWND>::iterator iter = pThis->vHWNDItem.begin(); iter != pThis->vHWNDItem.end(); iter++) {
		//	MoveWindow(*iter, 0, index*(pThis->ItemHeight + 1), pThis->MaxWidth, pThis->ItemHeight, TRUE);
		//	index++;
		//}
		pThis->TraverseTVItem(pThis->TreeViewData->tree.pRoot);
	}
	break;
	case WM_PAINT:
	{
		DWORD dControlID = GetWindowLongPtr(hWnd, GWLP_ID);
		DWORD map_id = (dControlID << 4) >> 20;
		TREEVIEWCONTROL *pThis = m_map[map_id];
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此处添加使用 hdc 的任何绘图代码...

		EndPaint(hWnd, &ps);
	}
	break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}
LRESULT CALLBACK  TREEVIEWCONTROL::Proc_WndTreeViewItem(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_CREATE:
	{

	}
	break;
	case WM_LBUTTONDBLCLK:
	{
		DWORD dControlID = GetWindowLongPtr(hWnd, GWLP_ID);
		DWORD map_id = (dControlID << 4) >> 20;
		DWORD v_id = (dControlID << 20) >> 20;
		//wchar_t tempStr1[32];
		//_itow_s(map_id, tempStr1, 32);
		//MessageBox(NULL, L"双击", TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
		TREEVIEWCONTROL *pThis = (TREEVIEWCONTROL*)m_map[map_id];
		pThis->TreeViewData->ExpandCollapsePath(pThis->TreeViewData->vTreeNode[v_id]);
		pThis->CreateTreeViewItem();
	}
	break;
	case WM_PAINT:
	{
		DWORD dControlID = GetWindowLongPtr(hWnd, GWLP_ID);
		DWORD map_id = (dControlID << 4) >> 20;
		DWORD v_id= (dControlID << 20) >> 20;
		//wchar_t tempStr1[32];
		//_itow_s(map_id, tempStr1, 32);
		//MessageBox(NULL, tempStr1, TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
		TREEVIEWCONTROL *pThis = (TREEVIEWCONTROL*)m_map[map_id];
		PAINTSTRUCT ps;

		HDC hdcDebug = GetDC(hWndMain);
		RECT rcDebug;
		SetRect(&rcDebug, 200, 20, 150, 20);
		wchar_t tempStr1[32];
		_itow_s((LONG)pThis, tempStr1, 32);
		TextOut(hdcDebug, 200, 0, tempStr1, wcslen(tempStr1));//wcslen(tempStr1)

		ReleaseDC(hWndMain, hdcDebug);

		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此处添加使用 hdc 的任何绘图代码...

		RECT rc;
		SetRect(&rc, 0, 0, 150, 20);
		HFONT hFont = CreateFontIndirect(&lfList);
		HFONT hOldFont = (HFONT)SelectObject(hdc, hFont); //把新字体选定为设备环境的当前字体，并返回之前的字体
		//MessageBox(NULL, tempStr1, TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
		TextOut(hdc, pThis->TVStyle->dIndent*(pThis->TreeViewData->m_HWND_TreeNode[hWnd]->data.dwNodeDepth), 0,
			pThis->TreeViewData->m_HWND_TreeNode[hWnd]->data.wsNodeName.c_str(),
			wcslen(pThis->TreeViewData->m_HWND_TreeNode[hWnd]->data.wsNodeName.c_str()));
		//pThis->TVStyle.dIndent*(pThis->TreeViewData.vTreeNode[v_id]->data.dNodeDepth)
		//pThis->TreeViewData.vTreeNode[v_id]->data.NodeName.c_str()
		//pThis->TreeViewData.vTreeNode[v_id]->data.NodeName.c_str()
		EndPaint(hWnd, &ps);
	}
	break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}
LRESULT CALLBACK  TREEVIEWCONTROL::Proc_WndTreeViewItemButton(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_CREATE:
	{

	}
	break;
	case WM_PAINT:
	{
		DWORD dControlID = GetWindowLongPtr(hWnd, GWLP_ID);
		DWORD map_id = (dControlID << 4) >> 20;
		TREEVIEWCONTROL *pThis = m_map[map_id];
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此处添加使用 hdc 的任何绘图代码...

		EndPaint(hWnd, &ps);
	}
	break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}