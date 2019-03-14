/******************************************************************
***
***
***				FREE WINDOWLESS FLASH CONTROL
***
***					   by Makarov Igor
***
***		for questions and remarks mailto: mak_july@list.ru
***
***
*******************************************************************/
// FlashWnd.h: interface for the CFlashWnd class.
//
//////////////////////////////////////////////////////////////////////
#include "OleWnd.h"

class AFX_EXT_CLASS CFlashWnd :
	public COleContainerWnd<ShockwaveFlashObjects::IShockwaveFlash>,
	public ShockwaveFlashObjects::_IShockwaveFlashEvents,
	public ShockwaveFlashObjects::IServiceProvider
	//public ShockwaveFlashObjects::ICanHandleException
{
public:
	long m_lVersion;
	IDirectDraw4Ptr *m_lpDD4;

public:
	CFlashWnd();
	virtual ~CFlashWnd();

	virtual void OnErrorClassNotReg();
	virtual	BOOL OnBeforeShowingContent();
	virtual	BOOL OnAfterShowingContent(CString filename);
public:
	//IUnknown
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void ** ppvObject);
	ULONG STDMETHODCALLTYPE AddRef();
	ULONG STDMETHODCALLTYPE Release();

	//IDispatch
    virtual HRESULT STDMETHODCALLTYPE GetTypeInfoCount( 
        /* [out] */ UINT __RPC_FAR *pctinfo);
    virtual HRESULT STDMETHODCALLTYPE GetTypeInfo( 
        /* [in] */ UINT iTInfo,
        /* [in] */ LCID lcid,
        /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
    virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames( 
        /* [in] */ REFIID riid,
        /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
        /* [in] */ UINT cNames,
        /* [in] */ LCID lcid,
        /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
    virtual /* [local] */ HRESULT STDMETHODCALLTYPE Invoke( 
        /* [in] */ DISPID dispIdMember,
        /* [in] */ REFIID riid,
        /* [in] */ LCID lcid,
        /* [in] */ WORD wFlags,
        /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
        /* [out] */ VARIANT __RPC_FAR *pVarResult,
        /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
        /* [out] */ UINT __RPC_FAR *puArgErr);

	//DShockwaveFlashEvents
    HRESULT STDMETHODCALLTYPE OnReadyStateChange(long newState);
    HRESULT STDMETHODCALLTYPE OnProgress(long percentDone);
    HRESULT STDMETHODCALLTYPE FSCommand(_bstr_t command, _bstr_t args);
	HRESULT STDMETHODCALLTYPE CFlashWnd::FlashCall(_bstr_t request);
	//iServiceProvider

/*   HRESULT RemoteQueryService (
       GUID * guidService,
       GUID * riid,
       IUnknown * * ppvObject );*/
    HRESULT __stdcall raw_RemoteQueryService (
        GUID * guidService,
        GUID * riid,
        IUnknown * * ppvObject );
#if 0
	//ICanHandleException
	virtual HRESULT __stdcall raw_CanHandleException (
        /*[in]*/ struct EXCEPINFO * pExcepInfo,
        /*[in]*/ VARIANT * pvar );
#endif

};

