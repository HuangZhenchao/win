#pragma once
//using namespace std;
//using namespace rapidjson;  //引入rapidjson命名空间
/*树形视图类需求：
**
**
**
*/
#include "global.h"
#include "ControlBase.h"
#include "ScrollPanelControl.h"

#define WM_CUSTOM_TVN WM_USER+30
#define TVN_SETDATA 1
#define TVN_SIZE 2
#define TVN_SETROOT	3
#define TVN_REPATH 4
class TreeViewDropFile :public IDropTargetEvent {
public:	
	TreeViewDropFile(HWND hWnd,FORMATETC *pFmtetc,UINT uFmtetcCount):hWndDropTarget(hWnd),pFormatEtcAcceptable(pFmtetc), uFmtetcCount(uFmtetcCount){
		pDropTarget = new TDropTarget(this, pFormatEtcAcceptable, (UINT)1);
		RegisterDragDrop(hWndDropTarget, pDropTarget);
	}
	void OnDrop(IDataObject *pDataObject, DWORD grfKeyState, POINTL pt, DWORD __RPC_FAR *pdwEffect);
	//void OnDrop(IDataObject *pDataObject, DWORD grfKeyState, POINTL pt, DWORD __RPC_FAR *pdwEffect, FORMATETC *pFormatEtcAcceptable, UINT uFmtetcCount);
private:
	FORMATETC *pFormatEtcAcceptable;
	UINT uFmtetcCount;

	HWND hWndDropTarget;
	IDropTarget *pDropTarget;
};
class NODEEXTINFO {
	//附加信息

};
class NODEDATA {
public:
	wstring wsNodeName;
	wstring wsNodePath;
	
	INT iIcon;
	DWORD dwNodeDepth = 0;
	DWORD dwExpanded = 0;
	DWORD dwHasChildren = 0;

	DWORD dwNodeType = 2;
	DWORD dwIconType = 2;	//默认1是文件夹，文件应该为2
	DWORD dwButtonType = 0;
	DWORD dwLinesType = 0;
	
	
	DWORD dwLoadChildrenDone = 0;
	HWND hWndItem;

	DWORD dwHasExtraInfo=0;
	void *pNodeExtInfo;
};

class TREEVIEWINFO {
public:
	HINSTANCE hInstance;
	HWND hWndControl;
	HWND hWndControlParent;
	HWND hWndContent;
	HWND hWndScroll;

	SCROLLPANELCONTROLINFO *scrollPanelControlInfo;

	Tree<NODEDATA *> tree;

	DWORD dwDataSource;//1:文件夹树 2：文件树 3：json 4：数据库（sqlite3） 
	//DWORD DataLoadWay = 2;//树的数据加载方式，1：初始化时全部加载，2：动态加载，初始化或打开一个节点时，加载显示节点的下一层
	wstring wsRootNodePath;
	wstring wsRootNodeName;
	wstring DefaultPath;
	HIMAGELIST himlSmallIcon;
	HIMAGELIST himlButtonAndLines;
	DWORD dIndent;
	DWORD ItemHeight;

	DWORD MaxWidth = 0;
	DWORD ItemCount;
	DWORD TotalHeight;
	TREEVIEWINFO();
};
void TVW_RegisterWnd(HINSTANCE);

void TVW_CreateItemWnd(TREEVIEWINFO *,TreeNode<NODEDATA *> *);

void RemoveData(TreeNode<NODEDATA *> *);
LRESULT CALLBACK  TVWP_Control(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK  TVWP_Content(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK  TVWP_Item(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK  TVWP_Button(HWND, UINT, WPARAM, LPARAM);