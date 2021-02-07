#include "stdafx.h"
#include "ControlBase.h"
map<DWORD, void*> IdDataBinding;
DWORD dwKeyID=0;
vector<DWORD> AlternativeIDs;

BOOL bDefWndRegistered = FALSE;
BOOL bDragStatus = FALSE;
DWORD dwDragWndID;
DWORD dwDropWndID;

HIMAGELIST hImgSysIconSmall;
DWORD GetIDFromPool() {
	if (AlternativeIDs.size() == 0) {
		DWORD tempCount = 20;
		while (tempCount>0)
		{
			AlternativeIDs.push_back(dwKeyID + tempCount);
			tempCount--;

		}
		dwKeyID = dwKeyID + 20;
	}
	DWORD returnData = *(AlternativeIDs.end() - 1);
	//[AlternativeIDs->size() - 1];
	AlternativeIDs.pop_back();
	return returnData;
}
DWORD PutIDToPool(DWORD dwID) {
	AlternativeIDs.push_back(dwID);
	return dwID;
}
DWORD BindIdData(void *pData) {
	DWORD ID = GetIDFromPool();
	IdDataBinding.insert(map<DWORD, void*>::value_type(ID, pData));
	return ID;
}

void RegisterDefWnd(HINSTANCE hInstance) {
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = 0;
	wcex.lpfnWndProc = DefWindowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = sizeof(LONG);
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(nullptr, IDC_HAND);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"WndDef";
	wcex.hIconSm = NULL;

	if (!GetClassInfoEx(hInstance, wcex.lpszClassName, &wcex)) {
		RegisterClassExW(&wcex);
	}
	bDefWndRegistered = TRUE;
}

/*
**控件初始化
*/
void CtrlInit() {
	//注册窗体
	OleInitialize(NULL);
	SHFILEINFO sfi;
	hImgSysIconSmall = (HIMAGELIST)SHGetFileInfo(_T(""), 0, &sfi, sizeof(sfi), SHGFI_SYSICONINDEX | SHGFI_SMALLICON);
}