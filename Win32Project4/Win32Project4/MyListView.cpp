#include "stdafx.h"
#include "MyListView.h"

YListView::YListView(HWND hWnd,wstring wsPath):hWndListView(hWnd),wsDataSource(wsPath) {
	fSetColumn();
	tLoadListItem = thread(&YListView::tFunLoadListItem, this);
	WCHAR szEventName_Start[MAX_PATH + 20];
	wsprintf(szEventName_Start, L"EventLoadStart%d%d", this, 0);
	hEventLoadStart = CreateEvent(NULL, TRUE, FALSE, szEventName_Start);
	WCHAR szEventName_Done[MAX_PATH + 20];
	wsprintf(szEventName_Done, L"EventLoadDone%d%d", this, 0);
	hEventLoadDone= CreateEvent(NULL, TRUE, FALSE, szEventName_Done);
	SetEvent(hEventLoadDone);			
}

YListView::~YListView() {
	tLoadListItem.join();
}

void YListView::fSetHIMAGELIST() {
	HIMAGELIST hImageList;
	Shell_GetImageLists(NULL, &hImageList);
	SendMessage(hWndListView, LVM_SETIMAGELIST, LVSIL_SMALL, (LPARAM)hImageList);
}
void YListView::tFunLoadListItem() {
	sqlite3 *pSqliteDB = NULL;
	sqlite3_open_v2(UnicodeToUTF8(wsDataSource).c_str(), &pSqliteDB, SQLITE_OPEN_READWRITE | SQLITE_OPEN_NOMUTEX, NULL);

	sqlite3_stmt *stmtSelect,*stmtCount, *stmtSelectTag, *stmtCountTag;
	sqlite3_stmt *stmtSelectPenetrated, *stmtCountPenetrated, *stmtSelectPenetratedTag, *stmtCountPenetratedTag;
		
	const char *sqlSelect="select * from tFileInfo where ParentPath=? and FileTag like ? and FileName like ?;";
	const char *sqlCount= "select count(*) from tFileInfo where ParentPath=? and FileTag like ? and FileName like ?;";
	
	const char *sqlSelectPenetrated = "select * from tFileInfo where FilePath>? and FilePath<? and FileTag like ? and FileName like ?;";
	const char *sqlCountPenetrated = "select count(*) from tFileInfo where FilePath>? and FilePath<? and FileTag like ? and FileName like ?;";

	const char *sqlSelectTag = "select * from tFileInfo where FilePath in(select FilePath from tFileTagInfo where FilePath=? and Tag like ?;";
	const char *sqlCountTag = "select count(*) from tFileInfo where FilePath in(select FilePath from tFileTagInfo where FilePath=? and Tag like ?;";
	const char *sqlSelectPenetratedTag = "select * from tFileInfo where FilePath in(select FilePath from tFileTagInfo where FilePath>? and FilePath<? and Tag like ?;";
	const char *sqlCountPenetratedTag = "select count(*) from tFileInfo where FilePath in(select FilePath from tFileTagInfo where FilePath>? and FilePath<? and Tag like ?;";
	//select * from tFileInfo where FilePath in(select FilePath from tFileTagInfo where FilePath>'' and FilePath<'[' and Tag like '%_%');
	sqlite3_prepare_v2(pSqliteDB, sqlSelect, strlen(sqlSelect), &stmtSelect, 0);
	sqlite3_prepare_v2(pSqliteDB, sqlCount, strlen(sqlCount), &stmtCount, 0);

	sqlite3_prepare_v2(pSqliteDB, sqlSelectPenetrated, strlen(sqlSelectPenetrated), &stmtSelectPenetrated, 0);
	sqlite3_prepare_v2(pSqliteDB, sqlCountPenetrated, strlen(sqlCountPenetrated), &stmtCountPenetrated, 0);
	sqlite3_prepare_v2(pSqliteDB, sqlSelectTag, strlen(sqlSelectTag), &stmtSelectTag, 0);
	sqlite3_prepare_v2(pSqliteDB, sqlCountTag, strlen(sqlCountTag), &stmtCountTag, 0);

	sqlite3_prepare_v2(pSqliteDB, sqlSelectPenetratedTag, strlen(sqlSelectPenetratedTag), &stmtSelectPenetratedTag, 0);
	sqlite3_prepare_v2(pSqliteDB, sqlCountPenetratedTag, strlen(sqlCountPenetratedTag), &stmtCountPenetratedTag, 0);


	sqlite3_stmt *stmtCountTemp,*stmtSelectTemp;
	while (!bTerminateFlag) {
		WaitForSingleObject(hEventLoadStart, INFINITE);
		stmtCountTemp = NULL;
		stmtSelectTemp = NULL;
		if (SelectConditional.bPenetrateFolderFlag) {
			//穿透文件夹，查询子文件夹里的内容
			wstring wsFilePathValue1, wsFilePathValue2;
			wstring wsFileTagValue;
			wstring wsFileNameValue;
			if (SelectConditional.wsPath.compare(L"") == 0) {
				wsFilePathValue1 = L"";
				wsFilePathValue2 = L"[";//这一区间里是字母，用在文件系统路径里是可行的，
			}
			else {
				wsFilePathValue1 = SelectConditional.wsPath;
				wsFilePathValue1.append(L"\\");
				wsFilePathValue2 = SelectConditional.wsPath;
				wsFilePathValue2.append(L"]");
			}
			switch (SelectConditional.dwTagType)
			{
			case 0://默认为0，加载全部项
			{
				//sqlite3_reset(stmtSelectAll);
				//sqlite3_bi
			}
			break;
			case 1://没有标签的项
			{

			}
			break;
			case 2://单个标签
			{
					
			}
			break;
			}
		}
		else {
			//不穿透文件夹，只查询此文件夹里的内容，不查询子文件夹里的内容
			wstring wsFileTagValue;
			wstring wsFileNameValue;
			if (SelectConditional.wsPath.compare(L"")==0) {
					
			}
			else {
				switch (SelectConditional.dwTagType)
				{
				case 0://默认为0，加载全部项
				{
					stmtCountTemp = stmtCount;
					stmtSelectTemp = stmtSelect;
					wsFileTagValue.append(L"%%");
				}
				break;
				case 1://没有标签的项
				{
					stmtCountTemp = stmtCount;
					stmtSelectTemp = stmtSelect;
					wsFileTagValue.append(L"%%");
				}
				break;
				case 2://所有标签项
				{
					stmtSelectTemp = stmtSelectTag;
					stmtCountTemp = stmtCountTag;
					wsFileTagValue.append(L"%%");
				}
				break;
				case 3://单个标签
				{
					stmtSelectTemp = stmtSelectTag;
					stmtCountTemp = stmtCountTag;
					wsFileTagValue.append(L"%");
					wsFileTagValue.append(SelectConditional.wsTag);
					wsFileTagValue.append(L"%");
				}
				break;
				}
				wsFileNameValue.append(L"%");
				wsFileNameValue.append(SelectConditional.wsSearch);
				wsFileNameValue.append(L"%");

				sqlite3_reset(stmtCountTemp);
				sqlite3_bind_text16(stmtSelect, 1, SelectConditional.wsPath.c_str(), -1, 0);
				sqlite3_bind_text16(stmtSelect, 2, wsFileTagValue.c_str(), -1, 0);
				sqlite3_bind_text16(stmtSelect, 3, wsFileNameValue.c_str(), -1, 0);
				sqlite3_step(stmtCountTemp);
				DWORD dwListItemCount = sqlite3_column_int64(stmtCountTemp,0);
				WCHAR szMessage[MAX_PATH];
				wsprintf(szMessage,L"ListItem总计%d项", dwListItemCount);
				//MessageBox(NULL, szMessage, TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
					
				//while (SQLITE_ROW == sqlite3_step(stmtSelectAll)) {

				//}
			}
				
		}
		SetEvent(hEventLoadDone);
	}
}

void YListView::fSetColumn() {
	LV_COLUMN lvc;
	lvc.mask = LVCF_TEXT | LVCF_SUBITEM | LVCF_WIDTH;
	lvc.pszText = L"文件名";
	lvc.iSubItem = 0;
	lvc.cx = 200;
	ListView_InsertColumn(hWndListView,0,&lvc);
	lvc.pszText = L"拓展名";
	lvc.iSubItem = 1;
	lvc.cx = 50;
	ListView_InsertColumn(hWndListView, 1, &lvc);
	lvc.pszText = L"大小";
	lvc.iSubItem = 2;
	lvc.cx = 50;
	ListView_InsertColumn(hWndListView, 2, &lvc);
	lvc.pszText = L"修改日期";
	lvc.iSubItem = 3;
	lvc.cx = 200;
	ListView_InsertColumn(hWndListView, 3, &lvc);
}

void YListView::ListViewReload(wstring wsPath)
	{
		bSkip = TRUE;
			
		WaitForSingleObject(hEventLoadDone, INFINITE);
		ResetEvent(hEventLoadDone);

		SelectConditional.wsPath = L"D:\\视频同步文件夹";
		SelectConditional.dwTagType = 2;
		SelectConditional.wsSearch = L"";
		SelectConditional.wsTag = L"";
		SetEvent(hEventLoadStart);
	}

