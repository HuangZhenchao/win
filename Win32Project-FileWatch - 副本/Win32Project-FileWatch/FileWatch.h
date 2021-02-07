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
	wstring wsFilePath;
};
class TESTINFO {
public:
	DWORD dwTaskType;
	wstring wsFilePath;
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
	HWND hWndDisplay;
	void tFunTaskDistribute();
	vector<wstring> vDriver;
	
	//vector<thread> vThreadWatchDir;
		
public:
	wstring wsDBName = L"FileInfo1.db";
	wstring wsDBPath;
	//char *dbName = "E:\\db\\EnumAllFile.db";
	FILEWATCH(HWND hWnd, wstring wsPath_ProData);
	~FILEWATCH();
	HANDLE hEventDistribute;
	HANDLE hEventTraverseStart;
	HANDLE hEventTraverseDone[THREADCOUNT];
	queue<TASKINFO> qTaskInfo;
	queue<wstring> qSubTask;
	thread tTaskDistribute;
	vector<thread> vThreadTraverseDir;
	vector<thread> vThreadWatchDir;
	mutex mtxTask;
	mutex mtxSubTask;

	map<wstring, wstring> mDriverInfo;
private:	

	BOOL bAllTaskDone = FALSE;
	BOOL bTerminateFlag = FALSE;

	void tFunTraverseDir(INT iThreadID);
	//void tFunDBInsert();
	void FunDBInsert(sqlite3 *pSqliteDB, sqlite3_stmt *stmtInsertFileInfo, sqlite3_stmt *stmtInsertFileTagInfo, wstring wsFilePath, wstring wsFileParentPath, WIN32_FIND_DATA *lpFindFileData);

	void tFunChangedWactch(INT iDriverIndex);
	void tFunGetFileIcon();
	
	map<wstring, INT> mTypeIcon;
	mutex mtx;
	HANDLE hEventGetIcon;
	void tFunGetIcon();
	thread tGetIcon;
	queue<GETICONINFO> qTaskGetIcon;
};


#endif // !FILEWATCH_H

