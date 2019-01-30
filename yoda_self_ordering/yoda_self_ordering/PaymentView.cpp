#include "stdafx.h"
#include "yoda_self_ordering.h"
#include "PaymentView.h"
#include "CashPop.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif
extern 	CList<LPORDERINFO, LPORDERINFO>	glstOrder;
#define TIMER_REDRAW					1001
#define ID_TEXT_TOTAL					160
#define ID_TEXT_TIP						161
#define ID_LINE							162
#define ID_IMAGE_IPAY					170
#define ID_IMAGE_NETS					171
#define ID_IMAGE_COUNTER				172
IMPLEMENT_SERIAL(CPayContainerCtrl, CBCGPVisualContainerCtrl, 1)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPayContainerCtrl::CPayContainerCtrl()
{
	CBCGPVisualContainer* pContainer = GetVisualContainer();
	ASSERT_VALID(pContainer);
	CBCGPVisualScrollBarColorTheme theme(CBCGPColor::SteelBlue);
	m_bShow = TRUE;
}
BEGIN_MESSAGE_MAP(CPayContainerCtrl, CBCGPVisualContainerCtrl)
	//{{AFX_MSG_MAP(CPayContainerCtrl)
	ON_WM_CREATE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

int CPayContainerCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPVisualContainerCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	CBCGPVisualContainer* pContainer = GetVisualContainer();
	ASSERT_VALID(pContainer);
	
	return 0;
}

void CPayContainerCtrl::OnTimer(UINT_PTR nIDEvent)
{
	CBCGPVisualContainer* pContainer = GetVisualContainer();
	ASSERT_VALID(pContainer);
	if (m_bShow)
	{
		if(gbChinese)
			SetTextInfo(ID_TEXT_TIP, L"请选择您的支付方式");
		else
			SetTextInfo(ID_TEXT_TIP, L"PLEASE SELECT PAYMENT");
	}
	else
	{
		SetTextInfo(ID_TEXT_TIP, L"");
	}
	m_bShow = !m_bShow;

}

void CPayContainerCtrl::InitImage()
{
	SetImage(ID_IMAGE_IPAY, IDB_IPAY);
	SetImage(ID_IMAGE_NETS, IDB_NETS);
	SetImage(ID_IMAGE_COUNTER, IDB_COUNTER);
}
void CPayContainerCtrl::SetImage(UINT nID, CString szImagePath)
{
	CBCGPVisualContainer* pContainer = GetVisualContainer();
	ASSERT_VALID(pContainer);
	CBCGPImageGaugeImpl* pImage = DYNAMIC_DOWNCAST(CBCGPImageGaugeImpl, pContainer->GetByID(nID));
	ASSERT_VALID(pImage);
	pImage->SetImage(CBCGPImage(szImagePath), CBCGPSize(pImage->GetRect().Width(), pImage->GetRect().Height()));
}
void CPayContainerCtrl::SetImage(UINT nID, UINT nImageID)
{
	CBCGPVisualContainer* pContainer = GetVisualContainer();
	ASSERT_VALID(pContainer);
	CBCGPImageGaugeImpl* pImage = DYNAMIC_DOWNCAST(CBCGPImageGaugeImpl, pContainer->GetByID(nID));
	ASSERT_VALID(pImage);
	pImage->SetImage(CBCGPImage(nImageID), CBCGPSize(pImage->GetRect().Width(), pImage->GetRect().Height()));
	pImage->SetRoundedShapes();
}

void CPayContainerCtrl::CommandInit()
{
	m_dbTotal = 0.;
	CBCGPVisualContainer* pContainer = GetVisualContainer();
	ASSERT_VALID(pContainer);
	InitImage();
	for (POSITION pos = glstOrder.GetHeadPosition(); pos!=NULL;)
	{
		LPORDERINFO order = glstOrder.GetNext(pos);
		m_dbTotal += order->productInfo.dbMoney * order->nProductCounts;
	}
	CString szMoney;
	szMoney.Format(L"TOTAL  $%0.2lf", m_dbTotal);
	SetTextInfo(ID_TEXT_TOTAL, szMoney);
	SetTextInfo(ID_TEXT_TIP, L"PLEASE SELECT PAYMENT");
	SetTimer(TIMER_REDRAW,600,NULL);
}

void CPayContainerCtrl::SetTextInfo(UINT nID, CString szText)
{
	CBCGPVisualContainer* pContainer = GetVisualContainer();
	ASSERT_VALID(pContainer);
	CBCGPTextGaugeImpl* pText = DYNAMIC_DOWNCAST(CBCGPTextGaugeImpl, pContainer->GetByID(nID));
	ASSERT_VALID(pText);
	pText->SetText(szText);
	pText->Redraw();
}

BOOL CPayContainerCtrl::OnMouseDown(int nButton, const CBCGPPoint& pt)
{
	USES_CONVERSION;
	CBCGPVisualContainer* pContainer = GetVisualContainer();
	ASSERT_VALID(pContainer);
	if (pContainer != NULL && !pContainer->IsSingleSel())
	{
		CBCGPBaseVisualObject* pObject = pContainer->GetFromPoint(pt);
		CBCGPImageGaugeImpl* pImage = DYNAMIC_DOWNCAST(CBCGPImageGaugeImpl, pObject);
		if (pImage != NULL && pImage->GetID() == ID_IMAGE_IPAY)
		{
			
		}
		else if (pImage != NULL && pImage->GetID() == ID_IMAGE_NETS)
		{

		}
		else if (pImage != NULL && pImage->GetID() == ID_IMAGE_COUNTER)
		{
			PrintReceipt(true);
			glstOrder.RemoveAll();
			CBCGPPopupWindow* pPopup = new CBCGPPopupWindow;
			CBCGPPopupWindowColors customColors;

			customColors.clrFill = RGB(103, 109, 134);
			customColors.clrText = RGB(255, 255, 255);
			customColors.clrBorder = RGB(103, 109, 134);
			customColors.clrLink = RGB(0, 255, 0);
			customColors.clrHoverLink = RGB(254, 233, 148);

			pPopup->SetCustomTheme(customColors);
			pPopup->SetAnimationType(
			CBCGPPopupMenu::SYSTEM_DEFAULT_ANIMATION);
			pPopup->SetTransparency((BYTE)255);
			pPopup->SetSmallCaption(FALSE);
			pPopup->SetSmallCaptionGripper(FALSE);
			pPopup->SetAutoCloseTime(5000);
			pPopup->SetRoundedCorners();
			pPopup->SetShadow();
			pPopup->EnablePinButton(FALSE);
			CPoint ptLocation(-1, -1);
			ptLocation.x = GetSystemMetrics(SM_CYSCREEN)/2;
			ptLocation.y = GetSystemMetrics(SM_CXSCREEN)/2;
			pPopup->EnableCloseButton(FALSE);
			pPopup->Create(this, IDD_CASH_POP,
			NULL, ptLocation, RUNTIME_CLASS(CCashPop));
			pPopup->CenterWindow();
		}
	}
	return FALSE;
}

void CPayContainerCtrl::OnDraw(CBCGPGraphicsManager * pGM, const CBCGPRect & rectClip)
{
	CBCGPVisualContainer* pVisualContainer = GetVisualContainer();
	if (pVisualContainer != NULL)
	{
		pVisualContainer->OnDraw(pGM, rectClip);
	}
}
