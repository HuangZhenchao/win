#pragma once
#include <map>
#include <string>
#include <vector>
using namespace std;
#include "sqlite3.h"  
#pragma comment(lib, "sqlite3.lib")

vector<map<string, string>> SelectItem;

vector<map<string, string>> GetSelectItem() 
{

	return SelectItem;
}

int callback_select(void*para, int nCount, char** pValue, char** pName) {
	map<string, string> row;
	for (int i = 0; i<nCount; i++) {
		row.insert(map<string, string>::value_type(pName[i], pValue[i]));
	}
	SelectItem.push_back(row);
	return 0;
}

#define CHECK_RC(rc,szInfo,szErrMsg,db) if(rc!=SQLITE_OK){	printf("%s error!/n", szInfo); printf("%s/n", szErrMsg); sqlite3_free(szErrMsg); sqlite3_close(db); return 0; }

