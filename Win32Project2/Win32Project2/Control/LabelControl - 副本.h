#pragma once
#ifndef _LABELCONTROL_H
#define _LABELCONTROL_H
#include "ControlBase.h"
//定义样式属性
#define LS_INVARWIDTH		0x00000000			//不可变宽，超出范围隐藏
#define LS_HSCROLL			0x00000001
#define LS_AUTOWRAP			0x00000002
#define LS_EXPWIDTH			0x00000004
#define LS_ADAPWIDTH		0x00000008
#define LS_INVARHEIGHT		0x00000000			//不可变宽，超出范围隐藏
#define LS_VSCROLL			0x00000010
#define LS_EXPHEIGHT		0x00000020
#define LS_ADAPHEIGHT		0x00000040
#define LS_HLEFT			0x00000000			//水平居左
#define LS_HCENTER			0x00000100			//水平居中
#define LS_HRIGHT			0x00000200			//水平居右
#define LS_PARAINDENT 		0x00000400			//段首缩进
#define LS_VUPPER			0x00000000			//垂直居上
#define LS_VMIDDLE			0x00001000			//垂直居中
#define LS_VLOWER			0x00002000			//垂直居下

#define WM_LC WM_USER+10

map<DWORD, LABELCONTROL*> rele_objectID_pLABELCONTROL;
class LABELCONTROLINFO {
public:
	//Input
	HINSTANCE hInstance;
	HWND hWndParent;

	wstring wsText;
	DWORD dwCS;
	DWORD x;
	DWORD y;
	DWORD dwWidth;
	DWORD dwHeight;
	DWORD dwPaddingLR;
	DWORD dwPaddingTB;

	DWORD dwBorderWidth;
	HBRUSH hBrushBorder;
	HBRUSH hBrushTextColor;
	HBRUSH hBrushBackgroundColor;
	LOGFONT lfText;
	//Output
	HWND hWndControl;
	HWND hWndContent;
	DWORD MaxRowLength;
	DWORD TotalRowsHeight;

	DWORD yLabelTextStart;
};

class ROWINFO {
public:
	wstring wsRowText;
	DWORD xRowText;
	DWORD yRowText;
};
class LABELCONTROL :public CONTROLBASE {
public:
	DWORD dwControlObjectID = 0;//控件对象ID，
	static map<DWORD, LABELCONTROL*> rele_objectID_pObject;
private:
	static BOOL bWndRegistered;
public:
	LABELCONTROLINFO *LabelControlInfo;
	HWND hWndTransponder;
	BOOL bLoadDataDone;
	LABELCONTROL(LABELCONTROLINFO *);
	vector<ROWINFO *> RowsInfo;
	
	void Init1();
	void LW_RegisterWnd();
	static LRESULT CALLBACK LWP_Control(HWND, UINT, WPARAM, LPARAM);
	static LRESULT CALLBACK LWP_TransMessage(HWND, UINT, WPARAM, LPARAM);
};


#endif // !_LABELCONTROL_H

