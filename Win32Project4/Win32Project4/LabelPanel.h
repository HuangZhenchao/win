#pragma once
class LabelPanel {
public:
	LPWSTR lpszClassName = L"LabelPanelHandler";
	LabelPanel(HINSTANCE);
	UINT GetObjectID();
	static void SetData(UINT, LabelPanel*);
	static LabelPanel* GetDataByObjectID(UINT);



	void RegisterWnd_LabelPanel(HINSTANCE);
private:
	UINT uObjectID;
	static map<UINT, LabelPanel*> m_Object_Data;
};

extern LRESULT CALLBACK  LabelPanelHandlerProc(HWND, UINT, WPARAM, LPARAM);