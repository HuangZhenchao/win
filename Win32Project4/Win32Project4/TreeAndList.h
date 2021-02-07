#pragma once
#include "sqlite3.h"  
#pragma comment(lib, "sqlite3.lib")
#include "Transcoder.h"
class FOLDERTREEITEMDATA {
public:
	BOOL bExpanded = FALSE;
	wstring wsPath = L"";
};
class TAGTREEITEMDATA {
public:
	BOOL bExpanded = FALSE;
	wstring wsTagType = L"";
	wstring wsTag = L"";
};
class SELECTINFO {
public:
	DWORD dwSelectType;
	wstring wsPath=L"";
	wstring wsTagType = L"";
	wstring wsTag = L"";
};
class TREEANDLIST {
public:
	//ÎÄ¼þ¼ÐÊ÷
	wstring wsLastPath=L"C:\\myapp";
	HTREEITEM hRootFolderTree;
	void SetRoot_FolderTree();
	mutex mtx;
	mutex mtxMap;

	HANDLE hEvent;
	thread tThread;
	#define THREADCOUNT 2
	thread tThreadArr[THREADCOUNT];
	HANDLE hEventLoadDone[THREADCOUNT];
	BOOL bTerminateFolderTreeLoadThread = FALSE;
	queue<HTREEITEM> qHTreeItem;
	void tFunThread();
	void tFunLoadChildren(int iThreadIndex);

	HWND hFolderTreeView,hTagTreeView, hListView;
	HANDLE hEventLoadListItem;
	BOOL bTerminateLoadListThread=FALSE,bBreakLoadLastList=FALSE;
	TAGTREEITEMDATA TreeItemParam;

	TREEANDLIST(HWND hFolderTreeView,HWND hTreeView,HWND hListView,wstring wsDBPath);
	void FolderTreeNotifyHandler(NMHDR *pHdr);
	void TagTreeNotifyHandler(NMHDR *pHdr);
	void ListNotifyHandler(NMHDR *pHdr);

	thread tLoadListItem;
	void tFunLoadListItem();
	wstring wsFolderTreeSelPath;
	//
	void SetColumn_FolderTree();
private:
	wstring wsDBPath;
	sqlite3 *pSqliteMain;

	sqlite3_stmt *stmtCountAll, *stmtCountAllNoTagged, *stmtCountAllTagged, *stmtCountTagGroup, *stmtCountTag;
	const char* sqlCountAll = "SELECT count(*) from tFileInfo where FilePath>? and FilePath<?;";
	const char* sqlCountAllNoTagged = "SELECT count(*) from tFileInfo where FilePath>? and FilePath<? and FileTag='';";
	const char* sqlCountAllTagged = "select count(DISTINCT TagType), count(DISTINCT Tag),  count(Tag),TagType from  tFileTagInfo where FilePath>? and FilePath<?;";	
	const char* sqlCountTagGroup = "SELECT count(DISTINCT Tag), count(*), TagType from tFileTagInfo where FilePath>? and FilePath<? Group by TagType";
	const char* sqlCountTag = "SELECT count(*) as count, Tag from tFileTagInfo where FilePath>? and FilePath<? and TagType = ? group by Tag";
	
	HTREEITEM hTreeItemAll=NULL, hTreeItemAllTagged=NULL, hTreeItemAllNoTagged=NULL;
	SELECTINFO SelectInfo;
	vector<sqlite3_stmt *> vStmt;
	sqlite3_stmt *stmtSelectAll, *stmtSelectAllNoTagged, *stmtSelectAllTagged, *stmtSelectTagGroup, *stmtSelectTag;
	const char* sqlSelectAll = "SELECT DISTINCT FilePath,* from tFileInfo where FilePath>? and FilePath<?;";
	const char* sqlSelectAllNoTagged = "SELECT DISTINCT FilePath,* from tFileInfo where FilePath>? and FilePath<? and FileTag='';";
	const char* sqlSelectAllTagged = "select DISTINCT tFileInfo.FilePath,tFileInfo.* from tFileTagInfo join tFileInfo on tFileTagInfo.FilePath=tFileInfo.FilePath where tFileTagInfo.FilePath>? and tFileTagInfo.FilePath<?;";
	const char* sqlSelectTagGroup = "select DISTINCT tFileInfo.FilePath,tFileInfo.* from tFileTagInfo join tFileInfo on tFileTagInfo.FilePath=tFileInfo.FilePath where tFileTagInfo.FilePath>? and tFileTagInfo.FilePath<? and tFileTagInfo.TagType=?;";
	const char* sqlSelectTag = "select DISTINCT tFileInfo.FilePath,tFileInfo.* from tFileTagInfo join tFileInfo on tFileTagInfo.FilePath=tFileInfo.FilePath where tFileTagInfo.FilePath>? and tFileTagInfo.FilePath<? and tFileTagInfo.TagType=? and tFileTagInfo.Tag=?;";

};