#pragma once
#include <string>
#include <windows.h> 
using namespace std;
extern wstring ANSIToUnicode(const string& str);
extern wstring UTF8ToUnicode(const string& str);
extern string UnicodeToANSI(const wstring& str);
extern string UnicodeToUTF8(const wstring& str);

extern wstring DWORD2BitString(DWORD dValue);