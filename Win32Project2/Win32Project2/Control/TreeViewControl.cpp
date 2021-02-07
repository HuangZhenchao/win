#include "stdafx.h"

#include "TreeViewControl.h"
void TreeViewDropFile::OnDrop(IDataObject *pDataObject, DWORD grfKeyState, POINTL pt, DWORD __RPC_FAR *pdwEffect) {
	//FORMATETC pFormatEtc = { (CLIPFORMAT)CF_HDROP,0,DVASPECT_CONTENT,-1,TYMED_HGLOBAL };
	MessageBox(NULL, L"进入环节", TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
	STGMEDIUM stgMedium;
	for (UINT i = 0; i<uFmtetcCount; i++)
	{
		if (FAILED(pDataObject->GetData(&pFormatEtcAcceptable[i], &stgMedium))) {			
			continue;
		}
		switch (stgMedium.tymed) {
		case TYMED_HGLOBAL:
		{
			PWNDDATA *pWndData = ((PWNDDATA *)IdDataBinding[(DWORD)GetWindowLongPtr(hWndDropTarget, GWLP_ID)]);
			TreeNode<NODEDATA*> *pNode = (TreeNode<NODEDATA*> *)pWndData->pExtInfo;
			MessageBox(NULL, pNode->data->wsNodePath.c_str(), TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
			HDROP hDrop = (HDROP)stgMedium.hGlobal;
			UINT nFileNum = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
			wchar_t strFileName[MAX_PATH];
			wstring wsFileFrom = L"";
			for (int i = 0; i < nFileNum; i++)
			{
				//2.2 参数 i ，将获得拖拽的第i个文件名
				DragQueryFile(hDrop, i, strFileName, MAX_PATH);
				//a 可将获得的文件名称，放入ListBox中
				MessageBox(NULL, strFileName, TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
				wsFileFrom.append(strFileName);
				wsFileFrom.append(L"/0");
			}
			SHFILEOPSTRUCT shfo;
			shfo.hwnd = hWndDropTarget;
			shfo.wFunc = FO_COPY;
			shfo.pFrom = wsFileFrom.c_str();
			shfo.pTo = pNode->data->wsNodePath.c_str();

		}
		break;
		default:break;
		}
		ReleaseStgMedium(&stgMedium);
	}
}
TREEVIEWINFO::TREEVIEWINFO() {
	himlSmallIcon = hImgSysIconSmall;
	himlButtonAndLines = hImgSysIconSmall;
}
void TVW_CreateItemWnd(TREEVIEWINFO *pCtrlInfo,TreeNode<NODEDATA *> *pNode) {
	if (pNode->pFirstChild == nullptr) {
		return;
	}
	PWNDDATA *pItemData = new PWNDDATA();
	pItemData->pCtrlInfo = pCtrlInfo;
	pItemData->pExtInfo = pNode->pFirstChild;
	DWORD dwID = BindIdData(pItemData);
	HWND hWndItem = CreateWindowEx(NULL, L"MyTreeViewItem", NULL, WS_CHILD | WS_VISIBLE,
		0, 0, 0, 0, pCtrlInfo->hWndContent, (HMENU)dwID, pCtrlInfo->hInstance, NULL);
	
	pNode->pFirstChild->data->hWndItem = hWndItem;
	TreeNode<NODEDATA *> *pTempNode;
	pTempNode = pNode->pFirstChild->pNextBrother;
	clock_t start, end;
	DOUBLE endtime;
	start = clock();
	while (pTempNode != nullptr) {
		
		PWNDDATA *pItemData = new PWNDDATA();
		pItemData->pCtrlInfo = pCtrlInfo;
		pItemData->pExtInfo = pTempNode;

		//IdDataBinding.insert(map<DWORD, void*>::value_type(IDsScrollControl->dwCMID, pThis->scrollPanelControlInfo));
		DWORD dwID = BindIdData(pItemData);
		
		HWND hWndItem = CreateWindowEx(NULL, L"MyTreeViewItem", NULL, WS_CHILD | WS_VISIBLE,
			0, 0, 0, 0, pCtrlInfo->hWndContent, (HMENU)dwID, pCtrlInfo->hInstance, NULL);
		
		pTempNode->data->hWndItem = hWndItem;
		
		pTempNode = pTempNode->pNextBrother;
		
	}
	end = clock();
	endtime = (double)(end - start) * 1000 / CLOCKS_PER_SEC;
	wchar_t tempStr1[32];
	_itow_s(endtime, tempStr1, 32);
	//MessageBox(NULL, tempStr1, TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
}
void TVW_CreateNodeWnd(TREEVIEWINFO *pCtrlInfo, TreeNode<NODEDATA *> *pNode) {
	PWNDDATA *pItemData = new PWNDDATA();
	pItemData->pCtrlInfo = pCtrlInfo;
	pItemData->pExtInfo = pNode;

	//IdDataBinding.insert(map<DWORD, void*>::value_type(IDsScrollControl->dwCMID, pThis->scrollPanelControlInfo));
	DWORD dwID = BindIdData(pItemData);

	HWND hWndItem = CreateWindowEx(NULL, L"MyTreeViewItem", NULL, WS_CHILD | WS_VISIBLE,
		0, 0, 0, 0, pCtrlInfo->hWndContent, (HMENU)dwID, pCtrlInfo->hInstance, NULL);

	pNode->data->hWndItem = hWndItem;
}
void RemoveData(TreeNode<NODEDATA *> *pNode) {
	if (pNode->pFirstChild==nullptr) {
		return;
	}
	TreeNode<NODEDATA *> *pTempNode;
	pTempNode = pNode->pFirstChild->pNextBrother;
	while (pTempNode != nullptr) {
		TreeNode<NODEDATA *> *pDelNode = pTempNode;
		pTempNode = pTempNode->pNextBrother;		
		DestroyWindow(pDelNode->data->hWndItem);
	
		//delete pDelNode;
	}

	DestroyWindow(pNode->pFirstChild->data->hWndItem);

	//delete pNode->pFirstChild;
	pNode->pFirstChild = nullptr;
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
void TVW_RegisterWnd(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = 0;
	wcex.lpfnWndProc = TVWP_Control;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = sizeof(LONG);
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(nullptr, IDC_HAND);
	wcex.hbrBackground = CreateSolidBrush(RGB(255, 0, 0));
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"MyTreeViewControl";
	wcex.hIconSm = NULL;

	if (!GetClassInfoEx(hInstance, wcex.lpszClassName, &wcex)) {
		RegisterClassExW(&wcex);
	}
	wcex.lpfnWndProc = TVWP_Content;
	wcex.hbrBackground = CreateSolidBrush(RGB(255, 0, 0));
	//窗体回调函数的前向声明
	wcex.lpszClassName = L"MyTreeViewContent";
	if (!GetClassInfoEx(hInstance, wcex.lpszClassName, &wcex)) {
		RegisterClassExW(&wcex);
	}
	wcex.style = CS_DBLCLKS;
	wcex.lpfnWndProc = TVWP_Item;
	wcex.hbrBackground = CreateSolidBrush(RGB(255, 0, 0));
	//窗体回调函数的前向声明
	wcex.lpszClassName = L"MyTreeViewItem";
	if (!GetClassInfoEx(hInstance, wcex.lpszClassName, &wcex)) {
		RegisterClassExW(&wcex);
	}
	//bWndRegistered = TRUE;
}

BOOL cbTreeTraversalHandler_MoveItemWnd(void *pTreeNode, void *pOtherData) {
	TreeNode<NODEDATA *> *pNode = (TreeNode<NODEDATA *> *)pTreeNode;
	TREEVIEWINFO *pCtrlInfo = (TREEVIEWINFO *)pOtherData;
	pCtrlInfo->ItemCount = pCtrlInfo->ItemCount + 1;

	//MessageBox(NULL, debugStr.c_str(), TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
	MoveWindow(pNode->data->hWndItem, 0, (pCtrlInfo->ItemCount - 1)*(pCtrlInfo->ItemHeight + 1), pCtrlInfo->MaxWidth, pCtrlInfo->ItemHeight, TRUE);
	//MessageBox(NULL, pNode->data->wsNodeName.c_str(), TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
	if (pNode->data->dwExpanded > 0) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}
BOOL cbTreeTraversalHandler_FlushSize(void *pTreeNode,void *pOtherData) {
	TreeNode<NODEDATA *> *pNode = (TreeNode<NODEDATA *> *)pTreeNode;
	TREEVIEWINFO *pCtrlInfo = (TREEVIEWINFO *)pOtherData;
	HDC hdc = GetDC(pCtrlInfo->hWndContent);
	HFONT hFont = CreateFontIndirect(&lfList);
	SelectObject(hdc, hFont);

	SIZE sizeTest;

	GetTextExtentPoint32(hdc, pNode->data->wsNodeName.c_str(), wcslen(pNode->data->wsNodeName.c_str()), &sizeTest);
	//项长度=缩进+层次线+展开按钮+图标+文字长度
	DWORD ItemWidth = pCtrlInfo->dIndent*(pNode->data->dwNodeDepth) + 32 + sizeTest.cx;
	wstring debugStr = pNode->data->wsNodeName;
	debugStr.append(L"\n");
	wchar_t tempStr1[32];
	_itow_s(ItemWidth, tempStr1, 32);
	debugStr.append(tempStr1);

	//MessageBox(NULL, debugStr.c_str(), TEXT("错误码"), MB_OK | MB_ICONINFORMATION);

	if (pCtrlInfo->MaxWidth<ItemWidth) {
		pCtrlInfo->MaxWidth = ItemWidth;
	}
	pCtrlInfo->ItemHeight = sizeTest.cy;

	ReleaseDC(pCtrlInfo->hWndContent, hdc);
	//MessageBox(NULL, pNode->data->wsNodeName.c_str(), TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
	if (pNode->data->dwExpanded > 0) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

LRESULT CALLBACK  TVWP_Control(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		clock_t start, end;
		double endtime;
		//start = clock();
		PWNDDATA *pWndData = (PWNDDATA *)IdDataBinding[(DWORD)((LPCREATESTRUCT)lParam)->hMenu];
		TREEVIEWINFO *pCtrlInfo = (TREEVIEWINFO *)pWndData->pCtrlInfo;

		pCtrlInfo->hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
		pCtrlInfo->hWndControlParent = ((LPCREATESTRUCT)lParam)->hwndParent;
		pCtrlInfo->hWndControl = hWnd;
		pCtrlInfo->scrollPanelControlInfo = new SCROLLPANELCONTROLINFO();
		pCtrlInfo->scrollPanelControlInfo->dwCS = 0;
		pCtrlInfo->scrollPanelControlInfo->dwHSBAreaWidth = 5;
		pCtrlInfo->scrollPanelControlInfo->dwVSBAreaWidth = 5;

		PWNDDATA *pScrollData = new PWNDDATA();
		pScrollData->pCtrlInfo = pCtrlInfo->scrollPanelControlInfo;
		pScrollData->pExtInfo = nullptr;

		//IdDataBinding.insert(map<DWORD, void*>::value_type(IDsScrollControl->dwCMID, pThis->scrollPanelControlInfo));

		DWORD dwScrollControlID = GetIDFromPool();
		IdDataBinding.insert(map<DWORD, void*>::value_type(dwScrollControlID, pScrollData));

		pCtrlInfo->hWndScroll =
			CreateWindowEx(NULL, L"MyScrollPanelControl", NULL, WS_CHILD | WS_VISIBLE,
				0, 0, ((LPCREATESTRUCT)lParam)->cx, ((LPCREATESTRUCT)lParam)->cy, hWnd, (HMENU)dwScrollControlID, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		
		wchar_t tempStr1[32];
		_itow_s((DWORD)((LPCREATESTRUCT)lParam)->cx, tempStr1, 32);
		//MessageBox(NULL, tempStr1, TEXT("错误码"), MB_OK | MB_ICONINFORMATION);

		PWNDDATA *pTVContentData = new PWNDDATA();
		pTVContentData->pCtrlInfo = pCtrlInfo;
		pTVContentData->pExtInfo = nullptr;

		DWORD dwTVContentID = GetIDFromPool();
		IdDataBinding.insert(map<DWORD, void*>::value_type(dwTVContentID, pTVContentData));

		pCtrlInfo->hWndContent =
			pCtrlInfo->scrollPanelControlInfo->hWndContent = CreateWindowEx(NULL, L"MyTreeViewContent", NULL, WS_CHILD | WS_VISIBLE,
				0, 0, 20, 30, pCtrlInfo->scrollPanelControlInfo->hWndContainer, (HMENU)dwTVContentID, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		//构建根目录
		//如果数据来源是文件系统，就在这里处理
		//如果不是文件系统，就发送消息给父窗口
		TreeNode<NODEDATA *>* pRoot = new TreeNode<NODEDATA *>();
		NODEDATA *pRootNodeData;
		if (1==pCtrlInfo->dwDataSource) {
			pRootNodeData = new NODEDATA();
			if (pCtrlInfo->wsRootNodePath==L"") {
				LPITEMIDLIST  pidworkDir = (LPITEMIDLIST)CoTaskMemAlloc(sizeof(LPITEMIDLIST));
				HRESULT hr = SHGetSpecialFolderLocation(hWndMain, CSIDL_DRIVES, &pidworkDir);
				SHFILEINFO sfi;
				if (SUCCEEDED(hr)) {
					SHGetFileInfo((LPCWSTR)pidworkDir, 0, &sfi, sizeof(sfi), SHGFI_PIDL | SHGFI_DISPLAYNAME|SHGFI_SYSICONINDEX);
					MessageBox(NULL, sfi.szDisplayName, TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
					//hr = SHGetSpecialFolderLocation(hWndMain, CSIDL_DRIVES, &pidworkDir);
				}
				CoTaskMemFree(pidworkDir);
				pRootNodeData->wsNodeName = sfi.szDisplayName;
				if (pCtrlInfo->wsRootNodeName != L"") {
					pRootNodeData->wsNodeName = pCtrlInfo->wsRootNodeName;
				}
				//根路径为空时，根节点为我的电脑
				//因为我的我的电脑是Windows下的虚拟文件夹，要调用Windows外壳编程
				pRootNodeData->wsNodePath = L"";
				pRootNodeData->iIcon = sfi.iIcon;
			}
			else {
				SHFILEINFO sfi;
				SHGetFileInfo((LPCWSTR)pCtrlInfo->wsRootNodePath.c_str(), 0, &sfi, sizeof(sfi),  SHGFI_DISPLAYNAME| SHGFI_SYSICONINDEX);
				pRootNodeData->wsNodeName = sfi.szDisplayName;
				if (pCtrlInfo->wsRootNodeName != L"") {
					pRootNodeData->wsNodeName = pCtrlInfo->wsRootNodeName;
				}
				pRootNodeData->wsNodePath = pCtrlInfo->wsRootNodePath;
				pRootNodeData->iIcon = sfi.iIcon;
			}
			pRootNodeData->dwNodeDepth = 0;
			pRootNodeData->dwExpanded = 0;

			pRootNodeData->dwNodeType = 1;
			pRootNodeData->dwHasChildren = 1;
			pRootNodeData->dwLoadChildrenDone = 0;
		}
		else {
			DWORD dwDataID = SendMessage(hWndMain, WM_CUSTOM_TVN, TVN_SETROOT, NULL);
			pRootNodeData =(NODEDATA *)IdDataBinding[dwDataID];
			IdDataBinding.erase(dwDataID);
			PutIDToPool(dwDataID);
		}		
		pRoot->data = pRootNodeData;
		pRoot->pFirstChild = nullptr;
		pRoot->pNextBrother = nullptr;

		PWNDDATA *pItemData = new PWNDDATA();
		pItemData->pCtrlInfo = pCtrlInfo;
		pItemData->pExtInfo = pRoot;

		DWORD dwTVItemID = BindIdData(pItemData);
		HWND hWndItem = CreateWindowEx(NULL, L"MyTreeViewItem", NULL, WS_CHILD | WS_VISIBLE,
			0, 0, 0, 0, pCtrlInfo->hWndContent, (HMENU)dwTVItemID, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

		pRoot->data->hWndItem = hWndItem;

		pCtrlInfo->tree.SetRoot(pRoot);

		PostMessage(hWnd, WM_USER + 11, NULL, dwTVItemID);

	}
	break;
	/*
	case WM_ERASEBKGND:
		return TRUE;
		*/
	case WM_SIZE:
	{
		TREEVIEWINFO *pThis = (TREEVIEWINFO *)((PWNDDATA *)IdDataBinding[(DWORD)GetWindowLongPtr(hWnd, GWLP_ID)])->pCtrlInfo;
	}
	break;
	case WM_USER+11:
	{
		clock_t start, end;
		double endtime;
		
		//MessageBox(NULL, L"经过消息处理WM_USER+11", TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
		//TREEVIEWINFO *pThis = (TREEVIEWINFO *)((PWNDDATA *)IdDataBinding[(DWORD)GetWindowLongPtr(hWnd, GWLP_ID)])->pCtrlInfo;
		PWNDDATA *pWndData=((PWNDDATA *)IdDataBinding[(DWORD)lParam]);
		TreeNode<NODEDATA *> *pNode = (TreeNode<NODEDATA *> *)pWndData->pExtInfo;
		TREEVIEWINFO *pCtrlInfo = (TREEVIEWINFO *)pWndData->pCtrlInfo;
		
		pNode->data->dwExpanded = (~pNode->data->dwExpanded) << 31 >> 31;
		wchar_t tempStr1[32];
		_itow_s(pNode->data->dwExpanded, tempStr1, 32);
		//MessageBox(NULL, tempStr1, TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
		if ((pNode->data->dwExpanded == 1) && (pNode->data->dwLoadChildrenDone==0)) {
			//MessageBox(NULL, L"打开一个节点", TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
			//LoadData(pNode);
			if (1 == pCtrlInfo->dwDataSource) {
				if (pNode->data->wsNodePath == L"") {
					IShellFolder *psfDesktop = NULL;
					IShellFolder *psfWorkDir = NULL;
					IEnumIDList  *penumIDList = NULL;
					LPITEMIDLIST  pidworkDir = (LPITEMIDLIST)CoTaskMemAlloc(sizeof(LPITEMIDLIST));
					SHFILEINFO sfi;
					HRESULT hr;
					hr = SHGetDesktopFolder(&psfDesktop);
					hr = SHGetSpecialFolderLocation(hWndMain, CSIDL_DRIVES, &pidworkDir);
					hr = psfDesktop->BindToObject(pidworkDir, NULL, IID_IShellFolder, (LPVOID*)&psfWorkDir);
					psfWorkDir->EnumObjects(NULL, SHCONTF_FOLDERS, &penumIDList);
					ULONG celtFetched = 0;
					LPITEMIDLIST pidlChild = (LPITEMIDLIST)CoTaskMemAlloc(sizeof(LPITEMIDLIST));
					LPITEMIDLIST pRealIDL = (LPITEMIDLIST)CoTaskMemAlloc(sizeof(LPITEMIDLIST));
					while (SUCCEEDED(penumIDList->Next(1, &pidlChild, &celtFetched)) && (1 == celtFetched)) {

						hr = SHGetRealIDL(psfWorkDir, pidlChild, &pRealIDL);

						STRRET strName;
						WCHAR szName[MAX_PATH] = { 0 };
						hr = psfWorkDir->GetDisplayNameOf(pRealIDL, SHGDN_FORPARSING, &strName);
						hr = StrRetToBuf(&strName, pRealIDL, szName, MAX_PATH);
						//SHGetPathFromIDList(pidChild, szName);
						//SHFILEINFO sfi;
						hr = SHGetFileInfo((LPCWSTR)szName, 0, &sfi, sizeof(sfi), SHGFI_DISPLAYNAME | SHGFI_SYSICONINDEX);
						
						if (SUCCEEDED(hr)) {
							NODEDATA *NodeData = new NODEDATA();
							NodeData->wsNodeName = sfi.szDisplayName;
							NodeData->wsNodePath = szName;
							NodeData->iIcon = sfi.iIcon;
							NodeData->dwNodeDepth = 1;
							NodeData->dwExpanded = 0;

							NodeData->dwNodeType = 1;
							NodeData->dwHasChildren = 1;
							NodeData->dwLoadChildrenDone = 1;
							TreeNode<NODEDATA *> *pNewNode = new TreeNode<NODEDATA *>;
							pNewNode->data = NodeData;
							pNewNode->pFirstChild = nullptr;
							pNewNode->pNextBrother = nullptr;
							pCtrlInfo->tree.InsertNode(pNode, pNewNode);
						}

					}
					CoTaskMemFree(pRealIDL);
					//CoTaskMemFree(pidDrives);
					CoTaskMemFree(pidlChild);
					CoTaskMemFree(pidworkDir);
				}
				/*
				else {
					IShellFolder *psfDesktop = NULL;
					IShellFolder *psfWorkDir = NULL;
					IEnumIDList  *penumIDList = NULL;
					LPITEMIDLIST  pidworkDir = (LPITEMIDLIST)CoTaskMemAlloc(sizeof(LPITEMIDLIST));
					SHFILEINFO sfi;
					HRESULT hr;
					hr = SHGetDesktopFolder(&psfDesktop);
					start = clock();
					psfDesktop->ParseDisplayName(hWnd,NULL, (LPWSTR)pNode->data->wsNodePath.c_str(),NULL,&pidworkDir,NULL);
					hr = psfDesktop->BindToObject(pidworkDir, NULL, IID_IShellFolder, (LPVOID*)&psfWorkDir);
					hr=psfWorkDir->EnumObjects(NULL, SHCONTF_FOLDERS| SHCONTF_NONFOLDERS, &penumIDList);
					if (!SUCCEEDED(hr)) {
						break;
					}
					
					ULONG celtFetched = 0;
					LPITEMIDLIST pidlChild = (LPITEMIDLIST)CoTaskMemAlloc(sizeof(LPITEMIDLIST));
					LPITEMIDLIST pRealIDL = (LPITEMIDLIST)CoTaskMemAlloc(sizeof(LPITEMIDLIST));
					STRRET strName;
					WCHAR szName[MAX_PATH] = { 0 };
					
					while (SUCCEEDED(penumIDList->Next(1, &pidlChild, &celtFetched)) && (1 == celtFetched)) {

						hr = SHGetRealIDL(psfWorkDir, pidlChild, &pRealIDL);

						
						hr = psfWorkDir->GetDisplayNameOf(pRealIDL, SHGDN_FORPARSING, &strName);
						hr = StrRetToBuf(&strName, pRealIDL, szName, MAX_PATH);
						//SHGetPathFromIDList(pidChild, szName);
						//MessageBox(NULL, szName, TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
						//SHFILEINFO sfi;
						//hr = SHGetFileInfo((LPCWSTR)szName, 0, &sfi, sizeof(sfi), SHGFI_DISPLAYNAME | SHGFI_SYSICONINDEX);

						if (SUCCEEDED(hr)) {
							NODEDATA *NodeData = new NODEDATA();
							NodeData->wsNodeName = L"替代字符串";//sfi.szDisplayName;
							NodeData->wsNodePath = szName;
							//MessageBox(NULL, szName, TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
							//NodeData->iIcon = sfi.iIcon;
							NodeData->dwNodeDepth = pNode->data->dwNodeDepth+1;
							NodeData->dwExpanded = 0;

							NodeData->dwNodeType = 1;
							NodeData->dwHasChildren = 1;
							NodeData->dwLoadChildrenDone = 1;
							TreeNode<NODEDATA *> *pNewNode = new TreeNode<NODEDATA *>;
							pNewNode->data = NodeData;
							pNewNode->pFirstChild = nullptr;
							pNewNode->pNextBrother = nullptr;
							pCtrlInfo->tree.InsertNode(pNode, pNewNode);
							TVW_CreateNodeWnd(pCtrlInfo, pNewNode);
						}

					}
					CoTaskMemFree(pRealIDL);
					//CoTaskMemFree(pidDrives);
					CoTaskMemFree(pidlChild);
					CoTaskMemFree(pidworkDir);
					end = clock();
					endtime = (double)(end - start) * 1000 / CLOCKS_PER_SEC;
					_itow_s(endtime, tempStr1, 32);
					MessageBox(NULL, tempStr1, TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
					penumIDList->Release();
					psfWorkDir->Release();
					psfDesktop->Release();
				}*/
				
				else {
					start = clock();
					WIN32_FIND_DATA FindFileData;

					//TODO:在这里修改DLL文件夹，现在是在exe路径下
					// 注释的代码可以用于查找所有以“.txt”结尾的文件 
					FINDFILEPARAM *findFileParam = new FINDFILEPARAM();
					findFileParam->dwFileFloderFlag = 0;
					findFileParam->szFileDirectory = pNode->data->wsNodePath;
					wstring tempPath;
					tempPath = pNode->data->wsNodePath;
					tempPath.append(L"\\*");
					//lstrcat(szFileDirectory, L"\\*.txt");
					//lstrcat(szFileDirectory, L"");

					// 查找第一个文件/目录，获得查找句柄 
					HANDLE hListFile = FindFirstFile(tempPath.c_str(), &FindFileData);
					// 判断句柄 
					if (hListFile == INVALID_HANDLE_VALUE)
					{
						//此处应当写入日志
					}
					else
					{
						do {

							if (wcscmp(FindFileData.cFileName, L".") == 0 | wcscmp(FindFileData.cFileName, L"..") == 0) {
								continue;
							}
							if (FindFileData.dwFileAttributes& FILE_ATTRIBUTE_DIRECTORY)
							{
								if (findFileParam->dwFileFloderFlag & 0x00000010) {
									continue;
								}
							}
							else
							{
								if (findFileParam->dwFileFloderFlag & 0x00000001) {
									continue;
								}
							}
							NODEDATA *NodeData = new NODEDATA();
							NodeData->wsNodeName = FindFileData.cFileName;//sfi.szDisplayName;
							wstring wsFilePath;
							wsFilePath = pNode->data->wsNodePath;
							wsFilePath.append(L"\\");
							wsFilePath.append(FindFileData.cFileName);
							NodeData->wsNodePath = wsFilePath;
							//MessageBox(NULL, szName, TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
							//NodeData->iIcon = sfi.iIcon;
							NodeData->dwNodeDepth = pNode->data->dwNodeDepth + 1;
							NodeData->dwExpanded = 0;

							NodeData->dwNodeType = 1;
							NodeData->dwHasChildren = 1;
							NodeData->dwLoadChildrenDone = 1;
							TreeNode<NODEDATA *> *pNewNode = new TreeNode<NODEDATA *>;
							pNewNode->data = NodeData;
							pNewNode->pFirstChild = nullptr;
							pNewNode->pNextBrother = nullptr;
							pCtrlInfo->tree.InsertNode(pNode, pNewNode);
							TVW_CreateNodeWnd(pCtrlInfo, pNewNode);
							//MessageBox(NULL, TEXT("错误码"), FindFileData.cFileName, MB_OK | MB_ICONINFORMATION);

						} while (FindNextFile(hListFile, &FindFileData));
					}
					end = clock();
					endtime = (double)(end - start) * 1000 / CLOCKS_PER_SEC;
					wchar_t tempStr1[32];
					_itow_s(endtime, tempStr1, 32);
					MessageBox(NULL, tempStr1, TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
					pNode->data->dwLoadChildrenDone=1;
				}
			}
			else {
				
				DWORD dwDataID = SendMessage(hWndMain, WM_CUSTOM_TVN, TVN_SETDATA, lParam);
				if (dwDataID) {
					//MessageBox(NULL, L"返回值是什么", TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
					for (vector<NODEDATA *>::iterator iter = ((vector<NODEDATA *> *)IdDataBinding[dwDataID])->begin(); iter != ((vector<NODEDATA *> *)IdDataBinding[dwDataID])->end(); iter++) {
						TreeNode<NODEDATA *> *pNewNode = new TreeNode<NODEDATA *>;
						pNewNode->data = *iter;
						pNewNode->pFirstChild = nullptr;
						pNewNode->pNextBrother = nullptr;

						pCtrlInfo->tree.InsertNode(pNode, pNewNode);
					}
					delete (vector<NODEDATA *> *)IdDataBinding[dwDataID];

					IdDataBinding.erase(dwDataID);
					PutIDToPool(dwDataID);

					
				}
			}
			
			//TVW_CreateItemWnd(pCtrlInfo, pNode);
			
		}
		else {
			
		}
		
		
		
		pCtrlInfo->MaxWidth = 0;
		//TraverseTree_FlushSize(pCtrlInfo,pCtrlInfo->tree.pRoot);
		//迭代遍历刷新尺寸
		pCtrlInfo->tree.TreePreorderTraversal(pCtrlInfo->tree.pRoot, cbTreeTraversalHandler_FlushSize, pCtrlInfo);
		
		pCtrlInfo->ItemCount = 0;
		//TraverseTree_MoveWndItem(pCtrlInfo,pCtrlInfo->tree.pRoot);
		//迭代遍历移动树节点的位置
		
		pCtrlInfo->tree.TreePreorderTraversal(pCtrlInfo->tree.pRoot, cbTreeTraversalHandler_MoveItemWnd, pCtrlInfo);
		
		MoveWindow(pCtrlInfo->hWndContent, 0, 0, pCtrlInfo->MaxWidth, (pCtrlInfo->ItemHeight + 1)*(pCtrlInfo->ItemCount), TRUE);
		PostMessage(pCtrlInfo->hWndScroll, WM_SIZE, NULL, NULL);
		//SendMessage(hWndMain, WM_CUSTOM_TVN, TVN_SIZE, NULL);

	}
	break;
	
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}
LRESULT CALLBACK  TVWP_Content(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

		EndPaint(hWnd, &ps);
	}
	break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}
LRESULT CALLBACK  TVWP_Item(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
	switch (message)
	{
	case WM_CREATE:
	{
		/*
		PWNDDATA *pWndData = (PWNDDATA *)IdDataBinding[(DWORD)((LPCREATESTRUCT)lParam)->hMenu];
		TREEVIEWINFO *pCtrlInfo = (TREEVIEWINFO *)pWndData->pCtrlInfo;

		FORMATETC *pFormatEtcAcceptable = new FORMATETC[1];
		pFormatEtcAcceptable[0] = { CF_HDROP,0,DVASPECT_CONTENT,-1,TYMED_HGLOBAL };
		//pWndData->pDropTargetEvent = new TreeViewDropFile(hWnd,pFormatEtcAcceptable, 1);
		*/
	}
	break;
	case WM_LBUTTONDOWN:
	{
		SetTimer(hWnd, 1, 500, NULL);
	}
	break;
	case WM_LBUTTONUP:
	{
		KillTimer(hWnd, 1); 
	}
	break;
	case WM_TIMER:
	{
		PWNDDATA *pWndData = ((PWNDDATA *)IdDataBinding[(DWORD)GetWindowLongPtr(hWnd, GWLP_ID)]);
		TREEVIEWINFO *pCtrlInfo = (TREEVIEWINFO *)pWndData->pCtrlInfo;
		TreeNode<NODEDATA*> *pNode = (TreeNode<NODEDATA*> *)pWndData->pExtInfo;

		KillTimer(hWnd, 1);
		//MessageBox(NULL, pNode->data->wsNodePath.c_str(), TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
		
		THDROP_Files DragFils;
		DragFils.push_back(UnicodeToANSI(pNode->data->wsNodePath));

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
		stgmed.pUnkForRelease=0;
		IDropSource *pDropSource = new TDropSource;
		TDataObject *pDataObject = new TDataObject();
		pDataObject->AddData(fmtetc, stgmed);
		DWORD dwEffect;
		DWORD dwOKEffect = DROPEFFECT_COPY | DROPEFFECT_MOVE| DROPEFFECT_LINK;
		DWORD dwResult = DoDragDrop(pDataObject, pDropSource, dwOKEffect, &dwEffect);
		pDataObject->ClearAndRelease();
	}
	break;
	case WM_LBUTTONDBLCLK:
	{
		PWNDDATA *pWndData = ((PWNDDATA *)IdDataBinding[(DWORD)GetWindowLongPtr(hWnd, GWLP_ID)]);
		TREEVIEWINFO *pCtrlInfo = (TREEVIEWINFO *)pWndData->pCtrlInfo;
		SendMessage(pCtrlInfo->hWndControl, WM_USER + 11, NULL, GetWindowLongPtr(hWnd, GWLP_ID));
	}
	break;
	case WM_PAINT:
	{
				
		PWNDDATA *pWndData = ((PWNDDATA *)IdDataBinding[(DWORD)GetWindowLongPtr(hWnd, GWLP_ID)]);
		TREEVIEWINFO *pCtrlInfo = (TREEVIEWINFO *)pWndData->pCtrlInfo;
		TreeNode<NODEDATA*> *pNode = (TreeNode<NODEDATA*> *)pWndData->pExtInfo;

		PAINTSTRUCT ps;
		HDC hdcDebug = GetDC(hWndMain);
		RECT rcDebug;

		wchar_t tempStr1[32];
		_itow_s((DWORD)(DWORD)GetWindowLongPtr(hWnd, GWLP_ID), tempStr1, 32);
		TextOut(hdcDebug, 700, 20 * (DWORD)GetWindowLongPtr(hWnd, GWLP_ID),
			tempStr1,
			wcslen(tempStr1));
		_itow_s((DWORD)pWndData, tempStr1, 32);
		TextOut(hdcDebug, 750, 20* (DWORD)GetWindowLongPtr(hWnd, GWLP_ID),
			tempStr1,
			wcslen(tempStr1));

		_itow_s((DWORD)pNode->data->iIcon, tempStr1, 32);
		TextOut(hdcDebug, 800, 20* (DWORD)GetWindowLongPtr(hWnd, GWLP_ID),
			tempStr1,
			wcslen(tempStr1));

		ReleaseDC(hWndMain, hdcDebug);

		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此处添加使用 hdc 的任何绘图代码...

		RECT rc;
		SetRect(&rc, 0, 0, 150, 20);
		HFONT hFont = CreateFontIndirect(&lfList);
		HFONT hOldFont = (HFONT)SelectObject(hdc, hFont); //把新字体选定为设备环境的当前字体，并返回之前的字体
		//MessageBox(NULL, pNode->data->wsNodeName.c_str(), TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
		ImageList_Draw(pCtrlInfo->himlSmallIcon,pNode->data->iIcon, hdc, pCtrlInfo->dIndent*(pNode->data->dwNodeDepth),0,0);
		TextOut(hdc, pCtrlInfo->dIndent*(pNode->data->dwNodeDepth)+16, 0,
			pNode->data->wsNodeName.c_str(),
			wcslen(pNode->data->wsNodeName.c_str()));
		
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
	{
		DWORD dItemID=GetWindowLongPtr(hWnd, GWLP_ID);
		PWNDDATA *pWndData = ((PWNDDATA *)IdDataBinding[dItemID]);
		delete pWndData->pDropTarget;
		delete pWndData->pDropTargetEvent;
		delete pWndData->pExtInfo;
		delete pWndData;

		IdDataBinding.erase(dItemID);
		PutIDToPool(dItemID);
	}
	break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}
LRESULT CALLBACK  TVWP_Button(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_CREATE:
	{

	}
	break;
	case WM_PAINT:
	{
		PWNDDATA *pWndData = ((PWNDDATA *)IdDataBinding[(DWORD)GetWindowLongPtr(hWnd, GWLP_ID)]);
		TREEVIEWINFO *pCtrlInfo = (TREEVIEWINFO *)pWndData->pCtrlInfo;
		TreeNode<NODEDATA> *pNode = (TreeNode<NODEDATA> *)pWndData->pExtInfo;
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