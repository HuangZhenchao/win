#pragma once
#ifndef FILEWATCH_H
#define FILEWATCH_H
#include "sqlite3.h"  
#pragma comment(lib, "sqlite3.lib")
#include <string>
#include <thread>
#include <mutex>
#include <vector>
#include <map>

using namespace std;
class DriverInfo {
public:
	PWCHAR lpszDriver;
	DWORD dwVolumeSerialNumber;
};
class TraverseFileInfo{
public:
	WCHAR lpszFilePath[MAX_PATH];
	WCHAR lpszFileParentPath[MAX_PATH];
	//WIN32_FIND_DATA *lpFindFileData=NULL;
	TraverseFileInfo() {
		ZeroMemory(lpszFilePath, sizeof(WCHAR) * MAX_PATH);
		ZeroMemory(lpszFileParentPath, sizeof(WCHAR) * MAX_PATH);
	}
};
class TASKINFO {
public:
	DWORD dwTaskType;
	WCHAR *lpszFilePath;
};
class GETICONINFO {
public:
	wstring wsTypeName;
	HANDLE hEventGetDone;
};
#define THREADCOUNT 6
#define MAX_PATH 1024
class FILEWATCH {
public:
	//
	HWND *hWnd;
	WCHAR szThisProFilePath[MAX_PATH];
	void tFunTaskDistribute(INT iDriverIndex);
	void Start();
	vector<WCHAR *> vDriver;
	vector<WCHAR *> vDBPath;
	vector<mutex *> vpMtxTask;
	vector<mutex*> vpMtxSubTask;
	vector<HANDLE> vHEventRun;
	vector<HANDLE *> vHEventDoneArray;
	vector<queue<TASKINFO *> *> vQueueTaskInfo;
	vector<queue<WCHAR *> *> vQueueSubTask;

	vector<HANDLE> vEventTaskDistribute;

	
	vector<thread> vThreadTaskDistribute;
	vector<thread> vThreadTraverseDir;
	vector<thread> vThreadWatchDir;
		
public:
	WCHAR *lpszProgramName = L"YiYuan";
	//char *dbName = "E:\\db\\EnumAllFile.db";
	FILEWATCH();
	~FILEWATCH();

private:	
	//vector<> vAllDriver;
	void CreateTable(WCHAR *lpszDBPath);

	BOOL bAllTaskDone = FALSE;
	BOOL bTerminateFlag = FALSE;

	void tFunTraverseDir(INT iDriverIndex, INT iThreadID);
	//void tFunDBInsert();
	void FunDBInsert(sqlite3 *pSqliteDB, sqlite3_stmt *stmtInsertFileInfo, sqlite3_stmt *stmtInsertFileTagInfo, WCHAR *lpszFilePath, WCHAR *lpszFileParentPath, WIN32_FIND_DATA *lpFindFileData);

	void tFunChangedWactch(INT iDriverIndex);
	void tFunGetFileIcon();
	
	map<wstring, INT> mTypeIcon;
	mutex mtx;
	HANDLE hEventGetIcon;
	void tFunGetIcon();
	thread t;
	queue<GETICONINFO*> qTaskGetIcon;
};


#endif // !FILEWATCH_H

