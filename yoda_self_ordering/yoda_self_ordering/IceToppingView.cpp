#include "stdafx.h"
#include "yoda_self_ordering.h"
#include "IceToppingView.h"

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
#define ID_MONEY_SIZE					153
IMPLEMENT_SERIAL(CIceToppingContainerCtrl, CBCGPVisualContainerCtrl, 1)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIceToppingContainerCtrl::CIceToppingContainerCtrl() :
	m_pView(NULL)
{
	CBCGPVisualContainer* pContainer = GetVisualContainer();
	ASSERT_VALID(pContainer);

	CBCGPVisualScrollBarColorTheme theme(CBCGPColor::SteelBlue);
	pContainer->EnableScrollBars(TRUE, &theme, CBCGPVisualScrollBar::BCGP_VISUAL_SCROLLBAR_STYLE::BCGP_VISUAL_SCROLLBAR_3D_ROUNDED);
	m_pLastImageGauge = NULL;
	m_pLastImageSize = NULL;
	m_brFill = pContainer->GetFillBrush();
}
BEGIN_MESSAGE_MAP(CIceToppingContainerCtrl, CBCGPVisualContainerCtrl)
	//{{AFX_MSG_MAP(CIceToppingContainerCtrl)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

int CIceToppingContainerCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPVisualContainerCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	CBCGPVisualContainer* pContainer = GetVisualContainer();
	ASSERT_VALID(pContainer);
	pContainer->SetOrderFlag(FALSE);
	return 0;
}

void CIceToppingContainerCtrl::InitImage()
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
void CIceToppingContainerCtrl::SetImage(UINT nID, CString szImagePath)
{
	CBCGPVisualContainer* pContainer = GetVisualContainer();
	ASSERT_VALID(pContainer);
	CBCGPImageGaugeImpl* pImage = DYNAMIC_DOWNCAST(CBCGPImageGaugeImpl, pContainer->GetByID(nID));
	ASSERT_VALID(pImage);
	pImage->SetImage(CBCGPImage(szImagePath), CBCGPSize(pImage->GetRect().Width(), pImage->GetRect().Height()));
}
void CIceToppingContainerCtrl::CleanAllTopping()
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
void CIceToppingContainerCtrl::SelectedTopping()
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
void CIceToppingContainerCtrl::SetImage(UINT nID, UINT nImageID)
{
	CBCGPVisualContainer* pContainer = GetVisualContainer();
	ASSERT_VALID(pContainer);

	CBCGPImageGaugeImpl* pImage = DYNAMIC_DOWNCAST(CBCGPImageGaugeImpl, pContainer->GetByID(nID));
	ASSERT_VALID(pImage);
	pImage->SetImage(CBCGPImage(nImageID), CBCGPSize(pImage->GetRect().Width(), pImage->GetRect().Height()));
}

void CIceToppingContainerCtrl::CommandInit(const CString& szProduct)
{
	CBCGPVisualContainer* pContainer = GetVisualContainer();
	ASSERT_VALID(pContainer);
	InitImage();
	m_dbToppingMoney = gCurProduct.dbMoney - gCurProduct.dbUnitMenoy - gCurProduct.dbSizeMoney;
	m_dbUnit = m_dbUnitMenoy = gCurProduct.dbUnitMenoy;
	gCurProduct.szHoney = "";
	gCurProduct.szSugar = "";
	if (!gCurProduct.szSize.CompareNoCase(L"L") || !gCurProduct.szSize.CompareNoCase(L"大杯"))
	{
		CBCGPImageGaugeImpl* pImage = DYNAMIC_DOWNCAST(CBCGPImageGaugeImpl, pContainer->GetByID(ID_SIZE_LARGE));
		pImage->SetFillBrush(CBCGPBrush(CBCGPColor::GreenYellow));
		m_pLastImageSize = pImage;
		gCurProduct.szSizeCode = pImage->GetDescription();
	}
	else if (!gCurProduct.szSize.CompareNoCase(L"M") || !gCurProduct.szSize.CompareNoCase(L"中杯"))
	{

		CBCGPImageGaugeImpl* pImage = DYNAMIC_DOWNCAST(CBCGPImageGaugeImpl, pContainer->GetByID(ID_SIZE_NORMAL));
		pImage->SetFillBrush(CBCGPBrush(CBCGPColor::GreenYellow));
		gCurProduct.szSizeCode = pImage->GetDescription();
		m_pLastImageSize = pImage;
	}
	else if (!gCurProduct.szSize.CompareNoCase(L"TALL") || !gCurProduct.szSize.CompareNoCase(L"高杯"))
	{
		CBCGPImageGaugeImpl* pImage = DYNAMIC_DOWNCAST(CBCGPImageGaugeImpl, pContainer->GetByID(ID_SIZE_TALL_L));
		pImage->SetFillBrush(CBCGPBrush(CBCGPColor::GreenYellow));
		m_pLastImageSize = pImage;
		gCurProduct.szSizeCode = pImage->GetDescription();
	}

	if (!gCurProduct.szICE.CompareNoCase(L"MORE ICE") || !gCurProduct.szICE.CompareNoCase(L"多冰"))
	{
		CBCGPImageGaugeImpl* pImage = DYNAMIC_DOWNCAST(CBCGPImageGaugeImpl, pContainer->GetByID(ID_IMAGE_MORE_ICE));
		pImage->SetFillBrush(CBCGPBrush(CBCGPColor::GreenYellow));
		m_pLastImageGauge = pImage;
		gCurProduct.szICECode = pImage->GetDescription();
	}
	else if (!gCurProduct.szICE.CompareNoCase(L"NORMAL ICE") || !gCurProduct.szICE.CompareNoCase(L"正常冰"))
	{
		CBCGPImageGaugeImpl* pImage = DYNAMIC_DOWNCAST(CBCGPImageGaugeImpl, pContainer->GetByID(ID_IMAGE_NORMAL_ICE));
		pImage->SetFillBrush(CBCGPBrush(CBCGPColor::GreenYellow));
		m_pLastImageGauge = pImage;
		gCurProduct.szICECode = pImage->GetDescription();
	}
	else if (!gCurProduct.szICE.CompareNoCase(L"LESS ICE") || !gCurProduct.szICE.CompareNoCase(L"少冰"))
	{
		CBCGPImageGaugeImpl* pImage = DYNAMIC_DOWNCAST(CBCGPImageGaugeImpl, pContainer->GetByID(ID_IMAGE_LESS_ICE));
		pImage->SetFillBrush(CBCGPBrush(CBCGPColor::GreenYellow));
		m_pLastImageGauge = pImage;
		gCurProduct.szICECode = pImage->GetDescription();
	}
	else if (!gCurProduct.szICE.CompareNoCase(L"FILTER ICE") || !gCurProduct.szICE.CompareNoCase(L"去冰"))
	{
		CBCGPImageGaugeImpl* pImage = DYNAMIC_DOWNCAST(CBCGPImageGaugeImpl, pContainer->GetByID(ID_IMAGE_FILTER_ICE));
		pImage->SetFillBrush(CBCGPBrush(CBCGPColor::GreenYellow));
		m_pLastImageGauge = pImage;
		gCurProduct.szICECode = pImage->GetDescription();
	}
	else if (!gCurProduct.szICE.CompareNoCase(L"ROOM TEMP") || !gCurProduct.szICE.CompareNoCase(L"常温"))
	{
		CBCGPImageGaugeImpl* pImage = DYNAMIC_DOWNCAST(CBCGPImageGaugeImpl, pContainer->GetByID(ID_IMAGE_ROOM_TEMP));
		pImage->SetFillBrush(CBCGPBrush(CBCGPColor::GreenYellow));
		m_pLastImageGauge = pImage;
		gCurProduct.szICECode = pImage->GetDescription();
	}
	else if (!gCurProduct.szICE.CompareNoCase(L"WARM") || !gCurProduct.szICE.CompareNoCase(L"温水"))
	{
		CBCGPImageGaugeImpl* pImage = DYNAMIC_DOWNCAST(CBCGPImageGaugeImpl, pContainer->GetByID(ID_IMAGE_WARM_WATER));
		pImage->SetFillBrush(CBCGPBrush(CBCGPColor::GreenYellow));
		m_pLastImageGauge = pImage;
		gCurProduct.szICECode = pImage->GetDescription();
	}
	m_szProduct = szProduct;
	SetTextInfo(ID_PRODUCT_TEXT, szProduct);
	m_szSize = gCurProduct.szSize;
	SetTextInfo(ID_SIZE_TEXT, L"- "+m_szSize);
	m_dbSizeMoney = gCurProduct.dbSizeMoney;
	CString szMoney, szSizeMoney;
	szMoney.Format(L"$%0.2lf", m_dbUnit);
	szSizeMoney.Format(L"$%0.2lf", m_dbSizeMoney);
	SetTextInfo(ID_MONEY_TEA, szMoney);
	if (m_dbSizeMoney > 0.1)
		SetTextInfo(ID_MONEY_SIZE, szSizeMoney);
	m_szIce = L"- " + gCurProduct.szICE;
	SetTextInfo(ID_ICE_TEXT, m_szIce);
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

void CIceToppingContainerCtrl::SetTextInfo(UINT nID, CString szText)
{
	CBCGPVisualContainer* pContainer = GetVisualContainer();
	ASSERT_VALID(pContainer);

	CBCGPTextGaugeImpl* pText = DYNAMIC_DOWNCAST(CBCGPTextGaugeImpl, pContainer->GetByID(nID));
	ASSERT_VALID(pText);
	CBCGPTextFormat  pFormat = pText->GetTextFormat();
	if (nID == ID_MONEY_SIZE || nID == ID_MONEY_TEA || nID == ID_MONEY_TOPPING)
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

BOOL CIceToppingContainerCtrl::OnMouseDown(int nButton, const CBCGPPoint& pt)
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
			m_szIce = pImage->GetName();
			SetTextInfo(ID_ICE_TEXT, _T("- ") + m_szIce);
			gCurProduct.szICE = m_szIce;
			gCurProduct.szICECode = pImage->GetDescription();
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
				m_szSize = pImage->GetName();
				m_dbSizeMoney = GetSizePrice(pImage->GetDescription());
				pImage->SetFillBrush(CBCGPBrush(CBCGPColor::GreenYellow));
				m_pLastImageSize = pImage;
			}
			else if (pImage->GetID() == ID_SIZE_LARGE)
			{
				m_szSize = pImage->GetName();
				m_dbSizeMoney = GetSizePrice(pImage->GetDescription());
				pImage->SetFillBrush(CBCGPBrush(CBCGPColor::GreenYellow));
				m_pLastImageSize = pImage;
			}
			else if (pImage->GetID() == ID_SIZE_TALL_L)
			{
				m_szSize = pImage->GetName();
				m_dbSizeMoney = GetSizePrice(pImage->GetDescription());
				pImage->SetFillBrush(CBCGPBrush(CBCGPColor::GreenYellow));
				m_pLastImageSize = pImage;
			}
			SetTextInfo(ID_SIZE_TEXT, L"- "+m_szSize);
			CString szMoney;
			szMoney.Format(L"$%.2lf", m_dbUnitMenoy);
			SetTextInfo(ID_MONEY_TEA, szMoney);
			if (m_dbToppingMoney > 0.)
			{
				szMoney.Format(L"$%.2lf", m_dbUnitMenoy + m_dbToppingMoney);
				SetTextInfo(ID_MONEY_TOTAL, szMoney);
			}
			gCurProduct.szSize = m_szSize;
			gCurProduct.dbMoney = m_dbUnitMenoy + m_dbToppingMoney;
			gCurProduct.dbUnitMenoy = m_dbUnitMenoy;
			gCurProduct.szSizeCode = pImage->GetDescription();
			pImage->SetDirty(TRUE, TRUE);

		}
		else if (pImage != NULL && pImage->GetID() >= ID_TOPPING_PEARL && pImage->GetID() <= ID_TOPPING_OREO)
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

			szTotalMoney.Format(L"=$%.2lf", m_dbToppingMoney + m_dbUnit + m_dbSizeMoney);
			SetTextInfo(ID_MONEY_TOTAL, szTotalMoney);
			m_szTopping.push_back(pImage->GetDescription());
			CString  szTopAndMoney;
			if (dbTopping > 0.1)
				szTopAndMoney.Format(_T("-%s(%.2lf)\n"), pImage->GetName(), dbTopping);
			else szTopAndMoney.Format(_T("-%s\n"), pImage->GetName());

			gCurProduct.szTopping += szTopAndMoney;
			SetTextInfo(ID_TOPPING_TEXT, gCurProduct.szTopping);
			gCurProduct.dbMoney = m_dbUnit + m_dbSizeMoney + m_dbToppingMoney;
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
			gCurProduct.dbMoney = m_dbUnit + m_dbSizeMoney;
			gCurProduct.nToppingCounts = 0;
			CleanAllTopping();
		}
	}
	return FALSE;
}
void CIceToppingContainerCtrl::OnDraw(CBCGPGraphicsManager * pGM, const CBCGPRect & rectClip)
{
	CBCGPVisualContainer* pVisualContainer = GetVisualContainer();
	if (pVisualContainer != NULL)
	{
		pVisualContainer->OnDraw(pGM, rectClip);
	}
}
