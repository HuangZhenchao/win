#include "stdafx.h"
#include "TreeAndList.h"

TREEANDLIST::TREEANDLIST(HWND hFolderTreeView,HWND hTreeView, HWND hListView, wstring wsDBPath):hFolderTreeView(hFolderTreeView),hTagTreeView(hTreeView), hListView(hListView), wsDBPath(wsDBPath) {
	//风格样式
	SetWindowTheme(hFolderTreeView, L"Explorer", NULL);
	SetWindowTheme(hTagTreeView, L"Explorer", NULL);
	SetWindowTheme(hListView, L"Explorer", NULL);
	//图标列表
	HIMAGELIST hImageList;
	Shell_GetImageLists(NULL, &hImageList);
	SendMessage(hFolderTreeView, TVM_SETIMAGELIST, TVSIL_NORMAL, (LPARAM)hImageList);
	SendMessage(hListView, LVM_SETIMAGELIST, LVSIL_SMALL, (LPARAM)hImageList);
	
	SetRoot_FolderTree();
	SetColumn_FolderTree();
	
	sqlite3_open_v2(UnicodeToUTF8(wsDBPath).c_str(), &pSqliteMain, SQLITE_OPEN_READWRITE | SQLITE_OPEN_NOMUTEX, NULL);
	sqlite3_prepare_v2(pSqliteMain, sqlCountAll, strlen(sqlCountAll), &stmtCountAll, 0);
	sqlite3_prepare_v2(pSqliteMain, sqlCountAllNoTagged, strlen(sqlCountAllNoTagged), &stmtCountAllNoTagged, 0);
	sqlite3_prepare_v2(pSqliteMain, sqlCountAllTagged, strlen(sqlCountAllTagged), &stmtCountAllTagged, 0);
	sqlite3_prepare_v2(pSqliteMain, sqlCountTagGroup, strlen(sqlCountTagGroup), &stmtCountTagGroup, 0);
	sqlite3_prepare_v2(pSqliteMain, sqlCountTag, strlen(sqlCountTag), &stmtCountTag, 0);

	hEvent = CreateEvent(NULL, TRUE, FALSE, L"LoadData");
	for (int i = 0; i < THREADCOUNT; i++) {
		WCHAR wcEventName[20];
		wsprintf(wcEventName, L"LoadDone%d", i);
		hEventLoadDone[i] = CreateEvent(NULL, TRUE, FALSE, wcEventName);
		tThreadArr[i] = thread(&TREEANDLIST::tFunLoadChildren, this, i);
	}
	tThread = thread(&TREEANDLIST::tFunThread, this);

	hEventLoadListItem = CreateEvent(NULL,TRUE,FALSE,L"EventLoadListItem");
	tLoadListItem = thread(&TREEANDLIST::tFunLoadListItem,this);
}

void TREEANDLIST::FolderTreeNotifyHandler(NMHDR* pHdr) {
	NMTREEVIEW* pnmtv = (NMTREEVIEW *)pHdr;

	//YTreeView *pYTreeViewTemp = pYTreeView;
	switch (pnmtv->hdr.code)
	{	
	case TVN_SELCHANGED: {

		FOLDERTREEITEMDATA* pFolderTreeItemData = (FOLDERTREEITEMDATA*)pnmtv->itemNew.lParam;
		wsFolderTreeSelPath= pFolderTreeItemData->wsPath;
		wstring wsTemp1=pFolderTreeItemData->wsPath;
		if (wsTemp1.compare(L"")!=0) {
			wsTemp1.append(L"\\");
		}		
		wstring wsTemp2 = pFolderTreeItemData->wsPath;
		wsTemp2.append(L"]");
		TreeView_DeleteItem(hTagTreeView,hTreeItemAll);
		TreeView_DeleteItem(hTagTreeView, hTreeItemAllTagged);
		TreeView_DeleteItem(hTagTreeView, hTreeItemAllNoTagged);
		
		TVINSERTSTRUCT tvInsert;
		tvInsert.hParent = NULL;
		tvInsert.hInsertAfter = TVI_ROOT;

		tvInsert.item.mask = TVIF_TEXT | TVIF_CHILDREN;//| TCIF_IMAGE | TVIF_SELECTEDIMAGE
		tvInsert.item.pszText = L"全部文件";
		//tvInsert.item.iImage = sfi.iIcon;
		//tvInsert.item.iSelectedImage = sfi.iIcon;
		tvInsert.item.cChildren = 0;
		hTreeItemAll = (HTREEITEM)SendMessage(hTagTreeView, TVM_INSERTITEM, 0, (LPARAM)&tvInsert);

		tvInsert.hInsertAfter = TVI_LAST;
		tvInsert.item.pszText = L"无标签文件";
		hTreeItemAllNoTagged=(HTREEITEM)SendMessage(hTagTreeView, TVM_INSERTITEM, 0, (LPARAM)&tvInsert);

		tvInsert.item.mask = TVIF_TEXT | TVIF_CHILDREN | TVIF_PARAM;
		tvInsert.item.pszText = L"标签文件";
		tvInsert.item.cChildren = 1;
		tvInsert.item.lParam = (LPARAM)new TAGTREEITEMDATA;
		//((TAGTREEITEMDATA*)tvInsert.item.lParam)->bTagged = FALSE;
		hTreeItemAllTagged = (HTREEITEM)SendMessage(hTagTreeView, TVM_INSERTITEM, 0, (LPARAM)&tvInsert);
		TreeView_Select(hTagTreeView, hTreeItemAllTagged,TVGN_CARET);
		sqlite3_reset(stmtCountTagGroup);
		sqlite3_bind_text16(stmtCountTagGroup, 1, wsTemp1.c_str(), -1, 0);
		sqlite3_bind_text16(stmtCountTagGroup, 2, wsTemp2.c_str(), -1, 0);
		while (SQLITE_ROW == sqlite3_step(stmtCountTagGroup)) {
			wstring wsItemText = (LPWSTR)sqlite3_column_text16(stmtCountTagGroup, 2);
			wsItemText.append(L"(");
			wsItemText.append((LPWSTR)sqlite3_column_text16(stmtCountTagGroup, 0));
			wsItemText.append(L"个标签,");
			wsItemText.append((LPWSTR)sqlite3_column_text16(stmtCountTagGroup, 1));
			wsItemText.append(L"项)");
			
			tvInsert.item.pszText = (LPWSTR)wsItemText.c_str();
			tvInsert.item.cChildren = 1;
			tvInsert.hParent = hTreeItemAllTagged;
			tvInsert.item.lParam = (LPARAM)new TAGTREEITEMDATA;
			((TAGTREEITEMDATA*)tvInsert.item.lParam)->wsTagType = (LPWSTR)sqlite3_column_text16(stmtCountTagGroup, 2);
			((TAGTREEITEMDATA*)tvInsert.item.lParam)->wsTag = L"";
			HTREEITEM hItemTagGroup = (HTREEITEM)SendMessage(hTagTreeView, TVM_INSERTITEM, 0, (LPARAM)&tvInsert);

			sqlite3_reset(stmtCountTag);
			sqlite3_bind_text16(stmtCountTag, 1, wsTemp1.c_str(), -1, 0);
			sqlite3_bind_text16(stmtCountTag, 2, wsTemp2.c_str(), -1, 0);
			sqlite3_bind_text16(stmtCountTag, 3, (LPWSTR)sqlite3_column_text16(stmtCountTagGroup, 2), -1, 0);
			while (SQLITE_ROW == sqlite3_step(stmtCountTag)) {
				wstring wsItemText = (LPWSTR)sqlite3_column_text16(stmtCountTag, 1);
				wsItemText.append(L"(");
				wsItemText.append((LPWSTR)sqlite3_column_text16(stmtCountTag, 0));
				wsItemText.append(L"项)");
				tvInsert.item.pszText = (LPWSTR)wsItemText.c_str();
				//tvInsert.item.iImage = sfi.iIcon;
				//tvInsert.item.iSelectedImage = sfi.iIcon;
				tvInsert.hParent = hItemTagGroup;
				tvInsert.item.cChildren = 0;
				tvInsert.item.lParam = (LPARAM)new TAGTREEITEMDATA;
				((TAGTREEITEMDATA*)tvInsert.item.lParam)->wsTagType = (LPWSTR)sqlite3_column_text16(stmtCountTagGroup, 2);
				((TAGTREEITEMDATA*)tvInsert.item.lParam)->wsTag = (LPWSTR)sqlite3_column_text16(stmtCountTag, 1);
				HTREEITEM hItemRoot = (HTREEITEM)SendMessage(hTagTreeView, TVM_INSERTITEM, 0, (LPARAM)&tvInsert);
			}
		}
	}break;

	case TVN_BEGINDRAG: {
		/*
		POINT curPoint;
		GetCursorPos(&curPoint); //当前点坐标
		ScreenToClient(hFolderTreeView, &curPoint); //屏幕坐标转换为TREE内坐标
		TVHITTESTINFO ht = { 0 };
		ht.pt = curPoint;
		SendMessage(hFolderTreeView, TVM_HITTEST, 0, (LPARAM)&ht);
		THDROP_Files DragFils;
		DragFils.push_back(UnicodeToANSI(mTreeData[ht.hItem].wsPath));

		FORMATETC fmtetc = {
			CF_HDROP,
			0,
			DVASPECT_CONTENT,
			-1,
			TYMED_HGLOBAL
		};
		STGMEDIUM stgmed;
		stgmed.tymed = TYMED_HGLOBAL;
		stgmed.hGlobal = DragFils.Create_HDROP();
		stgmed.pUnkForRelease = 0;
		IDropSource *pDropSource = new TDropSource;
		TDataObject *pDataObject = new TDataObject();
		pDataObject->AddData(fmtetc, stgmed);
		DWORD dwEffect;
		DWORD dwOKEffect = DROPEFFECT_COPY | DROPEFFECT_MOVE | DROPEFFECT_LINK;
		DWORD dwResult = DoDragDrop(pDataObject, pDropSource, dwOKEffect, &dwEffect);
		pDataObject->ClearAndRelease();
		*/
	}break;
	default:break;
	}
}
void TREEANDLIST::TagTreeNotifyHandler(NMHDR *pHdr) {
	NMTREEVIEW* pnmtv = (NMTREEVIEW *)pHdr;
	
	//YTreeView *pYTreeViewTemp = pYTreeView;
	switch (pnmtv->hdr.code)
	{
	case TVN_SELCHANGED:
	{
		bBreakLoadLastList = TRUE;
		SelectInfo.wsTagType = L"";
		SelectInfo.wsTag = L"";
		SelectInfo.wsPath = wsFolderTreeSelPath;
		if (hTreeItemAll==pnmtv->itemNew.hItem) {
			SelectInfo.dwSelectType = 0;
		}
		else if (hTreeItemAllNoTagged == pnmtv->itemNew.hItem) {
			SelectInfo.dwSelectType = 1;
		}
		else if (hTreeItemAllTagged == pnmtv->itemNew.hItem) {
			SelectInfo.dwSelectType = 2;
		}
		else {
			TAGTREEITEMDATA* pTreeItemData = (TAGTREEITEMDATA*)pnmtv->itemNew.lParam;
			SelectInfo.wsTagType = pTreeItemData->wsTagType;
			SelectInfo.wsTag = pTreeItemData->wsTag;
			if (SelectInfo.wsTag.compare(L"")==0) {
				SelectInfo.dwSelectType = 3;
			}
			else {
				SelectInfo.dwSelectType = 4;
			}
		}
		WCHAR szDis[1000] = {0};
		wsprintf(szDis,L"%d%ls%ls%ls", SelectInfo.dwSelectType, SelectInfo.wsPath.c_str(), SelectInfo.wsTagType.c_str(), SelectInfo.wsTag.c_str());
		//MessageBox(NULL, szDis, TEXT("提示"), MB_OK | MB_ICONINFORMATION);
		
		SetEvent(hEventLoadListItem);
	}
	break;
	default:break;
	}
}
void TREEANDLIST::ListNotifyHandler(NMHDR *pHdr) {
	NMTREEVIEW* pnmtv = (NMTREEVIEW *)pHdr;
	switch (pnmtv->hdr.code)
	{
		case NM_RCLICK:
		{
			int iPos = ListView_GetNextItem(hListView, -1, LVNI_SELECTED);
			while (iPos != -1) {
				// iPos is the index of a selected item
				// do whatever you want with it
				LVITEM lvItem;
				lvItem.mask = LVIF_TEXT;// | LVIF_PARAM;
				lvItem.iItem = iPos;
				lvItem.iSubItem = 0;
				lvItem.cchTextMax = MAX_PATH;
				WCHAR szText[MAX_PATH];
				lvItem.pszText = szText;
				ListView_GetItem(hListView, &lvItem);
				MessageBox(NULL, lvItem.pszText, TEXT("提示"), MB_OK | MB_ICONINFORMATION);
				// Get the next selected item
				iPos = ListView_GetNextItem(hListView, iPos, LVNI_SELECTED);
			}
		}
		break;
	}
}

void TREEANDLIST::tFunLoadListItem() {
	sqlite3 *pSqliteThread=NULL;
	//sqlite3_stmt *stmtSelectTagFile;
	sqlite3_open_v2(UnicodeToUTF8(wsDBPath).c_str(), &pSqliteThread, SQLITE_OPEN_READWRITE | SQLITE_OPEN_NOMUTEX, NULL);

	sqlite3_prepare_v2(pSqliteThread, sqlSelectAll, strlen(sqlSelectAll), &stmtSelectAll, 0);
	sqlite3_prepare_v2(pSqliteThread, sqlSelectAllNoTagged, strlen(sqlSelectAllNoTagged), &stmtSelectAllNoTagged, 0);
	sqlite3_prepare_v2(pSqliteThread, sqlSelectAllTagged, strlen(sqlSelectAllTagged), &stmtSelectAllTagged, 0);
	sqlite3_prepare_v2(pSqliteThread, sqlSelectTagGroup, strlen(sqlSelectTagGroup), &stmtSelectTagGroup, 0);
	sqlite3_prepare_v2(pSqliteThread, sqlSelectTag, strlen(sqlSelectTag), &stmtSelectTag, 0);
	vStmt.push_back(stmtSelectAll);vStmt.push_back(stmtSelectAllNoTagged); vStmt.push_back(stmtSelectAllTagged); vStmt.push_back(stmtSelectTagGroup); vStmt.push_back(stmtSelectTag);
	while (!bTerminateLoadListThread) {
		WaitForSingleObject(hEventLoadListItem,INFINITE);
		
		bBreakLoadLastList = FALSE;
		ListView_DeleteAllItems(hListView);

		wstring wsTemp1 = SelectInfo.wsPath;
		if (wsTemp1.compare(L"") != 0) {
			wsTemp1.append(L"\\");
		}
		wstring wsTemp2 = SelectInfo.wsPath;
		wsTemp2.append(L"]");

		sqlite3_stmt *stmtTemp=vStmt[SelectInfo.dwSelectType];
		sqlite3_reset(stmtTemp);
		sqlite3_bind_text16(stmtTemp, 1, wsTemp1.c_str(), -1, 0);
		sqlite3_bind_text16(stmtTemp, 2, wsTemp2.c_str(), -1, 0);
		switch (SelectInfo.dwSelectType)
		{
			case 3: {
				sqlite3_bind_text16(stmtTemp, 3, SelectInfo.wsTagType.c_str(), -1, 0);
			}break;
			case 4: {
				sqlite3_bind_text16(stmtTemp, 3, SelectInfo.wsTagType.c_str(), -1, 0);
				sqlite3_bind_text16(stmtTemp, 4, SelectInfo.wsTag.c_str(), -1, 0);
			}break;
			default: break;
		}

		int iItemIndex = 0;
		while (SQLITE_ROW == sqlite3_step(stmtTemp)) {
			if (bBreakLoadLastList) {
				break;
			}
			LVITEM vitem;
			vitem.mask = LVIF_TEXT|LVIF_IMAGE;
			vitem.pszText = (LPWSTR)sqlite3_column_text16(stmtTemp, 3);
			vitem.iItem = iItemIndex;
			vitem.iSubItem = 0;
			vitem.iImage = sqlite3_column_int(stmtTemp, 5);
			ListView_InsertItem(hListView, &vitem);
			//MessageBox(NULL, vitem.pszText, TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
			// 设置子项  
			vitem.iSubItem = 1;
			vitem.pszText = (LPWSTR)sqlite3_column_text16(stmtTemp, 4);
			ListView_SetItem(hListView, &vitem);

			vitem.iSubItem = 2;
			vitem.pszText = (LPWSTR)sqlite3_column_text16(stmtTemp, 6);
			ListView_SetItem(hListView, &vitem);

			vitem.iSubItem = 3;
			vitem.pszText = (LPWSTR)sqlite3_column_text16(stmtTemp, 9);
			ListView_SetItem(hListView, &vitem);
			iItemIndex++;
		}
		if (bBreakLoadLastList) {
			bBreakLoadLastList = FALSE;
			SetEvent(hEventLoadListItem);
		}
		else {
			ResetEvent(hEventLoadListItem);
		}		
	}

}

void TREEANDLIST::SetRoot_FolderTree() {
	LPITEMIDLIST  pidworkDir = (LPITEMIDLIST)CoTaskMemAlloc(sizeof(LPITEMIDLIST));
	HRESULT hr = SHGetSpecialFolderLocation(hFolderTreeView, CSIDL_DRIVES, &pidworkDir);

	SHFILEINFO sfi;

	if (SUCCEEDED(hr)) {
		SHGetFileInfo((LPCWSTR)pidworkDir, 0, &sfi, sizeof(sfi), SHGFI_PIDL | SHGFI_DISPLAYNAME | SHGFI_SYSICONINDEX | SHGFI_SMALLICON);
		//MessageBox(NULL, sfi.szDisplayName, TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
		//hr = SHGetSpecialFolderLocation(hWndMain, CSIDL_DRIVES, &pidworkDir);
	}
	CoTaskMemFree(pidworkDir);

	TVINSERTSTRUCT tvInsert;
	tvInsert.hParent = NULL;
	tvInsert.hInsertAfter = TVI_ROOT;

	tvInsert.item.mask = TVIF_TEXT | TCIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_CHILDREN | TVIF_PARAM;
	tvInsert.item.pszText = sfi.szDisplayName;
	tvInsert.item.iImage = sfi.iIcon;
	tvInsert.item.iSelectedImage = sfi.iIcon;
	tvInsert.item.cChildren = 1;
	tvInsert.item.lParam = (LPARAM)new FOLDERTREEITEMDATA();
	((FOLDERTREEITEMDATA*)tvInsert.item.lParam)->wsPath = L"";// servicing\\LCU\\Package_for_RollupFix~31bf3856ad364e35~amd64~~18362.535.1.6";
	hRootFolderTree = (HTREEITEM)SendMessage(hFolderTreeView, TVM_INSERTITEM, 0, (LPARAM)&tvInsert);
}
void TREEANDLIST::SetColumn_FolderTree() {
	LV_COLUMN lvc;
	lvc.mask = LVCF_TEXT | LVCF_SUBITEM | LVCF_WIDTH;
	lvc.pszText = L"文件名";
	lvc.iSubItem = 0;
	lvc.cx = 300;
	ListView_InsertColumn(hListView, 0, &lvc);
	lvc.pszText = L"拓展名";
	lvc.iSubItem = 1;
	lvc.cx = 50;
	ListView_InsertColumn(hListView, 1, &lvc);
	lvc.pszText = L"大小";
	lvc.iSubItem = 2;
	lvc.cx = 50;
	ListView_InsertColumn(hListView, 2, &lvc);
	lvc.pszText = L"修改日期";
	lvc.iSubItem = 3;
	lvc.cx = 200;
	ListView_InsertColumn(hListView, 3, &lvc);
}
void TREEANDLIST::tFunThread() {
	qHTreeItem.push(hRootFolderTree);
	SetEvent(hEvent);
	clock_t start, end;
	start = clock();
	WaitForMultipleObjects(THREADCOUNT, hEventLoadDone, TRUE, INFINITE);
	end = clock();
	WCHAR szDisplayString[1024];
	wsprintf(szDisplayString, L"加载完成，耗时%dms\r\n", end - start);
	MessageBox(NULL, szDisplayString, TEXT("提示"), MB_OK | MB_ICONINFORMATION);
	bTerminateFolderTreeLoadThread = TRUE;
	SetEvent(hEvent);
}

void TREEANDLIST::tFunLoadChildren(int iThreadIndex) {
	sqlite3 *pSqliteDB;
	sqlite3_stmt *stmtCountFolder;
	sqlite3_stmt *stmtSelectFolder;
	HRESULT rc = sqlite3_open_v2(UnicodeToUTF8(wsDBPath).c_str(), &pSqliteDB, SQLITE_OPEN_READWRITE | SQLITE_OPEN_NOMUTEX, NULL);
	const char* sqlCountFolder = "select count(*) from tFileInfo where ParentPath=? and FileExtName='Folder';";
	const char* sqlSelectFolder = "select FileName,FilePath,FileIconIndex from tFileInfo where ParentPath= ? and FileExtName='Folder';";
	sqlite3_prepare_v2(pSqliteDB, sqlCountFolder, strlen(sqlCountFolder), &stmtCountFolder, 0);
	sqlite3_prepare_v2(pSqliteDB, sqlSelectFolder, strlen(sqlSelectFolder), &stmtSelectFolder, 0);
	HTREEITEM hParentItem = NULL;
	while (!bTerminateFolderTreeLoadThread) {
		
		WaitForSingleObject(hEvent, INFINITE);
		mtx.lock();
		if (!qHTreeItem.empty()) {
			hParentItem = qHTreeItem.front();
			qHTreeItem.pop();
		}
		mtx.unlock();
		if (hParentItem) {
			ResetEvent(hEventLoadDone[iThreadIndex]);
			TVITEM tvItem;
			tvItem.mask = TVIF_PARAM;
			tvItem.hItem = hParentItem;
			TreeView_GetItem(hFolderTreeView, &tvItem);

			sqlite3_reset(stmtSelectFolder);
			sqlite3_bind_text16(stmtSelectFolder, 1, ((FOLDERTREEITEMDATA*)tvItem.lParam)->wsPath.c_str(), -1, 0);
			if (((FOLDERTREEITEMDATA*)tvItem.lParam)->wsPath.compare(L"C:\\Windows\\WinSxS") != 0) {
				//MessageBox(NULL, ((TREEITEMDATA*)tvItem.lParam)->wsPath.c_str(), TEXT("提示"), MB_OK | MB_ICONINFORMATION);
			}
			BOOL bHasChildren = FALSE;
			while (SQLITE_ROW == sqlite3_step(stmtSelectFolder)) {
				bHasChildren = TRUE;
				TVINSERTSTRUCT tvInsert;
				tvInsert.hParent = hParentItem;
				tvInsert.hInsertAfter = TVI_SORT;

				tvInsert.item.mask = TVIF_TEXT | TCIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_CHILDREN | TVIF_PARAM;
				tvInsert.item.pszText = (LPWSTR)sqlite3_column_text16(stmtSelectFolder, 0);// sfi.szDisplayName;
				tvInsert.item.iImage = sqlite3_column_int64(stmtSelectFolder, 2);
				tvInsert.item.iSelectedImage = sqlite3_column_int64(stmtSelectFolder, 2);
				tvInsert.item.cChildren = 0;
				tvInsert.item.lParam = (LPARAM)new FOLDERTREEITEMDATA();
				((FOLDERTREEITEMDATA*)tvInsert.item.lParam)->wsPath = (LPWSTR)sqlite3_column_text16(stmtSelectFolder, 1);

				sqlite3_reset(stmtCountFolder);
				sqlite3_bind_text16(stmtCountFolder, 1, (LPWSTR)sqlite3_column_text16(stmtSelectFolder, 1), -1, 0);
				sqlite3_step(stmtCountFolder);
				if (sqlite3_column_int64(stmtCountFolder, 0)>0) {
					tvInsert.item.cChildren = 1;
				}
				
				
				HTREEITEM hNewItem = (HTREEITEM)SendMessage(hFolderTreeView, TVM_INSERTITEM, 0, (LPARAM)&tvInsert);

				

				if (wsLastPath.compare((LPWSTR)sqlite3_column_text16(stmtSelectFolder, 1)) == 0) {
					TreeView_Select(hFolderTreeView, hNewItem, TVGN_CARET);
				}
				if (sqlite3_column_int64(stmtCountFolder, 0)>0) {
					mtx.lock();
					qHTreeItem.push(hNewItem);
					SetEvent(hEvent);
					mtx.unlock();
				}
				
			}
		}
		if (!(qHTreeItem.empty())) {
			SetEvent(hEvent);
			ResetEvent(hEventLoadDone[iThreadIndex]);
		}
		else {
			//wprintf(L"循环检测盘%ls线程%d\n", vDBPath[iDriverIndex], vEvent[iDriverIndex]);

			SetEvent(hEventLoadDone[iThreadIndex]);
			ResetEvent(hEvent);
		}
		hParentItem = NULL;

	}

}