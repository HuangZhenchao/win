// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
// Windows 头文件: 
#include <windows.h>

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <shellapi.h>
#include <shlobj.h>
#include <OLEIDL.h>
// TODO: 在此处引用程序需要的其他头文件
#include <vector>
#include <string>
#include <map>
#include <stack>
#include<ctime>

#include "base/Tree.h"
#include "base/FileSystem.h"
#include "base/function.h"
#include "base/SplitString.h"
//包含rapidjson必要头文件,rapidjson文件夹拷贝到工程目录，或者设置include路径，或者加入到工程树  
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