#pragma once
vector<wstring> getProjectPath() {
	wchar_t IniPath_Path[MAX_PATH];
	wcscpy_s(IniPath_Path, szPath);
	wcscat_s(IniPath_Path, L"\\NovelWriter.ini");

	const WCHAR lpKeyName1[20] = L"Path";
	const WCHAR lpKeyName2[20] = L"LastTime";
	const WCHAR lpAppNameStr[20] = L"NovelProject";

	DWORD dCampareNum = 0;
	vector<wstring> lpFilePath;
	for (DWORD dAppNameOrder = 1; dAppNameOrder < 100; dAppNameOrder++) {
		WCHAR lpAppNameOrder[20];
		_itow_s(dAppNameOrder, lpAppNameOrder, 20);

		WCHAR lpAppName[20];
		wcscpy_s(lpAppName, lpAppNameStr);
		wcscat_s(lpAppName, lpAppNameOrder);

		WCHAR lpReturnedString[MAX_PATH];
		WCHAR lpReturnedString_TimeString[20];
		
		if (GetPrivateProfileString(lpAppName, lpKeyName1, _T(""), lpReturnedString, sizeof(lpReturnedString), IniPath_Path) != 0)
		{
			wchar_t tempStr1[10];
			_itow_s(dAppNameOrder, tempStr1, 10);
			//tempStr1 = str.c_str();
			//_itow_s(GetLastError(), tempStr1, 10);
			//MessageBox(NULL, lpReturnedString, TEXT("´íÎóÂë"), MB_OK | MB_ICONINFORMATION);
			GetPrivateProfileString(lpAppName, lpKeyName2, _T(""), lpReturnedString_TimeString, sizeof(lpReturnedString_TimeString), IniPath_Path);
			std::wstring tempStr(lpReturnedString_TimeString);
			DWORD dLastTimeStamp = _wtoi(tempStr.c_str());



			if (dLastTimeStamp > dCampareNum) {
				dCampareNum = dLastTimeStamp;
				dCurrentNovelOrder = dAppNameOrder;
			}
			lpFilePath.push_back(lpReturnedString);
		}
		else {
			break;
		}
	}
	return lpFilePath;
}
