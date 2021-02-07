#include "stdafx.h"
//#include "global.h"

#include "LabelControl.h"

//map<DWORD, LABELCONTROL*> LABELCONTROL::rele_objectID_pObject;
void LW_RegisterWnd(HINSTANCE hInstance) {
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = 0;
	wcex.lpfnWndProc = LWP_Content;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = sizeof(LONG);
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(nullptr, IDC_HAND);
	//wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.hbrBackground = CreateSolidBrush(RGB(255, 0, 0));
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"MyLabelControl";
	wcex.hIconSm = NULL;

	if (!GetClassInfoEx(hInstance, wcex.lpszClassName, &wcex)) {
		RegisterClassExW(&wcex);
	}
	wcex.lpfnWndProc = LWP_TransMessage;
	wcex.hbrBackground = CreateSolidBrush(RGB(232, 232, 236));
	//窗体回调函数的前向声明
	wcex.lpszClassName = L"ControlTransMessage";
	if (!GetClassInfoEx(hInstance, wcex.lpszClassName, &wcex)) {
		RegisterClassExW(&wcex);
	}
	//bWndRegistered = TRUE;
}
wstring LABELCONTROLINFO::GetDebugString() {
	wstring DebugString = scrollPanelControlInfo->GetDebugString();
	wchar_t tempStr1[32];
	DebugString.append(L"\nLabel控件句柄：");
	_itow_s((LONG)hWndControl, tempStr1, 32);
	DebugString.append(tempStr1);

	DebugString.append(L"\nLabel控件Content句柄：");
	_itow_s((LONG)hWndContent, tempStr1, 32);
	DebugString.append(tempStr1);
	return DebugString;
}
LRESULT CALLBACK LWP_TransMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message)
	{
	case WM_CREATE:
	{
		LABELCONTROLINFO *pThis = (LABELCONTROLINFO *)((PWNDDATA *)IdDataBinding[(DWORD)((LPCREATESTRUCT)lParam)->hMenu])->pCtrlInfo;

		pThis->scrollPanelControlInfo = new SCROLLPANELCONTROLINFO();
		pThis->scrollPanelControlInfo->dwCS = 0;
		pThis->scrollPanelControlInfo->dwHSBAreaWidth = 5;
		pThis->scrollPanelControlInfo->dwVSBAreaWidth = 5;
		
		PWNDDATA *pScrollData = new PWNDDATA();
		pScrollData->pCtrlInfo = pThis->scrollPanelControlInfo;
		pScrollData->pExtInfo = nullptr;

		//IdDataBinding.insert(map<DWORD, void*>::value_type(IDsScrollControl->dwCMID, pThis->scrollPanelControlInfo));

		DWORD dwScrollControlID = GetIDFromPool();
		IdDataBinding.insert(map<DWORD, void*>::value_type(dwScrollControlID, pScrollData));
		
		pThis->hWndScroll =
			CreateWindowEx(NULL, L"MyScrollPanelControl", NULL, WS_CHILD | WS_VISIBLE,
				0, 0, ((LPCREATESTRUCT)lParam)->cx, ((LPCREATESTRUCT)lParam)->cy, hWnd, (HMENU)dwScrollControlID, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

		PWNDDATA *pWndData = new PWNDDATA();
		pWndData->pCtrlInfo = pThis;
		pWndData->pExtInfo = nullptr;

		DWORD dwLabelContentID = GetIDFromPool();
		IdDataBinding.insert(map<DWORD, void*>::value_type(dwLabelContentID, pWndData));

		pThis->hWndContent =
			pThis->scrollPanelControlInfo->hWndContent = CreateWindowEx(NULL, L"MyLabelControl", NULL, WS_CHILD | WS_VISIBLE,
				0, 0, 20, 30, pThis->scrollPanelControlInfo->hWndContainer, (HMENU)dwLabelContentID, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		
	}
	break;
	case WM_SIZE: {
		
		LABELCONTROLINFO *pThis = (LABELCONTROLINFO *)((PWNDDATA *)IdDataBinding[(DWORD)GetWindowLongPtr(hWnd, GWLP_ID)])->pCtrlInfo;
		
		if (!pThis->DataReload) {
			break;
		}
		//MessageBox(NULL, L"有没有经过这里123", TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
		DWORD ScrollFlag = SS_NOHSCROLL | SS_NOVSCROLL;
		RECT rcWndControlClient;
		GetWindowRect(hWnd, &rcWndControlClient);

		//TODO:此处因为减法运算可能出现负数，应进行判断
		DWORD dwControlWidth = rcWndControlClient.right - rcWndControlClient.left;//包含滚动栏在内的宽度
		DWORD dwControlHeight = rcWndControlClient.bottom - rcWndControlClient.top;//包含滚动栏在内的高度

		vector<wstring> rows;
		//首先是对行以及在水平方向的位置、显示方式进行处理
		if (pThis->dwCS & LS_AUTOWRAP) {
			//1.1段首是否缩进

		}
		else {
			rows = pThis->sections;
			pThis->MaxRowLength = dwControlWidth;
			HDC hdc = GetDC(pThis->hWndContent);
			HFONT hFont = CreateFontIndirect(&pThis->lfText);
			SelectObject(hdc, hFont);
			SIZE sizeTest;
			for (vector<wstring>::iterator iter = rows.begin(); iter != rows.end(); iter++) {
				wstring wsTemp = *iter;
				GetTextExtentPoint32(hdc, wsTemp.c_str(), wcslen(wsTemp.c_str()), &sizeTest);
				if (sizeTest.cx > pThis->MaxRowLength) {
					pThis->MaxRowLength = sizeTest.cx;
				}

				ROWINFO *RowInfo = new ROWINFO();
				RowInfo->wsRowText = *iter;
				RowInfo->dwLength = sizeTest.cx;
				
				pThis->RowsInfo.push_back(RowInfo);
			}
			ReleaseDC(pThis->hWndContent, hdc);
			if (pThis->dwCS & LS_HSCROLL) {
				ScrollFlag = ScrollFlag&~SS_NOHSCROLL;
			}
			
			if (pThis->dwCS & LS_EXPWIDTH) {
				if (pThis->MaxRowLength >dwControlWidth) {
					dwControlWidth = pThis->MaxRowLength;
					if ((pThis->dwCS & LS_VSCROLL) && (dwControlHeight < pThis->lfText.lfHeight*rows.size())) {
						dwControlWidth = dwControlWidth + pThis->scrollPanelControlInfo->dwVSBAreaWidth;
					}
				}
			}
			if (pThis->dwCS & LS_ADAPWIDTH) {
				dwControlWidth = pThis->MaxRowLength;
				if ((pThis->dwCS & LS_VSCROLL) && (dwControlHeight < pThis->lfText.lfHeight*rows.size())) {
					dwControlWidth = dwControlWidth + pThis->scrollPanelControlInfo->dwVSBAreaWidth;
				}
			}
		}

		
		if (pThis->dwCS & LS_ADAPHEIGHT) {
			dwControlHeight=pThis->lfText.lfHeight*rows.size();
			if ((ScrollFlag == 2) && (pThis->MaxRowLength >dwControlWidth)) {
				dwControlHeight = dwControlHeight + pThis->scrollPanelControlInfo->dwHSBAreaWidth;
			}
		}
		else {
			if (pThis->lfText.lfHeight*rows.size() < dwControlHeight) {
				if (pThis->dwCS & LS_VMIDDLE) { pThis->yLabelTextStart = (pThis->dwControlHeight - pThis->lfText.lfHeight*rows.size()) / 2; }
				if (pThis->dwCS & LS_VLOWER) { pThis->yLabelTextStart = pThis->dwControlHeight - pThis->lfText.lfHeight*rows.size(); }
			}
			else {
				if (pThis->dwCS & LS_VSCROLL) {
					ScrollFlag = ScrollFlag&~SS_NOVSCROLL;
				}
				if (pThis->dwCS & LS_EXPHEIGHT) {
					dwControlHeight = pThis->lfText.lfHeight*rows.size();
					if ((ScrollFlag == 2) && (pThis->MaxRowLength >dwControlWidth)) {
						dwControlHeight = dwControlHeight + pThis->scrollPanelControlInfo->dwHSBAreaWidth;
					}
				}
			}
		}
		
		pThis->bLoadDataDone = TRUE;
		
		pThis->scrollPanelControlInfo->dwCS = ScrollFlag;

		MoveWindow(pThis->hWndContent, 0, 0, pThis->MaxRowLength, pThis->lfText.lfHeight*rows.size(), TRUE);

			//PostMessage(LabelControlInfo->hWndContent, WM_PAINT, NULL, NULL);	
			//当dwHeight大于内容时需要主动发送消息

		MoveWindow(pThis->hWndScroll, 0, 0, dwControlWidth, dwControlHeight, TRUE);
		PostMessage(pThis->hWndScroll, WM_SIZE, NULL, NULL);

		wchar_t tempStr1[32];
		_itow_s((DWORD)pThis->hWndContent, tempStr1, 32);
		pThis->DataReload = FALSE;
		MoveWindow(hWnd, 215, 250, dwControlWidth, dwControlHeight, TRUE);
		//MoveWindow(hWndLabel, 215, 250, wParam, lParam, TRUE);
		//PostMessage(GetParent(hWnd), LN_SIZED, dwControlWidth, dwControlHeight);
		//MessageBox(NULL, tempStr1, TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
	}
	break;	
	case WM_MOUSEWHEEL:
	{
		MessageBox(NULL, L"鼠标滚轮滚动接收——控件", TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
	}
	break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
}
LRESULT CALLBACK  LWP_Content(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	
	switch (message)
	{
	case WM_SIZE:
	{
		RECT rcWndControlClient;
		GetWindowRect(hWnd, &rcWndControlClient);

		//TODO:此处因为减法运算可能出现负数，应进行判断
		DWORD dwControlWidth = rcWndControlClient.right - rcWndControlClient.left;//包含滚动栏在内的宽度
		DWORD dwControlHeight = rcWndControlClient.bottom - rcWndControlClient.top;//包含滚动栏在内的高度
		wchar_t tempStr1[32];
		_itow_s((DWORD)dwControlHeight, tempStr1, 32);
		//MessageBox(NULL, tempStr1, TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
	}
	break;
	case WM_PAINT:
	{
		//MessageBox(NULL, L"有没有经过这里123", TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
		/*
		IDS *IDs = (IDS *)IdDataBinding[(DWORD)GetWindowLongPtr(hWnd, GWLP_ID)];
		DWORD dwLabelControlID = IDs->dwCMID;

		LABELCONTROLINFO *pThis = (LABELCONTROLINFO*)IdDataBinding[dwLabelControlID];
		*/
		LABELCONTROLINFO *pThis = (LABELCONTROLINFO *)((PWNDDATA *)IdDataBinding[(DWORD)GetWindowLongPtr(hWnd, GWLP_ID)])->pCtrlInfo;
		//LABELCONTROLINFO *pThis = (LABELCONTROLINFO *)((PWNDDATA *)GetWindowLongPtr(hWnd, GWLP_ID))->pCtrlInfo;
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此处添加使用 hdc 的任何绘图代码...

		HFONT hFont = CreateFontIndirect(&pThis->lfText);
		HFONT hOldFont = (HFONT)SelectObject(hdc, hFont); //把新字体选定为设备环境的当前字体，并返回之前的字体
														  //MessageBox(NULL, tempStr1, TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
		if(pThis->bLoadDataDone){
			RECT rcWndControlClient;
			GetWindowRect(hWnd, &rcWndControlClient);

			//TODO:此处因为减法运算可能出现负数，应进行判断
			DWORD dwWidth = rcWndControlClient.right - rcWndControlClient.left;
			
			
			DWORD index = 0;
			/*if (sizeTest.cx < dwWidth) {
					switch ((pThis->dwCS << 20) >> 28)
					{
					case 1: {RowInfo->xRowText = (dwWidth - sizeTest.cx) / 2; }break;
					case 2: {RowInfo->xRowText = dwWidth - sizeTest.cx; }break;
					default:break;
					}
				}*/
			for (vector<ROWINFO *>::iterator iter = pThis->RowsInfo.begin(); iter != pThis->RowsInfo.end(); iter++) {
				//MessageBox(NULL, L"有没有经过这里123", TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
			
				ROWINFO *RowInfo = (ROWINFO *)*iter;
				
				TextOut(hdc, RowInfo->xRowText, pThis->lfText.lfHeight*index+ pThis->yLabelTextStart,
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
	case WM_MOUSELEAVE:
	{
		SetCursor(LoadCursor(NULL, IDC_NO));
	}
	break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	return 0;
}