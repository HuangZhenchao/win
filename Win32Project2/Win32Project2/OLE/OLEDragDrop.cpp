#include "OLEDragDrop.h"
//CLASS TDataObject
HRESULT __stdcall TDataObject::QueryInterface(REFIID iid, void **ppv)
{
	if ((iid == IID_IUnknown) || (iid == IID_IDataObject))
	{
		*ppv = this;
		AddRef();
		return S_OK;
	}
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}
}
ULONG   __stdcall TDataObject::AddRef(void) { return ++_iRefCount; }
ULONG   __stdcall TDataObject::Release(void) { if (--_iRefCount == 0) { delete this;  return 0; } return _iRefCount; }

HRESULT __stdcall TDataObject::QueryGetData(FORMATETC *pFormatEtc)
{
	return (LookupFormatEtc(pFormatEtc) == -1) ? DV_E_FORMATETC : S_OK;
}
HRESULT __stdcall TDataObject::GetData(FORMATETC *pFormatEtc, STGMEDIUM *pStgMedium)
{
	int idx;
	if ((idx = LookupFormatEtc(pFormatEtc)) == -1)
		return DV_E_FORMATETC;

	pStgMedium->tymed = _pFormatEtc[idx].tymed;
	pStgMedium->pUnkForRelease = 0;

	switch (_pFormatEtc[idx].tymed)
	{
	case TYMED_HGLOBAL:
		pStgMedium->hGlobal = DupGlobalMem(_pStgMedium[idx].hGlobal);
		break;
	//case TYMED_GDI:
	//	pStgMedium->hBitmap = (HBITMAP)CopyImage(_pStgMedium[idx].hBitmap, IMAGE_BITMAP, 0, 0, 0);
	//	break;
	default:
		return DV_E_FORMATETC;
	}
	return S_OK;
}
HRESULT __stdcall TDataObject::EnumFormatEtc(DWORD dwDirection, IEnumFORMATETC **ppEnumFormatEtc)
{
	// OLE仅仅支持得到方向成员
	if (dwDirection == DATADIR_GET)
	{
		// 在WIN2K下，你可以调用API函数SHCreateStdEnumFmtEtc来完成，但为了支持//所有的window平台，我们需要实现IEnumFormatEtc。
		return TEnumFormatEtc::CreateEnumFormatEtc(_pFormatEtc.size(), &_pFormatEtc[0], ppEnumFormatEtc);
	}
	else
	{
		// the direction specified is not supported for drag+drop
		return E_NOTIMPL;
	}
}

int TDataObject::LookupFormatEtc(FORMATETC *pFormatEtc)
{
	// 轮流检查格式看是否能找到匹配的格式
	for (size_t i = 0; i < _pFormatEtc.size(); i++)
	{
		if ((_pFormatEtc[i].tymed & pFormatEtc->tymed) &&
			_pFormatEtc[i].cfFormat == pFormatEtc->cfFormat &&
			_pFormatEtc[i].dwAspect == pFormatEtc->dwAspect)
		{
			return i;
		}
	}
	return -1;
}
HGLOBAL TDataObject::DupGlobalMem(HGLOBAL hMem)
{
	DWORD   len = GlobalSize(hMem);
	PVOID   source = GlobalLock(hMem);
	PVOID   dest = GlobalAlloc(GMEM_FIXED, len);
	memcpy(dest, source, len);
	GlobalUnlock(hMem);
	return dest;
}
void TDataObject::AddData(FORMATETC fmtetc, STGMEDIUM stgmeds)
{
	
	_pFormatEtc.push_back(fmtetc);
	_pStgMedium.push_back(stgmeds);
	
}
void TDataObject::AddDataArray(FORMATETC *fmtetc, STGMEDIUM *stgmeds, UINT count)
{
	for (UINT i = 0; i<count; i++)
	{
		_pFormatEtc.push_back(fmtetc[i]);
		_pStgMedium.push_back(stgmeds[i]);
	}
}

void TDataObject::Clear()
{
	_pFormatEtc.clear();
	_pStgMedium.clear();
}
void TDataObject::ClearAndRelease()
{
	for (size_t i = 0; i<_pFormatEtc.size(); i++)
		ReleaseStgMedium(&_pStgMedium[i]);
	Clear();
}

/*
**CLASS TEnumFormatEtc
*/

//IUnknown members
HRESULT __stdcall  TEnumFormatEtc::QueryInterface(REFIID iid, void ** ppv)
{
	if ((iid == IID_IUnknown) || (iid == IID_IEnumFORMATETC))
	{
		*ppv = this;
		AddRef();
		return S_OK;
	}
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}
}
ULONG   __stdcall TEnumFormatEtc::AddRef(void) { return ++_iRefCount; }
ULONG   __stdcall TEnumFormatEtc::Release(void) { if (--_iRefCount == 0) { delete this;  return 0; } return _iRefCount; }
//IEnumFORMATETC members
HRESULT __stdcall  TEnumFormatEtc::Reset(void)
{
	_nIndex = 0;
	return S_OK;
}
HRESULT __stdcall  TEnumFormatEtc::Clone(IEnumFORMATETC ** ppEnumFormatEtc)
{
	HRESULT hResult;
	hResult = CreateEnumFormatEtc(_nNumFormats, _pFormatEtc, ppEnumFormatEtc);
	if (hResult == S_OK)
	{
		((TEnumFormatEtc *)*ppEnumFormatEtc)->_nIndex = _nIndex;
	}
	return hResult;
}
HRESULT __stdcall  TEnumFormatEtc::Skip(ULONG celt)
{
	_nIndex += celt;
	return (_nIndex <= _nNumFormats) ? S_OK : S_FALSE;
}
HRESULT __stdcall TEnumFormatEtc::Next(ULONG celt, FORMATETC *pFormatEtc, ULONG *pceltFetched)
{
	ULONG copied = 0;
	// copy the FORMATETC structures into the caller's buffer
	while (_nIndex < _nNumFormats && copied < celt)
	{
		DeepCopyFormatEtc(&pFormatEtc[copied], &_pFormatEtc[_nIndex]);
		copied++;
		_nIndex++;
	}
	// store result
	if (pceltFetched != 0)
		*pceltFetched = copied;
	// did we copy all that was requested?
	return (copied == celt) ? S_OK : S_FALSE;
}
//构造、析构函数
TEnumFormatEtc::TEnumFormatEtc(FORMATETC *pFormatEtc, ULONG nNumFormats)
	:_iRefCount(1), _nIndex(0), _nNumFormats(nNumFormats)
{
	_pFormatEtc = new FORMATETC[nNumFormats];
	// make a new copy of each FORMATETC structure
	for (ULONG i = 0; i < nNumFormats; i++)
	{
		DeepCopyFormatEtc(&_pFormatEtc[i], &pFormatEtc[i]);
	}
}
TEnumFormatEtc::~TEnumFormatEtc()
{
	// first free any DVTARGETDEVICE structures
	for (ULONG i = 0; i < _nNumFormats; i++)
	{
		if (_pFormatEtc[i].ptd)
			CoTaskMemFree(_pFormatEtc[i].ptd);
	}
	// now free the main array
	delete[] _pFormatEtc;
}

//辅助函数

HRESULT TEnumFormatEtc::CreateEnumFormatEtc(UINT cfmt, FORMATETC *afmt, IEnumFORMATETC **ppEnumFormatEtc) {
	if (cfmt == 0 || afmt == 0 || ppEnumFormatEtc == 0)
		return E_INVALIDARG;
	*ppEnumFormatEtc = new TEnumFormatEtc(afmt, cfmt);
	if (*ppEnumFormatEtc)
	{
		return S_OK;
	}
	else
	{
		return E_OUTOFMEMORY;
	}
}

void TEnumFormatEtc::DeepCopyFormatEtc(FORMATETC *dest, FORMATETC *source)
{
	// copy the source FORMATETC into dest
	*dest = *source;
	if (source->ptd)
	{
		// allocate memory for the DVTARGETDEVICE if necessary
		dest->ptd = (DVTARGETDEVICE*)CoTaskMemAlloc(sizeof(DVTARGETDEVICE));
		// copy the contents of the source DVTARGETDEVICE into dest->ptd
		*(dest->ptd) = *(source->ptd);
	}
}

//------------------------------TDropSource---------------------------------------------
HRESULT __stdcall TDropSource::QueryInterface(REFIID iid, void **ppv)
{
	if ((iid == IID_IUnknown) || (iid == IID_IDropSource))
	{
		*ppv = this;
		AddRef();
	}
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}
	return S_OK;
}

HRESULT __stdcall TDropSource::QueryContinueDrag(BOOL fEscapePressed, DWORD grfKeyState)
{
	return fEscapePressed ? DRAGDROP_S_CANCEL : ((grfKeyState&MK_LBUTTON) ? S_OK : DRAGDROP_S_DROP);
}

HRESULT __stdcall TDropSource::GiveFeedback(DWORD dwEffect)
{
	return DRAGDROP_S_USEDEFAULTCURSORS;
}

void* THDROP_Files::Create_HDROP() const
{
	struct TMyDropFiles {
		DWORD pFiles;
		POINT pt;
		BOOL fNC;
		BOOL fWide;
		char szFiles[1];
	};

	size_t MemSize = 0;
	for (const_iterator i = begin(); i != end(); i++)
	{
		MemSize += i->length();
		MemSize++;
	}

	void* ptr = GlobalAlloc(GMEM_FIXED, sizeof(TMyDropFiles) + MemSize);
	ZeroMemory(ptr, sizeof(TMyDropFiles) + MemSize);

	TMyDropFiles *FileNames = new (ptr)TMyDropFiles;
	FileNames->pFiles = (unsigned)FileNames->szFiles - (unsigned)(&FileNames->pFiles);

	char *p = FileNames->szFiles;
	for (const_iterator i = begin(); i != end(); i++)
	{
		size_t len = i->length();
		CopyMemory(p, i->c_str(), len);
		p += len;
		*(p++) = 0;
	}
	*p = 0;

	return ptr;
}

HANDLE MemToHandle(void *pMem, int nSize)
{
	void* ptr = GlobalAlloc(GMEM_FIXED, nSize);
	CopyMemory(ptr, pMem, nSize);
	return ptr;
}


//DragTarget
HRESULT __stdcall TDropTarget::QueryInterface(REFIID iid, void **ppv)
{
	if ((iid == IID_IUnknown) || (iid == IID_IDropSource))
	{
		*ppv = this;
		AddRef();
	}
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}
	return S_OK;
}
HRESULT __stdcall  TDropTarget::DragEnter(IDataObject * pDataObject, DWORD grfKeyState, POINTL pt, DWORD * pdwEffect)
{	
	//FORMATETC pFormatEtc = { (CLIPFORMAT)CF_HDROP,0,DVASPECT_CONTENT,-1,TYMED_HGLOBAL };
	//STGMEDIUM stgMedium;
	for (UINT i = 0; i<uFmtetcCount; i++)
	{
		//FORMATETC pFormatEtc = { CF_HDROP,0,DVASPECT_CONTENT,-1,TYMED_HGLOBAL };
		if (SUCCEEDED(pDataObject->QueryGetData(&pFormatEtcAcceptable[i]))) {
			*pdwEffect = DROPEFFECT_COPY;
			if (grfKeyState&MK_SHIFT) {
				*pdwEffect = DROPEFFECT_MOVE;
			}
			bAcceptFlag = TRUE;
		}
		else {
			*pdwEffect = DROPEFFECT_NONE;
		}
	}	
	return S_OK;
}
HRESULT __stdcall  TDropTarget::DragOver(DWORD grfKeyState, POINTL pt, DWORD *pdwEffect)
{
	
	if (bAcceptFlag) {
		*pdwEffect = DROPEFFECT_COPY;
		if (grfKeyState&MK_SHIFT) {
			*pdwEffect = DROPEFFECT_MOVE;
		}
	}
	else { *pdwEffect = DROPEFFECT_NONE; }
	return S_OK;
}
HRESULT __stdcall TDropTarget::DragLeave() {
	bAcceptFlag = FALSE;
	return S_OK;
}
HRESULT __stdcall TDropTarget::Drop(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD __RPC_FAR *pdwEffect)
{
	if (bAcceptFlag) {
		m_pTargetEvent->OnDrop(pDataObj, grfKeyState, pt, pdwEffect);
	}
	/*
	IEnumFORMATETC *pEnumFmt = NULL;
	pDataObj->EnumFormatEtc(DATADIR_GET, &pEnumFmt);
	wchar_t tempStr1[32];
	_itow_s(SUCCEEDED(pEnumFmt->Reset()), tempStr1, 32);
	MessageBox(NULL, tempStr1, TEXT("错误码"), MB_OK | MB_ICONINFORMATION);



	HRESULT Ret = S_OK;
	FORMATETC *pFormatEtc = new FORMATETC();
	DWORD index = 1;//CF_TEXT  TYMED_HGLOBAL
					//CF_TEXT
	while (Ret == S_OK) {
		Ret = (HRESULT)pEnumFmt->Next(1, pFormatEtc, NULL);
		//MessageBox(NULL, L"tishi", TEXT("错误码"), MB_OK | MB_ICONINFORMATION);
		HDC hdc = GetDC(hWndMain);
		WCHAR lpszFormatName[100] = L"";
		GetClipboardFormatName(pFormatEtc->cfFormat, lpszFormatName, 100);
		TextOut(hdc, 100 * index, 0,
			lpszFormatName,
			wcslen(lpszFormatName));
		wchar_t tempStr1[32];
		_itow_s(pFormatEtc->cfFormat, tempStr1, 32);
		TextOut(hdc, 100 * index, 30,
			tempStr1,
			wcslen(tempStr1));



		_itow_s(pFormatEtc->tymed, tempStr1, 32);
		TextOut(hdc, 100 * index, 60,
			tempStr1,
			wcslen(tempStr1));
		index++;


		ReleaseDC(hWndMain, hdc);

	}
	index = 0;
	
	FORMATETC cFmt = {(CLIPFORMAT) CF_TEXT, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL};
	STGMEDIUM stgMedium;
	if(SUCCEEDED(pDataObj->GetData(&cFmt, &stgMedium)))
	{
	char *pBuff = NULL;
	pBuff=(LPSTR)GlobalLock(stgMedium.hGlobal);
	GlobalUnlock(stgMedium.hGlobal);
	USES_CONVERSION;
	if(m_pTarget) m_pTarget->OnDropText(A2T(pBuff));
	::ReleaseStgMedium(&stgMedium);
	}
	FORMATETC cFmt2 = {(CLIPFORMAT) CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL};
	if(SUCCEEDED(pDataObj->GetData(&cFmt2, &stgMedium)))
	{
	if(m_pTarget) m_pTarget->OnDropFile((HDROP)stgMedium.hGlobal);
	::ReleaseStgMedium(&stgMedium);
	}*/
	return S_OK;
}


