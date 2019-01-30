/*########################################################################
	Filename: 	ado.cpp
	----------------------------------------------------
	Remarks:	...
	----------------------------------------------------
	Author:		³ÉÕæ
	Email:		anyou@sina.com
				anyou@msn.com
	Created:	20/1/2003 19:19
  ########################################################################*/
#include "stdafx.h"
#include "ADO.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/*########################################################################
			  ------------------------------------------------
							    ÊýÖµÀàÐÍ×ª»»
			  ------------------------------------------------
  ########################################################################*/
COleDateTime vartodate(const _variant_t& var)
{
	COleDateTime value;
	switch (var.vt) 
	{
	case VT_DATE:
		{
			value = var.date;
		}
		break;
	case VT_EMPTY:
	case VT_NULL:
		value.SetStatus(COleDateTime::null);
		break;
	default:
		value.SetStatus(COleDateTime::null);
		TRACE(_T("Warning: Î´´¦ÀíµÄ _variant_t ÀàÐÍÖµ; ÎÄ¼þ: %s; ÐÐ: %d\n"), __FILE__, __LINE__);
	}
	return value;
}

COleCurrency vartocy(const _variant_t& var)
{
	COleCurrency value;
	switch (var.vt) 
	{
	case VT_CY:
		value = (CURRENCY)var.cyVal;
		break;
	case VT_EMPTY:
	case VT_NULL:
		value.m_status = COleCurrency::null;
		break;
	default:
		value.m_status = COleCurrency::null;
		TRACE(_T("Warning: Î´´¦ÀíµÄ _variant_t ÀàÐÍÖµ; ÎÄ¼þ: %s; ÐÐ: %d\n"), __FILE__, __LINE__);
	}
	return value;
}

bool vartobool(const _variant_t& var)
{
	bool value = false;
	switch (var.vt)
	{
	case VT_BOOL:
		value = var.boolVal ? true : false;
	case VT_EMPTY:
	case VT_NULL:
		break;
	default:
		TRACE(_T("Warning: Î´´¦ÀíµÄ _variant_t ÀàÐÍÖµ; ÎÄ¼þ: %s; ÐÐ: %d\n"), __FILE__, __LINE__);
	}
	return value;
}

BYTE vartoby(const _variant_t& var)
{
	BYTE value = 0;
	switch (var.vt)
	{
	case VT_I1:
	case VT_UI1:
		value = var.bVal;
		break;
	case VT_NULL:
	case VT_EMPTY:
		value = 0;
		break;
	default:
		TRACE(_T("Warning: Î´´¦ÀíµÄ _variant_t ÀàÐÍÖµ; ÎÄ¼þ: %s; ÐÐ: %d\n"), __FILE__, __LINE__);
	}	
	return value;
}

short vartoi(const _variant_t& var)
{
	short value = 0;
	switch (var.vt)
	{
	case VT_BOOL:
		value = var.boolVal;
		break;
	case VT_UI1:
	case VT_I1:
		value = var.bVal;
		break;
	case VT_I2:
	case VT_UI2:
		value = var.iVal;
		break;
	case VT_NULL:
	case VT_EMPTY:
		value = 0;
		break;
	default:
		TRACE(_T("Warning: Î´´¦ÀíµÄ _variant_t ÀàÐÍÖµ; ÎÄ¼þ: %s; ÐÐ: %d\n"), __FILE__, __LINE__);
	}	
	return value;
}

long vartol(const _variant_t& var)
{
	long value = 0;
	switch (var.vt)
	{
	case VT_BOOL:
		value = var.boolVal;
		break;
	case VT_UI1:
	case VT_I1:
		value = var.bVal;
		break;
	case VT_UI2:
	case VT_I2:
		value = var.iVal;
		break;
	case VT_I4:
	case VT_UI4:
		value = var.lVal;
		break;
	case VT_INT:
		value = var.intVal;
		break;
	case VT_R4:
		value = (long)(var.fltVal + 0.5);
		break;
	case VT_R8:
		value = (long)(var.dblVal + 0.5);
		break;
	case VT_DECIMAL:
		value = (long)var;
		break;
	case VT_CY:
		value = (long)var;
		break;
	case VT_BSTR://×Ö·û´®
	case VT_LPSTR://×Ö·û´®
	case VT_LPWSTR://×Ö·û´®
		value = atol((LPSTR)(_bstr_t)var);
		break;
	case VT_NULL:
	case VT_EMPTY:
		value = 0;
		break;
	default:
		break;
	}
	return value;
}

double vartof(const _variant_t& var)
{
	double value = 0;
	switch (var.vt)
	{
	case VT_R4:
		value = var.fltVal;
		break;
	case VT_R8:
		value = var.dblVal;
		break;
	case VT_DECIMAL:
		value = (double)var;
		break;
	case VT_CY:
		value = (double)var;
		break;
	case VT_BOOL:
		value = var.boolVal;
		break;
	case VT_UI1:
	case VT_I1:
		value = var.bVal;
		break;
	case VT_UI2:
	case VT_I2:
		value = var.iVal;
		break;
	case VT_UI4:
	case VT_I4:
		value = var.lVal;
		break;
	case VT_INT:
		value = var.intVal;
		break;
	case VT_BSTR://×Ö·û´®
	case VT_LPSTR://×Ö·û´®
	case VT_LPWSTR://×Ö·û´®
		value = atof((LPSTR)(_bstr_t)var);
		break;
	case VT_NULL:
	case VT_EMPTY:
		value = 0;
		break;
	default:
		value = 0;
	}
	return value;
}

CString vartostr(const _variant_t &var)
{
	CString strValue;
	
	switch (var.vt)
	{
	case VT_BSTR://×Ö·û´®
	case VT_LPSTR://×Ö·û´®
	case VT_LPWSTR://×Ö·û´®
		strValue = (LPCTSTR)(_bstr_t)var;
		break;
	case VT_I1:
	case VT_UI1:
		strValue.Format(_T("%d"), var.bVal);
		break;
	case VT_I2://¶ÌÕûÐÍ
		strValue.Format(_T("%d"), var.iVal);
		break;
	case VT_UI2://ÎÞ·ûºÅ¶ÌÕûÐÍ
		strValue.Format(_T("%d"), var.uiVal);
		break;
	case VT_INT://ÕûÐÍ
		strValue.Format(_T("%d"), var.intVal);
		break;
	case VT_I4: //ÕûÐÍ
		strValue.Format(_T("%d"), var.lVal);
		break;
	case VT_I8: //³¤ÕûÐÍ
		strValue.Format(_T("%d"), var.lVal);
		break;
	case VT_UINT://ÎÞ·ûºÅÕûÐÍ
		strValue.Format(_T("%d"), var.uintVal);
		break;
	case VT_UI4: //ÎÞ·ûºÅÕûÐÍ
		strValue.Format(_T("%d"), var.ulVal);
		break;
	case VT_UI8: //ÎÞ·ûºÅ³¤ÕûÐÍ
		strValue.Format(_T("%d"), var.ulVal);
		break;
	case VT_VOID:
		strValue.Format(_T("%8x"), var.byref);
		break;
	case VT_R4://¸¡µãÐÍ
		strValue.Format(_T("%.4f"), var.fltVal);
		break;
	case VT_R8://Ë«¾«¶ÈÐÍ
		strValue.Format(_T("%.8f"), var.dblVal);
		break;
	case VT_DECIMAL: //Ð¡Êý
		strValue.Format(_T("%.8f"), (double)var);
		break;
	case VT_CY:
		{
			COleCurrency cy = var.cyVal;
			strValue = cy.Format();
		}
		break;
	case VT_BLOB:
	case VT_BLOB_OBJECT:
	case 0x2011:
		strValue = "[BLOB]";
		break;
	case VT_BOOL://²¼¶ûÐÍ

		strValue = var.boolVal ? "TRUE" : "FALSE";
		break;
	case VT_DATE: //ÈÕÆÚÐÍ
		{
			DATE dt = var.date;
			COleDateTime da = COleDateTime(dt); 
			strValue = da.Format(_T("%Y-%m-%d %H:%M:%S"));
		}
		break;
	case VT_NULL://NULLÖµ
		strValue = "";
		break;
	case VT_EMPTY://¿Õ
		strValue = "";
		break;
	case VT_UNKNOWN://Î´ÖªÀàÐÍ
	default:
		strValue = "UN_KNOW";
		break;
	}
	return strValue;
}

/*########################################################################
			 ------------------------------------------------
					CAdoConnection class ¹¹Ôì/Îö¹¹º¯Êý
			 ------------------------------------------------
  ########################################################################*/

CAdoConnection::CAdoConnection()
{
	//´´½¨ Connection ¶ÔÏó---------------------------
	m_pConnection.CreateInstance("ADODB.Connection");
	#ifdef _DEBUG
	if (m_pConnection == NULL)
	{
//		AfxMessageBox("Connection ¶ÔÏó´´½¨Ê§°Ü! ÇëÈ·ÈÏÊÇ·ñ³õÊ¼»¯ÁËCOM»·¾³\r\n");
	}
	#endif
	ASSERT(m_pConnection != NULL);
}

CAdoConnection::~CAdoConnection()
{
	if (m_pConnection != NULL)
	{
		Release();
	}
}

/*========================================================================
	Name:		Á¬½Óµ½Êý¾ÝÔ´.
	-----------------------------------------------------
	Params:		[lpszConnect]: Á¬½Ó×Ö·û´®, °üº¬Á¬½ÓÐÅÏ¢.
				[lOptions]:	¿ÉÑ¡. ¾ö¶¨¸Ã·½·¨ÊÇÒÔÍ¬²½»¹ÊÇÒì²½µÄ·½Ê½Á¬½ÓÊý¾Ý
						Ô´. ¿ÉÒÔÊÇÈçÏÂÄ³¸ö³£Á¿:
		[³£Á¿]					[ËµÃ÷] 
		----------------------------------
		adConnectUnspecified	(Ä¬ÈÏ)Í¬²½·½Ê½´ò¿ªÁ¬½Ó. 
		adAsyncConnect			Òì²½·½Ê½´ò¿ªÁ¬½Ó. AdoÓÃ ConnectComplete ÊÂ
						¼þÀ´Í¨ÖªÒÑ¾­Íê³ÉÁ¬½Ó. 
==========================================================================*/
BOOL CAdoConnection::Open(CString lpszConnect, long lOptions)
{
	ASSERT(m_pConnection != NULL);
	ASSERT(AfxIsValidString(lpszConnect));
	
	if (!lpszConnect.IsEmpty())
	{
		m_strConnect = lpszConnect;
	}

	if (m_strConnect.IsEmpty())
	{
		ASSERT(FALSE);
		return FALSE;
	}

	if (IsOpen()) Close();

	try
	{
		// Á¬½ÓÊý¾Ý¿â ---------------------------------------------
		return (m_pConnection->Open(_bstr_t(LPCTSTR(m_strConnect)), "", "", lOptions) == S_OK);
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: Á¬½ÓÊý¾Ý¿â·¢ÉúÒì³£. ´íÎóÐÅÏ¢: %s; ÎÄ¼þ: %s; ÐÐ: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		TRACE(_T("%s\r\n"), GetLastErrorText());
		return FALSE;
	} 
	catch (...)
	{
		TRACE(_T("Warning: Á¬½ÓÊý¾Ý¿âÊ±·¢ÉúÎ´Öª´íÎó:"));
	}
	return FALSE;
}

/*========================================================================
	Name:	Á¬½Ó SQL SERVER Êý¾Ý¿â. 
	-----------------------------------------------------
	Params:		[dbsrc]:	SQL SERVER ·þÎñÆ÷Ãû.
				[dbname]:	Ä¬ÈÏµÄÊý¾Ý¿âÃû.
				[user]:		ÓÃ»§Ãû.
				[pass]:		ÃÜÂë.
==========================================================================*/
BOOL CAdoConnection::ConnectSQLServer(CString dbsrc, CString dbname, CString user, CString pass, long lOptions)
{
	CString strConnect = _T("Provider=SQLOLEDB.1; Data Source=") + dbsrc + 
						 _T("; Initial Catalog=") + dbname  +
						 _T("; User ID=") + user + 
						 _T("; PWD=") + pass;
	return Open(LPCTSTR(strConnect), lOptions);
}
BOOL CAdoConnection::ConnectDB2(CString dbsrc, CString dbname, CString user, CString pass, long lOptions)
{
	CString strConnect =  _T("Provider=IBMDADB2;PROTOCOL=TCPIP;PORT=50000;HOSTNAME=")+dbsrc+
                           _T(";Database=")+ dbname+
                           _T(";User ID=")+user+
						   _T(";Password=")+pass;
	return Open(LPCTSTR(strConnect), lOptions);
}
BOOL CAdoConnection::ConnectOracle(CString dbsrc, CString dbname, CString user, CString pass, long lOptions)
{
	CString strConnect = _T("Provider=OraOLEDB.Oracle; Data Source=") + dbsrc + 
						 _T("; User ID=") + user + 
						 _T("; Password=") + pass;
	return Open(LPCTSTR(strConnect), lOptions);
}
/*========================================================================
	Name:	Á¬½Ó SQL SERVER Êý¾Ý¿â. 
	-----------------------------------------------------
	Params:		[dbsrc]:	SQL SERVER ·þÎñÆ÷Ãû.
				[dbname]:	Ä¬ÈÏµÄÊý¾Ý¿âÃû.
				[user]:		ÓÃ»§Ãû.
				[pass]:		ÃÜÂë.
==========================================================================*/
BOOL CAdoConnection::ConnectMySQL(CString driver,CString dbsrc, CString dbname, CString user, CString pass, long lOptions /* = adConnectUnspecified */)
{
	CString strConnect = _T("DRIVER={")+ driver + _T("}; SERVER=") + dbsrc + 
						 						 _T("; DATABASE=") + dbname  +
						 				        _T("; UID=") + user + 
						 					 _T("; PWD=") + pass+"; OPTION=3";
	return Open(LPCTSTR(strConnect), lOptions);
}

/*========================================================================
	Name:	Á¬½Ó ACCESS Êý¾Ý¿â. 
	-----------------------------------------------------
	Params:		[dbpath]:	MDB Êý¾Ý¿âÎÄ¼þÂ·¾¶Ãû.
				[pass]:		·ÃÎÊÃÜÂë.
===========================================================================*/
BOOL CAdoConnection::ConnectAccess(CString dbpath, CString pass, long lOptions)
{
	CString strConnect = _T("");

	strConnect.Format(_T("Driver={Microsoft Access Driver (*.mdb)};Dbq=%s;Uid=Admin;Pwd=%s;"), dbpath,pass);


	return Open(strConnect, lOptions);
}

/*========================================================================
	Name:	Í¨¹ý udl ÎÄ¼þÁ¬½ÓÊý¾Ý¿â. 
	-----------------------------------------------------
	Params:		[strFileName]:	UDL Êý¾Ý¿âÁ¬½ÓÎÄ¼þÂ·¾¶Ãû.
==========================================================================*/
BOOL CAdoConnection::OpenUDLFile(LPCTSTR strFileName, long lOptions)
{
	CString strConnect = _T("File Name=");
	strConnect += strFileName;
	return Open(LPCTSTR(strConnect), lOptions);
}

/*========================================================================
	Name:	¹Ø±ÕÓëÊý¾ÝÔ´µÄÁ¬½Ó.
	-----------------------------------------------------
	Remarks: Ê¹ÓÃ Close ·½·¨¿É¹Ø±Õ Connection ¶ÔÏóÒÔ±ãÊÍ·ÅËùÓÐ¹ØÁªµÄÏµÍ³×ÊÔ´. 
==========================================================================*/
void CAdoConnection::Close()
{
	try
	{
		if (m_pConnection != NULL && IsOpen()) 
		{
			m_pConnection->Close();
		}
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: ¹Ø±ÕÊý¾Ý¿â·¢ÉúÒì³£. ´íÎóÐÅÏ¢: %s; ÎÄ¼þ: %s; ÐÐ: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
	} 
}

/*========================================================================
	Name:	¹Ø±ÕÁ¬½Ó²¢ÊÍ·Å¶ÔÏó.
	-----------------------------------------------------
	Remarks: ¹Ø±ÕÁ¬½Ó²¢ÊÍ·Åconnection¶ÔÏó.
==========================================================================*/
void CAdoConnection::Release()
{
	if (IsOpen()) Close();
	m_pConnection.Release();
}

/*========================================================================
	Name:		Ö´ÐÐÖ¸¶¨µÄ²éÑ¯¡¢SQL Óï¾ä¡¢´æ´¢¹ý³ÌµÈ.
    ----------------------------------------------------------
	Remarks:	Çë²Î¿¼ CAdoRecordSet ÀàµÄOpen·½·¨. ·µ»ØµÄ Recordset ¶ÔÏóÊ¼
			ÖÕÎªÖ»¶Á¡¢½öÏòÇ°µÄÓÎ±ê.
==========================================================================*/
_RecordsetPtr CAdoConnection::Execute(LPCTSTR lpszSQL, long lOptions)
{
	ASSERT(m_pConnection != NULL);
	ASSERT(AfxIsValidString(lpszSQL));

	try
	{
		return m_pConnection->Execute(_bstr_t(lpszSQL), NULL, lOptions);
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: Execute ·½·¨·¢ÉúÒì³£. ´íÎóÐÅÏ¢: %s; ÎÄ¼þ: %s; ÐÐ: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
	return FALSE;
}

/*========================================================================
	Remarks:	Çë²Î¿¼ CAdoRecordSet Àà Cancel ·½·¨.
==========================================================================*/
BOOL CAdoConnection::Cancel()
{
	ASSERT(m_pConnection != NULL);
	try
	{
		return m_pConnection->Cancel();
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: Cancel ·½·¨·¢ÉúÒì³£. ´íÎóÐÅÏ¢: %s; ÎÄ¼þ: %s; ÐÐ: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
	return FALSE;
}

/*========================================================================
	Name:	È¡µÃ×îºó·¢ÉúµÄ´íÎóÐÅÏ¢.
==========================================================================*/
CString CAdoConnection::GetLastErrorText()
{
	ASSERT(m_pConnection != NULL);
	CString strErrors = _T("");
	try
	{
		if (m_pConnection != NULL)
		{
			ErrorsPtr pErrors = m_pConnection->Errors;
			CString strError;
			for (long n = 0; n < pErrors->Count; n++)
			{
				ErrorPtr pError = pErrors->GetItem(n);
				strError.Format(_T("Description: %s\r\nState: %s, Native: %d, Source: %s\r\n"),
								(LPCTSTR)pError->Description,
								(LPCTSTR)pError->SQLState,
										 pError->NativeError,
								(LPCTSTR)pError->Source);
				strErrors += strError;
			}
		}
		return strErrors;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetLastError ·½·¨·¢ÉúÒì³£. ´íÎóÐÅÏ¢: %s; ÎÄ¼þ: %s; ÐÐ: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return strErrors;
	} 
	return strErrors;
}

ErrorsPtr CAdoConnection::GetErrors()
{
	ASSERT(m_pConnection != NULL);
	try
	{
		if (m_pConnection != NULL)
		{
			return m_pConnection->Errors;
		}
		return NULL;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetErrors ·½·¨·¢ÉúÒì³£. ´íÎóÐÅÏ¢: %s; ÎÄ¼þ: %s; ÐÐ: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return NULL;
	} 
	return NULL;
}

ErrorPtr CAdoConnection::GetError(long index)
{
	ASSERT(m_pConnection != NULL);
	try
	{
		if (m_pConnection != NULL)
		{
			ErrorsPtr pErrors =  m_pConnection->Errors;
			if (index >= 0 && index < pErrors->Count)
			{
				return pErrors->GetItem(_variant_t(index));
			}
		}
		return NULL;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetError ·½·¨·¢ÉúÒì³£. ´íÎóÐÅÏ¢: %s; ÎÄ¼þ: %s; ÐÐ: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return NULL;
	} 
	return NULL;
}

/*========================================================================
	Name:		È¡µÃÁ¬½ÓÊ±¼ä.
==========================================================================*/
long CAdoConnection::GetConnectTimeOut()
{
	ASSERT(m_pConnection != NULL);
	try
	{
		return m_pConnection->GetConnectionTimeout();
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetConnectTimeOut ·½·¨·¢ÉúÒì³£. ´íÎóÐÅÏ¢: %s; ÎÄ¼þ: %s; ÐÐ: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return -1;
	} 
	return -1;
}

/*========================================================================
	Name:		ÉèÖÃÁ¬½ÓÊ±¼ä.
==========================================================================*/
BOOL CAdoConnection::SetConnectTimeOut(long lTime)
{
	ASSERT(m_pConnection != NULL);
	try
	{
		m_pConnection->PutConnectionTimeout(lTime);
		return TRUE;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: SetConnectTimeOut ·½·¨·¢ÉúÒì³£. ´íÎóÐÅÏ¢: %s; ÎÄ¼þ: %s; ÐÐ: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
}

/*========================================================================
	Name:		È¡µÃÄ¬ÈÏÊý¾Ý¿âµÄÃû³Æ.
==========================================================================*/
CString CAdoConnection::GetDefaultDatabase()
{
	ASSERT(m_pConnection != NULL);
	try
	{
		return CString(LPCTSTR(_bstr_t(m_pConnection->GetDefaultDatabase())));
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetDefaultDatabase ·½·¨·¢ÉúÒì³£. ´íÎóÐÅÏ¢: %s; ÎÄ¼þ: %s; ÐÐ: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return CString(_T(""));
	} 
}

/*========================================================================
	Name:		È¡µÃ Connection ¶ÔÏóÌá¹©ÕßµÄÃû³Æ.
==========================================================================*/
CString CAdoConnection::GetProviderName()
{
	ASSERT(m_pConnection != NULL);
	try
	{
		return CString(LPCTSTR(_bstr_t(m_pConnection->GetProvider())));
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetProviderName ·½·¨·¢ÉúÒì³£. ´íÎóÐÅÏ¢: %s; ÎÄ¼þ: %s; ÐÐ: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return CString(_T(""));
	} 
}

/*========================================================================
	Name:		È¡µÃ ADO µÄ°æ±¾ºÅ
==========================================================================*/
CString CAdoConnection::GetVersion()
{
	ASSERT(m_pConnection != NULL);
	try
	{
		return CString(LPCTSTR(_bstr_t(m_pConnection->GetVersion())));
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetVersion ·½·¨·¢ÉúÒì³£. ´íÎóÐÅÏ¢: %s; ÎÄ¼þ: %s; ÐÐ: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return CString(_T(""));
	} 
}

/*========================================================================
	Name:		È¡µÃ¶ÔÏóµÄ×´Ì¬(Í¬ Recordset ¶ÔÏóµÄ GetState ·½·¨).
	-----------------------------------------------------
	returns:	·µ»ØÏÂÁÐ³£Á¿Ö®Ò»µÄ³¤ÕûÐÍÖµ.
		[³£Á¿]				[ËµÃ÷] 
		----------------------------------
		adStateClosed		Ö¸Ê¾¶ÔÏóÊÇ¹Ø±ÕµÄ. 
		adStateOpen			Ö¸Ê¾¶ÔÏóÊÇ´ò¿ªµÄ. 
	-----------------------------------------------------
	Remarks:		¿ÉÒÔËæÊ±Ê¹ÓÃ State ÊôÐÔÈ¡µÃÖ¸¶¨¶ÔÏóµÄµ±Ç°×´Ì¬.
==========================================================================*/
long CAdoConnection::GetState()
{
	ASSERT(m_pConnection != NULL);
	try
	{
		return m_pConnection->GetState();
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetState ·¢ÉúÒì³£. ´íÎóÐÅÏ¢: %s; ÎÄ¼þ: %s; ÐÐ: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return -1;
	} 
	return -1;
}

/*========================================================================
	Name:	¼ì²âÁ¬½Ó¶ÔÏóÊÇ·ñÎª´ò¿ª×´Ì¬.
==========================================================================*/
BOOL CAdoConnection::IsOpen()
{
	try
	{
		return (m_pConnection != NULL && (m_pConnection->State & adStateOpen));
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: IsOpen ·½·¨·¢ÉúÒì³£. ´íÎóÐÅÏ¢: %s; ÎÄ¼þ: %s; ÐÐ: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
	return FALSE;
}

/*========================================================================
	Name:		È¡µÃÔÚ Connection ¶ÔÏóÖÐÐÞ¸ÄÊý¾ÝµÄ¿ÉÓÃÈ¨ÏÞ.
    ----------------------------------------------------------
	returns:	·µ»ØÒÔÏÂÄ³¸ö ConnectModeEnum µÄÖµ.
		[³£Á¿]				 [ËµÃ÷] 
		----------------------------------
		adModeUnknown		 Ä¬ÈÏÖµ. ±íÃ÷È¨ÏÞÉÐÎ´ÉèÖÃ»òÎÞ·¨È·¶¨. 
		adModeRead			 ±íÃ÷È¨ÏÞÎªÖ»¶Á. 
		adModeWrite			 ±íÃ÷È¨ÏÞÎªÖ»Ð´. 
		adModeReadWrite		 ±íÃ÷È¨ÏÞÎª¶Á/Ð´. 
		adModeShareDenyRead  ·ÀÖ¹ÆäËûÓÃ»§Ê¹ÓÃ¶ÁÈ¨ÏÞ´ò¿ªÁ¬½Ó. 
		adModeShareDenyWrite ·ÀÖ¹ÆäËûÓÃ»§Ê¹ÓÃÐ´È¨ÏÞ´ò¿ªÁ¬½Ó. 
		adModeShareExclusive ·ÀÖ¹ÆäËûÓÃ»§´ò¿ªÁ¬½Ó. 
		adModeShareDenyNone  ·ÀÖ¹ÆäËûÓÃ»§Ê¹ÓÃÈÎºÎÈ¨ÏÞ´ò¿ªÁ¬½Ó.
    ----------------------------------------------------------
	Remarks: Ê¹ÓÃ Mode ÊôÐÔ¿ÉÉèÖÃ»ò·µ»Øµ±Ç°Á¬½ÓÉÏÌá¹©ÕßÕýÔÚÊ¹ÓÃµÄ·ÃÎÊÈ¨ÏÞ.
==========================================================================*/
ConnectModeEnum CAdoConnection::GetMode()
{
	ASSERT(m_pConnection != NULL);
	try
	{
		return m_pConnection->GetMode();
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetMode ·¢ÉúÒì³£. ´íÎóÐÅÏ¢: %s; ÎÄ¼þ: %s; ÐÐ: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return adModeUnknown;
	} 
	return adModeUnknown;
}

/*========================================================================
	Name:		ÉèÖÃÔÚ Connection ÖÐÐÞ¸ÄÊý¾ÝµÄ¿ÉÓÃÈ¨ÏÞ. Çë²Î¿¼ GetMode ·½·¨.
    ----------------------------------------------------------
	Remarks:	Ö»ÄÜÔÚ¹Ø±Õ Connection ¶ÔÏóÊ±·½¿ÉÉèÖÃ Mode ÊôÐÔ.
==========================================================================*/
BOOL CAdoConnection::SetMode(ConnectModeEnum mode)
{
	ASSERT(m_pConnection != NULL);
	ASSERT(!IsOpen());

	try
	{
		m_pConnection->PutMode(mode);
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: SetMode ·¢ÉúÒì³£. ´íÎóÐÅÏ¢: %s; ÎÄ¼þ: %s; ÐÐ: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
	return TRUE;
}

/*========================================================================
    Name:		´ÓÊý¾ÝÔ´»ñÈ¡Êý¾Ý¿âÐÅÏ¢.
	-----------------------------------------------------
	Params:		QueryType  ËùÒªÔËÐÐµÄÄ£Ê½²éÑ¯ÀàÐÍ.
	-----------------------------------------------------
	returns:	·µ»Ø°üº¬Êý¾Ý¿âÐÅÏ¢µÄ Recordset ¶ÔÏó. Recordset ½«ÒÔÖ»¶Á¡¢¾²Ì¬
			ÓÎ±ê´ò¿ª.
==========================================================================*/
_RecordsetPtr CAdoConnection::OpenSchema(SchemaEnum QueryType)
{
	ASSERT(m_pConnection != NULL);
	try
	{
		return m_pConnection->OpenSchema(QueryType, vtMissing, vtMissing);
	}
	catch(_com_error e)
	{
		TRACE(_T("Warning: OpenSchema·½·¨·¢ÉúÒì³£. ´íÎóÐÅÏ¢: %s; ÎÄ¼þ: %s; ÐÐ: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return NULL;
	} 
	return NULL;
}

/*########################################################################
			  ------------------------------------------------
									ÊÂÎñ´¦Àí
			  ------------------------------------------------
  ########################################################################*/

/*========================================================================
    Name:		¿ªÊ¼ÐÂÊÂÎñ.
	-----------------------------------------------------
	returns:	¶ÔÓÚÖ§³ÖÇ¶Ì×ÊÂÎñµÄÊý¾Ý¿âÀ´Ëµ, ÔÚÒÑ´ò¿ªµÄÊÂÎñÖÐµ÷ÓÃ BeginTrans 
	·½·¨½«¿ªÊ¼ÐÂµÄÇ¶Ì×ÊÂÎñ. ·µ»ØÖµ½«Ö¸Ê¾Ç¶Ì×²ã´Î: ·µ»ØÖµÎª 1 ±íÊ¾ÒÑ´ò¿ª¶¥²ã
	ÊÂÎñ (¼´ÊÂÎñ²»±»ÁíÒ»¸öÊÂÎñËùÇ¶Ì×), ·µ»ØÖµÎª 2 ±íÊ¾ÒÑ´ò¿ªµÚ¶þ²ãÊÂÎñ(Ç¶Ì×
	ÔÚ¶¥²ãÊÂÎñÖÐµÄÊÂÎñ), ÒÀ´ÎÀàÍÆ.
	-----------------------------------------------------
	Remarks:	Ò»µ©µ÷ÓÃÁË BeginTrans ·½·¨, ÔÚµ÷ÓÃ CommitTrans »ò RollbackTrans
	½áÊøÊÂÎñÖ®Ç°, Êý¾Ý¿â½«²»ÔÙÁ¢¼´Ìá½»Ëù×÷µÄÈÎºÎ¸ü¸Ä.
		
==========================================================================*/
long CAdoConnection::BeginTrans()
{
	ASSERT(m_pConnection != NULL);
	try
	{
		return m_pConnection->BeginTrans();
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: BeginTrans ·½·¨·¢ÉúÒì³£. ´íÎóÐÅÏ¢: %s; ÎÄ¼þ: %s; ÐÐ: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return -1;
	} 
	return -1;
}

/*========================================================================
    Name:		±£´æÈÎºÎ¸ü¸Ä²¢½áÊøµ±Ç°ÊÂÎñ.
	-----------------------------------------------------
	Remarks:	µ÷ÓÃ CommitTrans »ò RollbackTrans Ö»Ó°Ïì×îÐÂ´ò¿ªµÄÊÂÎñ; ÔÚ
	´¦ÀíÈÎºÎ¸ü¸ß²ãÊÂÎñÖ®Ç°±ØÐë¹Ø±Õ»ò»Ø¾íµ±Ç°ÊÂÎñ.
==========================================================================*/
BOOL CAdoConnection::CommitTrans()
{
	ASSERT(m_pConnection != NULL);
	try
	{
		return SUCCEEDED(m_pConnection->CommitTrans());
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: CommitTrans ·½·¨·¢ÉúÒì³£. ´íÎóÐÅÏ¢: %s; ÎÄ¼þ: %s; ÐÐ: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
	return FALSE;
}

/*========================================================================
    Name:		È¡Ïûµ±Ç°ÊÂÎñÖÐËù×÷µÄÈÎºÎ¸ü¸Ä²¢½áÊøÊÂÎñ.
==========================================================================*/
BOOL CAdoConnection::RollbackTrans()
{
	ASSERT(m_pConnection != NULL);
	try
	{
		return SUCCEEDED(m_pConnection->RollbackTrans());
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: RollbackTrans ·½·¨·¢ÉúÒì³£. ´íÎóÐÅÏ¢: %s; ÎÄ¼þ: %s; ÐÐ: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
	return FALSE;
}


