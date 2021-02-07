#include "stdafx.h"
#include "global.h"
#include "ScrollPanelControl.h"

DWORD SCROLLPANELCONTROL::dwControlType = 2;
DWORD SCROLLPANELCONTROL::dwCSC_1=1;
DWORD SCROLLPANELCONTROL::dwCSC_2=2;
map<DWORD, SCROLLPANELCONTROL*> SCROLLPANELCONTROL::m_id_pSPC;
BOOL SCROLLPANELCONTROL::bWndRegistered;
SCROLLPANELCONTROL::SCROLLPANELCONTROL(SCROLLPANELCONTROLINFO *scrollPanelControlInfo)
{
	dwControlObjectID = m_id_pSPC.size() + 1;
	m_id_pSPC.insert(map<DWORD, SCROLLPANELCONTROL*>::value_type(dwControlObjectID, this));

	ScrollPanelControlInfo = scrollPanelControlInfo;

	Hsi = new SPCSCROLLBARINFO();
	Vsi = new SPCSCROLLBARINFO();


	if (!bWndRegistered) { SPW_RegisterWnd(); }
	DWORD dwWndID = (dwControlType << 28) | (dwControlObjectID << 16);
	ScrollPanelControlInfo->hWndControl= 
		CreateWindowEx(NULL, L"MyScrollPanelControl", NULL, WS_CHILD | WS_VISIBLE,
			ScrollPanelControlInfo->cx, ScrollPanelControlInfo->cy, ScrollPanelControlInfo->dwWidth, ScrollPanelControlInfo->dwHeight, ScrollPanelControlInfo->hWndParent, (HMENU)dwWndID, ScrollPanelControlInfo->hInstance, NULL);
	
	ScrollPanelControlInfo->hWndContainer =
		CreateWindowEx(NULL, L"WndDef", NULL, WS_CHILD | WS_VISIBLE,
			0, 0, 0, 0, ScrollPanelControlInfo->hWndControl, NULL, ScrollPanelControlInfo->hInstance, NULL);
	ScrollPanelControlInfo->hWndVSBArea =
		CreateWindowEx(NULL, L"WndDef", NULL, WS_CHILD | WS_VISIBLE,
			0, 0, 0, 0, ScrollPanelControlInfo->hWndControl, NULL, ScrollPanelControlInfo->hInstance, NULL);
	ScrollPanelControlInfo->hWndHSBArea =
		CreateWindowEx(NULL, L"WndDef", NULL, WS_CHILD | WS_VISIBLE,
			0, 0, 0, 0, ScrollPanelControlInfo->hWndControl, NULL, ScrollPanelControlInfo->hInstance, NULL);
	dwWndID= (dwControlType << 28) | (dwControlObjectID << 16) | (dwCSC_1 << 12) ;
	ScrollPanelControlInfo->hWndVSB =
		CreateWindowEx(NULL, L"SPP_ScrollBar", NULL, WS_CHILD | WS_VISIBLE,
			0, 0, 0, 0, ScrollPanelControlInfo->hWndVSBArea, (HMENU)dwWndID, ScrollPanelControlInfo->hInstance, NULL);
	dwWndID = (dwControlType << 28) | (dwControlObjectID << 16) | (dwCSC_2 << 12) ;
	ScrollPanelControlInfo->hWndHSB =
		CreateWindowEx(NULL, L"SPP_ScrollBar", NULL, WS_CHILD | WS_VISIBLE,
			0, 0, 0, 0, ScrollPanelControlInfo->hWndHSBArea, (HMENU)dwWndID, ScrollPanelControlInfo->hInstance, NULL);
	
}
void SCROLLPANELCONTROL::SPW_RegisterWnd(){
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = 0;
	wcex.lpfnWndProc = DefWindowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = sizeof(LONG);
	wcex.hInstance = ScrollPanelControlInfo->hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(nullptr, IDC_HAND);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"WndDef";
	wcex.hIconSm = NULL;

	if (!GetClassInfoEx(ScrollPanelControlInfo->hInstance, wcex.lpszClassName, &wcex)) {
		RegisterClassExW(&wcex);
	}
	wcex.lpfnWndProc = SPWP_Control;
	wcex.hbrBackground = CreateSolidBrush(RGB(232, 232, 236));
	//����ص�������ǰ������
	wcex.lpszClassName = L"MyScrollPanelControl";
	if (!GetClassInfoEx(ScrollPanelControlInfo->hInstance, wcex.lpszClassName, &wcex)) {
		RegisterClassExW(&wcex);
	}
	
	wcex.lpfnWndProc = SPWP_ScrollBar;
	wcex.hbrBackground = CreateSolidBrush(RGB(233, 207, 174));
	//����ص�������ǰ������
	wcex.lpszClassName = L"SPP_ScrollBar";
	if (!GetClassInfoEx(ScrollPanelControlInfo->hInstance, wcex.lpszClassName, &wcex)) {
		RegisterClassExW(&wcex);
	}
	bWndRegistered = TRUE;
}

LRESULT CALLBACK SCROLLPANELCONTROL::SPWP_Control(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		
	}
	break;
	case WM_SIZE:
	{
		DWORD dwWndID = GetWindowLongPtr(hWnd, GWLP_ID);
		DWORD map_id = (dwWndID << 4) >> 20;
		SCROLLPANELCONTROL *pThis = (SCROLLPANELCONTROL*)m_id_pSPC[map_id];
		//MessageBox(NULL, L"�������Ｘ��", TEXT("������"), MB_OK | MB_ICONINFORMATION);
		if (pThis->ScrollPanelControlInfo->hWndContent==NULL) {
			//MessageBox(NULL, L"��û�о�������", TEXT("������"), MB_OK | MB_ICONINFORMATION);
			break;
		}
		pThis->Hsi->dwSBWidth = 0;
		pThis->Vsi->dwSBWidth = 0;
		//MessageBox(NULL, L"ʲôʱ�򾭹�����", TEXT("������"), MB_OK | MB_ICONINFORMATION);
		//1�����ȵõ��ؼ��Ŀ��
		RECT rcWndControlClient;
		GetWindowRect(hWnd, &rcWndControlClient);

		//TODO:�˴���Ϊ����������ܳ��ָ�����Ӧ�����ж�
		pThis->Hsi->dwContentLength = pThis->ScrollPanelControlInfo->dwWidth = rcWndControlClient.right - rcWndControlClient.left;//�������������ڵĿ��
		pThis->Vsi->dwContentLength = pThis->ScrollPanelControlInfo->dwHeight = rcWndControlClient.bottom - rcWndControlClient.top;//�������������ڵĸ߶�

		//2���ٵõ����ݵĿ�ߣ���ؼ�������Ƚ�
		RECT rcWndContentClient;
		GetWindowRect(pThis->ScrollPanelControlInfo->hWndContent, &rcWndContentClient);
		DWORD dwUndeterminedFlag = 0;
		//�ж���ʾ�����������
		//�����ݵĿ�  �������ô�ֱ�������Ŀ�� С�� ���费ֱ�������Ŀ��ʱ��Ҫ�ж����
		//�����ݸߴ�������ʱ����Ҫ���ô�ֱ����������ʱ�����С��Ҳ������ˮƽ������
		if (!(pThis->ScrollPanelControlInfo->dwCS & SS_NOHSCROLL)){
			if (pThis->ScrollPanelControlInfo->dwWidth < rcWndContentClient.right - rcWndContentClient.left) {
				
				pThis->Hsi->dwContentLength = rcWndContentClient.right - rcWndContentClient.left;
				pThis->Hsi->dwSBWidth = pThis->ScrollPanelControlInfo->dwHSBAreaWidth;
			}
			BOOL bGreaterThanContainerWidth = rcWndContentClient.right - rcWndContentClient.left > pThis->ScrollPanelControlInfo->dwWidth - pThis->ScrollPanelControlInfo->dwVSBAreaWidth;
			BOOL bLessThanContainerWidth = rcWndContentClient.right - rcWndContentClient.left < pThis->ScrollPanelControlInfo->dwWidth;
			if (bGreaterThanContainerWidth && bLessThanContainerWidth) {
				pThis->Hsi->dwContentLength = pThis->ScrollPanelControlInfo->dwWidth;
				//pThis->Hsi->dwContentLength = rcWndContentClient.right - rcWndContentClient.left;
				dwUndeterminedFlag = 1;
			}
		}
		if (!(pThis->ScrollPanelControlInfo->dwCS & SS_NOVSCROLL)){
			if(pThis->ScrollPanelControlInfo->dwHeight < rcWndContentClient.bottom - rcWndContentClient.top) {
				pThis->Vsi->dwContentLength = rcWndContentClient.bottom - rcWndContentClient.top;
				pThis->Vsi->dwSBWidth = pThis->ScrollPanelControlInfo->dwVSBAreaWidth;
				if (dwUndeterminedFlag == 1) {
					pThis->Hsi->dwContentLength = rcWndContentClient.right - rcWndContentClient.left;
					pThis->Hsi->dwSBWidth = pThis->ScrollPanelControlInfo->dwHSBAreaWidth;
				}
			}
			BOOL bGreaterThanContainerWidth = rcWndContentClient.bottom - rcWndContentClient.top > pThis->ScrollPanelControlInfo->dwHeight - pThis->ScrollPanelControlInfo->dwHSBAreaWidth;
			BOOL bLessThanContainerWidth = rcWndContentClient.bottom - rcWndContentClient.top < pThis->ScrollPanelControlInfo->dwHeight;
			if (bGreaterThanContainerWidth && bLessThanContainerWidth) {
				if (pThis->Hsi->dwSBWidth > 0) {
					
					pThis->Vsi->dwContentLength = rcWndContentClient.bottom - rcWndContentClient.top;
					pThis->Vsi->dwSBWidth = pThis->ScrollPanelControlInfo->dwVSBAreaWidth;
				}
				
			}
		}
		//�����ݴ��ڿؼ����Լ����������˹�����ѡ��ʱ��

		pThis->Hsi->dwSBAreaLength = pThis->ScrollPanelControlInfo->dwWidth - pThis->Vsi->dwSBWidth;//ˮƽ�������ܿ��
		pThis->Vsi->dwSBAreaLength = pThis->ScrollPanelControlInfo->dwHeight - pThis->Hsi->dwSBWidth;//ˮƽ�������ܸ߶�
		//vHandlerOfScrollCtrl[CurrentOrder].Hsi.cxContent = vHandlerOfScrollCtrl[CurrentOrder].Hsi.cxWnd - vHandlerOfScrollCtrl[CurrentOrder].Vsi.width;//����Ĭ����С��ȣ����������
		
		

		HDC hdc = GetDC(hWndMain);
		RECT rc;
		SetRect(&rc, 200, 20, 150, 20);
		wchar_t tempStr1[10];
		_itow_s((LONG)pThis->Hsi->dwContentLength, tempStr1, 10);
		TextOut(hdc, 400, 0,
			tempStr1,
			wcslen(tempStr1));



		_itow_s((LONG)pThis->Vsi->dwContentLength, tempStr1, 10);
		TextOut(hdc, 400, 20,
			tempStr1,
			wcslen(tempStr1));


		ReleaseDC(hWndMain, hdc);
		//����ֵ������������Ȼ�߶�
		pThis->Hsi ->dwSBLength = (pThis->Hsi->dwSBAreaLength*pThis->Hsi->dwSBAreaLength) / pThis->Hsi->dwContentLength;
		pThis->Vsi->dwSBLength = (pThis->Vsi->dwSBAreaLength*pThis->Vsi->dwSBAreaLength) / pThis->Vsi->dwContentLength;
		//������λ��
		//wchar_t tempStr1[32];
		//_itow_s((LONG)pThis->Vsi->dwSBPos, tempStr1, 32);
		//MessageBox(NULL, tempStr1, TEXT("������"), MB_OK | MB_ICONINFORMATION);
		MoveWindow(pThis->ScrollPanelControlInfo->hWndContainer,
			0, 0, pThis->Hsi->dwSBAreaLength, pThis->Vsi->dwSBAreaLength, TRUE);
		MoveWindow(pThis->ScrollPanelControlInfo->hWndContent,
			pThis->Hsi->dwSBPos, pThis->Vsi->dwSBPos, pThis->Hsi->dwContentLength, pThis->Vsi->dwContentLength, TRUE);
		MoveWindow(pThis->ScrollPanelControlInfo->hWndHSBArea,
			0, pThis->Vsi->dwSBAreaLength, pThis->Hsi->dwSBAreaLength, pThis->Hsi->dwSBWidth, TRUE);//ˮƽ
		MoveWindow(pThis->ScrollPanelControlInfo->hWndVSBArea,
			pThis->Hsi->dwSBAreaLength, 0, pThis->Vsi->dwSBWidth, pThis->Vsi->dwSBAreaLength, TRUE);//��ֱ

		MoveWindow(pThis->ScrollPanelControlInfo->hWndHSB, pThis->Hsi->dwSBPos, 0, pThis->Hsi->dwSBLength, pThis->Hsi->dwSBWidth, TRUE);
		MoveWindow(pThis->ScrollPanelControlInfo->hWndVSB, 0, pThis->Vsi->dwSBPos, pThis->Vsi->dwSBWidth, pThis->Vsi->dwSBLength, TRUE);

	}
	break;
	case WM_USER + 2:
	{
		
		DWORD dwWndID = GetWindowLongPtr(hWnd, GWLP_ID);
		DWORD map_id = (dwWndID << 4) >> 20;
		SCROLLPANELCONTROL *pThis = (SCROLLPANELCONTROL*)m_id_pSPC[map_id];
		int x = (short)LOWORD(lParam);
		

		//x�ǵ��������������ƶ�����λ�ã�xPointDown�ǵ��ʱ�ĵ㣬��x�Ƚ�ȷ���ƶ������ټ����ƶ�����
		if (x>pThis->Hsi->xPointDown) {
			//x>vHandlerOfScrollCtrl[i].Hsi.xPointDown,distance������
			int distance = x - pThis->Hsi->xPointDown;
			pThis->Hsi->dwSBPos = pThis->Hsi->dwSBPos + distance;
			if (pThis->Hsi->dwSBPos > pThis->Hsi->dwSBAreaLength - pThis->Hsi->dwSBLength) {
				pThis->Hsi->dwSBPos = pThis->Hsi->dwSBAreaLength - pThis->Hsi->dwSBLength;
			}
		}
		else {
			int distance = pThis->Hsi->xPointDown - x;
			if (pThis->Hsi->dwSBPos > distance) {
				pThis->Hsi->dwSBPos = pThis->Hsi->dwSBPos - distance;
			}
			else {
				pThis->Hsi->dwSBPos = 0;
			}

		}

		MoveWindow(pThis->ScrollPanelControlInfo->hWndHSB, pThis->Hsi->dwSBPos, 0, pThis->Hsi->dwSBLength, pThis->Hsi->dwSBWidth, TRUE);
		pThis->Hsi->dwContentPos = pThis->Hsi->dwSBPos*pThis->Hsi->dwContentLength / pThis->Hsi->dwSBAreaLength;
		MoveWindow(pThis->ScrollPanelControlInfo->hWndContent, -pThis->Hsi->dwContentPos, -pThis->Vsi->dwContentPos, pThis->Hsi->dwContentLength, pThis->Vsi->dwContentLength, TRUE);

	}
	break;
	case WM_USER + 3:
	{
		DWORD dwWndID = GetWindowLongPtr(hWnd, GWLP_ID);
		DWORD map_id = (dwWndID << 4) >> 20;
		SCROLLPANELCONTROL *pThis = (SCROLLPANELCONTROL*)m_id_pSPC[map_id];
		int y = (short)HIWORD(lParam);


		
		
		if (y>pThis->Vsi->yPointDown) {
			//x>vHandlerOfScrollCtrl[i].Vsi.xPointDown,distance������
			int distance = y - pThis->Vsi->yPointDown;
			pThis->Vsi->dwSBPos = pThis->Vsi->dwSBPos + distance;
			if (pThis->Vsi->dwSBPos > pThis->Vsi->dwSBAreaLength - pThis->Vsi->dwSBLength) {
				pThis->Vsi->dwSBPos = pThis->Vsi->dwSBAreaLength - pThis->Vsi->dwSBLength;
			}
		}
		else {
			int distance = pThis->Vsi->yPointDown - y;
			if (pThis->Vsi->dwSBPos > distance) {
				pThis->Vsi->dwSBPos = pThis->Vsi->dwSBPos - distance;
			}
			else {
				pThis->Vsi->dwSBPos = 0;
			}

		}

		HDC hdc = GetDC(hWndMain);

		wchar_t tempStr1[10];
		_itow_s((LONG)y, tempStr1, 10);
		TextOut(hdc, 500, 0,
			tempStr1,
			wcslen(tempStr1));



		_itow_s((LONG)pThis->Vsi->yPointDown, tempStr1, 10);
		TextOut(hdc, 500, 20,
			tempStr1,
			wcslen(tempStr1));


		ReleaseDC(hWndMain, hdc);


		MoveWindow(pThis->ScrollPanelControlInfo->hWndVSB, 0, pThis->Vsi->dwSBPos, pThis->Vsi->dwSBWidth, pThis->Vsi->dwSBLength,  TRUE);
		pThis->Vsi->dwContentPos = pThis->Vsi->dwSBPos*pThis->Vsi->dwContentLength / pThis->Vsi->dwSBAreaLength;
		MoveWindow(pThis->ScrollPanelControlInfo->hWndContent, -pThis->Hsi->dwContentPos, -pThis->Vsi->dwContentPos, pThis->Hsi->dwContentLength, pThis->Vsi->dwContentLength, TRUE);

	}
	break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	return 0;
}

// ԭʼ���ڹ���
LRESULT CALLBACK SCROLLPANELCONTROL::SPWP_ScrollBar(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{

	}
	break;
	case WM_LBUTTONDOWN:
	{
		//MessageBox(NULL, L"dianji", TEXT("������"), MB_OK | MB_ICONINFORMATION);
		DWORD dwWndID = GetWindowLongPtr(hWnd, GWLP_ID);
		DWORD map_id = (dwWndID << 4) >> 20;
		SCROLLPANELCONTROL *pThis = (SCROLLPANELCONTROL*)m_id_pSPC[map_id];

		SetCapture(hWnd);

		if (hWnd==pThis->ScrollPanelControlInfo->hWndHSB) {
			//MessageBox(NULL, L"111", TEXT("������"), MB_OK | MB_ICONINFORMATION);
			pThis->Hsi->dragFlag = 1;
			pThis->Hsi->xPointDown = LOWORD(lParam);
			pThis->Hsi->yPointDown = HIWORD(lParam);
		}
		if (hWnd == pThis->ScrollPanelControlInfo->hWndVSB) {
			pThis->Vsi->dragFlag = 1;
			pThis->Vsi->xPointDown = LOWORD(lParam);
			pThis->Vsi->yPointDown = HIWORD(lParam);
		}
		
	}
	break;
	case WM_MOUSEMOVE:
	{
		DWORD dwWndID = GetWindowLongPtr(hWnd, GWLP_ID);
		DWORD map_id = (dwWndID << 4) >> 20;
		SCROLLPANELCONTROL *pThis = (SCROLLPANELCONTROL*)m_id_pSPC[map_id];
		if (hWnd == pThis->ScrollPanelControlInfo->hWndHSB&&pThis->Hsi->dragFlag == 1) {
			//MessageBox(NULL, L"dianji", TEXT("������"), MB_OK | MB_ICONINFORMATION);
			PostMessage(pThis->ScrollPanelControlInfo->hWndControl, WM_USER + 2, wParam, lParam);
		}
		if (hWnd == pThis->ScrollPanelControlInfo->hWndVSB&&pThis->Vsi->dragFlag == 1) {
			PostMessage(pThis->ScrollPanelControlInfo->hWndControl, WM_USER + 3, wParam, lParam);
		}
		

	}
	break;
	case WM_LBUTTONUP:
	{
		DWORD dwWndID = GetWindowLongPtr(hWnd, GWLP_ID);
		DWORD map_id = (dwWndID << 4) >> 20;
		SCROLLPANELCONTROL *pThis = (SCROLLPANELCONTROL*)m_id_pSPC[map_id];
		ReleaseCapture();   // �ͷ��������
		pThis->Hsi->dragFlag = 0;
		pThis->Vsi->dragFlag = 0;
		pThis->Hsi->xPointDown = 0;
		pThis->Hsi->yPointDown = 0;
		pThis->Vsi->xPointDown = 0;
		pThis->Vsi->yPointDown = 0;
		
	}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;

}