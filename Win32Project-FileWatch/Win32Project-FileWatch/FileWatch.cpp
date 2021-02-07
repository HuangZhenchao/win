#include "stdafx.h"
#include "FileWatch.h"
FILEWATCH::FILEWATCH() {
	//setlocale(LC_ALL, "");
	WCHAR szAllUsersProFilePath[MAX_PATH];
	
	GetEnvironmentVariable(L"ALLUSERSPROFILE", szAllUsersProFilePath, MAX_PATH);
	ZeroMemory(szThisProFilePath, sizeof(WCHAR) * MAX_PATH);
	wcscpy_s(szThisProFilePath, MAX_PATH, szAllUsersProFilePath);
	wcscat_s(szThisProFilePath, MAX_PATH, L"\\");
	wcscat_s(szThisProFilePath, MAX_PATH, lpszProgramName);
	CreateDirectory(szThisProFilePath, NULL);
	//初始化事件

	WCHAR szLogicDriverStrings[1024];
	ZeroMemory(szLogicDriverStrings, sizeof(szLogicDriverStrings));
	DWORD DSLength = GetLogicalDriveStrings(1024, szLogicDriverStrings);
	//wprintf(L"目标路径为%ls\n", lpszDriverCopy);
	PWCHAR lpszDriver;
	lpszDriver = (PWCHAR)szLogicDriverStrings;
	do
	{

		WCHAR *lpszDriverCopy = new WCHAR[4];
		wcscpy_s(lpszDriverCopy, 4, lpszDriver);

		DWORD dwVolumeSerialNumber = 0;
		GetVolumeInformation(lpszDriverCopy, NULL, MAX_PATH, &dwVolumeSerialNumber, NULL, NULL, NULL, 0);
		if (0 == dwVolumeSerialNumber) {
			lpszDriver += (lstrlen(lpszDriver) + 1);
			continue;
		}
		vDriver.push_back(lpszDriverCopy);
		lpszDriver += (lstrlen(lpszDriver) + 1);
	} while (*lpszDriver != '\x00');
	mTypeIcon.insert(map<wstring, INT>::value_type(L"Folder", 3));
	mTypeIcon.insert(map<wstring, INT>::value_type(L"File", 0));
	hEventGetIcon = CreateEvent(NULL, TRUE, FALSE, L"EventGetIcon");
	t=thread(&FILEWATCH::tFunGetIcon, this);
}
void FILEWATCH::Start(){
	for (INT iDriverIndex = 0; iDriverIndex < vDriver.size(); iDriverIndex++) {
		DWORD dwVolumeSerialNumber = 0;
		WCHAR *lpszVolumeSerialNumber=new WCHAR[50];
		ZeroMemory(lpszVolumeSerialNumber, sizeof(lpszVolumeSerialNumber));
		GetVolumeInformation(vDriver[iDriverIndex], NULL, MAX_PATH, &dwVolumeSerialNumber, NULL, NULL, NULL, 0);
		_itow_s(dwVolumeSerialNumber, lpszVolumeSerialNumber, 50, 10);

		WCHAR szDBName[30];
		wcscpy_s(szDBName, 30, GetFirstDriverCharW(vDriver[iDriverIndex]));
		wcscat_s(szDBName, 30, lpszVolumeSerialNumber);
		wcscat_s(szDBName, 30, L".db");

		WCHAR *lpszDBPath = new WCHAR[MAX_PATH];
		wcscpy_s(lpszDBPath, MAX_PATH, szThisProFilePath);
		wcscat_s(lpszDBPath, MAX_PATH, L"\\");
		wcscat_s(lpszDBPath, MAX_PATH, szDBName);
		vDBPath.push_back(lpszDBPath);

		mutex *pMtxTask = new mutex;
		vpMtxTask.push_back(pMtxTask);
		mutex *pMtxSubTask = new mutex;
		vpMtxSubTask.push_back(pMtxSubTask);

		vEventTaskDistribute.push_back( CreateEvent(NULL, TRUE, FALSE, GetFirstDriverCharW(vDriver[iDriverIndex])));
		WCHAR szEventNameTask[50];
		wcscpy_s(szEventNameTask, 50, szDBName);
		wcscat_s(szEventNameTask, 50, L"Task");
		vHEventRun.push_back(CreateEvent(NULL, TRUE, FALSE, szEventNameTask));


		HANDLE *pHEventDoneArray = new HANDLE[THREADCOUNT];
		for (INT i = 0; i < THREADCOUNT; i++) {
			WCHAR szEventNameTaskDone[50];
			wcscpy_s(szEventNameTask, 50, szDBName);
			wcscat_s(szEventNameTask, 50, L"TaskDone");
			WCHAR szTemp[2];
			_itow_s(i, szTemp, 2, 10);
			wcscat_s(szEventNameTask, 50, szTemp);
			pHEventDoneArray[i] = CreateEvent(NULL, TRUE, FALSE, szEventNameTask);

		}
		vHEventDoneArray.push_back(pHEventDoneArray);
		//先为每个盘创建任务分发的队列
		queue<TASKINFO *> *pQTaskInfo = new queue<TASKINFO *>();
		queue<WCHAR *> *pQSubTask = new queue<WCHAR *>();
		vQueueTaskInfo.push_back(pQTaskInfo);
		vQueueSubTask.push_back(pQSubTask);

		if (!IsPathExists(lpszDBPath)) {

			CreateTable(lpszDBPath);
			SendMessageA(hWnd[iDriverIndex], EM_SETSEL, -2, -1);
			SendMessage(hWnd[iDriverIndex], EM_REPLACESEL, 0, (LPARAM)L"创建数据库\r\n");
			//WCHAR *lpszTemp = new WCHAR[20];
			//wcscpy_s(lpszTemp, 20, L"D:\\视频3：日韩欧美影片归档\\女优");
			TASKINFO *pTaskInfo = new TASKINFO();
			pTaskInfo->dwTaskType = 1;
			pTaskInfo->lpszFilePath =  GetFileParentPath(vDriver[iDriverIndex]);//lpszTemp;
			pQTaskInfo->push(pTaskInfo);
		}


		vThreadTaskDistribute.push_back(thread(&FILEWATCH::tFunTaskDistribute, this, iDriverIndex));
		for (INT i = 0; i < THREADCOUNT; i++) {
			vThreadTraverseDir.push_back(thread(&FILEWATCH::tFunTraverseDir, this, iDriverIndex, i));
		}

		vThreadWatchDir.push_back(thread(&FILEWATCH::tFunChangedWactch, this,iDriverIndex));
		
		//SendMessage(hWnd[iDriverIndex], EM_REPLACESEL, 0, (LPARAM)vDriver[iDriverIndex]);
		SetEvent(vEventTaskDistribute[iDriverIndex]);

	} 
}
FILEWATCH::~FILEWATCH() {
	bTerminateFlag = TRUE;
	t.join();
	for (int i = 0; i < vThreadTaskDistribute.size(); i++) {
		SetEvent(vEventTaskDistribute[i]);
		for (int y = 0; y < THREADCOUNT;y++) {
			SetEvent(vHEventDoneArray[i][y]);
		}
		SetEvent(vHEventRun[i]);
		vThreadTaskDistribute[i].join();
	}

	for (int i = 0; i < vThreadTraverseDir.size(); i++) {
		vThreadTraverseDir[i].join();
	}

	for (int i = 0; i < vThreadWatchDir.size(); i++) {
		vThreadWatchDir[i].join();
	}
}
void FILEWATCH::tFunTaskDistribute(INT iDriverIndex) {
	//CoInitialize(NULL);
	sqlite3 *pSqliteDB = NULL;
	sqlite3_open_v2(UnicodeToANSI(vDBPath[iDriverIndex]).c_str(), &pSqliteDB, SQLITE_OPEN_READWRITE | SQLITE_OPEN_NOMUTEX, NULL);
	sqlite3_stmt *stmtDeleteFileInfo;
	sqlite3_stmt *stmtDeleteFileTagInfo;
	const char* sqlDeleteFileInfo = "delete from tFileInfo where FilePath=? or (FilePath>? and FilePath<?);";
	const char* sqlDeleteFileTagInfo = "delete from tFileTagInfo where FilePath=? or (FilePath>? and FilePath<?);";

	sqlite3_prepare_v2(pSqliteDB, sqlDeleteFileInfo, strlen(sqlDeleteFileInfo), &stmtDeleteFileInfo, 0);
	sqlite3_prepare_v2(pSqliteDB, sqlDeleteFileTagInfo, strlen(sqlDeleteFileTagInfo), &stmtDeleteFileTagInfo, 0);
	WCHAR *lpszDriver=vDriver[iDriverIndex];		
	TASKINFO *pTaskInfo = NULL;
	WCHAR *lpszFilePath;
	WCHAR szFilePath1[MAX_PATH + 1];
	WCHAR szFilePath2[MAX_PATH + 1];
	while (!bTerminateFlag) {
		WaitForSingleObject(vEventTaskDistribute[iDriverIndex], INFINITE);
		vpMtxTask[iDriverIndex]->lock();
		if (!(vQueueTaskInfo[iDriverIndex]->empty())) {
			pTaskInfo = vQueueTaskInfo[iDriverIndex]->front();
			vQueueTaskInfo[iDriverIndex]->pop();
			//SetEvent(vEvent[iDriverIndex]);
		}
		else {
			//wprintf(L"循环检测盘%ls线程%d\n", vDBPath[iDriverIndex], vEvent[iDriverIndex]);
			ResetEvent(vEventTaskDistribute[iDriverIndex]);
		}
		vpMtxTask[iDriverIndex]->unlock();
		if (pTaskInfo) {
			//wprintf(L"盘序号%d,操作标志%d,目标路径为%ls\r\n", iDriverIndex,pTaskInfo->dwTaskType, pTaskInfo->lpszFilePath);
			switch (pTaskInfo->dwTaskType)
			{
			case 1: 
			{
				//sqlite3_exec(vSQLite[iDriverIndex], "begin immediate;", 0, 0, 0);
				WCHAR szDisplayString[MAX_PATH + 20];
				wsprintf(szDisplayString, L"正在检索路径%ls下的文件...\r\n", pTaskInfo->lpszFilePath);
				if(SendMessage(hWnd[iDriverIndex], EM_GETLINECOUNT, 0, 0)>100){
					SendMessage(hWnd[iDriverIndex], EM_SETSEL, 0, -1);
				}
				else {
					SendMessage(hWnd[iDriverIndex], EM_SETSEL, -2, -1);
				}
				
				SendMessage(hWnd[iDriverIndex], EM_REPLACESEL, 0, (LPARAM)szDisplayString);
				clock_t start, end;
				start = clock();
				
				vpMtxSubTask[iDriverIndex]->lock();
				vQueueSubTask[iDriverIndex]->push(pTaskInfo->lpszFilePath);//(pTaskInfo->lpszFilePath);
				SetEvent(vHEventRun[iDriverIndex]);
				vpMtxSubTask[iDriverIndex]->unlock();

				WaitForMultipleObjects(THREADCOUNT, vHEventDoneArray[iDriverIndex],TRUE, INFINITE);
				end = clock();
				wsprintf(szDisplayString, L"检索完成，耗时%dms\r\n",end-start);
				SendMessage(hWnd[iDriverIndex], EM_SETSEL, -2, -1);
				SendMessage(hWnd[iDriverIndex], EM_REPLACESEL, 0, (LPARAM)szDisplayString);
				//sqlite3_exec(vSQLite[iDriverIndex], "commit;", 0, 0, 0);
			}
			break;
				
			case 2:
			{
				WCHAR szDisplayString[MAX_PATH + 20];
				wsprintf(szDisplayString, L"移除路径%ls相关信息...\r\n", pTaskInfo->lpszFilePath);
				if (SendMessage(hWnd[iDriverIndex], EM_GETLINECOUNT, 0, 0)>100) {
					SendMessage(hWnd[iDriverIndex], EM_SETSEL, 0, -1);
				}
				else {
					SendMessage(hWnd[iDriverIndex], EM_SETSEL, -2, -1);
				}
				SendMessage(hWnd[iDriverIndex], EM_REPLACESEL, 0, (LPARAM)szDisplayString);

				lpszFilePath = pTaskInfo->lpszFilePath;
				ZeroMemory(&szFilePath1,sizeof(szFilePath1));
				ZeroMemory(&szFilePath2,sizeof(szFilePath2));
				wcscpy_s(szFilePath1, MAX_PATH + 1, lpszFilePath);
				wcscpy_s(szFilePath2, MAX_PATH + 1, lpszFilePath);
				wcscat_s(szFilePath1, MAX_PATH + 1, L"\\");
				wcscat_s(szFilePath2, MAX_PATH + 1, L"]");
				//sqlite3_exec(db, "begin immediate;", 0, 0, 0);
				while (SQLITE_BUSY == sqlite3_exec(pSqliteDB, "begin immediate;", 0, 0, 0)) {
					wprintf(L"%ls\n", L"DBDelete线程begin immediate操作返回值为SQLITE_BUSY;");
					Sleep(100);
				}
				sqlite3_reset(stmtDeleteFileInfo);
				sqlite3_bind_text16(stmtDeleteFileInfo, 1, lpszFilePath, -1, 0);
				sqlite3_bind_text16(stmtDeleteFileInfo, 2, szFilePath1, -1, 0);
				sqlite3_bind_text16(stmtDeleteFileInfo, 3, szFilePath2, -1, 0);
				sqlite3_step(stmtDeleteFileInfo);

				sqlite3_reset(stmtDeleteFileTagInfo);
				sqlite3_bind_text16(stmtDeleteFileTagInfo, 1, lpszFilePath, -1, 0);
				sqlite3_bind_text16(stmtDeleteFileTagInfo, 2, szFilePath1, -1, 0);
				sqlite3_bind_text16(stmtDeleteFileTagInfo, 3, szFilePath2, -1, 0);
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

		pTaskInfo = NULL;
		/*
		
		*/
	}
}
void FILEWATCH::CreateTable(WCHAR *lpszDBPath) {
	sqlite3 *db = NULL;
	printf("数据库路径%s\n", UnicodeToANSI(lpszDBPath).c_str());
	sqlite3_open_v2(UnicodeToANSI(lpszDBPath).c_str(), &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_NOMUTEX | SQLITE_OPEN_CREATE, NULL);
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
		GetFileIconDone int not null default 0);";
	const char *sqlCreateTableTagFile = "Create table if not exists tFileTagInfo(FilePath text not null,ParentPath text not null,Tag text not null);";
	//sqlite3_exec(db, sqlCreateTableDriverInfo, 0, 0, 0);
	sqlite3_exec(db, sqlCreateTableFileInfo, 0, 0, 0);
	sqlite3_exec(db, sqlCreateTableTagFile, 0, 0, 0);
	sqlite3_exec(db, "CREATE INDEX indexParentPath ON tFileInfo(ParentPath);", 0, 0, 0);
	sqlite3_exec(db, "CREATE INDEX indexFilePath ON tFileInfo(FilePath);", 0, 0, 0);
	sqlite3_exec(db, "PRAGMA journal_mode=WAL;", 0, 0, 0);
	//sqlite3_exec(db, "PRAGMA journal_mode=OFF;", 0, 0, 0);
	sqlite3_close_v2(db);
}

void FILEWATCH::tFunTraverseDir(INT iDriverIndex,INT iThreadID) {
	sqlite3 *pSqliteDB = NULL;
	//sqlite3 *pSqliteDB = vSQLite[iDriverIndex];
	const char* sqlInsertFileInfo = "insert into tFileInfo values(?,?,?,?,?,?,?,?,?,?,?,?);";
	const char* sqlInsertFileTagInfo = "insert into tFileTagInfo values(?,?,?);";
	sqlite3_stmt *stmtInsertFileInfo;
	sqlite3_stmt *stmtInsertFileTagInfo;

	sqlite3_open_v2(UnicodeToANSI(vDBPath[iDriverIndex]).c_str(), &pSqliteDB, SQLITE_OPEN_READWRITE | SQLITE_OPEN_NOMUTEX, NULL);
	sqlite3_prepare_v2(pSqliteDB, sqlInsertFileInfo, strlen(sqlInsertFileInfo), &stmtInsertFileInfo, 0);
	sqlite3_prepare_v2(pSqliteDB, sqlInsertFileTagInfo, strlen(sqlInsertFileTagInfo), &stmtInsertFileTagInfo, 0);

	WCHAR *lpszDir = NULL;
	WCHAR szFindStr[MAX_PATH];
	//BOOL bTaskDone=FALSE;
	while (!bTerminateFlag) {
		WaitForSingleObject(vHEventRun[iDriverIndex], INFINITE);
		
		vpMtxSubTask[iDriverIndex]->lock();
		if (!(vQueueSubTask[iDriverIndex]->empty())) {

			lpszDir = vQueueSubTask[iDriverIndex]->front();
			//wprintf(L"步骤检测%ls\n", lpszDir);
			vQueueSubTask[iDriverIndex]->pop();
			
		}
		
		vpMtxSubTask[iDriverIndex]->unlock();

		if (lpszDir) {
			ZeroMemory(szFindStr, sizeof(WCHAR) * MAX_PATH);
			wcscpy_s(szFindStr, MAX_PATH, lpszDir);
			wcscat_s(szFindStr, MAX_PATH, L"\\*");

			WIN32_FIND_DATA FindFileData;


			wprintf(L"线程%d:查找路径下文件%ls\n", iThreadID, szFindStr);
			
			while (SQLITE_BUSY == sqlite3_exec(pSqliteDB, "begin immediate;", 0, 0, 0)) {
				//wprintf(L"线程%d:%ls\n", iThreadID, L"DBInsert线程begin immediate操作返回值为SQLITE_BUSY;");
				Sleep(5);
			}
			
			if (!(INVALID_HANDLE_VALUE == FindFirstFile(lpszDir, &FindFileData))) {
				WCHAR *lpszParentDir = GetFileParentPath(lpszDir);
				FunDBInsert(pSqliteDB, stmtInsertFileInfo, stmtInsertFileTagInfo, lpszDir, lpszParentDir, &FindFileData);
				delete lpszParentDir;
			}
			//wprintf(L"线程%d:FindFirst文件%ls\n", iThreadID, lpszDir);
			HANDLE hListFile = FindFirstFile(szFindStr, &FindFileData);
			// 判断句柄 
			if (!(hListFile == INVALID_HANDLE_VALUE))
			{
				do {

					if (wcscmp(FindFileData.cFileName, L".") == 0 | wcscmp(FindFileData.cFileName, L"..") == 0) {
						continue;
					}
					WCHAR lpszFilePath[MAX_PATH];
					ZeroMemory(lpszFilePath, sizeof(WCHAR) * MAX_PATH);
					wcscpy_s(lpszFilePath, MAX_PATH, lpszDir);
					wcscat_s(lpszFilePath, MAX_PATH, L"\\");
					wcscat_s(lpszFilePath, MAX_PATH, FindFileData.cFileName);

					if (FindFileData.dwFileAttributes& FILE_ATTRIBUTE_DIRECTORY) {
						WCHAR *szFolderPath = new WCHAR[MAX_PATH];
						ZeroMemory(szFolderPath, sizeof(WCHAR) * MAX_PATH);
						wcscpy_s(szFolderPath, MAX_PATH, lpszFilePath);
						//wprintf(L"文件夹%ls\n", szFolderPath);
						vpMtxSubTask[iDriverIndex]->lock();
						vQueueSubTask[iDriverIndex]->push(szFolderPath);
						//SetEvent(vHEventRun[iDriverIndex]);
						vpMtxSubTask[iDriverIndex]->unlock();
						
					}
					else {
						FunDBInsert(pSqliteDB, stmtInsertFileInfo, stmtInsertFileTagInfo, lpszFilePath, lpszDir, &FindFileData);
					}
					//lpFindFileData = new WIN32_FIND_DATA;
					//delete lpszFilePath;
				} while (FindNextFile(hListFile, &FindFileData));
				
			}
			
			while (SQLITE_BUSY == sqlite3_exec(pSqliteDB, "commit;", 0, 0, 0)) {
				//wprintf(L"线程%d:%ls\n", iThreadID, L"DBInsert线程commit操作返回值为SQLITE_BUSY;");
				Sleep(5);
			}
			
			delete lpszDir;
		}
		lpszDir = NULL;
		if (!(vQueueSubTask[iDriverIndex]->empty())) {
			SetEvent(vHEventRun[iDriverIndex]);
			ResetEvent(vHEventDoneArray[iDriverIndex][iThreadID]);
		}
		else {
			//wprintf(L"循环检测盘%ls线程%d\n", vDBPath[iDriverIndex], vEvent[iDriverIndex]);

			SetEvent(vHEventDoneArray[iDriverIndex][iThreadID]);
			ResetEvent(vHEventRun[iDriverIndex]);
		}
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
void FILEWATCH::FunDBInsert(sqlite3 *pSqliteDB, sqlite3_stmt *stmtInsertFileInfo, sqlite3_stmt *stmtInsertFileTagInfo, WCHAR *lpszFilePath, WCHAR *lpszFileParentPath, WIN32_FIND_DATA *lpFindFileData) {
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
		GETICONINFO *pGetIconInfo = new GETICONINFO();
		pGetIconInfo->wsTypeName = wsTypeName;
		pGetIconInfo->hEventGetDone = CreateEvent(NULL, TRUE, FALSE, szTypeName);
		mtx.lock();
		qTaskGetIcon.push(pGetIconInfo);
		SetEvent(hEventGetIcon);
		mtx.unlock();
		WaitForSingleObject(pGetIconInfo->hEventGetDone, INFINITE);
		CloseHandle(pGetIconInfo->hEventGetDone);
		//delete pGetIconInfo;
	}
		
	//绑定文件信息表插入语句数据
	sqlite3_reset(stmtInsertFileInfo);

	//wprintf(L"%ls\n", pszFilePath);
	sqlite3_bind_text16(stmtInsertFileInfo, 1, lpszFilePath, -1, 0);//数据库中的FilePath列
	sqlite3_bind_text16(stmtInsertFileInfo, 2, lpszFileParentPath, -1, 0);//数据库中的ParentPath列


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
	sqlite3_bind_int(stmtInsertFileInfo, 12, 0);//数据库中的GetFileIconDone列
	sqlite3_step(stmtInsertFileInfo);
	//vector<wstring> vTag = ExtractTagFromFileName(FindFileData.cFileName);
	for (int i = 0; i < pvTag->size(); i++) {
		//绑定文件标签信息表插入语句数据
		sqlite3_reset(stmtInsertFileTagInfo);
		sqlite3_bind_text16(stmtInsertFileTagInfo, 1, lpszFilePath, -1, 0);//数据库中的FilePath列
		sqlite3_bind_text16(stmtInsertFileTagInfo, 2, lpszFileParentPath, -1, 0);//数据库中的FilePath列
		sqlite3_bind_text16(stmtInsertFileTagInfo, 3, (*pvTag)[i].c_str(), -1, 0);//数据库中的FilePath列
		sqlite3_step(stmtInsertFileTagInfo);
	}

	delete pvTag;
}
//监控路径的线程

void FILEWATCH::tFunChangedWactch(INT iDriverIndex) {
	
	WCHAR *lpszDir = vDriver[iDriverIndex];
	HANDLE hDir = CreateFile(lpszDir,
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
	SendMessage(hWnd[iDriverIndex], EM_SETSEL, -2, -1);
	SendMessage(hWnd[iDriverIndex], EM_REPLACESEL, 0, (LPARAM)szDisplayString);
	DWORD BytesReturned = 0;
	WCHAR szBuffer[1024];
	//memset(szBuffer, 0, sizeof(szBuffer));
	FILE_NOTIFY_INFORMATION *pnotify;
	//FILE_NOTIFY_INFORMATION *pnotify = (FILE_NOTIFY_INFORMATION *)notify;

	WCHAR szFilePath[MAX_PATH];
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
				ZeroMemory(szFilePath, MAX_PATH * sizeof(WCHAR));
				wcscpy_s(szFilePath, MAX_PATH, lpszDir);
				wcscat_s(szFilePath, MAX_PATH, szFileName);

				if (IsStrFinded(szFilePath,L".db-journal")) {
					continue;
				}
				switch (pnotify->Action)
				{
				case FILE_ACTION_REMOVED:
				{
					wprintf(L"%ls%ls\n", szFilePath, L" FILE REMOVED");
					WCHAR *lpszFilePath = new WCHAR[MAX_PATH];
					wcscpy_s(lpszFilePath, MAX_PATH, szFilePath);
					//FunDBDelete(pSqliteDB, stmtDeleteFileInfo, stmtDeleteFileTagInfo, lpszFilePath);
					
					TASKINFO *pTaskInfo = new TASKINFO();
					pTaskInfo->dwTaskType = 2;
					pTaskInfo->lpszFilePath = lpszFilePath;
					vpMtxTask[iDriverIndex]->lock();
					vQueueTaskInfo[iDriverIndex]->push(pTaskInfo);
					vpMtxTask[iDriverIndex]->unlock();
					SetEvent(vEventTaskDistribute[iDriverIndex]);
				}
				break;
				case FILE_ACTION_ADDED:
				{
					wprintf(L"%ls%ls\n", szFilePath, L" FILE ADDED");
					WCHAR *lpszFilePath = new WCHAR[MAX_PATH];
					ZeroMemory(lpszFilePath, MAX_PATH * sizeof(WCHAR));
					wcscpy_s(lpszFilePath, MAX_PATH, szFilePath);

					TASKINFO *pTaskInfo = new TASKINFO();
					pTaskInfo->dwTaskType = 1;
					pTaskInfo->lpszFilePath = lpszFilePath;
					vpMtxTask[iDriverIndex]->lock();
					vQueueTaskInfo[iDriverIndex]->push(pTaskInfo);
					vpMtxTask[iDriverIndex]->unlock();
					SetEvent(vEventTaskDistribute[iDriverIndex]);

				}
				break;

				case FILE_ACTION_MODIFIED:
				{
					wprintf(L"%ls%ls\n", szFilePath, L" FILE MODIFIED");
					WCHAR *lpszFilePath = new WCHAR[MAX_PATH];
					ZeroMemory(lpszFilePath, MAX_PATH * sizeof(WCHAR));
					wcscpy_s(lpszFilePath, MAX_PATH, szFilePath);

					TASKINFO *pTaskInfo = new TASKINFO();
					pTaskInfo->dwTaskType = 2;
					pTaskInfo->lpszFilePath = lpszFilePath;

					TASKINFO *pTaskInfo1 = new TASKINFO();
					pTaskInfo1->dwTaskType = 1;
					pTaskInfo1->lpszFilePath = lpszFilePath;

					vpMtxTask[iDriverIndex]->lock();
					vQueueTaskInfo[iDriverIndex]->push(pTaskInfo);
					vQueueTaskInfo[iDriverIndex]->push(pTaskInfo1);
					vpMtxTask[iDriverIndex]->unlock();
					SetEvent(vEventTaskDistribute[iDriverIndex]);
				}
				break;
				case FILE_ACTION_RENAMED_OLD_NAME:
				{
					//wcscpy_s(szRenameOldname, MAX_PATH, szFilePath);
					WCHAR *lpszFilePath = new WCHAR[MAX_PATH];
					wcscpy_s(lpszFilePath, MAX_PATH, szFilePath);

					TASKINFO *pTaskInfo = new TASKINFO();
					pTaskInfo->dwTaskType = 2;
					pTaskInfo->lpszFilePath = lpszFilePath;
					vpMtxTask[iDriverIndex]->lock();
					vQueueTaskInfo[iDriverIndex]->push(pTaskInfo);
					vpMtxTask[iDriverIndex]->unlock();
					SetEvent(vEventTaskDistribute[iDriverIndex]);
				}
				break;
				case FILE_ACTION_RENAMED_NEW_NAME:
				{
					WCHAR *lpszFilePath = new WCHAR[MAX_PATH];
					ZeroMemory(lpszFilePath, MAX_PATH * sizeof(WCHAR));
					wcscpy_s(lpszFilePath, MAX_PATH, szFilePath);

					TASKINFO *pTaskInfo = new TASKINFO();
					pTaskInfo->dwTaskType = 1;
					pTaskInfo->lpszFilePath = lpszFilePath;
					vpMtxTask[iDriverIndex]->lock();
					vQueueTaskInfo[iDriverIndex]->push(pTaskInfo);
					vpMtxTask[iDriverIndex]->unlock();
					SetEvent(vEventTaskDistribute[iDriverIndex]);

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
	GETICONINFO *pGetIconInfo = NULL;
	while (!bTerminateFlag) {
		WaitForSingleObject(hEventGetIcon, INFINITE);
		mtx.lock();
		if (!qTaskGetIcon.empty()) {
			pGetIconInfo = qTaskGetIcon.front();
			qTaskGetIcon.pop();
		}
		mtx.unlock();
		if (pGetIconInfo) {
			WCHAR szFileType[MAX_PATH];
			ZeroMemory(szFileType,sizeof(szFileType));
			wcscpy_s(szFileType, MAX_PATH,L"*.");
			wcscat_s(szFileType, MAX_PATH, pGetIconInfo->wsTypeName.c_str());

			SHFILEINFO sfi;
			ZeroMemory(&sfi, sizeof(SHFILEINFO));
			//WCHAR *lpszPath = L"E:\\RECYCLED";
			SHGetFileInfo(szFileType, FILE_ATTRIBUTE_NORMAL, &sfi, sizeof(sfi), SHGFI_SYSICONINDEX|SHGFI_USEFILEATTRIBUTES);
			mTypeIcon.insert(map<wstring, INT>::value_type(pGetIconInfo->wsTypeName, sfi.iIcon));
			SetEvent(pGetIconInfo->hEventGetDone);
		}
		if (!qTaskGetIcon.empty()) {
			SetEvent(hEventGetIcon);
		}
		else {
			ResetEvent(hEventGetIcon);
		}
		pGetIconInfo = NULL;
	}
}