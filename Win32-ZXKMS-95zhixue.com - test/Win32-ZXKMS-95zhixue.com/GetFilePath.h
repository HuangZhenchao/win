#pragma once
/*
**功能：返回一个路径下的文件信息
**参数：WCHAR
**返回值：vector<WIN32_FIND_DATA>
*/

vector<WIN32_FIND_DATA> getFileDirectory(wstring szFileDirectory)
{
	//FreeLibrary(hDll);
	vector<WIN32_FIND_DATA> returnFindFileData;
	WIN32_FIND_DATA FindFileData;
	
	//TODO:在这里修改DLL文件夹，现在是在exe路径下
	// 注释的代码可以用于查找所有以“.txt”结尾的文件 
	//lstrcat(szFileDirectory, L"\\*");
	//lstrcat(szFileDirectory, L"\\*.txt");
	//lstrcat(szFileDirectory, L"");
	
	// 查找第一个文件/目录，获得查找句柄 
	HANDLE hListFile = FindFirstFile(szFileDirectory.c_str(), &FindFileData);
	// 判断句柄 
	if (hListFile == INVALID_HANDLE_VALUE)
	{
		
	}
	else
	{
		do {
			returnFindFileData.push_back(FindFileData);
			
			//MessageBox(NULL, TEXT("错误码"), FindFileData.cFileName, MB_OK | MB_ICONINFORMATION);

		} while (FindNextFile(hListFile, &FindFileData));
	}
	return returnFindFileData;
}

vector<wstring> EnumerateFolder(wstring szFileDirectory)
{
	vector<wstring> returnVector;
	WIN32_FIND_DATA FindFileData;

	//TODO:在这里修改DLL文件夹，现在是在exe路径下
	// 注释的代码可以用于查找所有以“.txt”结尾的文件 
	//lstrcat(szFileDirectory, L"\\*");
	//lstrcat(szFileDirectory, L"\\*.txt");
	//lstrcat(szFileDirectory, L"");

	// 查找第一个文件/目录，获得查找句柄 
	HANDLE hListFile = FindFirstFile(szFileDirectory.c_str(), &FindFileData);
	// 判断句柄 
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
			//MessageBox(NULL, TEXT("错误码"), FindFileData.cFileName, MB_OK | MB_ICONINFORMATION);

		} while (FindNextFile(hListFile, &FindFileData));
	}
}