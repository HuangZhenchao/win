#pragma once
#include <commctrl.h>
#pragma comment(lib,"comctl32.lib")
#include "MyTreeView.h"

class SELECTCOMDITIONAL {
public:
	BOOL bPenetrateFolderFlag = FALSE;
	wstring wsPath;
	DWORD dwTagType;
	wstring wsTag;
	wstring wsSearch;
};
class YListView {
public:
	HWND hWndListView;
	wstring wsDataSource;
	YListView(HWND hWnd,wstring wsPath);
	~YListView();

	SELECTCOMDITIONAL SelectConditional;
	thread tLoadListItem;
	HANDLE hEventLoadStart;
	HANDLE hEventLoadDone;
	BOOL bTerminateFlag = FALSE;
	BOOL bSkip = FALSE;

	void fSetHIMAGELIST();
	void fSetColumn();
	void tFunLoadListItem();
	void ListViewReload(wstring wsPath);
	//void fDeleteChildItem(HTREEITEM hTreeItem);	
	map<HWND, YListView*> m_HWND_Data;
};
