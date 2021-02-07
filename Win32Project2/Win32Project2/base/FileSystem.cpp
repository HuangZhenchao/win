#include "stdafx.h"
#include "base\FileSystem.h"
vector<WIN32_FIND_DATA> FileSystem::FindFile(FINDFILEPARAM *findFileParam)
{
	vector<WIN32_FIND_DATA> returnVector;
	WIN32_FIND_DATA FindFileData;

	//TODO:�������޸�DLL�ļ��У���������exe·����
	// ע�͵Ĵ���������ڲ��������ԡ�.txt����β���ļ� 
	wstring tempStr1;
	tempStr1 = findFileParam->szFileDirectory;
	tempStr1.append(L"\\*");
	//lstrcat(szFileDirectory, L"\\*.txt");
	//lstrcat(szFileDirectory, L"");

	// ���ҵ�һ���ļ�/Ŀ¼����ò��Ҿ�� 
	HANDLE hListFile = FindFirstFile(tempStr1.c_str(), &FindFileData);
	// �жϾ�� 
	if (hListFile == INVALID_HANDLE_VALUE)
	{
		//�˴�Ӧ��д����־
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
			//MessageBox(NULL, TEXT("������"), FindFileData.cFileName, MB_OK | MB_ICONINFORMATION);

		} while (FindNextFile(hListFile, &FindFileData));
	}
	return returnVector;
}

void FileSystem::FindFileRecursion(FINDFILEPARAM *findFileParam, lpCallbackFindFileRecursion lpcbFindDataHandler)
{
	WIN32_FIND_DATA FindFileData;

	//TODO:�������޸�DLL�ļ��У���������exe·����
	// ע�͵Ĵ���������ڲ��������ԡ�.txt����β���ļ� 
	wstring tempStr1;
	tempStr1 = findFileParam->szFileDirectory;
	tempStr1.append(L"\\*");
	//lstrcat(szFileDirectory, L"\\*.txt");
	//lstrcat(szFileDirectory, L"");

	// ���ҵ�һ���ļ�/Ŀ¼����ò��Ҿ�� 
	HANDLE hListFile = FindFirstFile(tempStr1.c_str(), &FindFileData);
	// �жϾ�� 
	if (hListFile == INVALID_HANDLE_VALUE)
	{
		//�˴�Ӧ��д����־
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
			//MessageBox(NULL, TEXT("������"), FindFileData.cFileName, MB_OK | MB_ICONINFORMATION);

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
		//MessageBox(NULL, szDrive, TEXT("������"), MB_OK | MB_ICONINFORMATION);

		UINT uDriverType;
		DWORD dwVolumeSerialNumber;
		DWORD dwMaximumComponentlength;
		DWORD dwFileSystemFlags;
		WCHAR szFileSystemNameBuffer[BUFSIZE];
		WCHAR szDriverName[MAX_PATH];

		uDriverType = GetDriveType(szDriver);
		map<DWORD, wstring> DriverType;
		DriverType[0] = L"δ֪�豸";
		DriverType[1] = L"��·����Ч������δ����";
		DriverType[2] = L"���ƶ�����";
		DriverType[3] = L"Ӳ��";
		DriverType[4] = L"�������";
		DriverType[5] = L"����";
		DriverType[6] = L"RAM";
		//MessageBox(NULL, DriverType[uDriverType].c_str(), TEXT("������"), MB_OK | MB_ICONINFORMATION);
		GetVolumeInformation(
			szDriver,
			szDriverName,
			MAX_PATH,
			&dwVolumeSerialNumber,		//�����к�
			&dwMaximumComponentlength,
			&dwFileSystemFlags,
			szFileSystemNameBuffer,
			BUFSIZE);

		if (0 == lstrlen(szDriverName))
		{
			wcscpy(szDriverName, L"���ش���");
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