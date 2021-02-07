#pragma once
#include "sqlite3.h"  
#pragma comment(lib, "sqlite3.lib")
#include <commctrl.h>
#pragma comment(lib,"comctl32.lib")
#include <oleidl.h>
#include <objidl.h>
#include <Shellapi.h>
#include <Shlobj.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib") 
#include "Transcoder.h"
#include "OLE/OLEDragDrop.h"
#include <string>
using namespace std;
#include <map>
#include <queue>
#include <thread>
#include <mutex>

/*

class YTreeView {
public:
	HTREEITEM hItemRoot=NULL;
	BOOL bThreadStatus=TRUE;
	
	//queue<HTREEITEM> qDeleteItem;//目前删除节点的速度还行，也就没采用线程方式
	map<HTREEITEM, TREEITEMDATA> mTreeData;

	YTreeView(HWND,wstring);
	void SetRoot();
	void TreeNotifyHandler(NMHDR* pHdr);

	
};

class TDropTarget : public IDropTarget
{
public:
	TDropTarget(HWND, YTreeView *);
	~TDropTarget(void) {}

	HRESULT __stdcall QueryInterface(REFIID iid, void **ppv);
	ULONG __stdcall AddRef(void) { return ++_iRefCount; }
	ULONG __stdcall Release(void) { if (--_iRefCount == 0) { delete this; return 0; } return _iRefCount; }
	HRESULT __stdcall  DragOver(DWORD grfKeyState, POINTL pt, DWORD *pdwEffect);
	HRESULT __stdcall  DragEnter(IDataObject * pDataObject, DWORD grfKeyState, POINTL pt, DWORD * pdwEffect);
	HRESULT __stdcall DragLeave();
	HRESULT __stdcall Drop(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD __RPC_FAR *pdwEffect);

private:
	ULONG _iRefCount;
	BOOL bFormatAcceptableFlag = FALSE;
	BOOL bItemPathValid = TRUE;
	FORMATETC *pFormatEtcAcceptable;
	UINT uFmtetcCount;

	HWND hWndDropTarget;
	YTreeView *pTreeView;
	//HTREEITEM hItemLastOver;
};

*/