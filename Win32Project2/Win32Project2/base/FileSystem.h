#pragma once
using namespace std;
#define BUFSIZE 1024
/*
**功能：返回一个路径下的文件信息
**参数：WCHAR
**返回值：vector<WIN32_FIND_DATA>
*/
typedef void(*lpCallbackFindFileRecursion)(WIN32_FIND_DATA);

class FINDFILEPARAM {
public:
	DWORD dwFileFloderFlag = 0x00000011L;
	wstring szFileDirectory;
};

class FileSystem {
public:
	static vector<WIN32_FIND_DATA> FindFile(FINDFILEPARAM *);
	//static void FindFile(DWORD, wstring,vector<wstring>&);
	static void FindFileRecursion(FINDFILEPARAM *,lpCallbackFindFileRecursion);
	static void FindFileTraverseDisk(FINDFILEPARAM *, lpCallbackFindFileRecursion);
	static vector<map<wstring, wstring>> GetDriversInfo();
};

