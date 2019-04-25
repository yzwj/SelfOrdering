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
#define ID_VIEW_HOME					0
#define ID_VIEW_CATEGORY				1
#define ID_VIEW_PRODUCT					2
#define ID_VIEW_ICE_SUGAR_TOPPING		3
#define ID_VIEW_ICE_HONEY_TOPPING		4
#define ID_VIEW_ICE_TOPPING				5
#define ID_VIEW_SIZE_TOPPING			6
#define ID_VIEW_TOPPING					7
#define ID_VIEW_CHECK_ORDERING			8
#define ID_VIEW_PAYMENT					9
enum paymentModes
{
	UNKNOW = 0,
	NETS = 12,
	EWALLET =13,
};
typedef struct Topping_info
{
	int nItemId;
	double ItemPrice;
	CString szItemCode;
	CString szItemName;
	CString szItemNameCN;
}TOPPINGINFO, *LPTOPPINGINFO;
typedef struct Size_info
{
	int nItemId;
	double ItemPrice;
	CString szItemCode;
	CString szItemName;
	CString szItemNameCN;
}SIZEINFO, *LPSIZEINFO;
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
		szSugar = _T("75%");
		szSugarCode = "";
		szHoney = _T("100%");
		szHoneyCode = "";
		dbMoney = dbDiscount = dbSizeMoney = dbMoney = 0.;
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
		szSugar = _T("75%");
		szHoney = _T("100%");
		dbSizeMoney = dbMoney = 0.;
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
	int				nProductCounts;
	double			dbMoney;
	double			dbSizeMoney;
	double			dbUnitMenoy;
	double			dbDiscount;
	CString			szItemCode;
	CString			szProductName;
	CString			szProductNameCN;
	CString			szSize;
	CString			szSizeCode;
	CString			szICE;
	CString			szICECode;
	CString			szSugar;
	CString			szSugarCode;
	CString			szHoney;
	CString			szHoneyCode;
	std::vector<CString>    ToppingArr;
	CString			szTopping;
	CString			szToppingMoney;
	CString			szProductFileName;
	CString			szProductFileNameCN;
	CString			szProductThumbFile;
	CString			szPrinter;
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
		nOrderID = 0;
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
enum TRANS_STATUS {
	TRANS_INIT = 0,
	TRANS_START = 1,
	TRANS_PROCESSING = 2,
	TRANS_END = 3,
};
enum voucherStatus {
	processing,
	waiting,
	launched,
	completed,
	archived,
};
enum voucherType {
	Product,
	Upgrade,
	Cash,
	Discount,
	BuyOneGetOne,
	Toppings
};
typedef struct voucher_info {
	int VoucherID;
	CString szName;
	CString szDescription;
	CString szInvicode;
	voucherType type;
	int limitProductNumber;
	double discount_percent;
	double discount_price;
	bool size_upgrade;
	std::vector<int> limitRedeemOutlet;
	std::vector<int> limitRedeemProduct;
	std::vector<int> limitRedeemCategroy;
	std::vector<int> limitRedeemTopping;
	int number_purchase;
	int number_complimentary_drinks;
	int nProductSize;
	int nToppings;
	CString szEffectiveDate;
	CString ExpiringDate;
	voucherStatus status;
}VOUCHERINFO, *LPVOUCHERINFO;
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
BOOL ImageFromIDResource(UINT nID, LPCTSTR sTR, Image *&pImg);
BOOL SaveOrder(ORDERSTATUS orderStatus);
int GetCurOrderPayTimes();
BOOL UpdateOrderStatus(ORDERSTATUS orderStatus);
double GetToppingPrice(CString szItemCode);
double GetSizePrice(CString szItemCode);
CString GetToppingName(CString szItemCode);
int GetOrderCounts();
bool printTicket(CString szPrinter);
int GetVoucherInfo(CString szCode);
int GetSizeID(CString szItemCode);
int GetToppingID(CString szItemCode);
BOOL VoucherEnable(LPVOUCHERINFO voucher);
double RedeemVoucherIncomePrice(int nVoucherID, double Amount);