#pragma once
#ifndef _DragDrop_H
#define _DragDrop_H
#include <oleidl.h>
#include <objidl.h>
#include <vector>
#include <string>

class TDataObject : public IDataObject
{
public:
	// IUnknown members
	HRESULT __stdcall QueryInterface(REFIID iid, void ** ppv);
	ULONG   __stdcall AddRef(void);
	ULONG   __stdcall Release(void);

	// IDataObject members
	HRESULT __stdcall GetData(FORMATETC *pFormatEtc, STGMEDIUM *pStgMedium);
	HRESULT __stdcall QueryGetData(FORMATETC *pFormatEtc);
	HRESULT __stdcall EnumFormatEtc(DWORD dwDirection, IEnumFORMATETC **ppEnumFormatEtc);

	HRESULT __stdcall GetDataHere(FORMATETC *pFormatEtc, STGMEDIUM *pmedium) {
		return DATA_E_FORMATETC;
	}
	HRESULT __stdcall GetCanonicalFormatEtc(FORMATETC *pFormatEct, FORMATETC *pFormatEtcOut) {
		// Apparently we have to set this field to NULL even though we don't do anything else
		pFormatEtcOut->ptd = NULL;
		return E_NOTIMPL;
	}
	HRESULT __stdcall SetData(FORMATETC *pFormatEtc, STGMEDIUM *pMedium, BOOL fRelease) {
		return E_NOTIMPL;
	}
	HRESULT __stdcall DAdvise(FORMATETC *pFormatEtc, DWORD advf, IAdviseSink *, DWORD *) {
		return OLE_E_ADVISENOTSUPPORTED;
	}
	HRESULT __stdcall DUnadvise(DWORD dwConnection) {
		return OLE_E_ADVISENOTSUPPORTED;
	}
	HRESULT __stdcall EnumDAdvise(IEnumSTATDATA **ppEnumAdvise) {
		return OLE_E_ADVISENOTSUPPORTED;
	}

	// Constructor / Destructor
	TDataObject() :_iRefCount(1) { ; }
	virtual ~TDataObject() { ; }

	//void Add(FORMATETC *fmtetc, STGMEDIUM *stgmeds, UINT count);
	void AddData(FORMATETC fmtetc, STGMEDIUM stgmeds);
	void AddDataArray(FORMATETC *fmtetc, STGMEDIUM *stgmeds, UINT count = 0);
	size_t size() { return _pFormatEtc.size(); }
	bool empty() { return _pFormatEtc.empty(); }
	void Clear();
	void ClearAndRelease();

private:
	ULONG _iRefCount;
	std::vector<FORMATETC> _pFormatEtc;
	std::vector<STGMEDIUM> _pStgMedium;
	int LookupFormatEtc(FORMATETC *pFormatEtc);
	HGLOBAL DupGlobalMem(HGLOBAL hMem);
};

class TEnumFormatEtc : public IEnumFORMATETC
{
public:
	// IUnknown members
	HRESULT __stdcall  QueryInterface(REFIID iid, void ** ppv);
	ULONG   __stdcall AddRef(void);
	ULONG   __stdcall Release(void);

	// IEnumFormatEtc members
	HRESULT __stdcall  Next(ULONG celt, FORMATETC * rgelt, ULONG * pceltFetched);
	HRESULT __stdcall  Skip(ULONG celt);
	HRESULT __stdcall  Reset(void);
	HRESULT __stdcall  Clone(IEnumFORMATETC ** ppEnumFormatEtc);

	// Construction / Destruction
	TEnumFormatEtc(FORMATETC *pFormatEtc, ULONG nNumFormats);
	~TEnumFormatEtc();

private:
	LONG        _iRefCount;        // Reference count for this COM interface
	ULONG       _nIndex;           // current enumerator index
	ULONG       _nNumFormats;      // number of FORMATETC members
	FORMATETC * _pFormatEtc;       // array of FORMATETC objects
public:
	static HRESULT CreateEnumFormatEtc(UINT cfmt, FORMATETC *afmt, IEnumFORMATETC **ppEnumFormatEtc);
	void DeepCopyFormatEtc(FORMATETC *dest, FORMATETC *source);
};

class TDropSource:public IDropSource
{
private:
	ULONG _iRefCount;
public:
	// IUnknown members
	HRESULT __stdcall QueryInterface(REFIID iid, void **ppv);
	ULONG __stdcall AddRef(void) { return ++_iRefCount; }
	ULONG __stdcall Release(void) { if (--_iRefCount == 0) { delete this; return 0; } return _iRefCount; }

	// IDropSource
	HRESULT __stdcall QueryContinueDrag(BOOL fEscapePressed, DWORD grfKeyState);
	HRESULT __stdcall GiveFeedback(DWORD dwEffect);

	//
	TDropSource() :_iRefCount(1) { ; }
	virtual ~TDropSource() { ; }
};

struct THDROP_Files :
	std::vector<std::string> {
	void *Create_HDROP() const;
};
HANDLE MemToHandle(void *pMem, int nSize);


//IDropTarget接口实现
class IDropTargetEvent
{
public:
	virtual void OnDrop(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD __RPC_FAR *pdwEffect) {};
};

class TDropTarget : public IDropTarget
{
public:
	TDropTarget(IDropTargetEvent * pTargetEvent, FORMATETC *pFormatEtc,UINT uCount) :m_pTargetEvent(pTargetEvent),pFormatEtcAcceptable(pFormatEtc), uFmtetcCount(uCount), _iRefCount(1){}
	~TDropTarget(void) {}
	HRESULT __stdcall QueryInterface(REFIID iid, void **ppv);
	ULONG __stdcall AddRef(void) { return ++_iRefCount; }
	ULONG __stdcall Release(void) { if (--_iRefCount == 0) { delete this; return 0; } return _iRefCount; }
	HRESULT __stdcall  DragOver(DWORD grfKeyState, POINTL pt, DWORD *pdwEffect);
	HRESULT __stdcall  DragEnter(IDataObject * pDataObject, DWORD grfKeyState, POINTL pt, DWORD * pdwEffect);
	HRESULT __stdcall DragLeave();
	HRESULT __stdcall Drop(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD __RPC_FAR *pdwEffect);	

private:
	ULONG _iRefCount;
	BOOL bAcceptFlag=FALSE;
	IDropTargetEvent *	m_pTargetEvent;
	FORMATETC *pFormatEtcAcceptable;
	UINT uFmtetcCount;
};

class Editer :public IDropTargetEvent {
public:
	Editer() {
		FORMATETC pFormatEtcAcceptable[1];
		pFormatEtcAcceptable[1] = { CF_HDROP,0,DVASPECT_CONTENT,-1,TYMED_HGLOBAL };
		pDropTarget = new TDropTarget(this, pFormatEtcAcceptable, (UINT)1);
		//RegisterDragDrop(hWnd, pDropTarget);		
	}
	HRESULT RegisterDragDropTarget(HWND hWnd) {
		return RegisterDragDrop(hWnd, pDropTarget);
	}
	void OnDrop() {}
private:
	TDropTarget *pDropTarget;
};
#endif
