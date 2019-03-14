/******************************************************************
***
***
***				FREE WINDOWLESS FLASH CONTROL
***
***			with special thanks to Makarov Igor
***
***			adapted by Fred van Lieshout:
***         - improved CFlashWnd::Invoke() 
***			- improved CFlashWnd::FlashCall()
***         - added CFlashWnd::addNumbers() for demo
***
***		for questions and remarks mailto: mak_july@list.ru
***
*******************************************************************/
// FlashWnd.cpp: implementation of the COleContainerWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FlashWnd.h"

#include <direct.h>
#include <comutil.h>

//#include "../tinyxml/tinyxml.h"


//DirectDraw GUIDS

DEFINE_GUID2(CLSID_DirectDraw,0xD7B70EE0,0x4340,0x11CF,0xB0,0x63,0x00,0x20,0xAF,0xC2,0xCD,0x35);
DEFINE_GUID2(CLSID_DirectDraw7,0x3c305196,0x50db,0x11d3,0x9c,0xfe,0x00,0xc0,0x4f,0xd9,0x30,0xc5);

DEFINE_GUID2(IID_IDirectDraw,0x6C14DB80,0xA733,0x11CE,0xA5,0x21,0x00,0x20,0xAF,0x0B,0xE5,0x60);
DEFINE_GUID2(IID_IDirectDraw3,0x618f8ad4,0x8b7a,0x11d0,0x8f,0xcc,0x0,0xc0,0x4f,0xd9,0x18,0x9d);
DEFINE_GUID2(IID_IDirectDraw4,0x9c59509a,0x39bd,0x11d1,0x8c,0x4a,0x00,0xc0,0x4f,0xd9,0x30,0xc5);
DEFINE_GUID2(IID_IDirectDraw7,0x15e65ec0,0x3b9c,0x11d2,0xb9,0x2f,0x00,0x60,0x97,0x97,0xea,0x5b);


CFlashWnd::CFlashWnd()
{
	m_lVersion = 0;
	m_lpDD4 = NULL;
}

CFlashWnd::~CFlashWnd()
{
	if (m_lpDD4)
		m_lpDD4->Release();
}

//DShockwaveFlashEvents
HRESULT STDMETHODCALLTYPE CFlashWnd::OnReadyStateChange(long newState)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CFlashWnd::OnProgress(long percentDone)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CFlashWnd::FSCommand(_bstr_t command, _bstr_t args)
{
	return S_OK;
}

//
// Handle a call from Flash ActionScript (in .swf file)
//
// Flash marshalls function calls to XML, for example:
// request: <invoke name="addNumbers" returntype="xml"><arguments><number>0</number><number>1</number></arguments></invoke>
//
HRESULT STDMETHODCALLTYPE CFlashWnd::FlashCall(_bstr_t request)
{
	
	HRESULT hr = S_FALSE;
/*
	if (m_lpControl != NULL)
	{
		TiXmlDocument doc;
		const char *c_str = _com_util::ConvertBSTRToString(request);
		doc.Parse(c_str);
		delete[] c_str;

		TiXmlHandle hDoc(&doc);
		TiXmlElement *pInvokeElement = hDoc.FirstChildElement("invoke").Element();
		if (pInvokeElement != NULL)
		{
			std::string functionName;
			int result = pInvokeElement->QueryStringAttribute("name", &functionName);
			if (result == 0)
			{
				if (functionName == "addNumbers")
				{
					hr = addNumbers(pInvokeElement);
				}
			}
		}
	}*/
	return hr;
}

void CFlashWnd::OnErrorClassNotReg()
{
	//some notification code here
}

BOOL CFlashWnd::OnBeforeShowingContent()
{
	HRESULT hr;

	m_lVersion = m_lpControl->FlashVersion();
	if ((m_lVersion & 0x00FF0000) == 0x00080000)
		m_bFixTransparency = TRUE;
	else
		m_bFixTransparency = FALSE;
	
	hr = m_lpControl->QueryInterface(IID_IConnectionPointContainer, (void**)&m_lpConCont);
	if (FAILED(hr))
		return FALSE;

	hr = m_lpConCont->FindConnectionPoint(ShockwaveFlashObjects::DIID__IShockwaveFlashEvents, &m_lpConPoint);
	if (FAILED(hr))
		return FALSE;

	hr = m_lpConPoint->Advise((ShockwaveFlashObjects::_IShockwaveFlashEvents *)this, &m_dwConPointID);
	if (FAILED(hr))
		return FALSE;

	if (m_bTransparent)
		m_lpControl->PutWMode(L"transparent");

	m_lpControl->PutScale(L"showAll");
	//m_lpControl->PutBackgroundColor(0x00000000);

	return TRUE;
}

BOOL CFlashWnd::OnAfterShowingContent(CString filename)
{
	HRESULT hr;
// 	WCHAR swf_filepath[512]={0};
	USES_CONVERSION;
// 	LPWSTR pText = T2W(filename);
// 	swprintf_s(swf_filepath,512, L"%s", pText);
	m_lpControl->PutEmbedMovie(TRUE);

	hr = m_lpControl->LoadMovie(0, /*swf_filepath*/filename.GetBuffer());
	if (FAILED(hr))
		return FALSE;

// 	hr = m_lpControl->Play();
// 	if (FAILED(hr))
// 		return FALSE;

	// For demonstration: call a (registered) callback function in the Flash ActionScript
//      BSTR _result = m_lpControl->CallFunction(L"<invoke name=\"setButtonText\" returntype=\"xml\"><arguments><string>Click me!</string></arguments></invoke>");

	return TRUE;
}

//
// Methods for interface 'IUnknown'
//
HRESULT STDMETHODCALLTYPE CFlashWnd::QueryInterface(REFIID riid, void ** ppvObject)
{
	HRESULT hr = COleContainerWnd<ShockwaveFlashObjects::IShockwaveFlash>::QueryInterface(riid, ppvObject);
	if (hr != E_NOINTERFACE)
		return hr;

	if (IsEqualGUID(riid, ShockwaveFlashObjects::DIID__IShockwaveFlashEvents))
	{
		*ppvObject = (void*)dynamic_cast<ShockwaveFlashObjects::_IShockwaveFlashEvents *>(this);
	}
	else if (IsEqualGUID(riid, ShockwaveFlashObjects::IID_IServiceProvider))
	{
		*ppvObject = (void*)dynamic_cast<ShockwaveFlashObjects::IServiceProvider *>(this);
	}
	else if (IsEqualGUID(riid, ShockwaveFlashObjects::IID_ICanHandleException))
	{
		//*ppvObject = (void*)dynamic_cast<ShockwaveFlashObjects::ICanHandleException *>(this);
	}
	else
	{
		*ppvObject = 0;
		return E_NOINTERFACE;
	}
	if (!(*ppvObject))
		return E_NOINTERFACE; //if dynamic_cast returned 0

	m_iRef++;
	return S_OK;
}

ULONG STDMETHODCALLTYPE CFlashWnd::AddRef()
{
	m_iRef++;
	return m_iRef;
}

ULONG STDMETHODCALLTYPE CFlashWnd::Release()
{
	m_iRef--;
	return m_iRef;
}

//
// Methods for interface 'IDispatch'
//
HRESULT STDMETHODCALLTYPE CFlashWnd::GetTypeInfoCount(UINT __RPC_FAR *pctinfo)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE CFlashWnd::GetTypeInfo( 
    /* [in] */ UINT iTInfo,
    /* [in] */ LCID lcid,
    /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE CFlashWnd::GetIDsOfNames( 
    /* [in] */ REFIID riid,
    /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
    /* [in] */ UINT cNames,
    /* [in] */ LCID lcid,
    /* [size_is][out] */ DISPID __RPC_FAR *rgDispId)
{
	NOTIMPLEMENTED;
}

/*!
	\brief Callback handler for interface '_IShockwaveFlashEvents' (AKA 'Sink' in COM/Windows lingo)
*/
HRESULT STDMETHODCALLTYPE CFlashWnd::Invoke( 
    /* [in] */ DISPID dispIdMember,
    /* [in] */ REFIID riid,
    /* [in] */ LCID lcid,
    /* [in] */ WORD wFlags,
    /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
    /* [out] */ VARIANT __RPC_FAR *pVarResult,
    /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
    /* [out] */ UINT __RPC_FAR *puArgErr)
{
	if (wFlags == DISPATCH_METHOD)
	{
		switch (dispIdMember)          
		{          
		case 0xc5: // FlashCall (from ActionScript)
			if (pDispParams->cArgs != 1 || pDispParams->rgvarg[0].vt != VT_BSTR) 
				return E_INVALIDARG;
			return this->FlashCall(pDispParams->rgvarg[0].bstrVal);
			break;

		case 0x96: // FSCommand (from ActionScript)
			if (pDispParams->cArgs != 2 || pDispParams->rgvarg[0].vt != VT_BSTR || pDispParams->rgvarg[1].vt != VT_BSTR) 
				return E_INVALIDARG;
			return this->FSCommand(pDispParams->rgvarg[1].bstrVal, pDispParams->rgvarg[0].bstrVal);
			break;

		case 0x7a6: // OnProgress                  
			return OnProgress(pDispParams->rgvarg[0].intVal);
			break;

		case DISPID_READYSTATECHANGE:                  
			return E_NOTIMPL;
			break;
		}
	}

	return E_NOTIMPL;
}
//提交重画的性能
HRESULT __stdcall CFlashWnd::raw_RemoteQueryService (
       GUID * guidService,
       GUID * riid,
       IUnknown * * ppvObject )
{
	HRESULT hr;
	if (IsEqualGUID(*guidService, IID_IDirectDraw3))
	{
		if (!m_lpDD4)
		{
			m_lpDD4 = new IDirectDraw4Ptr;
			hr = m_lpDD4->CreateInstance(CLSID_DirectDraw, NULL, CLSCTX_INPROC_SERVER); 
			if (FAILED(hr))
			{
				delete m_lpDD4;
				m_lpDD4 = NULL;
				return E_NOINTERFACE;
			}
		}
		if (m_lpDD4 && m_lpDD4->GetInterfacePtr())
		{
			*ppvObject = m_lpDD4->GetInterfacePtr();
			m_lpDD4->AddRef();
			return S_OK;
		}
	}
	return E_NOINTERFACE;
}

#if 0
//ICanHandleException
HRESULT __stdcall CFlashWnd::raw_CanHandleException (
        /*[in]*/ struct EXCEPINFO * pExcepInfo,
        /*[in]*/ VARIANT * pvar )
{
	return S_OK;
}
#endif
