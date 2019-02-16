// yoda_self_ordering.h : main header file for the yoda_self_ordering application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


// Cyoda_self_orderingApp:
// See yoda_self_ordering.cpp for the implementation of this class
//
typedef struct  Categroy_Info
{
	Categroy_Info()
	{
		nCategroyID = 0;
		szCategroyName = _T("");
		szCategroyNameCN = _T("");
		szCateGroyFileName = _T("");
		szCateGroyFileNameCN = _T("");
		text_clr = CBCGPColor::Black;
		background_clr = CBCGPColor::Wheat;
	}
	int nCategroyID;
	CString szCategroyName;
	CString szCategroyNameCN;
	CString szCateGroyFileName;
	CString szCateGroyFileNameCN;
	CBCGPColor text_clr;
	CBCGPColor background_clr;
}CATEGROYINFO, *LPCATEGROYINFO;
struct PRODUCTINFO
{
	PRODUCTINFO()
	{
		nCategroyID = 0;
		nProductID = 0;
		szProductName = _T("");
		szSize = _T("M");
		szICE = _T("NORMAL ICE");
		szSugar = _T("50%");
		szHoney = _T("100%");
		dbMoney = 0.;
		szProductFileName = _T("");
		szTopping = _T("");
		text_clr = CBCGPColor::Black;
		background_clr = CBCGPColor::Wheat;
		bShowHoneyLevel = FALSE;
		bShowSugarLevel = TRUE;
		bShowSize = TRUE;
		bShowIce = TRUE;
		nToppingCounts = 0;
	}
	void clear()
	{
		nCategroyID = 0;
		nProductID = 0;
		szProductName = _T("");
		szProductNameCN = _T("");
		szSize = _T("M");
		szICE = _T("NORMAL ICE");
		szSugar = _T("50%");
		szHoney = _T("100%");
		dbMoney = 0.;
		szProductFileName = _T("");
		szProductFileNameCN = _T("");
		szTopping = _T("");
		text_clr = CBCGPColor::Black;
		background_clr = CBCGPColor::Wheat;
		bShowHoneyLevel = FALSE;
		bShowSugarLevel = TRUE;
		bShowSize = TRUE;
		bShowIce = TRUE;
		nToppingCounts = 0;
	}
public:
	int				nCategroyID;
	int             nProductID;
	int				nToppingCounts;
	CString			szProductName;
	CString			szProductNameCN;
	CString			szSize;
	CString			szICE;
	CString			szSugar;
	CString			szHoney;
	CString			szTopping;
	CString			szToppingMoney;
	double			dbMoney;
	double			dbTeaMoney;
	CString			szProductFileName;
	CString			szProductFileNameCN;
	CString			szProductThumbFile;
	CBCGPColor		text_clr;
	CBCGPColor		background_clr;
	BOOL			bShowHoneyLevel;
	BOOL			bShowSugarLevel;
	BOOL			bShowSize;
	BOOL			bShowIce;
};

typedef struct HeaderOrderInfo
{
	HeaderOrderInfo()
	{
		//productInfo = NULL;
		nOrderID = 1;
		nProductCounts = 1;
		bEdit = FALSE;
	}
	int				nOrderID;
	PRODUCTINFO     productInfo;
	int             nProductCounts;
	BOOL			bEdit;
}ORDERINFO, *LPORDERINFO;
enum ORDERSTATUS {
	ORDER_INIT      = 0,
	ORDER_COUNTER   = 1,
	ORDER_PAID      = 2, 
	ORDER_WORKING   = 3,
	ORDER_COMPLETED = 4
};
class Cyoda_self_orderingApp : public CBCGPWinApp
{
public:
	Cyoda_self_orderingApp();
public:
	BOOL ConnectDB();
	BOOL IsConnection();
	CBCGPColor GetBrushColor(const CBCGPBrush& br);
// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
// Implementation
	DECLARE_MESSAGE_MAP()
public:
	ULONG_PTR m_gdiplusToken;

};

extern Cyoda_self_orderingApp theApp;
BOOL PrintReceipt(bool bCounter);
long getFreeID(CAdoConnection *pDB, const CString szTable);
long getnewFreeID(CAdoConnection *pDB, const CString szTable, CString sColName);
int	IsHaveSameID(CAdoConnection *pDB, CString sColName, CString sTableName, CString szValue);
bool execSQL(CAdoConnection *pDB, CString &szSQL);
BOOL GenerateQRCode(CString szText);