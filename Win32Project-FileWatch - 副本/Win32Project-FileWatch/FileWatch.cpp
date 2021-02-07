#include "stdafx.h"
#include "FileWatch.h"
FILEWATCH::FILEWATCH(HWND hWnd,wstring wsPath_ProData) {
	hWndDisplay = hWnd;

	//setlocale(LC_ALL, "");	
	//初始化事件
	//数据库路径及建表
	wsDBPath.append(wsPath_ProData); wsDBPath.append(L"\\"); wsDBPath.append(wsDBName);

	sqlite3 *db = NULL;
	HRESULT rc=sqlite3_open_v2(UnicodeToUTF8(wsDBPath).c_str(), &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_NOMUTEX | SQLITE_OPEN_CREATE, NULL);
	
	//const char *sqlCreateTableDriverInfo = "Create table if not exists tDriverInfo(VolumeSerialNumber INTEGER not null primary key,Driver text not null);";
	const char *sqlCreateTableFileInfo = "Create table if not exists tFileInfo(\
		FilePath text not null primary key,\
		ParentPath text not null,\
		FileName text not null,\
		FileExtName text not null,\
		FileIconIndex int not null,\
		FileSize INTEGER not null,\
		FileAttributes INTEGER not null,\
		FileCreationTime text not null,\
		FileLastWriteTime text not null,\
		FileLastAccessTime text not null,\
		FileTag text,\
		DriverInfo text not null);";
	const char *sqlCreateTableTagFile = "Create table if not exists tFileTagInfo(FilePath text not null,ParentPath text not null,Tag text not null);";
	//sqlite3_exec(db, sqlCreateTableDriverInfo, 0, 0, 0);
	sqlite3_exec(db, sqlCreateTableFileInfo, 0, 0, 0);
	sqlite3_exec(db, sqlCreateTableTagFile, 0, 0, 0);
	sqlite3_exec(db, "CREATE INDEX indexParentPath ON tFileInfo(ParentPath);", 0, 0, 0);
	sqlite3_exec(db, "CREATE INDEX indexFilePath ON tFileInfo(FilePath);", 0, 0, 0);
	//sqlite3_exec(db, "PRAGMA journal_mode=WAL;", 0, 0, 0);
	
	sqlite3_stmt *stmtCountDriver;
	const char *sqlCountDriver = "select count(*) from tFileInfo where DriverInfo=?;";
	sqlite3_prepare_v2(db, sqlCountDriver, strlen(sqlCountDriver), &stmtCountDriver, 0);

	char szSqlDelete[500] = {0};
	char szTmp[200] = {0};
	//获取盘符、序列号，用盘符、序列号的组合作为识别，每次开启时，先把已卸载的盘的记录删除，在
	WCHAR szLogicDriverStrings[1024];
	ZeroMemory(szLogicDriverStrings, sizeof(szLogicDriverStrings));
	DWORD DSLength = GetLogicalDriveStrings(1024, szLogicDriverStrings);
	PWCHAR lpszDriver = (PWCHAR)szLogicDriverStrings;
	do{
		wstring wsDriver = lpszDriver;

		DWORD dwVolumeSerialNumber = 0;
		GetVolumeInformation(wsDriver.c_str(), NULL, MAX_PATH, &dwVolumeSerialNumber, NULL, NULL, NULL, 0);
		if (0 == dwVolumeSerialNumber) {
			lpszDriver += (lstrlen(lpszDriver) + 1);
			continue;
		}
		vDriver.push_back(wsDriver);
		WCHAR szDriverInfo[20];
		wsprintf(szDriverInfo,L"%ls%d", wsDriver.c_str(), dwVolumeSerialNumber);
		
		mDriverInfo.insert(map<wstring, wstring>::value_type(GetFirstDriverCharW(wsDriver), szDriverInfo));
		sqlite3_reset(stmtCountDriver);
		sqlite3_bind_text16(stmtCountDriver, 1, szDriverInfo, -1, 0);
		sqlite3_step(stmtCountDriver);
		int count = sqlite3_column_int(stmtCountDriver,0);
		if (0== count) {
			TASKINFO TaskInfo;
			WCHAR *lpszTemp = new WCHAR[20];
			wcscpy_s(lpszTemp,20,L"D:\\视频2：待归档");

			TaskInfo.dwTaskType = 1;
			TaskInfo.wsFilePath = GetParentPath(wsDriver);//lpszTemp;lpszTemp;// 
			qTaskInfo.push(TaskInfo);
		}
		sprintf(szTmp, "%s and DriverInfo<>'%s'", szTmp, UnicodeToANSI(szDriverInfo).c_str());
		lpszDriver += (lstrlen(lpszDriver) + 1);
	} while (*lpszDriver != '\x00');
	sprintf(szSqlDelete, "delete from tFileInfo where 1=1 %s;", szTmp);
	
	sqlite3_exec(db, szSqlDelete, 0, 0, 0);
	sqlite3_finalize(stmtCountDriver);
	sqlite3_close_v2(db);

	//创建事件，创建线程
	WCHAR szEventName_Distribute[MAX_PATH + 20];
	wsprintf(szEventName_Distribute, L"EventDistribute%d", this);
	hEventDistribute = CreateEvent(NULL, TRUE, FALSE, szEventName_Distribute);
	WCHAR szEventName_TraverseStart[MAX_PATH + 20];
	wsprintf(szEventName_TraverseStart, L"EventTraverseStart%d", this);
	hEventTraverseStart = CreateEvent(NULL, TRUE, FALSE, szEventName_TraverseStart);
	for (int i = 0; i < THREADCOUNT; i++) {
		WCHAR szEventName_TraverseDone[MAX_PATH + 20];
		wsprintf(szEventName_TraverseDone, L"EventTraverseDone%d%d", this, i);
		hEventTraverseDone[i] = CreateEvent(NULL, TRUE, FALSE, szEventName_TraverseDone);
	}
	
	mTypeIcon.insert(map<wstring, INT>::value_type(L"Folder", 3));
	mTypeIcon.insert(map<wstring, INT>::value_type(L"File", 0));
	hEventGetIcon = CreateEvent(NULL, TRUE, FALSE, L"EventGetIcon");
	tGetIcon=thread(&FILEWATCH::tFunGetIcon, this);

	for (int i = 0; i < vDriver.size();i++) {
		//vThreadWatchDir.push_back(thread(&FILEWATCH::tFunChangedWactch, this, i));
	}
	tTaskDistribute = thread(&FILEWATCH::tFunTaskDistribute, this);
	for (INT i = 0; i < THREADCOUNT; i++) {
		vThreadTraverseDir.push_back(thread(&FILEWATCH::tFunTraverseDir, this, i));
	}
	SetEvent(hEventDistribute);
}

FILEWATCH::~FILEWATCH() {
	bTerminateFlag = TRUE;
	tGetIcon.join();
	SetEvent(hEventTraverseStart);
	for (int i = 0; i < vThreadTraverseDir.size(); i++) {
		vThreadTraverseDir[i].join();
		SetEvent(hEventTraverseDone[i]);
	}
	SetEvent(hEventDistribute);
	tTaskDistribute.join();
	for (int i = 0; i < vThreadWatchDir.size(); i++) {
		vThreadWatchDir[i].join();
	}
}
void FILEWATCH::tFunTaskDistribute() {
	//CoInitialize(NULL);
	sqlite3 *pSqliteDB = NULL;
	sqlite3_open_v2(UnicodeToUTF8(wsDBPath).c_str(), &pSqliteDB, SQLITE_OPEN_READWRITE | SQLITE_OPEN_NOMUTEX, NULL);
	sqlite3_stmt *stmtDeleteFileInfo;
	sqlite3_stmt *stmtDeleteFileTagInfo;
	const char* sqlDeleteFileInfo = "delete from tFileInfo where FilePath=? or (FilePath>? and FilePath<?);";
	const char* sqlDeleteFileTagInfo = "delete from tFileTagInfo where FilePath=? or (FilePath>? and FilePath<?);";

	sqlite3_prepare_v2(pSqliteDB, sqlDeleteFileInfo, strlen(sqlDeleteFileInfo), &stmtDeleteFileInfo, 0);
	sqlite3_prepare_v2(pSqliteDB, sqlDeleteFileTagInfo, strlen(sqlDeleteFileTagInfo), &stmtDeleteFileTagInfo, 0);
	//WCHAR *lpszDriver=vDriver[iDriverIndex];		
	TASKINFO pTaskInfo;
	BOOL bFlag=FALSE;
	WCHAR szFilePath1[MAX_PATH + 1];
	WCHAR szFilePath2[MAX_PATH + 1];
	while (!bTerminateFlag) {
		WaitForSingleObject(hEventDistribute, INFINITE);
		mtxTask.lock();
		if (!(qTaskInfo.empty())) {
			pTaskInfo = qTaskInfo.front();
			bFlag = TRUE;
			qTaskInfo.pop();
			//SetEvent(vEvent[iDriverIndex]);
		}
		else {
			//wprintf(L"循环检测盘%ls线程%d\n", vDBPath[iDriverIndex], vEvent[iDriverIndex]);
			ResetEvent(hEventDistribute);
		}
		mtxTask.unlock();
		if (bFlag) {
			//wprintf(L"盘序号%d,操作标志%d,目标路径为%ls\r\n", iDriverIndex,pTaskInfo->dwTaskType, pTaskInfo->lpszFilePath);
			switch (pTaskInfo.dwTaskType)
			{
			case 1: 
			{
				//sqlite3_exec(vSQLite[iDriverIndex], "begin immediate;", 0, 0, 0);
				WCHAR szDisplayString[MAX_PATH + 20];
				wsprintf(szDisplayString, L"正在检索路径%ls下的文件...\r\n", pTaskInfo.wsFilePath.c_str());
				
				if(SendMessage(hWndDisplay, EM_GETLINECOUNT, 0, 0)>100){
					SendMessage(hWndDisplay, EM_SETSEL, 0, -1);
				}
				else {
					SendMessage(hWndDisplay, EM_SETSEL, -2, -1);
				}
				
				SendMessage(hWndDisplay, EM_REPLACESEL, 0, (LPARAM)szDisplayString);
				clock_t start, end;
				start = clock();
				
				mtxSubTask.lock();
				qSubTask.push(pTaskInfo.wsFilePath);//(pTaskInfo->lpszFilePath);
				SetEvent(hEventTraverseStart);
				mtxSubTask.unlock();

				WaitForMultipleObjects(THREADCOUNT, hEventTraverseDone,TRUE, INFINITE);
				for (int i = 0; i < THREADCOUNT;i++) {
					ResetEvent(hEventTraverseDone[i]);
				}
				end = clock();
				wsprintf(szDisplayString, L"检索完成，耗时%dms\r\n",end-start);
				SendMessage(hWndDisplay, EM_SETSEL, -2, -1);
				SendMessage(hWndDisplay, EM_REPLACESEL, 0, (LPARAM)szDisplayString);
				//sqlite3_exec(vSQLite[iDriverIndex], "commit;", 0, 0, 0);
			}
			break;
				
			case 2:
			{
				WCHAR szDisplayString[MAX_PATH + 20];
				wsprintf(szDisplayString, L"移除路径%ls相关信息...\r\n", pTaskInfo.wsFilePath.c_str());
				if (SendMessage(hWndDisplay, EM_GETLINECOUNT, 0, 0)>100) {
					SendMessage(hWndDisplay, EM_SETSEL, 0, -1);
				}
				else {
					SendMessage(hWndDisplay, EM_SETSEL, -2, -1);
				}
				SendMessage(hWndDisplay, EM_REPLACESEL, 0, (LPARAM)szDisplayString);

				wstring wsFilePath1= pTaskInfo.wsFilePath,wsFilePath2= pTaskInfo.wsFilePath;
				wsFilePath1.append(L"\\"); wsFilePath1.append(L"]");

				//sqlite3_exec(db, "begin immediate;", 0, 0, 0);
				while (SQLITE_BUSY == sqlite3_exec(pSqliteDB, "begin immediate;", 0, 0, 0)) {
					wprintf(L"%ls\n", L"DBDelete线程begin immediate操作返回值为SQLITE_BUSY;");
					Sleep(100);
				}
				sqlite3_reset(stmtDeleteFileInfo);
				sqlite3_bind_text16(stmtDeleteFileInfo, 1, pTaskInfo.wsFilePath.c_str(), -1, 0);
				sqlite3_bind_text16(stmtDeleteFileInfo, 2, wsFilePath1.c_str(), -1, 0);
				sqlite3_bind_text16(stmtDeleteFileInfo, 3, wsFilePath2.c_str(), -1, 0);
				sqlite3_step(stmtDeleteFileInfo);

				sqlite3_reset(stmtDeleteFileTagInfo);
				sqlite3_bind_text16(stmtDeleteFileTagInfo, 1, pTaskInfo.wsFilePath.c_str(), -1, 0);
				sqlite3_bind_text16(stmtDeleteFileTagInfo, 2, wsFilePath1.c_str(), -1, 0);
				sqlite3_bind_text16(stmtDeleteFileTagInfo, 3, wsFilePath2.c_str(), -1, 0);
				sqlite3_step(stmtDeleteFileTagInfo);
				while (SQLITE_BUSY == sqlite3_exec(pSqliteDB, "commit;", 0, 0, 0)) {
					wprintf(L"%ls\n", L"DBDelete线程commit操作返回值为SQLITE_BUSY;");
					Sleep(100);
				}
			}
			break;
			default:
				break;
			} 
		}

		bFlag = FALSE;
		/*
		
		*/
	}
}

void FILEWATCH::tFunTraverseDir(INT iThreadID) {
	sqlite3 *pSqliteDB = NULL;
	//sqlite3 *pSqliteDB = vSQLite[iDriverIndex];
	const char* sqlInsertFileInfo = "insert into tFileInfo values(?,?,?,?,?,?,?,?,?,?,?,?);";
	const char* sqlInsertFileTagInfo = "insert into tFileTagInfo values(?,?,?);";
	sqlite3_stmt *stmtInsertFileInfo;
	sqlite3_stmt *stmtInsertFileTagInfo;

	sqlite3_open_v2(UnicodeToUTF8(wsDBPath).c_str(), &pSqliteDB, SQLITE_OPEN_READWRITE | SQLITE_OPEN_NOMUTEX, NULL);
	sqlite3_prepare_v2(pSqliteDB, sqlInsertFileInfo, strlen(sqlInsertFileInfo), &stmtInsertFileInfo, 0);
	sqlite3_prepare_v2(pSqliteDB, sqlInsertFileTagInfo, strlen(sqlInsertFileTagInfo), &stmtInsertFileTagInfo, 0);
	BOOL bFlag = FALSE;
	

	//BOOL bTaskDone=FALSE;
	while (!bTerminateFlag) {
		WaitForSingleObject(hEventTraverseStart, INFINITE);
		wstring wsDir;
		mtxSubTask.lock();
		if (!(qSubTask.empty())) {
			bFlag = TRUE;
			wsDir = qSubTask.front();
			//wprintf(L"步骤检测%ls\n", lpszDir);
			qSubTask.pop();
			
		}
		
		mtxSubTask.unlock();
		
		if (bFlag) {
			wstring wsFind=wsDir;
			wsFind.append(L"\\*");

			WIN32_FIND_DATA FindFileData;
		
			while (SQLITE_BUSY == sqlite3_exec(pSqliteDB, "begin immediate;", 0, 0, 0)) {
				//wprintf(L"线程%d:%ls\n", iThreadID, L"DBInsert线程begin immediate操作返回值为SQLITE_BUSY;");
				Sleep(5);
			}
			
			if (!(INVALID_HANDLE_VALUE == FindFirstFile(wsDir.c_str(), &FindFileData))) {
				wstring wsParentDir = GetParentPath(wsDir);
				FunDBInsert(pSqliteDB, stmtInsertFileInfo, stmtInsertFileTagInfo, wsDir, wsParentDir, &FindFileData);
			}
			//wprintf(L"线程%d:FindFirst文件%ls\n", iThreadID, lpszDir);
			HANDLE hListFile = FindFirstFile(wsFind.c_str(), &FindFileData);
			// 判断句柄 
			if (!(hListFile == INVALID_HANDLE_VALUE))
			{
				do {

					if (wcscmp(FindFileData.cFileName, L".") == 0 | wcscmp(FindFileData.cFileName, L"..") == 0) {
						continue;
					}
					wstring wsFilePath;
					wsFilePath.append(wsDir); wsFilePath.append(L"\\"); wsFilePath.append(FindFileData.cFileName);
					//MessageBox(NULL, wsDir.c_str(), TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
					if (FindFileData.dwFileAttributes& FILE_ATTRIBUTE_DIRECTORY) {
						wstring wsFolderPath;
						wsFolderPath = wsFilePath;

						mtxSubTask.lock();
						qSubTask.push(wsFolderPath);
						//SetEvent(vHEventRun[iDriverIndex]);
						mtxSubTask.unlock();
						
					}
					else {
						FunDBInsert(pSqliteDB, stmtInsertFileInfo, stmtInsertFileTagInfo, wsFilePath, wsDir, &FindFileData);
					}
					//lpFindFileData = new WIN32_FIND_DATA;
					//delete lpszFilePath;
				} while (FindNextFile(hListFile, &FindFileData));
				
			}
			
			while (SQLITE_BUSY == sqlite3_exec(pSqliteDB, "commit;", 0, 0, 0)) {
				//wprintf(L"线程%d:%ls\n", iThreadID, L"DBInsert线程commit操作返回值为SQLITE_BUSY;");
				//WCHAR tempStr1[1024];
				//_itow_s(rc, tempStr1, 32, 10);
				//MessageBox(NULL, L"SQLITE_BUSY", TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
				Sleep(5);
			}

		}

		if (!(qSubTask.empty())) {
			SetEvent(hEventTraverseStart);
			ResetEvent(hEventTraverseDone[iThreadID]);
		}
		else {
			//wprintf(L"循环检测盘%ls线程%d\n", vDBPath[iDriverIndex], vEvent[iDriverIndex]);

			SetEvent(hEventTraverseDone[iThreadID]);
			ResetEvent(hEventTraverseStart);
		}
		bFlag = FALSE;
	}
	//sqlite3_close_v2(vSQLite[iDriverIndex]);
	wprintf(L"线程结束%d\n", iThreadID);
}
/*
void FILEWATCH::tFunGetFileIcon() {
sqlite3 *pSqliteDB = NULL;
sqlite3_open_v2(dbName, &pSqliteDB, SQLITE_OPEN_READWRITE | SQLITE_OPEN_NOMUTEX, NULL);
sqlite3_stmt *stmtSelectCount;


sqlite3_stmt *stmtSelect;


sqlite3_stmt *stmtUpdate;
const char* sqlUpdateFileInfo = "update tFileInfo set FileIconIndex=? ,GetFileIconDone=1 where FilePath=?; ";
sqlite3_prepare_v2(pSqliteDB, sqlUpdateFileInfo, strlen(sqlUpdateFileInfo), &stmtUpdate, 0);
while(TRUE){

WaitForSingleObject(hEventDBUpdateIcon, INFINITE);

const char* sqlSelectCount = "select count(*) from tFileInfo where GetFileIconDone=0;";
sqlite3_prepare_v2(pSqliteDB, sqlSelectCount, strlen(sqlSelectCount), &stmtSelectCount, 0);

const char* sqlSelectFileInfo = "select FilePath from tFileInfo where GetFileIconDone=0;";
sqlite3_prepare_v2(pSqliteDB, sqlSelectFileInfo, strlen(sqlSelectFileInfo), &stmtSelect, 0);
wprintf(L"获取图标线程开始\n");


while (SQLITE_BUSY == sqlite3_exec(pSqliteDB, "begin immediate;", 0, 0, 0)) {
wprintf(L"%ls\n", L"GetFileIcon线程begin immediate操作返回值为SQLITE_BUSY;");
Sleep(5);
}
//sqlite3_exec(pSqliteDB, "begin;", 0, 0, 0);
wprintf(L"%ls\n", L"GetFileIcon线程begin immediate操作;");
sqlite3_step(stmtSelectCount);
DWORD dwTaskCount = sqlite3_column_int64(stmtSelectCount, 0);
wprintf(L"有%d条记录没有获取文件图标\n", dwTaskCount);

DWORD dwIndex = 0;

SHFILEINFO sfi;
vector<WCHAR *> vFilePath;
vector<INT> vFileIcon;

//sqlite3_reset(stmtSelect);
while (SQLITE_ROW == sqlite3_step(stmtSelect)) {
WCHAR *lpszFilePath = new WCHAR[MAX_PATH];
WCHAR *lpszSelect = (WCHAR *)sqlite3_column_text16(stmtSelect, 0);
wcscpy_s(lpszFilePath, MAX_PATH, lpszSelect);
vFilePath.push_back(lpszFilePath);
ZeroMemory(&sfi, sizeof(SHFILEINFO));
//wprintf(L"线程结束%ls\n", (LPCWSTR)sqlite3_column_text16(stmtSelect, 0));
SHGetFileInfo(lpszFilePath, 0, &sfi, sizeof(sfi), SHGFI_SYSICONINDEX);
vFileIcon.push_back(sfi.iIcon);
}
sqlite3_exec(pSqliteDB, "commit;", 0, 0, 0);
sqlite3_finalize(stmtSelectCount);
sqlite3_finalize(stmtSelect);

for (int i = 0; i < vFilePath.size(); i++) {
dwIndex = dwIndex + 1;
wprintf(L"获取文件图标进度%d/%d\n", dwIndex, dwTaskCount);

sqlite3_reset(stmtUpdate);
sqlite3_bind_int64(stmtUpdate, 1, vFileIcon[i]);
sqlite3_bind_text16(stmtUpdate, 2, vFilePath[i], -1, 0);
sqlite3_step(stmtUpdate);

}

while (SQLITE_BUSY == sqlite3_exec(pSqliteDB, "commit;", 0, 0, 0)) {
wprintf(L"%ls\n", L"GetFileIcon线程commit操作返回值为SQLITE_BUSY;");
Sleep(5);
}
wprintf(L"%ls\n", L"GetFileIcon线程commit操作;");

ResetEvent(hEventDBUpdateIcon);
}

sqlite3_close_v2(pSqliteDB);
}
*/
void FILEWATCH::FunDBInsert(sqlite3 *pSqliteDB, sqlite3_stmt *stmtInsertFileInfo, sqlite3_stmt *stmtInsertFileTagInfo, wstring wsFilePath, wstring wsFileParentPath, WIN32_FIND_DATA *lpFindFileData) {
	WCHAR szTypeName[MAX_PATH];
	WCHAR szTag[MAX_PATH];
	vector<wstring> *pvTag = new vector<wstring>;

	if (lpFindFileData->dwFileAttributes& FILE_ATTRIBUTE_DIRECTORY) {
		ExtractTypeNameAndTagFromFileName(lpFindFileData->cFileName, TRUE, pvTag, szTag, szTypeName);
	}
	else {
		ExtractTypeNameAndTagFromFileName(lpFindFileData->cFileName, FALSE, pvTag, szTag, szTypeName);
	}
	wstring wsTypeName = szTypeName;
	if (mTypeIcon.count(wsTypeName)==0) {
		GETICONINFO GetIconInfo;
		GetIconInfo.wsTypeName = wsTypeName;
		GetIconInfo.hEventGetDone = CreateEvent(NULL, TRUE, FALSE, szTypeName);
		mtx.lock();
		qTaskGetIcon.push(GetIconInfo);
		SetEvent(hEventGetIcon);
		mtx.unlock();
		WaitForSingleObject(GetIconInfo.hEventGetDone, INFINITE);
		CloseHandle(GetIconInfo.hEventGetDone);
		//delete pGetIconInfo;
	}
		
	//绑定文件信息表插入语句数据
	sqlite3_reset(stmtInsertFileInfo);

	//wprintf(L"%ls\n", pszFilePath);
	sqlite3_bind_text16(stmtInsertFileInfo, 1, wsFilePath.c_str(), -1, 0);//数据库中的FilePath列
	sqlite3_bind_text16(stmtInsertFileInfo, 2, wsFileParentPath.c_str(), -1, 0);//数据库中的ParentPath列


	sqlite3_bind_text16(stmtInsertFileInfo, 3, lpFindFileData->cFileName, -1, 0);//数据库中的FileName列
	sqlite3_bind_text16(stmtInsertFileInfo, 4, szTypeName, -1, 0);//数据库中的FileExtName列
	sqlite3_bind_int(stmtInsertFileInfo, 5, mTypeIcon[wsTypeName]);//数据库中的FileIconIndex列

	long long FileSize = (lpFindFileData->nFileSizeHigh * MAXDWORD + 1) + lpFindFileData->nFileSizeLow;
	sqlite3_bind_int64(stmtInsertFileInfo, 6, FileSize);//数据库中的FileSize列

	sqlite3_bind_int64(stmtInsertFileInfo, 7, lpFindFileData->dwFileAttributes);//数据库中的FileSize列
	WCHAR szCreationTime[20] = { 0 };
	WCHAR szLastAccessTime[20];
	WCHAR szLastWriteTime[20];

	FileTimeToWCHAR(&lpFindFileData->ftCreationTime, szCreationTime);
	FileTimeToWCHAR(&lpFindFileData->ftLastAccessTime, szLastAccessTime);
	FileTimeToWCHAR(&lpFindFileData->ftLastWriteTime, szLastWriteTime);
	sqlite3_bind_text16(stmtInsertFileInfo, 8, szCreationTime, -1, 0);//数据库中的FileExtName列
	sqlite3_bind_text16(stmtInsertFileInfo, 9, szLastWriteTime, -1, 0);//数据库中的FileExtName列
	sqlite3_bind_text16(stmtInsertFileInfo, 10, szLastAccessTime, -1, 0);//数据库中的FileExtName列
	sqlite3_bind_text16(stmtInsertFileInfo, 11, szTag, -1, 0);//数据库中的FileTag列

	sqlite3_bind_text16(stmtInsertFileInfo, 12, mDriverInfo[GetFirstDriverCharW(wsFilePath)].c_str(),-1, 0);//数据库中的GetFileIconDone列
	sqlite3_step(stmtInsertFileInfo);
	//vector<wstring> vTag = ExtractTagFromFileName(FindFileData.cFileName);
	for (int i = 0; i < pvTag->size(); i++) {
		//绑定文件标签信息表插入语句数据
		sqlite3_reset(stmtInsertFileTagInfo);
		sqlite3_bind_text16(stmtInsertFileTagInfo, 1, wsFilePath.c_str(), -1, 0);//数据库中的FilePath列
		sqlite3_bind_text16(stmtInsertFileTagInfo, 2, wsFileParentPath.c_str(), -1, 0);//数据库中的FilePath列
		sqlite3_bind_text16(stmtInsertFileTagInfo, 3, (*pvTag)[i].c_str(), -1, 0);//数据库中的FilePath列
		sqlite3_step(stmtInsertFileTagInfo);
	}

	delete pvTag;
}
//监控路径的线程

void FILEWATCH::tFunChangedWactch(INT iDriverIndex) {
	
	wstring wsDir = vDriver[iDriverIndex];
	HANDLE hDir = CreateFile(wsDir.c_str(),
		GENERIC_READ | GENERIC_WRITE | FILE_LIST_DIRECTORY,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_BACKUP_SEMANTICS,
		NULL);
	if (hDir == INVALID_HANDLE_VALUE)
	{
		return;
	}
	WCHAR szDisplayString[MAX_PATH + 20];
	wsprintf(szDisplayString, L"%ls开启文件监控\r\n", vDriver[iDriverIndex]);
	SendMessage(hWndDisplay, EM_SETSEL, -2, -1);
	SendMessage(hWndDisplay, EM_REPLACESEL, 0, (LPARAM)szDisplayString);
	DWORD BytesReturned = 0;
	WCHAR szBuffer[1024];
	//memset(szBuffer, 0, sizeof(szBuffer));
	FILE_NOTIFY_INFORMATION *pnotify;
	//FILE_NOTIFY_INFORMATION *pnotify = (FILE_NOTIFY_INFORMATION *)notify;

	wstring wsFilePath;
	WCHAR szFileName[MAX_PATH];
	while (!bTerminateFlag)
	{
		//wprintf(L"%d\n", sizeof(WCHAR) * 1024);
		ZeroMemory(szBuffer, sizeof(WCHAR) * 1024);
		if (ReadDirectoryChangesW(hDir,
			(LPVOID)szBuffer,
			sizeof(szBuffer),
			TRUE,
			FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_SIZE,//为什么加上这个FILE_NOTIFY_CHANGE_LAST_WRITE，删除文件时不能接收到FILE_ACTION_REMOVED
			(LPDWORD)&BytesReturned,
			NULL,
			NULL))
		{
			//wprintf(L"%ls\n", L"执行一次ReadDirectoryChangesW");
			DWORD dwOffset = 0;
			do {

				pnotify = (FILE_NOTIFY_INFORMATION *)&szBuffer[dwOffset / 2];
				dwOffset += pnotify->NextEntryOffset;
				ZeroMemory(szFileName, MAX_PATH * sizeof(WCHAR));
				CopyMemory(szFileName, pnotify->FileName, pnotify->FileNameLength);
				
				wsFilePath.append(wsDir);
				wsFilePath.append(szFileName);

				if (IsStrFinded(wsFilePath,L".db-journal")) {
					continue;
				}
				switch (pnotify->Action)
				{
				case FILE_ACTION_REMOVED:
				{

					TASKINFO TaskInfo;
					TaskInfo.dwTaskType = 2;
					TaskInfo.wsFilePath = wsFilePath;
					mtxTask.lock();
					qTaskInfo.push(TaskInfo);
					mtxTask.unlock();
					SetEvent(hEventDistribute);
				}
				break;
				case FILE_ACTION_ADDED:
				{

					TASKINFO TaskInfo;
					TaskInfo.dwTaskType = 1;
					TaskInfo.wsFilePath = wsFilePath;
					mtxTask.lock();
					qTaskInfo.push(TaskInfo);
					mtxTask.unlock();
					SetEvent(hEventDistribute);

				}
				break;

				case FILE_ACTION_MODIFIED:
				{

					TASKINFO TaskInfo;
					TaskInfo.dwTaskType = 2;
					TaskInfo.wsFilePath = wsFilePath;

					TASKINFO TaskInfo1;
					TaskInfo1.dwTaskType = 1;
					TaskInfo1.wsFilePath = wsFilePath;

					mtxTask.lock();
					qTaskInfo.push(TaskInfo);
					qTaskInfo.push(TaskInfo1);
					mtxTask.unlock();
					SetEvent(hEventDistribute);
				}
				break;
				case FILE_ACTION_RENAMED_OLD_NAME:
				{

					TASKINFO TaskInfo;
					TaskInfo.dwTaskType = 2;
					TaskInfo.wsFilePath = wsFilePath;
					mtxTask.lock();
					qTaskInfo.push(TaskInfo);
					mtxTask.unlock();
					SetEvent(hEventDistribute);
				}
				break;
				case FILE_ACTION_RENAMED_NEW_NAME:
				{

					TASKINFO TaskInfo;
					TaskInfo.dwTaskType = 1;
					TaskInfo.wsFilePath = wsFilePath;
					mtxTask.lock();
					qTaskInfo.push(TaskInfo);
					mtxTask.unlock();
					SetEvent(hEventDistribute);

				}
				break;

				}
			} while (pnotify->NextEntryOffset != 0);
			if (BytesReturned == 0) {
				printf("*********************sys-alloc buffer OVERFLOW!************************\n");
			}

		}
	}
}
void FILEWATCH::tFunGetIcon() {
	CoInitialize(NULL);
	GETICONINFO pGetIconInfo;
	BOOL bFlag;
	while (!bTerminateFlag) {
		WaitForSingleObject(hEventGetIcon, INFINITE);
		mtx.lock();
		if (!qTaskGetIcon.empty()) {
			pGetIconInfo = qTaskGetIcon.front();
			bFlag = TRUE;
			qTaskGetIcon.pop();
		}
		mtx.unlock();
		if (bFlag) {
			WCHAR szFileType[MAX_PATH];
			ZeroMemory(szFileType,sizeof(szFileType));
			wcscpy_s(szFileType, MAX_PATH,L"*.");
			wcscat_s(szFileType, MAX_PATH, pGetIconInfo.wsTypeName.c_str());

			SHFILEINFO sfi;
			ZeroMemory(&sfi, sizeof(SHFILEINFO));
			//WCHAR *lpszPath = L"E:\\RECYCLED";
			SHGetFileInfo(szFileType, FILE_ATTRIBUTE_NORMAL, &sfi, sizeof(sfi), SHGFI_SYSICONINDEX|SHGFI_USEFILEATTRIBUTES);
			WCHAR szMessageStr[1024];
			wsprintf(szMessageStr, L"%ls图标索引%d", szFileType,sfi.iIcon);
			//MessageBox(NULL, szMessageStr, TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
			mtx.lock();
			mTypeIcon.insert(map<wstring, INT>::value_type(pGetIconInfo.wsTypeName, sfi.iIcon));
			mtx.unlock();
			SetEvent(pGetIconInfo.hEventGetDone);
		}
		if (!qTaskGetIcon.empty()) {
			SetEvent(hEventGetIcon);
		}
		else {
			ResetEvent(hEventGetIcon);
		}
		bFlag=FALSE;
	}
}