#pragma once
/*
**���ܣ�����һ��·���µ��ļ���Ϣ
**������WCHAR
**����ֵ��vector<WIN32_FIND_DATA>
*/

vector<WIN32_FIND_DATA> getFileDirectory(wstring szFileDirectory)
{
	//FreeLibrary(hDll);
	vector<WIN32_FIND_DATA> returnFindFileData;
	WIN32_FIND_DATA FindFileData;
	
	//TODO:�������޸�DLL�ļ��У���������exe·����
	// ע�͵Ĵ���������ڲ��������ԡ�.txt����β���ļ� 
	//lstrcat(szFileDirectory, L"\\*");
	//lstrcat(szFileDirectory, L"\\*.txt");
	//lstrcat(szFileDirectory, L"");
	
	// ���ҵ�һ���ļ�/Ŀ¼����ò��Ҿ�� 
	HANDLE hListFile = FindFirstFile(szFileDirectory.c_str(), &FindFileData);
	// �жϾ�� 
	if (hListFile == INVALID_HANDLE_VALUE)
	{
		
	}
	else
	{
		do {
			returnFindFileData.push_back(FindFileData);
			
			//MessageBox(NULL, TEXT("������"), FindFileData.cFileName, MB_OK | MB_ICONINFORMATION);

		} while (FindNextFile(hListFile, &FindFileData));
	}
	return returnFindFileData;
}

vector<wstring> EnumerateFolder(wstring szFileDirectory)
{
	vector<wstring> returnVector;
	WIN32_FIND_DATA FindFileData;

	//TODO:�������޸�DLL�ļ��У���������exe·����
	// ע�͵Ĵ���������ڲ��������ԡ�.txt����β���ļ� 
	//lstrcat(szFileDirectory, L"\\*");
	//lstrcat(szFileDirectory, L"\\*.txt");
	//lstrcat(szFileDirectory, L"");

	// ���ҵ�һ���ļ�/Ŀ¼����ò��Ҿ�� 
	HANDLE hListFile = FindFirstFile(szFileDirectory.c_str(), &FindFileData);
	// �жϾ�� 
	if (hListFile == INVALID_HANDLE_VALUE)
	{

	}
	else
	{
		do {
			if (FindFileData.dwFileAttributes& FILE_ATTRIBUTE_DIRECTORY) 
			{
				returnVector.push_back(FindFileData.cFileName);
			}
			//MessageBox(NULL, TEXT("������"), FindFileData.cFileName, MB_OK | MB_ICONINFORMATION);

		} while (FindNextFile(hListFile, &FindFileData));
	}
}