#pragma once
#ifndef _CONTROLBASE_H
#define _CONTROLBASE_H
#include <shellapi.h>
#include <shlobj.h>
#include <OLEIDL.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib") 
#include <commctrl.h>
#pragma comment(lib, "Comctl32.lib") 
#define CC_LABEL 9

#define DF_EXTINFO		0x00000001
#define DF_DRAGGABLE	0x00000002
#define DF_DROPPABLE	0x00000004
#define DRAGDROP_REPATH  0x00000001

class PWNDDATA {
public:	
	void* pCtrlInfo;
	void* pExtInfo;
	IDropTarget *pDropTarget=NULL;
	IDropTargetEvent *pDropTargetEvent = NULL;
};
extern map<DWORD, void*> IdDataBinding;
extern DWORD dwKeyID;
extern vector<DWORD> AlternativeIDs;
extern DWORD GetIDFromPool();
extern DWORD PutIDToPool(DWORD);

extern BOOL bDefWndRegistered;
extern void RegisterDefWnd(HINSTANCE);
extern BOOL bDragStatus;
extern DWORD dwDragWndID;
extern DWORD dwDropWndID;
extern DWORD BindIdData(void *);
extern void CtrlInit();
extern HIMAGELIST hImgSysIconSmall;
#endif // !_CONTROLBASE_H

