#include "stdafx.h"
//#include "global.h"

#include "LabelControl.h"
#include "ScrollPanelControl.h"
BOOL LABELCONTROL::bWndRegistered=FALSE;
map<DWORD, LABELCONTROL*> LABELCONTROL::rele_objectID_pObject;
void LABELCONTROL::LW_RegisterWnd() {
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = 0;
	wcex.lpfnWndProc = LWP_Control;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = sizeof(LONG);
	wcex.hInstance = LabelControlInfo->hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(nullptr, IDC_HAND);
	//wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.hbrBackground = CreateSolidBrush(RGB(255, 0, 0));
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"MyLabelControl";
	wcex.hIconSm = NULL;

	if (!GetClassInfoEx(LabelControlInfo->hInstance, wcex.lpszClassName, &wcex)) {
		RegisterClassExW(&wcex);
	}
	wcex.lpfnWndProc = LWP_TransMessage;
	wcex.hbrBackground = CreateSolidBrush(RGB(232, 232, 236));
	//����ص�������ǰ������
	wcex.lpszClassName = L"ControlTransMessage";
	if (!GetClassInfoEx(LabelControlInfo->hInstance, wcex.lpszClassName, &wcex)) {
		RegisterClassExW(&wcex);
	}
	bWndRegistered = TRUE;
}

LABELCONTROL::LABELCONTROL(LABELCONTROLINFO *labelControlInfo):CONTROLBASE(labelControlInfo->hInstance){
	dwControlObjectID = rele_objectID_pObject.size() + 1;
	rele_objectID_pObject.insert(map<DWORD, LABELCONTROL*>::value_type(dwControlObjectID, this));

	LabelControlInfo = labelControlInfo;

	if (!bWndRegistered) { LW_RegisterWnd(); }
	hWndTransponder=CreateWindowEx(NULL, L"ControlTransMessage", NULL, WS_CHILD | WS_VISIBLE,
				0, 0, 0, 0, LabelControlInfo->hWndParent, NULL, LabelControlInfo->hInstance, NULL);

	Init1();
}
void LABELCONTROL::Init1() {
	//һЩ�����Ķ��壬����
	vector<ROWINFO *>().swap(RowsInfo);
	LabelControlInfo->yLabelTextStart = 0;
	DWORD ScrollFlag = SS_NOHSCROLL | SS_NOVSCROLL;
	BOOL bAddLengthFlag = FALSE;
	SCROLLPANELCONTROL *scrollPanelControl;
	//�����ж��Ƿ���Ҫʹ�ù������ؼ����ֱ���д��ڵĴ���
	//��Ҫ������������һ��SCROLLPANELCONTROL�࣬
	if ((LabelControlInfo->dwCS & LS_HSCROLL)|| (LabelControlInfo->dwCS & LS_VSCROLL)) {

		SCROLLPANELCONTROLINFO *scrollPanelControlInfo = new SCROLLPANELCONTROLINFO();
		scrollPanelControlInfo->hInstance = hInstance;
		scrollPanelControlInfo->hWndParent = LabelControlInfo->hWndParent;
		scrollPanelControlInfo->dwCS = 0;
		scrollPanelControlInfo->cx = LabelControlInfo->x;
		scrollPanelControlInfo->cy = LabelControlInfo->y;
		scrollPanelControlInfo->dwWidth = LabelControlInfo->dwWidth;
		scrollPanelControlInfo->dwHeight = LabelControlInfo->dwHeight;
		scrollPanelControlInfo->dwHSBAreaWidth = 5;
		scrollPanelControlInfo->dwVSBAreaWidth = 5;
		scrollPanelControl = new SCROLLPANELCONTROL(scrollPanelControlInfo);
		LabelControlInfo->hWndControl = scrollPanelControl->ScrollPanelControlInfo->hWndControl;

		DWORD dwWndID = (CC_LABEL << 28) | (dwControlObjectID << 16);
		LabelControlInfo->hWndContent =
			scrollPanelControl->ScrollPanelControlInfo->hWndContent = CreateWindowEx(NULL, L"MyLabelControl", NULL, WS_CHILD | WS_VISIBLE,
				0, 0, 0, 0, scrollPanelControl->ScrollPanelControlInfo->hWndContainer, (HMENU)dwWndID, LabelControlInfo->hInstance, NULL);

	}
	else {
		DWORD dwWndID = (CC_LABEL << 28) | (dwControlObjectID << 16);
		LabelControlInfo->hWndContent =
			CreateWindowEx(NULL, L"MyLabelControl", NULL, WS_CHILD | WS_VISIBLE,
				LabelControlInfo->x, LabelControlInfo->y, LabelControlInfo->dwWidth, LabelControlInfo->dwHeight, LabelControlInfo->hWndParent, (HMENU)dwWndID, LabelControlInfo->hInstance, NULL);
	}



	//1�����Ȱ�wsText�ı��Ի��з����зָ�õ�һ��������
	vector<wstring> sections;
	sections.push_back(LabelControlInfo->wsText);
	sections.push_back(L"���ǵ�һ�ε����ݣ������");
	sections.push_back(L"���ǵڶ��ε����ݣ������");
	sections.push_back(L"���ǵ����ε����ݣ������");
	sections.push_back(L"���ǵ��Ķε����ݣ������");
	sections.push_back(L"���ǵ���ε����ݣ������");
	//������Ҫ�Զ����У���ÿһ�ν��д���
	vector<wstring> rows;
	//�����Ƕ����Լ���ˮƽ�����λ�á���ʾ��ʽ���д���
	if (LabelControlInfo->dwCS & LS_AUTOWRAP) {
		//1.1�����Ƿ�����

	}
	else {
		rows = sections;
		LabelControlInfo->MaxRowLength = 0;
		HDC hdc = GetDC(LabelControlInfo->hWndContent);
		HFONT hFont = CreateFontIndirect(&LabelControlInfo->lfText);
		SelectObject(hdc, hFont);
		SIZE sizeTest;
		for (vector<wstring>::iterator iter = rows.begin(); iter != rows.end(); iter++) {
			wstring wsTemp = *iter;
			GetTextExtentPoint32(hdc, wsTemp.c_str(), wcslen(wsTemp.c_str()), &sizeTest);
			if (sizeTest.cx > LabelControlInfo->MaxRowLength) {
				LabelControlInfo->MaxRowLength = sizeTest.cx;

			}

			ROWINFO *RowInfo = new ROWINFO();
			RowInfo->wsRowText = *iter;

			if (sizeTest.cx<LabelControlInfo->dwWidth) {
				switch ((LabelControlInfo->dwCS << 20) >> 28)
				{
				case 1: {RowInfo->xRowText = (LabelControlInfo->dwWidth - sizeTest.cx) / 2; }break;
				case 2: {RowInfo->xRowText = LabelControlInfo->dwWidth - sizeTest.cx; }break;
				default:break;
				}
			}
			RowsInfo.push_back(RowInfo);
		}
		
		ReleaseDC(LabelControlInfo->hWndContent, hdc);
		

		if (LabelControlInfo->dwCS & LS_HSCROLL) {
			ScrollFlag = ScrollFlag&~SS_NOHSCROLL;
		}
		else if (LabelControlInfo->dwCS & LS_EXPWIDTH) {
			if (LabelControlInfo->MaxRowLength > LabelControlInfo->dwWidth) {
				 LabelControlInfo->dwWidth = LabelControlInfo->MaxRowLength;
			}
			bAddLengthFlag = TRUE;
		}
		else if (LabelControlInfo->dwCS & LS_ADAPWIDTH) { 
			LabelControlInfo->dwWidth = LabelControlInfo->MaxRowLength;
			bAddLengthFlag = TRUE;
		}

	}
	//�Դ�ֱ�����λ�á��߶ȱ仯���д���
	LabelControlInfo->TotalRowsHeight = LabelControlInfo->lfText.lfHeight*rows.size();
	if (LabelControlInfo->TotalRowsHeight < LabelControlInfo->dwHeight) {
		LabelControlInfo->TotalRowsHeight = LabelControlInfo->dwHeight;
		if (LabelControlInfo->dwCS & LS_VMIDDLE) { LabelControlInfo->yLabelTextStart = (LabelControlInfo->dwHeight - LabelControlInfo->lfText.lfHeight*rows.size()) / 2; }
		if (LabelControlInfo->dwCS & LS_VLOWER) { LabelControlInfo->yLabelTextStart = LabelControlInfo->dwHeight - LabelControlInfo->lfText.lfHeight*rows.size(); }
		if (LabelControlInfo->dwCS & LS_ADAPHEIGHT) {
			LabelControlInfo->yLabelTextStart = 0; 
			LabelControlInfo->dwHeight = LabelControlInfo->lfText.lfHeight*rows.size(); 
			if ((LabelControlInfo->dwCS & LS_HSCROLL) && (LabelControlInfo->MaxRowLength > LabelControlInfo->dwWidth)) {
				LabelControlInfo->dwHeight = LabelControlInfo->dwHeight+ scrollPanelControl->ScrollPanelControlInfo->dwHSBAreaWidth;
			}
		}
	}
	else {
		if ((LabelControlInfo->dwCS & LS_ADAPHEIGHT)|| (LabelControlInfo->dwCS & LS_EXPHEIGHT)) {
			LabelControlInfo->dwHeight = LabelControlInfo->lfText.lfHeight*rows.size();
			if ((LabelControlInfo->dwCS & LS_HSCROLL)&& (LabelControlInfo->MaxRowLength > LabelControlInfo->dwWidth)) {
				LabelControlInfo->dwHeight = LabelControlInfo->dwHeight + scrollPanelControl->ScrollPanelControlInfo->dwHSBAreaWidth;
			}
		}
	}
	if (LabelControlInfo->dwCS & LS_VSCROLL) {
		ScrollFlag = ScrollFlag&~SS_NOVSCROLL;
		if (bAddLengthFlag&& (LabelControlInfo->TotalRowsHeight > LabelControlInfo->dwHeight)) {
			LabelControlInfo->dwWidth= LabelControlInfo->dwWidth+ scrollPanelControl->ScrollPanelControlInfo->dwVSBAreaWidth;
		}
	}
	bLoadDataDone = TRUE;
	if ((LabelControlInfo->dwCS & LS_HSCROLL) || (LabelControlInfo->dwCS & LS_VSCROLL)) {
		scrollPanelControl->ScrollPanelControlInfo->dwCS = ScrollFlag;

		MoveWindow(LabelControlInfo->hWndContent, 0, 0, LabelControlInfo->MaxRowLength, LabelControlInfo->TotalRowsHeight, TRUE);
		//PostMessage(LabelControlInfo->hWndContent, WM_PAINT, NULL, NULL);	
		//��dwHeight��������ʱ��Ҫ����������Ϣ

		MoveWindow(scrollPanelControl->ScrollPanelControlInfo->hWndControl, LabelControlInfo->x, LabelControlInfo->y, LabelControlInfo->dwWidth, LabelControlInfo->dwHeight, TRUE);
		PostMessage(scrollPanelControl->ScrollPanelControlInfo->hWndControl, WM_SIZE, NULL, NULL);
	}
	else {
		MoveWindow(LabelControlInfo->hWndContent, LabelControlInfo->x, LabelControlInfo->y, LabelControlInfo->dwWidth, LabelControlInfo->dwHeight, TRUE);
	}
	
}

LRESULT CALLBACK LABELCONTROL::LWP_TransMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message)
	{
	case WM_LC +1:
	{
		
		DWORD dwWndID = lParam;
		DWORD dwObjectID = (dwWndID << 4) >> 20;
		LABELCONTROL *pThis = (LABELCONTROL*)rele_objectID_pObject[dwObjectID];
		wchar_t tempStr1[32];
		_itow_s(pThis->LabelControlInfo->MaxRowLength, tempStr1, 32);

		MessageBox(NULL, tempStr1, TEXT("������"), MB_OK | MB_ICONINFORMATION);
		MoveWindow(pThis->LabelControlInfo->hWndContent, 0, 0, pThis->LabelControlInfo->MaxRowLength, pThis->LabelControlInfo->dwHeight, TRUE);
	}
	break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
}
LRESULT CALLBACK  LABELCONTROL::LWP_Control(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	
	switch (message)
	{
	case WM_PAINT:
	{
		//MessageBox(NULL, L"��û�о�������123", TEXT("������"), MB_OK | MB_ICONINFORMATION);
		DWORD dwWndID = GetWindowLongPtr(hWnd, GWLP_ID);
		DWORD dwObjectID = (dwWndID << 4) >> 20;
		LABELCONTROL *pThis = (LABELCONTROL*)rele_objectID_pObject[dwObjectID];
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: �ڴ˴����ʹ�� hdc ���κλ�ͼ����...

		HFONT hFont = CreateFontIndirect(&pThis->LabelControlInfo->lfText);
		HFONT hOldFont = (HFONT)SelectObject(hdc, hFont); //��������ѡ��Ϊ�豸�����ĵ�ǰ���壬������֮ǰ������
														  //MessageBox(NULL, tempStr1, TEXT("������"), MB_OK | MB_ICONINFORMATION);
		if(pThis->bLoadDataDone){
			RECT rcWndControlClient;
			GetWindowRect(hWnd, &rcWndControlClient);

			//TODO:�˴���Ϊ����������ܳ��ָ�����Ӧ�����ж�
			DWORD dwWidth = rcWndControlClient.right - rcWndControlClient.left;
			wchar_t tempStr1[32];
			_itow_s((LONG)dwWidth, tempStr1, 32);
			//MessageBox(NULL, tempStr1, TEXT("������"), MB_OK | MB_ICONINFORMATION);
			DWORD index = 0;
			for (vector<ROWINFO *>::iterator iter = pThis->RowsInfo.begin(); iter != pThis->RowsInfo.end(); iter++) {
				//MessageBox(NULL, L"��û�о�������123", TEXT("������"), MB_OK | MB_ICONINFORMATION);
			
				ROWINFO *RowInfo = (ROWINFO *)*iter;

			
				TextOut(hdc, RowInfo->xRowText, pThis->LabelControlInfo->lfText.lfHeight*index+ pThis->LabelControlInfo->yLabelTextStart,
					RowInfo->wsRowText.c_str(),
					wcslen(RowInfo->wsRowText.c_str()));
				index++;
			}
		}
		//pThis->TVStyle.dIndent*(pThis->TreeViewData.vTreeNode[v_id]->data.dNodeDepth)
		//pThis->TreeViewData.vTreeNode[v_id]->data.NodeName.c_str()
		//pThis->TreeViewData.vTreeNode[v_id]->data.NodeName.c_str()
		EndPaint(hWnd, &ps);
		
		
	}
	break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	return 0;
}