#include "stdafx.h"
#include "LabelPanel.h"
map<UINT, LabelPanel*> LabelPanel::m_Object_Data;
LabelPanel::LabelPanel(HINSTANCE hInstance) {
	uObjectID = 0x00020000 | (m_Object_Data.size() + 1);
	LabelPanel::SetData(uObjectID, this);
	RegisterWnd_LabelPanel(hInstance);
}
UINT LabelPanel::GetObjectID() {
	return uObjectID;
}
void LabelPanel::SetData(UINT uID, LabelPanel* pListView) {
	m_Object_Data.insert(map<UINT, LabelPanel*>::value_type(uID, pListView));
}
LabelPanel* LabelPanel::GetDataByObjectID(UINT uID) {
	return (LabelPanel*)m_Object_Data[uID];
}
void LabelPanel::RegisterWnd_LabelPanel(HINSTANCE hInstance) {
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = 0;
	wcex.lpfnWndProc = LabelPanelHandlerProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = sizeof(LONG);
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)COLOR_WINDOW;//; CreateSolidBrush(RGB(255, 0, 0));
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = lpszClassName;
	wcex.hIconSm = NULL;

	if (!GetClassInfoEx(hInstance, wcex.lpszClassName, &wcex)) {
		RegisterClassExW(&wcex);
	}
}




LRESULT CALLBACK  LabelPanelHandlerProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message)
	{
	case WM_CREATE:
	{
		
	}
	break;
	default:
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}