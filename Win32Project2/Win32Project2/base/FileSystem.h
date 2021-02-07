#pragma once
using namespace std;
#define BUFSIZE 1024
/*
**���ܣ�����һ��·���µ��ļ���Ϣ
**������WCHAR
**����ֵ��vector<WIN32_FIND_DATA>
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

