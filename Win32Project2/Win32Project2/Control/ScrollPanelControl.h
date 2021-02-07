#pragma once
#include "ControlBase.h"
#define SS_NOHSCROLL 0x00000001
#define SS_NOVSCROLL 0x00000002
class SPCSCROLLBARINFO {
public:
	DWORD dwSBWidth;
	DWORD dwSBAreaLength;
	DWORD dwSBLength;
	DWORD dwSBPos;
	DWORD dwContentPos;
	DWORD dwContentLength;

	DWORD xPointDown;
	DWORD yPointDown;
	DWORD dragFlag;
};
class SCROLLPANELCONTROLINFO {
public:
	DWORD dwCS;
	HWND hWndControl;
	HWND hWndContainer;
	HWND hWndContent=NULL;
	HWND hWndVSBArea;
	HWND hWndHSBArea;
	HWND hWndVSB;
	HWND hWndHSB;

	DWORD cx;
	DWORD cy;
	DWORD dwWidth;
	DWORD dwHeight;

	DWORD dwHSBAreaWidth;
	DWORD dwVSBAreaWidth;
	SPCSCROLLBARINFO *Hsi;
	SPCSCROLLBARINFO *Vsi;
	//DWORD dwSBHeight;
	wstring GetDebugString();
};


void SPW_RegisterWnd(HINSTANCE);
LRESULT CALLBACK SPWP_Control(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK SPWP_ScrollBar(HWND, UINT, WPARAM, LPARAM);