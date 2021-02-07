#include "stdafx.h"
#include "base\FileSystem.h"
vector<WIN32_FIND_DATA> FileSystem::FindFile(FINDFILEPARAM *findFileParam)
{
	vector<WIN32_FIND_DATA> returnVector;
	WIN32_FIND_DATA FindFileData;

	//TODO:在这里修改DLL文件夹，现在是在exe路径下
	// 注释的代码可以用于查找所有以“.txt”结尾的文件 
	wstring tempStr1;
	tempStr1 = findFileParam->szFileDirectory;
	tempStr1.append(L"\\*");
	//lstrcat(szFileDirectory, L"\\*.txt");
	//lstrcat(szFileDirectory, L"");

	// 查找第一个文件/目录，获得查找句柄 
	HANDLE hListFile = FindFirstFile(tempStr1.c_str(), &FindFileData);
	// 判断句柄 
	if (hListFile == INVALID_HANDLE_VALUE)
	{
		//此处应当写入日志
	}
	else
	{
		do {

			if (wcscmp(FindFileData.cFileName, L".") == 0 | wcscmp(FindFileData.cFileName, L"..") == 0) {
				continue;
			}
			if (FindFileData.dwFileAttributes& FILE_ATTRIBUTE_DIRECTORY)
			{
				if (findFileParam->dwFileFloderFlag & 0x00000010) {
					returnVector.push_back(FindFileData);
				}
			}
			else
			{
				if (findFileParam->dwFileFloderFlag & 0x00000001) {
					returnVector.push_back(FindFileData);
				}
			}
			//MessageBox(NULL, TEXT("错误码"), FindFileData.cFileName, MB_OK | MB_ICONINFORMATION);

		} while (FindNextFile(hListFile, &FindFileData));
	}
	return returnVector;
}

void FileSystem::FindFileRecursion(FINDFILEPARAM *findFileParam, lpCallbackFindFileRecursion lpcbFindDataHandler)
{
	WIN32_FIND_DATA FindFileData;

	//TODO:在这里修改DLL文件夹，现在是在exe路径下
	// 注释的代码可以用于查找所有以“.txt”结尾的文件 
	wstring tempStr1;
	tempStr1 = findFileParam->szFileDirectory;
	tempStr1.append(L"\\*");
	//lstrcat(szFileDirectory, L"\\*.txt");
	//lstrcat(szFileDirectory, L"");

	// 查找第一个文件/目录，获得查找句柄 
	HANDLE hListFile = FindFirstFile(tempStr1.c_str(), &FindFileData);
	// 判断句柄 
	if (hListFile == INVALID_HANDLE_VALUE)
	{
		//此处应当写入日志
	}
	else
	{
		do {
			if (wcscmp(FindFileData.cFileName, L".") == 0 | wcscmp(FindFileData.cFileName, L"..") == 0) {
				continue;
			}
			if (FindFileData.dwFileAttributes& FILE_ATTRIBUTE_DIRECTORY)
			{
				if (findFileParam->dwFileFloderFlag & 0x00000010) {
					lpcbFindDataHandler(FindFileData);
					wstring tempStr2;
					tempStr2 = findFileParam->szFileDirectory;
					tempStr2.append(L"\\");
					tempStr2.append(FindFileData.cFileName);
					findFileParam->szFileDirectory = tempStr2;
					FindFileRecursion(findFileParam, lpcbFindDataHandler);
				}
			}
			else
			{
				if (findFileParam->dwFileFloderFlag & 0x00000001) {
					lpcbFindDataHandler(FindFileData);
				}
			}
			//MessageBox(NULL, TEXT("错误码"), FindFileData.cFileName, MB_OK | MB_ICONINFORMATION);

		} while (FindNextFile(hListFile, &FindFileData));
	}
	return;
}

void FileSystem::FindFileTraverseDisk(FINDFILEPARAM *findFileParam, lpCallbackFindFileRecursion lpcbFindDataHandler)
{

	//FileSystem::FindFileRecursion(FindFileParam, fun1);
}

vector<map<wstring, wstring>> FileSystem::GetDriversInfo()
{
	wstring DriverStr;
	WCHAR szLogicDriverStrings[1024];
	DWORD DSLength = GetLogicalDriveStrings(1024, szLogicDriverStrings);
	PWCHAR szDriver;
	szDriver = (PWCHAR)szLogicDriverStrings;


	vector<map<wstring, wstring>> Drivers;


	do
	{
		//MessageBox(NULL, szDrive, TEXT("错误码"), MB_OK | MB_ICONINFORMATION);

		UINT uDriverType;
		DWORD dwVolumeSerialNumber;
		DWORD dwMaximumComponentlength;
		DWORD dwFileSystemFlags;
		WCHAR szFileSystemNameBuffer[BUFSIZE];
		WCHAR szDriverName[MAX_PATH];

		uDriverType = GetDriveType(szDriver);
		map<DWORD, wstring> DriverType;
		DriverType[0] = L"未知设备";
		DriverType[1] = L"根路径无效，磁盘未挂载";
		DriverType[2] = L"可移动磁盘";
		DriverType[3] = L"硬盘";
		DriverType[4] = L"网络磁盘";
		DriverType[5] = L"光驱";
		DriverType[6] = L"RAM";
		//MessageBox(NULL, DriverType[uDriverType].c_str(), TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
		GetVolumeInformation(
			szDriver,
			szDriverName,
			MAX_PATH,
			&dwVolumeSerialNumber,		//卷序列号
			&dwMaximumComponentlength,
			&dwFileSystemFlags,
			szFileSystemNameBuffer,
			BUFSIZE);

		if (0 == lstrlen(szDriverName))
		{
			wcscpy(szDriverName, L"本地磁盘");
		}
		map<wstring, wstring> DriverInfo;
		DriverInfo[L"szDriver"] = szDriver;
		DriverInfo[L"szDriverName"] = szDriverName;
		DriverInfo[L"szDriverType"] = DriverType[uDriverType];
		DriverInfo[L"dwVolumeSerialNumber"] = to_wstring(dwVolumeSerialNumber);
		DriverInfo[L"dwMaximumComponentlength"] = to_wstring(dwMaximumComponentlength);
		DriverInfo[L"dwFileSystemFlags"] = to_wstring(dwFileSystemFlags);
		DriverInfo[L"szFileSystemNameBuffer"] = szFileSystemNameBuffer;
		Drivers.push_back(DriverInfo);

		szDriver += (lstrlen(szDriver) + 1);
	} while (*szDriver != '\x00');


	return Drivers;
}