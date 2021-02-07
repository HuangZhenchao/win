#include "stdafx.h"
#include "global.h"
#include "ScrollPanelControl.h"

void SPW_RegisterWnd(HINSTANCE hInstance){
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = 0;
	wcex.lpfnWndProc = DefWindowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = sizeof(LONG);
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(nullptr, IDC_HAND);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"WndDef";
	wcex.hIconSm = NULL;

	if (!GetClassInfoEx(hInstance, wcex.lpszClassName, &wcex)) {
		RegisterClassExW(&wcex);
	}
	wcex.lpfnWndProc = SPWP_Control;
	wcex.hbrBackground = CreateSolidBrush(RGB(232, 232, 236));
	//����ص�������ǰ������
	wcex.lpszClassName = L"MyScrollPanelControl";
	if (!GetClassInfoEx(hInstance, wcex.lpszClassName, &wcex)) {
		RegisterClassExW(&wcex);
	}
	
	wcex.lpfnWndProc = SPWP_ScrollBar;
	wcex.hbrBackground = CreateSolidBrush(RGB(233, 207, 174));
	//����ص�������ǰ������
	wcex.lpszClassName = L"SPP_ScrollBar";
	if (!GetClassInfoEx(hInstance, wcex.lpszClassName, &wcex)) {
		RegisterClassExW(&wcex);
	}
	//bWndRegistered = TRUE;
}
wstring SCROLLPANELCONTROLINFO::GetDebugString() {
	wstring DebugString = L"";
	wchar_t tempStr1[10];
	DebugString.append(L"�������ؼ������");	
	_itow_s((LONG)hWndControl, tempStr1, 10);
	DebugString.append(tempStr1);
	DebugString.append(L"\n�������ؼ�Container�����");
	_itow_s((LONG)hWndContainer, tempStr1, 10);
	DebugString.append(tempStr1);
	DebugString.append(L"\n�������ؼ�Content�����");
	_itow_s((LONG)hWndContent, tempStr1, 10);
	DebugString.append(tempStr1);
	return DebugString;
}
LRESULT CALLBACK SPWP_Control(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
	switch (message)
	{
	case WM_CREATE:
	{
		SCROLLPANELCONTROLINFO *pThis = (SCROLLPANELCONTROLINFO *)((PWNDDATA *)IdDataBinding[(DWORD)((LPCREATESTRUCT)lParam)->hMenu])->pCtrlInfo;
		pThis->hWndControl = hWnd;
		pThis->Hsi = new SPCSCROLLBARINFO();
		pThis->Vsi = new SPCSCROLLBARINFO();
		pThis->hWndContainer =
			CreateWindowEx(NULL, L"WndDef", NULL, WS_CHILD | WS_VISIBLE,
				0, 0, 0, 0, hWnd, NULL, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		
		pThis->hWndVSBArea =
			CreateWindowEx(NULL, L"WndDef", NULL, WS_CHILD | WS_VISIBLE,
				0, 0, 0, 0, hWnd, NULL, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		pThis->hWndHSBArea =
			CreateWindowEx(NULL, L"WndDef", NULL, WS_CHILD | WS_VISIBLE,
				0, 0, 0, 0, hWnd, NULL, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

		PWNDDATA *pScrollBarData = new PWNDDATA();
		pScrollBarData->pCtrlInfo = pThis;
		pScrollBarData->pExtInfo = nullptr;

		DWORD dwScrollBarID = GetIDFromPool();
		IdDataBinding.insert(map<DWORD, void*>::value_type(dwScrollBarID, pScrollBarData));
		
		
		pThis->hWndVSB =
			CreateWindowEx(NULL, L"SPP_ScrollBar", NULL, WS_CHILD | WS_VISIBLE,
				0, 0, 0, 0, pThis->hWndVSBArea, (HMENU)dwScrollBarID, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

		dwScrollBarID = GetIDFromPool();
		IdDataBinding.insert(map<DWORD, void*>::value_type(dwScrollBarID, pScrollBarData));

		pThis->hWndHSB =
			CreateWindowEx(NULL, L"SPP_ScrollBar", NULL, WS_CHILD | WS_VISIBLE,
				0, 0, 0, 0, pThis->hWndHSBArea, (HMENU)dwScrollBarID, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
	}
	break;
	case WM_SIZE:
	{
		SCROLLPANELCONTROLINFO *pThis = (SCROLLPANELCONTROLINFO *)((PWNDDATA *)IdDataBinding[(DWORD)GetWindowLongPtr(hWnd, GWLP_ID)])->pCtrlInfo;

		//MessageBox(NULL, L"�������Ｘ��", TEXT("������"), MB_OK | MB_ICONINFORMATION);
		if (pThis->hWndContent==NULL) {
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
		pThis->Hsi->dwContentLength = pThis->dwWidth = rcWndControlClient.right - rcWndControlClient.left;//�������������ڵĿ��
		pThis->Vsi->dwContentLength = pThis->dwHeight = rcWndControlClient.bottom - rcWndControlClient.top;//�������������ڵĸ߶�

		//2���ٵõ����ݵĿ�ߣ���ؼ�������Ƚ�
		RECT rcWndContentClient;
		GetWindowRect(pThis->hWndContent, &rcWndContentClient);
		DWORD dwUndeterminedFlag = 0;
		//�ж���ʾ�����������
		//�����ݵĿ�  �������ô�ֱ�������Ŀ�� С�� ���费ֱ�������Ŀ��ʱ��Ҫ�ж����
		//�����ݸߴ�������ʱ����Ҫ���ô�ֱ����������ʱ�����С��Ҳ������ˮƽ������
		if (!(pThis->dwCS & SS_NOHSCROLL)){
			if (pThis->dwWidth < rcWndContentClient.right - rcWndContentClient.left) {
				
				pThis->Hsi->dwContentLength = rcWndContentClient.right - rcWndContentClient.left;
				pThis->Hsi->dwSBWidth = pThis->dwHSBAreaWidth;
			}
			BOOL bGreaterThanContainerWidth = rcWndContentClient.right - rcWndContentClient.left > pThis->dwWidth - pThis->dwVSBAreaWidth;
			BOOL bLessThanContainerWidth = rcWndContentClient.right - rcWndContentClient.left < pThis->dwWidth;
			if (bGreaterThanContainerWidth && bLessThanContainerWidth) {
				pThis->Hsi->dwContentLength = pThis->dwWidth;
				//pThis->Hsi->dwContentLength = rcWndContentClient.right - rcWndContentClient.left;
				dwUndeterminedFlag = 1;
			}
		}
		if (!(pThis->dwCS & SS_NOVSCROLL)){
			if(pThis->dwHeight < rcWndContentClient.bottom - rcWndContentClient.top) {
				pThis->Vsi->dwContentLength = rcWndContentClient.bottom - rcWndContentClient.top;
				pThis->Vsi->dwSBWidth = pThis->dwVSBAreaWidth;
				if (dwUndeterminedFlag == 1) {
					pThis->Hsi->dwContentLength = rcWndContentClient.right - rcWndContentClient.left;
					pThis->Hsi->dwSBWidth = pThis->dwHSBAreaWidth;
				}
			}
			BOOL bGreaterThanContainerWidth = rcWndContentClient.bottom - rcWndContentClient.top > pThis->dwHeight - pThis->dwHSBAreaWidth;
			BOOL bLessThanContainerWidth = rcWndContentClient.bottom - rcWndContentClient.top < pThis->dwHeight;
			if (bGreaterThanContainerWidth && bLessThanContainerWidth) {
				if (pThis->Hsi->dwSBWidth > 0) {
					
					pThis->Vsi->dwContentLength = rcWndContentClient.bottom - rcWndContentClient.top;
					pThis->Vsi->dwSBWidth = pThis->dwVSBAreaWidth;
				}
				
			}
		}
		//�����ݴ��ڿؼ����Լ����������˹�����ѡ��ʱ��

		pThis->Hsi->dwSBAreaLength = pThis->dwWidth - pThis->Vsi->dwSBWidth;//ˮƽ�������ܿ��
		pThis->Vsi->dwSBAreaLength = pThis->dwHeight - pThis->Hsi->dwSBWidth;//ˮƽ�������ܸ߶�
		//vHandlerOfScrollCtrl[CurrentOrder].Hsi.cxContent = vHandlerOfScrollCtrl[CurrentOrder].Hsi.cxWnd - vHandlerOfScrollCtrl[CurrentOrder].Vsi.width;//����Ĭ����С��ȣ����������
		
		

		HDC hdc = GetDC(hWndMain);
		RECT rc;
		SetRect(&rc, 200, 20, 150, 20);
		wchar_t tempStr1[10];
		_itow_s((LONG)pThis->Hsi->dwSBPos, tempStr1, 10);
		TextOut(hdc, 400, 0,
			tempStr1,
			wcslen(tempStr1));



		_itow_s((LONG)pThis->Vsi->dwSBPos, tempStr1, 10);
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
		MoveWindow(pThis->hWndContainer,
			0, 0, pThis->Hsi->dwSBAreaLength, pThis->Vsi->dwSBAreaLength, TRUE);
		MoveWindow(pThis->hWndContent,
			-pThis->Hsi->dwContentPos, -pThis->Vsi->dwContentPos, pThis->Hsi->dwContentLength, pThis->Vsi->dwContentLength, TRUE);
		

		MoveWindow(pThis->hWndHSBArea,
			0, pThis->Vsi->dwSBAreaLength, pThis->Hsi->dwSBAreaLength, pThis->Hsi->dwSBWidth, TRUE);//ˮƽ
		MoveWindow(pThis->hWndVSBArea,
			pThis->Hsi->dwSBAreaLength, 0, pThis->Vsi->dwSBWidth, pThis->Vsi->dwSBAreaLength, TRUE);//��ֱ

		MoveWindow(pThis->hWndHSB, pThis->Hsi->dwSBPos, 0, pThis->Hsi->dwSBLength, pThis->Hsi->dwSBWidth, TRUE);
		MoveWindow(pThis->hWndVSB, 0, pThis->Vsi->dwSBPos, pThis->Vsi->dwSBWidth, pThis->Vsi->dwSBLength, TRUE);

	}
	break;
	case WM_USER + 2:
	{
		SCROLLPANELCONTROLINFO *pThis = (SCROLLPANELCONTROLINFO *)((PWNDDATA *)IdDataBinding[(DWORD)GetWindowLongPtr(hWnd, GWLP_ID)])->pCtrlInfo;
		//MessageBox(NULL, L"dianji333", TEXT("������"), MB_OK | MB_ICONINFORMATION);
		
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

		MoveWindow(pThis->hWndHSB, pThis->Hsi->dwSBPos, 0, pThis->Hsi->dwSBLength, pThis->Hsi->dwSBWidth, TRUE);
		pThis->Hsi->dwContentPos = pThis->Hsi->dwSBPos*pThis->Hsi->dwContentLength / pThis->Hsi->dwSBAreaLength;
		MoveWindow(pThis->hWndContent, -pThis->Hsi->dwContentPos, -pThis->Vsi->dwContentPos, pThis->Hsi->dwContentLength, pThis->Vsi->dwContentLength, TRUE);

	}
	break;
	case WM_USER + 3:
	{
		SCROLLPANELCONTROLINFO *pThis = (SCROLLPANELCONTROLINFO *)((PWNDDATA *)IdDataBinding[(DWORD)GetWindowLongPtr(hWnd, GWLP_ID)])->pCtrlInfo;

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


		MoveWindow(pThis->hWndVSB, 0, pThis->Vsi->dwSBPos, pThis->Vsi->dwSBWidth, pThis->Vsi->dwSBLength,  TRUE);
		pThis->Vsi->dwContentPos = pThis->Vsi->dwSBPos*pThis->Vsi->dwContentLength / pThis->Vsi->dwSBAreaLength;
		MoveWindow(pThis->hWndContent, -pThis->Hsi->dwContentPos, -pThis->Vsi->dwContentPos, pThis->Hsi->dwContentLength, pThis->Vsi->dwContentLength, TRUE);

	}
	break;
	//���ֹ�������
	case WM_MOUSEWHEEL:
	{
		int zDelta = (short)HIWORD(wParam);
		wchar_t tempStr1[32];
		_itow_s(abs(zDelta), tempStr1, 32);
		//MessageBox(NULL, tempStr1, TEXT("������"), MB_OK | MB_ICONINFORMATION);
		if (zDelta>0) {
			SCROLLPANELCONTROLINFO *pThis = (SCROLLPANELCONTROLINFO *)((PWNDDATA *)IdDataBinding[(DWORD)GetWindowLongPtr(hWnd, GWLP_ID)])->pCtrlInfo;


			
			if (pThis->Vsi->dwContentPos > abs(zDelta)) {
				pThis->Vsi->dwContentPos = pThis->Vsi->dwContentPos - abs(zDelta); 
			}
			else { pThis->Vsi->dwContentPos = 0; }
			//pThis->Vsi->dwContentPos = pThis->Vsi->dwSBPos*pThis->Vsi->dwContentLength / pThis->Vsi->dwSBAreaLength;
			pThis->Vsi->dwSBPos = pThis->Vsi->dwContentPos*pThis->Vsi->dwSBAreaLength / pThis->Vsi->dwContentLength;
			MoveWindow(pThis->hWndVSB, 0, pThis->Vsi->dwSBPos, pThis->Vsi->dwSBWidth, pThis->Vsi->dwSBLength, TRUE);
			MoveWindow(pThis->hWndContent, -pThis->Hsi->dwContentPos, -pThis->Vsi->dwContentPos, pThis->Hsi->dwContentLength, pThis->Vsi->dwContentLength, TRUE);

		}
		else {
			SCROLLPANELCONTROLINFO *pThis = (SCROLLPANELCONTROLINFO *)((PWNDDATA *)IdDataBinding[(DWORD)GetWindowLongPtr(hWnd, GWLP_ID)])->pCtrlInfo;

		
			pThis->Vsi->dwContentPos = pThis->Vsi->dwContentPos + abs(zDelta);
			if (pThis->Vsi->dwContentPos > pThis->Vsi->dwContentLength - pThis->Vsi->dwSBAreaLength) {
				pThis->Vsi->dwContentPos = pThis->Vsi->dwContentLength - pThis->Vsi->dwSBAreaLength;
			}
			//pThis->Vsi->dwContentPos = pThis->Vsi->dwSBPos*pThis->Vsi->dwContentLength / pThis->Vsi->dwSBAreaLength;
			pThis->Vsi->dwSBPos = pThis->Vsi->dwContentPos*pThis->Vsi->dwSBAreaLength / pThis->Vsi->dwContentLength;
			MoveWindow(pThis->hWndVSB, 0, pThis->Vsi->dwSBPos, pThis->Vsi->dwSBWidth, pThis->Vsi->dwSBLength, TRUE);
			MoveWindow(pThis->hWndContent, -pThis->Hsi->dwContentPos, -pThis->Vsi->dwContentPos, pThis->Hsi->dwContentLength, pThis->Vsi->dwContentLength, TRUE);

		}
		
	}
	break;
	
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	return 0;
}

// ԭʼ���ڹ���
LRESULT CALLBACK SPWP_ScrollBar(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
		SCROLLPANELCONTROLINFO *pThis = (SCROLLPANELCONTROLINFO *)((PWNDDATA *)IdDataBinding[(DWORD)GetWindowLongPtr(hWnd, GWLP_ID)])->pCtrlInfo;


		SetCapture(hWnd);

		if (hWnd==pThis->hWndHSB) {
			//MessageBox(NULL, L"111", TEXT("������"), MB_OK | MB_ICONINFORMATION);
			pThis->Hsi->dragFlag = 1;
			pThis->Hsi->xPointDown = LOWORD(lParam);
			pThis->Hsi->yPointDown = HIWORD(lParam);
		}
		if (hWnd == pThis->hWndVSB) {
			pThis->Vsi->dragFlag = 1;
			pThis->Vsi->xPointDown = LOWORD(lParam);
			pThis->Vsi->yPointDown = HIWORD(lParam);
		}
		
	}
	break;
	case WM_MOUSEMOVE:
	{
		SCROLLPANELCONTROLINFO *pThis = (SCROLLPANELCONTROLINFO *)((PWNDDATA *)IdDataBinding[(DWORD)GetWindowLongPtr(hWnd, GWLP_ID)])->pCtrlInfo;

		if (hWnd == pThis->hWndHSB&&pThis->Hsi->dragFlag == 1) {
			//MessageBox(NULL, L"dianjiˮƽ", TEXT("������"), MB_OK | MB_ICONINFORMATION);
			PostMessage(GetParent(GetParent(hWnd)), WM_USER + 2, wParam, lParam);
		}
		if (hWnd == pThis->hWndVSB&&pThis->Vsi->dragFlag == 1) {
			PostMessage(GetParent(GetParent(hWnd)), WM_USER + 3, wParam, lParam);
		}	
	}
	break;
	case WM_LBUTTONUP:
	{
		SCROLLPANELCONTROLINFO *pThis = (SCROLLPANELCONTROLINFO *)((PWNDDATA *)IdDataBinding[(DWORD)GetWindowLongPtr(hWnd, GWLP_ID)])->pCtrlInfo;

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