#include "stdafx.h"
#include "yoda_self_ordering.h"
#include "ToppingView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif
extern PRODUCTINFO gCurProduct;

#define ID_PRODUCT_TEXT					101
#define ID_SIZE_TEXT					102
#define ID_MONEY_TEA					103
#define ID_ICE_TEXT						104
#define ID_SUGAR_TEXT					105
#define ID_TOPPING_TEXT					106
#define ID_SELECT_TEXT					107
#define ID_ICE_LABEL					108
#define ID_SUGAR_LABEL					109
#define ID_IMAGE_PRODUCT				110
#define ID_IMAGE_MORE_ICE				111
#define ID_IMAGE_NORMAL_ICE				112
#define ID_IMAGE_LESS_ICE				113
#define ID_IMAGE_WARM_WATER				114
#define ID_IMAGE_FILTER_ICE				116
#define ID_IMAGE_ROOM_TEMP				117
#define ID_SWITCH_SIZE					118
#define ID_SWITCH_SIZE_LABEL			119
#define ID_TOPPING_PEARL				130
#define ID_TOPPING_WHITE_PEARL			131
#define ID_TOPPING_AIYU					132
#define ID_TOPPING_MANGO_PUDDING		133
#define ID_TOPPING_GRASS_JELLY			134
#define ID_TOPPING_COCONUT_JELLY		135
#define ID_TOPPING_ICE_CREAM			136
#define ID_TOPPING_ALEO_VERA			137
#define ID_TOPPING_GREEN_APPLE			138
#define ID_TOPPING_RED_BEAN				139
#define ID_TOPPING_OREO					140
#define ID_SIZE_NORMAL					141
#define ID_SIZE_LARGE					142
#define ID_SIZE_TALL_L					143

#define ID_MONEY_TOPPING				150
#define ID_MONEY_TOTAL					151
#define ID_NO_TOPPING					152
IMPLEMENT_SERIAL(CToppingContainerCtrl, CBCGPVisualContainerCtrl, 1)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CToppingContainerCtrl::CToppingContainerCtrl() :
	m_pView(NULL)
{
	CBCGPVisualContainer* pContainer = GetVisualContainer();
	ASSERT_VALID(pContainer);
	CBCGPVisualScrollBarColorTheme theme(CBCGPColor::SteelBlue);
}
BEGIN_MESSAGE_MAP(CToppingContainerCtrl, CBCGPVisualContainerCtrl)
	//{{AFX_MSG_MAP(CToppingContainerCtrl)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

int CToppingContainerCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPVisualContainerCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	CBCGPVisualContainer* pContainer = GetVisualContainer();
	ASSERT_VALID(pContainer);
	pContainer->SetOrderFlag(FALSE);
	return 0;
}

void CToppingContainerCtrl::InitImage()
{
	SetImage(ID_IMAGE_PRODUCT, gCurProduct.szProductThumbFile);
	//TOPPING IMAGE
	SetImage(ID_TOPPING_PEARL, IDB_BLACK_PEARLS);
	SetImage(ID_TOPPING_WHITE_PEARL, IDB_WHITE_PEARLS);
	SetImage(ID_TOPPING_AIYU, IDB_AIYU);
	SetImage(ID_TOPPING_MANGO_PUDDING, IDB_MANGO_PUDDING);
	SetImage(ID_TOPPING_GRASS_JELLY, IDB_GRASS_JELLY);
	SetImage(ID_TOPPING_COCONUT_JELLY, IDB_COCONUT_JELLY);
	SetImage(ID_TOPPING_ICE_CREAM, IDB_ICE_CREAM);
	SetImage(ID_TOPPING_ALEO_VERA, IDB_ALEO_VERA);
	SetImage(ID_TOPPING_GREEN_APPLE, IDB_GREEN_APPLE);
	SetImage(ID_TOPPING_RED_BEAN, IDB_RED_BEANS);
	SetImage(ID_TOPPING_OREO, IDB_OREO);
	SetImage(ID_NO_TOPPING, IDB_NO_TOPPING);
}
void CToppingContainerCtrl::SetImage(UINT nID, CString szImagePath)
{
	CBCGPVisualContainer* pContainer = GetVisualContainer();
	ASSERT_VALID(pContainer);
	CBCGPImageGaugeImpl* pImage = DYNAMIC_DOWNCAST(CBCGPImageGaugeImpl, pContainer->GetByID(nID));
	ASSERT_VALID(pImage);
	pImage->SetImage(CBCGPImage(szImagePath), CBCGPSize(pImage->GetRect().Width(), pImage->GetRect().Height()));
}
void CToppingContainerCtrl::CleanAllTopping()
{
	CBCGPVisualContainer* pContainer = GetVisualContainer();
	ASSERT_VALID(pContainer);
	for (int i = ID_TOPPING_PEARL; i <= ID_TOPPING_OREO; i++)
	{
		CBCGPImageGaugeImpl* pImage = DYNAMIC_DOWNCAST(CBCGPImageGaugeImpl, pContainer->GetByID(i));
		ASSERT_VALID(pImage);
		pImage->SetFillBrush(m_brFill);
		pImage->SetDirty(TRUE, TRUE);
	}
}
void CToppingContainerCtrl::SelectedTopping()
{
	CBCGPVisualContainer* pContainer = GetVisualContainer();
	ASSERT_VALID(pContainer);
	for (int i = ID_TOPPING_PEARL; i <= ID_TOPPING_OREO; i++)
	{
		CBCGPImageGaugeImpl* pImage = DYNAMIC_DOWNCAST(CBCGPImageGaugeImpl, pContainer->GetByID(i));
		ASSERT_VALID(pImage);
		if (-1 != gCurProduct.szTopping.Find(pImage->GetName()))
		{
			pImage->SetFillBrush(CBCGPBrush(CBCGPColor::GreenYellow));
			pImage->SetDirty(TRUE, TRUE);
		}
	}
}
void CToppingContainerCtrl::SetImage(UINT nID, UINT nImageID)
{
	CBCGPVisualContainer* pContainer = GetVisualContainer();
	ASSERT_VALID(pContainer);

	CBCGPImageGaugeImpl* pImage = DYNAMIC_DOWNCAST(CBCGPImageGaugeImpl, pContainer->GetByID(nID));
	ASSERT_VALID(pImage);
	pImage->SetImage(CBCGPImage(nImageID), CBCGPSize(pImage->GetRect().Width(), pImage->GetRect().Height()));
}

void CToppingContainerCtrl::CommandInit(const CString& szProduct)
{
	CBCGPVisualContainer* pContainer = GetVisualContainer();
	ASSERT_VALID(pContainer);
	InitImage();
	m_dbToppingMoney = gCurProduct.dbMoney - gCurProduct.dbUnitMenoy;
	m_dbUnit = gCurProduct.dbUnitMenoy;
	gCurProduct.szHoney = "";
	gCurProduct.szSugar = "";
	gCurProduct.szICE = "";
	gCurProduct.szSize = "";
	m_szProduct = szProduct;
	SetTextInfo(ID_PRODUCT_TEXT, szProduct);
	CString szMoney;
	szMoney.Format(L"$%0.2lf", m_dbUnit);
	SetTextInfo(ID_MONEY_TEA, szMoney);
	m_szIce = L"- " + gCurProduct.szICE;
	if (!gCurProduct.szTopping.IsEmpty() && gCurProduct.nToppingCounts>0)
	{
		SetTextInfo(ID_TOPPING_TEXT, gCurProduct.szTopping);
		CString szMoney;
		szMoney.Format(L"$%.2lf", gCurProduct.dbMoney);
		SetTextInfo(ID_MONEY_TOTAL, szMoney);
		SetTextInfo(ID_MONEY_TOPPING, gCurProduct.szToppingMoney);
		m_szTopping = gCurProduct.ToppingArr;
		m_szToppingMoney = gCurProduct.szToppingMoney;
	}
	SelectedTopping();
}

void CToppingContainerCtrl::SetTextInfo(UINT nID, CString szText)
{
	CBCGPVisualContainer* pContainer = GetVisualContainer();
	ASSERT_VALID(pContainer);

	CBCGPTextGaugeImpl* pText = DYNAMIC_DOWNCAST(CBCGPTextGaugeImpl, pContainer->GetByID(nID));
	ASSERT_VALID(pText);
	CBCGPTextFormat  pFormat = pText->GetTextFormat();
	if (nID == ID_SIZE_TEXT || nID == ID_MONEY_TEA || nID == ID_MONEY_TOPPING)
	{
		pFormat.SetTextAlignment(CBCGPTextFormat::BCGP_TEXT_ALIGNMENT::BCGP_TEXT_ALIGNMENT_TRAILING);
		pFormat.SetTextVerticalAlignment(CBCGPTextFormat::BCGP_TEXT_ALIGNMENT::BCGP_TEXT_ALIGNMENT_TRAILING);
		pText->SetTextFormat(pFormat);
	}
	else if (nID == ID_PRODUCT_TEXT)
	{
		pFormat.SetTextAlignment(CBCGPTextFormat::BCGP_TEXT_ALIGNMENT::BCGP_TEXT_ALIGNMENT_CENTER);
		pText->SetTextFormat(pFormat);
	}
	else
	{
		pFormat.SetTextAlignment(CBCGPTextFormat::BCGP_TEXT_ALIGNMENT::BCGP_TEXT_ALIGNMENT_LEADING);
		pFormat.SetTextVerticalAlignment(CBCGPTextFormat::BCGP_TEXT_ALIGNMENT::BCGP_TEXT_ALIGNMENT_TRAILING);
		pText->SetTextFormat(pFormat);
	}
	pText->SetText(szText);
	pText->Redraw();
}

BOOL CToppingContainerCtrl::OnMouseDown(int nButton, const CBCGPPoint& pt)
{
	USES_CONVERSION;
	CBCGPVisualContainer* pContainer = GetVisualContainer();
	ASSERT_VALID(pContainer);
	if (pContainer != NULL && !pContainer->IsSingleSel())
	{
		CBCGPBaseVisualObject* pObject = pContainer->GetFromPoint(pt);
		CBCGPImageGaugeImpl* pImage = DYNAMIC_DOWNCAST(CBCGPImageGaugeImpl, pObject);
		if (pImage != NULL && pImage->GetID() >= ID_TOPPING_PEARL && pImage->GetID() <= ID_TOPPING_OREO)
		{
			if (gCurProduct.nToppingCounts >= 4)
				return FALSE;
			gCurProduct.nToppingCounts++;
			double dbTopping;
			dbTopping = GetToppingPrice(pImage->GetDescription());
			m_dbToppingMoney += dbTopping;
			CString szToppingMoney, szTotalMoney;
			szToppingMoney.Format(L"+$%.2lf", dbTopping);
			m_szToppingMoney += szToppingMoney + L"\n";
			SetTextInfo(ID_MONEY_TOPPING, m_szToppingMoney);

			szTotalMoney.Format(L"=$%.2lf", m_dbToppingMoney + m_dbUnit);
			SetTextInfo(ID_MONEY_TOTAL, szTotalMoney);
			m_szTopping.push_back(pImage->GetDescription());
			CString  szTopAndMoney;
			if (dbTopping > 0.1)
				szTopAndMoney.Format(_T("-%s(%.2lf)\n"), pImage->GetName(), dbTopping);
			else szTopAndMoney.Format(_T("-%s\n"), pImage->GetName());

			gCurProduct.szTopping += szTopAndMoney;
			SetTextInfo(ID_TOPPING_TEXT, gCurProduct.szTopping);
			gCurProduct.dbMoney = m_dbUnit + m_dbToppingMoney;
			gCurProduct.ToppingArr = m_szTopping;
			gCurProduct.szToppingMoney = m_szToppingMoney;
			pImage->SetFillBrush(CBCGPBrush(CBCGPColor::GreenYellow));
			pImage->SetDirty(TRUE, TRUE);
		}
		else if (pImage != NULL && pImage->GetID() == ID_NO_TOPPING)
		{
			m_dbToppingMoney = 0;
			m_szToppingMoney = "";
			m_szTopping.clear();
			SetTextInfo(ID_MONEY_TOPPING, m_szToppingMoney);
			SetTextInfo(ID_MONEY_TOTAL, L"");
			SetTextInfo(ID_TOPPING_TEXT, L"");
			pContainer->Redraw();
			gCurProduct.szTopping = L"";
			gCurProduct.ToppingArr = m_szTopping;
			gCurProduct.dbMoney = m_dbUnit;
			gCurProduct.nToppingCounts = 0;
			CleanAllTopping();
		}
	}
	return FALSE;
}
void CToppingContainerCtrl::OnDraw(CBCGPGraphicsManager * pGM, const CBCGPRect & rectClip)
{
	CBCGPVisualContainer* pVisualContainer = GetVisualContainer();
	if (pVisualContainer != NULL)
	{
		pVisualContainer->OnDraw(pGM, rectClip);
	}
}
