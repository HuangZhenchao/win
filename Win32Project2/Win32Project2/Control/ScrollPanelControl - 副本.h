#pragma once
#define SS_NOHSCROLL 0x00000001
#define SS_NOVSCROLL 0x00000002
class SCROLLPANELCONTROLINFO {
public:
	DWORD dwCS;
	HINSTANCE hInstance;
	HWND hWndParent;
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

	//DWORD dwSBHeight;
};

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
class SCROLLPANELCONTROL {
private:
	//�ؼ��ڲ���һЩ��ʶ��Ϣ
	static DWORD dwControlType;//�ؼ����ͣ�ÿ�ֿؼ�����һ�����������ͣ��˿ؼ�Ϊ������ͼ�ؼ�
	DWORD dwControlObjectID = 0;//�ؼ�����ID��
	//CSC
	static DWORD dwCSC_1;
	static DWORD dwCSC_2;
	//CDC
	//static DWORD dwCDC_1;
	//DWORD dwCDC_1_FloorID = 0;
	//vector<DWORD> vCDC_1_AlternativeIDs;
	static map<DWORD, SCROLLPANELCONTROL*> m_id_pSPC;
private:
	static BOOL bWndRegistered;
public:
	
	SCROLLPANELCONTROLINFO *ScrollPanelControlInfo;
	SPCSCROLLBARINFO *Hsi;
	SPCSCROLLBARINFO *Vsi;

	SCROLLPANELCONTROL(SCROLLPANELCONTROLINFO *);
	void SPW_RegisterWnd();

	static LRESULT CALLBACK SPWP_Control(HWND, UINT, WPARAM, LPARAM);
	static LRESULT CALLBACK SPWP_ScrollBar(HWND, UINT, WPARAM, LPARAM);
};