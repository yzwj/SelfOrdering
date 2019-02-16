#include "stdafx.h"
#include "yoda_self_ordering.h"
#include "HoneyView.h"

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
#define ID_HONEY_TEXT					105
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
#define ID_HONEY_125					120
#define ID_HONEY_100					121
#define ID_HONEY_75						122
#define ID_HONEY_50						123
#define ID_HONEY_25						124
#define ID_HONEY_0						125
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
IMPLEMENT_SERIAL(CHoneyContainerCtrl, CBCGPVisualContainerCtrl, 1)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHoneyContainerCtrl::CHoneyContainerCtrl()
{
	CBCGPVisualContainer* pContainer = GetVisualContainer();
	ASSERT_VALID(pContainer);
	CBCGPVisualScrollBarColorTheme theme(CBCGPColor::SteelBlue);
	pContainer->EnableScrollBars(TRUE, &theme, CBCGPVisualScrollBar::BCGP_VISUAL_SCROLLBAR_STYLE::BCGP_VISUAL_SCROLLBAR_3D_ROUNDED);
	m_pLastColorIndicator = NULL;
	m_pLastImageGauge = NULL;
	m_pLastImageSize = NULL;
	m_brFill = pContainer->GetFillBrush();
}
BEGIN_MESSAGE_MAP(CHoneyContainerCtrl, CBCGPVisualContainerCtrl)
	//{{AFX_MSG_MAP(CHoneyContainerCtrl)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

int CHoneyContainerCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPVisualContainerCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	CBCGPVisualContainer* pContainer = GetVisualContainer();
	ASSERT_VALID(pContainer);
	pContainer->SetOrderFlag(FALSE);
	return 0;
}

void CHoneyContainerCtrl::InitImage()
{
	//ICE IMAGE
	SetImage(ID_IMAGE_PRODUCT, gCurProduct.szProductThumbFile);
	SetImage(ID_IMAGE_MORE_ICE, IDB_MORE_ICE);
	SetImage(ID_IMAGE_NORMAL_ICE, IDB_NORMAL_ICE);
	SetImage(ID_IMAGE_LESS_ICE, IDB_LESS_ICE);
	SetImage(ID_IMAGE_FILTER_ICE, IDB_FILTER_ICE);
	SetImage(ID_IMAGE_WARM_WATER, IDB_WARM_WATER);
	SetImage(ID_IMAGE_ROOM_TEMP, IDB_ROOM_TEMP);

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
	//SIZE
	SetImage(ID_SIZE_NORMAL, IDB_MEDIUM);
	SetImage(ID_SIZE_LARGE, IDB_LARGE);
	SetImage(ID_SIZE_TALL_L, IDB_TALL);

}
void CHoneyContainerCtrl::SetImage(UINT nID, CString szImagePath)
{
	CBCGPVisualContainer* pContainer = GetVisualContainer();
	ASSERT_VALID(pContainer);
	CBCGPImageGaugeImpl* pImage = DYNAMIC_DOWNCAST(CBCGPImageGaugeImpl, pContainer->GetByID(nID));
	ASSERT_VALID(pImage);
	pImage->SetImage(CBCGPImage(szImagePath), CBCGPSize(pImage->GetRect().Width(), pImage->GetRect().Height()));
}
void CHoneyContainerCtrl::CleanAllTopping()
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
void CHoneyContainerCtrl::SelectedTopping()
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
void CHoneyContainerCtrl::SetImage(UINT nID, UINT nImageID)
{
	CBCGPVisualContainer* pContainer = GetVisualContainer();
	ASSERT_VALID(pContainer);

	CBCGPImageGaugeImpl* pImage = DYNAMIC_DOWNCAST(CBCGPImageGaugeImpl, pContainer->GetByID(nID));
	ASSERT_VALID(pImage);
	pImage->SetImage(CBCGPImage(nImageID), CBCGPSize(pImage->GetRect().Width(), pImage->GetRect().Height()));
}

void CHoneyContainerCtrl::CommandInit(const CString& szProduct)
{
	CBCGPVisualContainer* pContainer = GetVisualContainer();
	ASSERT_VALID(pContainer);
	InitImage();
	m_dbToppingMoney = gCurProduct.dbMoney - gCurProduct.dbTeaMoney;
	m_dbUnit = m_dbTeaMoney = gCurProduct.dbTeaMoney;
	if (!gCurProduct.szHoney.CompareNoCase(L"125%"))
	{
		CBCGPColorIndicatorImpl* pColor = DYNAMIC_DOWNCAST(CBCGPColorIndicatorImpl, pContainer->GetByID(ID_HONEY_125));
		m_pLastColorIndicator = pColor;
		pColor->SetSelectColor(CBCGPBrush(CBCGPColor::GreenYellow), TRUE);
	}
	else if (!gCurProduct.szHoney.CompareNoCase(L"100%"))
	{
		CBCGPColorIndicatorImpl* pColor = DYNAMIC_DOWNCAST(CBCGPColorIndicatorImpl, pContainer->GetByID(ID_HONEY_100));
		m_pLastColorIndicator = pColor;
		pColor->SetSelectColor(CBCGPBrush(CBCGPColor::GreenYellow), TRUE);
	}
	else if (!gCurProduct.szHoney.CompareNoCase(L"75%"))
	{
		CBCGPColorIndicatorImpl* pColor = DYNAMIC_DOWNCAST(CBCGPColorIndicatorImpl, pContainer->GetByID(ID_HONEY_75));
		m_pLastColorIndicator = pColor;
		pColor->SetSelectColor(CBCGPBrush(CBCGPColor::GreenYellow), TRUE);
	}
	else if (!gCurProduct.szHoney.CompareNoCase(L"25%"))
	{
		CBCGPColorIndicatorImpl* pColor = DYNAMIC_DOWNCAST(CBCGPColorIndicatorImpl, pContainer->GetByID(ID_HONEY_25));
		m_pLastColorIndicator = pColor;
		pColor->SetSelectColor(CBCGPBrush(CBCGPColor::GreenYellow), TRUE);
	}
	else if (!gCurProduct.szHoney.CompareNoCase(L"0%"))
	{
		CBCGPColorIndicatorImpl* pColor = DYNAMIC_DOWNCAST(CBCGPColorIndicatorImpl, pContainer->GetByID(ID_HONEY_0));
		m_pLastColorIndicator = pColor;
		pColor->SetSelectColor(CBCGPBrush(CBCGPColor::GreenYellow), TRUE);
	}
	else if (!gCurProduct.szHoney.CompareNoCase(L"50%"))
	{
		CBCGPColorIndicatorImpl* pColor = DYNAMIC_DOWNCAST(CBCGPColorIndicatorImpl, pContainer->GetByID(ID_HONEY_50));
		m_pLastColorIndicator = pColor;
		pColor->SetSelectColor(CBCGPBrush(CBCGPColor::GreenYellow), TRUE);
	}

	if (!gCurProduct.szSize.CompareNoCase(L"L") || !gCurProduct.szSize.CompareNoCase(L"大杯"))
	{
		CBCGPImageGaugeImpl* pImage = DYNAMIC_DOWNCAST(CBCGPImageGaugeImpl, pContainer->GetByID(ID_SIZE_LARGE));
		pImage->SetFillBrush(CBCGPBrush(CBCGPColor::GreenYellow));
		m_pLastImageSize = pImage;
	}
	else if (!gCurProduct.szSize.CompareNoCase(L"M") || !gCurProduct.szSize.CompareNoCase(L"中杯"))
	{

		CBCGPImageGaugeImpl* pImage = DYNAMIC_DOWNCAST(CBCGPImageGaugeImpl, pContainer->GetByID(ID_SIZE_NORMAL));
		pImage->SetFillBrush(CBCGPBrush(CBCGPColor::GreenYellow));

		m_pLastImageSize = pImage;
	}
	else if (!gCurProduct.szSize.CompareNoCase(L"TALL") || !gCurProduct.szSize.CompareNoCase(L"高杯"))
	{
		CBCGPImageGaugeImpl* pImage = DYNAMIC_DOWNCAST(CBCGPImageGaugeImpl, pContainer->GetByID(ID_SIZE_TALL_L));
		pImage->SetFillBrush(CBCGPBrush(CBCGPColor::GreenYellow));
		m_pLastImageSize = pImage;
	}

	if (!gCurProduct.szICE.CompareNoCase(L"MORE ICE") || !gCurProduct.szICE.CompareNoCase(L"多冰"))
	{
		CBCGPImageGaugeImpl* pImage = DYNAMIC_DOWNCAST(CBCGPImageGaugeImpl, pContainer->GetByID(ID_IMAGE_MORE_ICE));
		pImage->SetFillBrush(CBCGPBrush(CBCGPColor::GreenYellow));
		m_pLastImageGauge = pImage;
	}
	else if (!gCurProduct.szICE.CompareNoCase(L"NORMAL ICE") || !gCurProduct.szICE.CompareNoCase(L"正常冰"))
	{
		CBCGPImageGaugeImpl* pImage = DYNAMIC_DOWNCAST(CBCGPImageGaugeImpl, pContainer->GetByID(ID_IMAGE_NORMAL_ICE));
		pImage->SetFillBrush(CBCGPBrush(CBCGPColor::GreenYellow));
		m_pLastImageGauge = pImage;
	}
	else if (!gCurProduct.szICE.CompareNoCase(L"LESS ICE") || !gCurProduct.szICE.CompareNoCase(L"少冰"))
	{
		CBCGPImageGaugeImpl* pImage = DYNAMIC_DOWNCAST(CBCGPImageGaugeImpl, pContainer->GetByID(ID_IMAGE_LESS_ICE));
		pImage->SetFillBrush(CBCGPBrush(CBCGPColor::GreenYellow));
		m_pLastImageGauge = pImage;
	}
	else if (!gCurProduct.szICE.CompareNoCase(L"FILTER ICE") || !gCurProduct.szICE.CompareNoCase(L"去冰"))
	{
		CBCGPImageGaugeImpl* pImage = DYNAMIC_DOWNCAST(CBCGPImageGaugeImpl, pContainer->GetByID(ID_IMAGE_FILTER_ICE));
		pImage->SetFillBrush(CBCGPBrush(CBCGPColor::GreenYellow));
		m_pLastImageGauge = pImage;
	}
	else if (!gCurProduct.szICE.CompareNoCase(L"ROOM TEMP") || !gCurProduct.szICE.CompareNoCase(L"常温"))
	{
		CBCGPImageGaugeImpl* pImage = DYNAMIC_DOWNCAST(CBCGPImageGaugeImpl, pContainer->GetByID(ID_IMAGE_ROOM_TEMP));
		pImage->SetFillBrush(CBCGPBrush(CBCGPColor::GreenYellow));
		m_pLastImageGauge = pImage;
	}
	else if (!gCurProduct.szICE.CompareNoCase(L"WARM") || !gCurProduct.szICE.CompareNoCase(L"温水"))
	{
		CBCGPImageGaugeImpl* pImage = DYNAMIC_DOWNCAST(CBCGPImageGaugeImpl, pContainer->GetByID(ID_IMAGE_WARM_WATER));
		pImage->SetFillBrush(CBCGPBrush(CBCGPColor::GreenYellow));
		m_pLastImageGauge = pImage;
	}
	m_szProduct = szProduct;
	SetTextInfo(ID_PRODUCT_TEXT, szProduct);
	m_szSize = gCurProduct.szSize;
	SetTextInfo(ID_SIZE_TEXT, m_szSize);
	CString szMoney;
	szMoney.Format(L"$%0.2lf", m_dbTeaMoney);
	SetTextInfo(ID_MONEY_TEA, szMoney);
	m_szIce = L"- " + gCurProduct.szICE;
	SetTextInfo(ID_ICE_TEXT, m_szIce);
	m_szHoney = L"- " + gCurProduct.szHoney;
	SetTextInfo(ID_HONEY_TEXT, m_szHoney);
	if (!gCurProduct.szTopping.IsEmpty() && gCurProduct.nToppingCounts>0)
	{
		SetTextInfo(ID_TOPPING_TEXT, gCurProduct.szTopping);
		CString szMoney;
		szMoney.Format(L"$%.2lf", gCurProduct.dbMoney);
		SetTextInfo(ID_MONEY_TOTAL, szMoney);
		SetTextInfo(ID_MONEY_TOPPING, gCurProduct.szToppingMoney);
		m_szTopping = gCurProduct.szTopping;
		m_szToppingMoney = gCurProduct.szToppingMoney;
	}
	SelectedTopping();
}
void CHoneyContainerCtrl::SetTextInfo(UINT nID, CString szText)
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

BOOL CHoneyContainerCtrl::OnMouseDown(int nButton, const CBCGPPoint& pt)
{
	USES_CONVERSION;
	CBCGPVisualContainer* pContainer = GetVisualContainer();
	ASSERT_VALID(pContainer);
	if (pContainer != NULL && !pContainer->IsSingleSel())
	{
		CBCGPBaseVisualObject* pObject = pContainer->GetFromPoint(pt);
		CBCGPImageGaugeImpl* pImage = DYNAMIC_DOWNCAST(CBCGPImageGaugeImpl, pObject);
		if (pImage != NULL && pImage->GetID() >= ID_IMAGE_MORE_ICE && pImage->GetID() <= ID_IMAGE_ROOM_TEMP)
		{
			if (m_pLastImageGauge != NULL)
			{
				m_pLastImageGauge->SetFillBrush(m_brFill);
				m_pLastImageGauge->SetDirty(TRUE, TRUE);
			}
			m_pLastImageGauge = pImage;
			int nID = pImage->GetID();
			m_szIce = pImage->GetName();
			SetTextInfo(ID_ICE_TEXT, _T("- ") + m_szIce);
			gCurProduct.szICE = m_szIce;
			pImage->SetFillBrush(CBCGPBrush(CBCGPColor::GreenYellow));
			pImage->SetDirty(TRUE, TRUE);
		}
		else if (pImage != NULL && pImage->GetID() >= ID_SIZE_NORMAL && pImage->GetID() <= ID_SIZE_TALL_L)
		{
			if (m_pLastImageSize != NULL)
			{
				m_pLastImageSize->SetFillBrush(m_brFill);
				m_pLastImageSize->SetDirty(TRUE, TRUE);
			}

			if (pImage->GetID() == ID_SIZE_NORMAL)
			{
				m_szSize = pImage->GetName();;
				m_dbTeaMoney = m_dbUnit;
				pImage->SetFillBrush(CBCGPBrush(CBCGPColor::GreenYellow));
				m_pLastImageSize = pImage;
			}
			else if (pImage->GetID() == ID_SIZE_LARGE)
			{
				m_szSize = pImage->GetName();;
				m_dbTeaMoney = m_dbUnit + atof(W2A(pImage->GetDescription()));
				pImage->SetFillBrush(CBCGPBrush(CBCGPColor::GreenYellow));
				m_pLastImageSize = pImage;
			}
			else if (pImage->GetID() == ID_SIZE_TALL_L)
			{
				m_szSize = pImage->GetName();;
				m_dbTeaMoney = m_dbUnit + atof(W2A(pImage->GetDescription()));
				pImage->SetFillBrush(CBCGPBrush(CBCGPColor::GreenYellow));
				m_pLastImageSize = pImage;
			}
			SetTextInfo(ID_SIZE_TEXT, m_szSize);
			CString szMoney;
			szMoney.Format(L"$%.2lf", m_dbTeaMoney);
			SetTextInfo(ID_MONEY_TEA, szMoney);
			if (m_dbToppingMoney > 0.)
			{
				szMoney.Format(L"$%.2lf", m_dbTeaMoney + m_dbToppingMoney);
				SetTextInfo(ID_MONEY_TOTAL, szMoney);
			}
			gCurProduct.szSize = m_szSize;
			gCurProduct.dbMoney = m_dbTeaMoney + m_dbToppingMoney;
			gCurProduct.dbTeaMoney = m_dbTeaMoney;
			pImage->SetDirty(TRUE, TRUE);

		}
		else if (pImage != NULL && pImage->GetID() >= ID_TOPPING_PEARL && pImage->GetID() <= ID_TOPPING_OREO)
		{
			if (gCurProduct.nToppingCounts >= 4)
				return FALSE;
			gCurProduct.nToppingCounts++;
			m_dbToppingMoney += atof(W2A(pImage->GetDescription()));
			CString szToppingMoney, szTotalMoney;
			szToppingMoney.Format(L"+$%.2lf", atof(W2A(pImage->GetDescription())));
			m_szToppingMoney += szToppingMoney + L"\n";
			SetTextInfo(ID_MONEY_TOPPING, m_szToppingMoney);

			szTotalMoney.Format(L"=$%.2lf", m_dbToppingMoney + m_dbTeaMoney);
			SetTextInfo(ID_MONEY_TOTAL, szTotalMoney);
			m_szTopping += L"- " + pImage->GetName() + L"\n";
			SetTextInfo(ID_TOPPING_TEXT, m_szTopping);
			gCurProduct.dbMoney = m_dbTeaMoney + m_dbToppingMoney;
			gCurProduct.szTopping = m_szTopping;
			gCurProduct.szToppingMoney = m_szToppingMoney;
			pImage->SetFillBrush(CBCGPBrush(CBCGPColor::GreenYellow));
			pImage->SetDirty(TRUE, TRUE);
		}
		else if (pImage != NULL && pImage->GetID() == ID_NO_TOPPING)
		{
			m_dbToppingMoney = 0;
			m_szToppingMoney = "";
			SetTextInfo(ID_MONEY_TOPPING, m_szToppingMoney);
			SetTextInfo(ID_MONEY_TOTAL, L"");
			m_szTopping = "";
			SetTextInfo(ID_TOPPING_TEXT, m_szTopping);
			pContainer->Redraw();
			gCurProduct.szTopping = m_szTopping;
			gCurProduct.dbMoney = m_dbTeaMoney;
			gCurProduct.nToppingCounts = 0;
			CleanAllTopping();
		}
		CBCGPColorIndicatorImpl* pColor = DYNAMIC_DOWNCAST(CBCGPColorIndicatorImpl, pObject);
		if (pColor != NULL)
		{
			if (m_pLastColorIndicator != NULL)
			{
				m_pLastColorIndicator->SetSelectColor(pColor->GetFillBrush());
				m_pLastColorIndicator->SetDirty(TRUE, TRUE);
			}
			m_pLastColorIndicator = pColor;
			pColor->SetSelectColor(CBCGPBrush(CBCGPColor::GreenYellow), TRUE);
			switch (pColor->GetID())
			{
			case ID_HONEY_125:
				SetTextInfo(ID_HONEY_TEXT, L"- 125%");
				m_szHoney = "125%";
				break;
			case ID_HONEY_100:
				SetTextInfo(ID_HONEY_TEXT, L"- 100%");
				m_szHoney = "100%";
				break;
			case ID_HONEY_75:
				SetTextInfo(ID_HONEY_TEXT, L"- 75%");
				m_szHoney = "75%";
				break;
			case ID_HONEY_50:
				SetTextInfo(ID_HONEY_TEXT, L"- 50%");
				m_szHoney = "50%";
				break;
			case ID_HONEY_25:
				SetTextInfo(ID_HONEY_TEXT, L"- 25%");
				m_szHoney = "25%";
				break;
			case ID_HONEY_0:
				SetTextInfo(ID_HONEY_TEXT, L"- 0%");
				m_szHoney = "0%";
				break;
			}
			gCurProduct.szHoney = m_szHoney;
		}
	}
	return FALSE;
}

void CHoneyContainerCtrl::OnDraw(CBCGPGraphicsManager * pGM, const CBCGPRect & rectClip)
{
	CBCGPVisualContainer* pVisualContainer = GetVisualContainer();
	if (pVisualContainer != NULL)
	{
		pVisualContainer->OnDraw(pGM, rectClip);
	}
}
