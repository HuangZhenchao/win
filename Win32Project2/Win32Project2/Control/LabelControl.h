#pragma once
#ifndef _LABELCONTROL_H
#define _LABELCONTROL_H
//#include "ControlBase.h"
#include "ScrollPanelControl.h"
#define CC_LABEL 9
//������ʽ����
#define LS_INVARWIDTH		0x00000000			//���ɱ��������Χ����
#define LS_HSCROLL			0x00000001
#define LS_AUTOWRAP			0x00000002
#define LS_EXPWIDTH			0x00000004
#define LS_ADAPWIDTH		0x00000008
#define LS_INVARHEIGHT		0x00000000			//���ɱ��������Χ����
#define LS_VSCROLL			0x00000010
#define LS_EXPHEIGHT		0x00000020
#define LS_ADAPHEIGHT		0x00000040
#define LS_HLEFT			0x00000000			//ˮƽ����
#define LS_HCENTER			0x00000100			//ˮƽ����
#define LS_HRIGHT			0x00000200			//ˮƽ����
#define LS_PARAINDENT 		0x00000400			//��������
#define LS_VUPPER			0x00000000			//��ֱ����
#define LS_VMIDDLE			0x00001000			//��ֱ����
#define LS_VLOWER			0x00002000			//��ֱ����

#define WM_LC WM_USER+10
#define LN_SIZED WM_LC+1
//BOOL bWndRegistered = FALSE;
void LW_RegisterWnd(HINSTANCE);
class ROWINFO {
public:
	wstring wsRowText;
	DWORD dwLength;
	DWORD xRowText;
	DWORD yRowText;
};
class LABELCONTROLINFO {
public:
	
	DWORD dwControlWidth;
	DWORD dwControlHeight;
	BOOL DataReload=TRUE;
	BOOL bLoadDataDone=FALSE;
	//Input
	DWORD dwControlObjectID = 0;
	
	SCROLLPANELCONTROLINFO *scrollPanelControlInfo;
	HWND hWndScroll;
	wstring wsText;
	vector<wstring> sections;
	DWORD dwCS;
	
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

	DWORD yLabelTextStart=0;
	vector<ROWINFO *> RowsInfo;
	wstring GetDebugString();
};



LRESULT CALLBACK LWP_Content(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK LWP_TransMessage(HWND, UINT, WPARAM, LPARAM);

#endif // !_LABELCONTROL_H

