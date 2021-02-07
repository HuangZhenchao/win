#include "stdafx.h"
#include "MyTreeView.h"
/*
void YTreeView::fDeleteChildItem(HTREEITEM hTreeItem) {
	queue<HTREEITEM> qRemoveItem;
	qRemoveItem.push(hTreeItem);
	HTREEITEM hRemoveItem;
	while (!qRemoveItem.empty()) {
		hRemoveItem = qRemoveItem.front();
		UINT uFlag = TVGN_CHILD;
		HTREEITEM hChildItem = TreeView_GetNextItem(hWndTreeView, hRemoveItem, uFlag);
		while (hChildItem) {
			qRemoveItem.push(hChildItem);

			uFlag = TVGN_NEXT;
			hChildItem = TreeView_GetNextItem(hWndTreeView, hChildItem, uFlag);
		}
		if (!(hRemoveItem == hTreeItem)) {
			mtxMap.lock();
			TREEITEMDATA ItemData = (TREEITEMDATA)mTreeData[hRemoveItem];
			mtxMap.unlock();
			//MessageBox(NULL, pItemData->wsPath.c_str(), L"提示", MB_OK | MB_ICONINFORMATION);
			mTreeData.erase(hRemoveItem);
			TreeView_DeleteItem(hWndTreeView, hRemoveItem);
		}
		qRemoveItem.pop();
	}
}

//DragTarget
TDropTarget::TDropTarget(HWND hWnd, YTreeView *pTreeView) :pTreeView(pTreeView), hWndDropTarget(hWnd), _iRefCount(1) {
	pFormatEtcAcceptable = new FORMATETC[1];
	pFormatEtcAcceptable[0] = { CF_HDROP,0,DVASPECT_CONTENT,-1,TYMED_HGLOBAL };
	uFmtetcCount = 1;
	RegisterDragDrop(hWndDropTarget, this);
}

HRESULT __stdcall TDropTarget::QueryInterface(REFIID iid, void **ppv)
{
	if ((iid == IID_IUnknown) || (iid == IID_IDropSource))
	{
		*ppv = this;
		AddRef();
	}
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}
	return S_OK;
}
HRESULT __stdcall  TDropTarget::DragEnter(IDataObject * pDataObject, DWORD grfKeyState, POINTL pt, DWORD * pdwEffect)
{
	//FORMATETC pFormatEtc = { (CLIPFORMAT)CF_HDROP,0,DVASPECT_CONTENT,-1,TYMED_HGLOBAL };
	//STGMEDIUM stgMedium;
	for (UINT i = 0; i<uFmtetcCount; i++)
	{
		//FORMATETC pFormatEtc = { CF_HDROP,0,DVASPECT_CONTENT,-1,TYMED_HGLOBAL };
		if (SUCCEEDED(pDataObject->QueryGetData(&pFormatEtcAcceptable[i]))) {

			bFormatAcceptableFlag = TRUE;
		}
	}
	if (bFormatAcceptableFlag) {
		*pdwEffect = DROPEFFECT_COPY;
		if (grfKeyState&MK_SHIFT) {
			*pdwEffect = DROPEFFECT_MOVE;
		}
	}
	else { *pdwEffect = DROPEFFECT_NONE; }
	return S_OK;
}
HRESULT __stdcall  TDropTarget::DragOver(DWORD grfKeyState, POINTL pt, DWORD *pdwEffect)
{
	bItemPathValid = TRUE;
	POINT curPoint;
	GetCursorPos(&curPoint); //当前点坐标
	ScreenToClient(hWndDropTarget, &curPoint); //屏幕坐标转换为TREE内坐标
	TVHITTESTINFO ht = { 0 };
	ht.pt = curPoint;
	HTREEITEM hItemLastOver = (HTREEITEM)SendMessage(hWndDropTarget, TVM_HITTEST, 0, (LPARAM)&ht);
	if (hItemLastOver) {
		if (hItemLastOver == (HTREEITEM)SendMessage(hWndDropTarget, TVM_GETNEXTITEM, TVGN_ROOT, (LPARAM)0)) {
			bItemPathValid = FALSE;
		}
		//ITreeView* pTreeView = ITreeView::GetDataByObjectID((UINT)(GetWindowLongPtr(hWndDropTarget, GWLP_ID)));
		//TREEITEMDATA *pTreeItemData = (TREEITEMDATA *)pTreeView->mTreeData[hItemLastOver];
		//if (pTreeItemData->wsPath.compare(L"")==0) {

		//}
		SendMessage(hWndDropTarget, TVM_SELECTITEM, TVGN_DROPHILITE, (LPARAM)hItemLastOver);
		//SendMessage(hWndDropTarget, TVM_SELECTITEM, TVGN_DROPHILITE, (LPARAM)SendMessage(hWndDropTarget, TVM_GETNEXTITEM, TVGN_ROOT, (LPARAM)0));
	}
	else {
		SendMessage(hWndDropTarget, TVM_SELECTITEM, TVGN_DROPHILITE, (LPARAM)0);
		bItemPathValid = FALSE;
	}
	if (bFormatAcceptableFlag&&bItemPathValid) {
		*pdwEffect = DROPEFFECT_COPY;
		if (grfKeyState&MK_SHIFT) {
			*pdwEffect = DROPEFFECT_MOVE;
		}
	}
	else { *pdwEffect = DROPEFFECT_NONE; }
	return S_OK;
}
HRESULT __stdcall TDropTarget::DragLeave() {
	SendMessage(hWndDropTarget, TVM_SELECTITEM, TVGN_DROPHILITE, (LPARAM)0);
	bFormatAcceptableFlag = FALSE;
	return S_OK;
}

HRESULT __stdcall TDropTarget::Drop(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD __RPC_FAR *pdwEffect)
{
	if (!(bFormatAcceptableFlag&&bItemPathValid)) {
		return S_OK;
	}
	SendMessage(hWndDropTarget, TVM_SELECTITEM, TVGN_DROPHILITE, (LPARAM)0);
	//FORMATETC pFormatEtc = { (CLIPFORMAT)CF_HDROP,0,DVASPECT_CONTENT,-1,TYMED_HGLOBAL };
	//MessageBox(NULL, L"进入环节", TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
	STGMEDIUM stgMedium;
	for (UINT i = 0; i<uFmtetcCount; i++)
	{
		if (FAILED(pDataObj->GetData(&pFormatEtcAcceptable[i], &stgMedium))) {
			continue;
		}
		switch (pFormatEtcAcceptable[i].cfFormat) {
		case CF_HDROP:
		{
			HDROP hDrop = (HDROP)stgMedium.hGlobal;
			UINT nFileNum = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
			wchar_t strFileName[MAX_PATH];
			wstring wsFileFrom;
			LPCWSTR *lpszFrom;
			for (int i = 0; i < nFileNum; i++)
			{
				//2.2 参数 i ，将获得拖拽的第i个文件名
				DragQueryFile(hDrop, i, strFileName, MAX_PATH);
				//a 可将获得的文件名称，放入ListBox中
				//MessageBox(NULL, strFileName, TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
				wsFileFrom.append(strFileName);
				wsFileFrom += L'\0';
			}
			//wsFileFrom.append(L'\0');
			wsFileFrom += L'\0';
			POINT curPoint;
			GetCursorPos(&curPoint); //当前点坐标
			ScreenToClient(hWndDropTarget, &curPoint); //屏幕坐标转换为TREE内坐标
			TVHITTESTINFO ht = { 0 };
			ht.pt = curPoint;
			HTREEITEM hItemLastOver = (HTREEITEM)SendMessage(hWndDropTarget, TVM_HITTEST, 0, (LPARAM)&ht);

			TREEITEMDATA TreeItemData = (TREEITEMDATA)pTreeView->mTreeData[hItemLastOver];

			SHFILEOPSTRUCT shfo;
			shfo.hwnd = hWndDropTarget;
			shfo.wFunc = FO_COPY;
			if (grfKeyState&MK_SHIFT) {
				shfo.wFunc = FO_MOVE;
			}

			shfo.pFrom = wsFileFrom.c_str();
			shfo.pTo = TreeItemData.wsPath.c_str();
			shfo.fFlags = FOF_ALLOWUNDO;
			shfo.lpszProgressTitle = L"SHFILEOPSTRUCT的尝试";
			MessageBox(NULL, TreeItemData.wsPath.c_str(), TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
			SHFileOperation(&shfo);
		}
		break;
		default:break;
		}
		ReleaseStgMedium(&stgMedium);
		break;
	}
	return S_OK;
}
*/