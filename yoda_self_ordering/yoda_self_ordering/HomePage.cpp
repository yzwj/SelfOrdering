#include "stdafx.h"
#include "yoda_self_ordering.h"
#include "HomePage.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif
#include "TipInfo.h"

BOOL gbChinese;
#define ID_IMAGE_ADVERTISING			160
#define ID_IMAGE_LANGUAGE				161
#define ID_IMAGE_ORDER					162
#define ID_IMAGE_MEMBER					163
IMPLEMENT_SERIAL(CHomeContainerCtrl, CBCGPVisualContainerCtrl, 1)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHomeContainerCtrl::CHomeContainerCtrl()
{
	CBCGPVisualContainer* pContainer = GetVisualContainer();
	ASSERT_VALID(pContainer);
	gbChinese = FALSE;
}
BEGIN_MESSAGE_MAP(CHomeContainerCtrl, CBCGPVisualContainerCtrl)
	//{{AFX_MSG_MAP(CHomeContainerCtrl)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

int CHomeContainerCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPVisualContainerCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}
void CHomeContainerCtrl::InitImage()
{
	SetImage(ID_IMAGE_ADVERTISING, L"C:\\Advertising.png");
	SetImage(ID_IMAGE_LANGUAGE, !gbChinese ? IDB_CHINESE : IDB_ENGLISH);
	SetImage(ID_IMAGE_ORDER, IDB_HOME_ORDER);
	SetImage(ID_IMAGE_MEMBER, IDB_HOME_CARD);
	CBCGPVisualContainer* pContainer = GetVisualContainer();
	ASSERT_VALID(pContainer);
	CBCGPImage image(L"C:\\background.png");
	pContainer->SetFillBrush(CBCGPBrush(image, CBCGPColor::SteelBlue, TRUE));
}
void CHomeContainerCtrl::SetImage(UINT nID, CString szImagePath)
{
	CBCGPVisualContainer* pContainer = GetVisualContainer();
	ASSERT_VALID(pContainer);
	CBCGPImageGaugeImpl* pImage = DYNAMIC_DOWNCAST(CBCGPImageGaugeImpl, pContainer->GetByID(nID));
	ASSERT_VALID(pImage);
	pImage->SetImage(CBCGPImage(szImagePath), CBCGPSize(pImage->GetRect().Width(), pImage->GetRect().Height()));
	pImage->SetRoundedShapes();
}
void CHomeContainerCtrl::SetImage(UINT nID, UINT nImageID)
{
	CBCGPVisualContainer* pContainer = GetVisualContainer();
	ASSERT_VALID(pContainer);
	CBCGPImageGaugeImpl* pImage = DYNAMIC_DOWNCAST(CBCGPImageGaugeImpl, pContainer->GetByID(nID));
	
	ASSERT_VALID(pImage);
	pImage->SetImage(CBCGPImage(nImageID), CBCGPSize(pImage->GetRect().Width(), pImage->GetRect().Height()));
	pImage->SetRoundedShapes();
}
void CHomeContainerCtrl::CommandInit()
{
	InitImage();
}
BOOL CHomeContainerCtrl::OnMouseDown(int nButton, const CBCGPPoint& pt)
{
	USES_CONVERSION;
	CBCGPVisualContainer* pContainer = GetVisualContainer();
	ASSERT_VALID(pContainer);
	if (pContainer != NULL && !pContainer->IsSingleSel())
	{
		CBCGPBaseVisualObject* pObject = pContainer->GetFromPoint(pt);
		CBCGPImageGaugeImpl* pImage = DYNAMIC_DOWNCAST(CBCGPImageGaugeImpl, pObject);
		if (pImage != NULL)
		{
			if (pImage->GetID() == ID_IMAGE_LANGUAGE)
			{
				gbChinese = !gbChinese;
				SetImage(ID_IMAGE_LANGUAGE, !gbChinese ? IDB_CHINESE:IDB_ENGLISH);
				SetImage(ID_IMAGE_ORDER, gbChinese ? IDB_HOME_ORDER: IDB_HOME_ORDER);
				SetImage(ID_IMAGE_MEMBER, gbChinese?IDB_HOME_CARD: IDB_HOME_CARD);
			}
			else if (pImage->GetID() == ID_IMAGE_MEMBER)
			{
				//CTipInfo::c_pTip->ShowFloatWindow(this);
				//::PostMessage(AfxGetMainWnd()->m_hWnd, ID_SHOWTOOLTIP, IDS_TIP_MAX_TOPPING, 0);
			}
			else if (pImage->GetID() == ID_IMAGE_ORDER)
			{
				::PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLICK_HOME, 0, 0);
			}
		}
	}
	return FALSE;
}

void CHomeContainerCtrl::OnDraw(CBCGPGraphicsManager * pGM, const CBCGPRect & rectClip)
{
	CBCGPVisualContainer* pVisualContainer = GetVisualContainer();
	if (pVisualContainer != NULL)
	{
		pVisualContainer->OnDraw(pGM, rectClip);
		Invalidate();
	}
}
