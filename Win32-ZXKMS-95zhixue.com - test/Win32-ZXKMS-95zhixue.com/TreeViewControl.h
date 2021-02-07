#pragma once
#include <map>
#include "TreeViewData.h"
class TREEVIEWCONTROL {
	TREEVIEWDATA TreeViewData;
	//vector<TreeNode<NODEDATA> *> vTreeNode;
	int m_id;
	static int static_m_id;
	void SetCur();
	vector<HWND> vHWNDItem;
	static map<int, TREEVIEWCONTROL*> m_map;
	TREEVIEWCONTROL(DWORD dataSource, wstring rootPath, wstring defaultPath);

	static LRESULT CALLBACK  Proc_WndTreeViewItem(HWND, UINT, WPARAM, LPARAM);
};

TREEVIEWCONTROL::TREEVIEWCONTROL(DWORD dataSource, wstring rootPath, wstring defaultPath) :TreeViewData(dataSource, rootPath, defaultPath)
{
	//vTreeNode = TreeViewData.vTreeNode;
}

LRESULT CALLBACK  TREEVIEWCONTROL::Proc_WndTreeViewItem(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_CREATE:
	{

	}
	break;
	case WM_PAINT:
	{
		TREEVIEWCONTROL *pThis = m_map[static_m_id];
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: �ڴ˴����ʹ�� hdc ���κλ�ͼ����...

		RECT rc;
		SetRect(&rc, 0, 0, 150, 20);
		HFONT hFont = CreateFontIndirect(&lfList);
		HFONT hOldFont = (HFONT)SelectObject(hdc, hFont); //��������ѡ��Ϊ�豸�����ĵ�ǰ���壬������֮ǰ������

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