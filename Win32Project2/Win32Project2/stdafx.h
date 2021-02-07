// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�: 
#include <windows.h>

// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <shellapi.h>
#include <shlobj.h>
#include <OLEIDL.h>
// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include <vector>
#include <string>
#include <map>
#include <stack>
#include<ctime>

#include "base/Tree.h"
#include "base/FileSystem.h"
#include "base/function.h"
#include "base/SplitString.h"
//����rapidjson��Ҫͷ�ļ�,rapidjson�ļ��п���������Ŀ¼����������include·�������߼��뵽������  
#include "rapidjson/document.h"  
#include "rapidjson/filereadstream.h" 
#include "rapidjson/filewritestream.h"   
#include "rapidjson/encodedstream.h"    // AutoUTFInputStream
#include "rapidjson/prettywriter.h"  
#include "rapidjson/stringbuffer.h" 
#include "rapidjson/encodings.h"

//#pragma comment(lib,"shell32.lib")
#include "base\Transcoder.h"
#include "OLE\OLEDragDrop.h"