#include "stdafx.h"
#include "yoda_self_ordering.h"
#include "OrderView.h"
#include "MainFrm.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

CList<LPORDERINFO, LPORDERINFO>		glstOrder;
int gCurOrderIndex = 1;
extern PRODUCTINFO gCurProduct;
#define ID_CONTINUE_ORDERING	160
#define ID_ORDER_COUNTS			161
#define ID_TOTAL_MONEY			162
#define ID_PAY_BUTTON			163
//////////////////////////////////////////////////////////
//COrderContainerCtrl
IMPLEMENT_SERIAL(COrderContainerCtrl, CBCGPVisualContainerCtrl,1)
COrderContainerCtrl::COrderContainerCtrl() :
	m_pView(NULL)
{
	CBCGPVisualContainer* pContainer = GetVisualContainer();
	ASSERT_VALID(pContainer);
	m_curOrderMode = ORDER_MODE_NONE;
	m_nCurOrderIndex = 0;
}

COrderContainerCtrl::~COrderContainerCtrl()
{

}

BEGIN_MESSAGE_MAP(COrderContainerCtrl, CBCGPVisualContainerCtrl)
	//{{AFX_MSG_MAP(CSettingsView)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

int COrderContainerCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPVisualContainerCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	CBCGPVisualContainer* pContainer = GetVisualContainer();
	ASSERT_VALID(pContainer);
	pContainer->SetOrderFlag(TRUE);
	CBCGPImageGaugeImpl* pImage = DYNAMIC_DOWNCAST(CBCGPImageGaugeImpl, pContainer->GetByID(ID_CONTINUE_ORDERING));
	ASSERT_VALID(pImage);
	pImage->SetImage(CBCGPImage(IDB_CONTINUE), CBCGPSize(pImage->GetRect().Width(), pImage->GetRect().Height()));
	pImage = DYNAMIC_DOWNCAST(CBCGPImageGaugeImpl, pContainer->GetByID(ID_PAY_BUTTON));
	ASSERT_VALID(pImage);
	pImage->SetImage(CBCGPImage(IDB_PAY), CBCGPSize(pImage->GetRect().Width(), pImage->GetRect().Height()));
	return 0;
}
void COrderContainerCtrl::OnInitOrder()
{
	CBCGPVisualContainer* pContainer = GetVisualContainer();
	ASSERT_VALID(pContainer);

	pContainer->RemoveAllOrder();
	for (POSITION pos = glstOrder.GetHeadPosition(); pos != NULL;)
	{
		LPORDERINFO info = glstOrder.GetNext(pos);
		CString szUnit,szSizePrice = L"";
		szUnit.Format(_T("  %.2lf"), info->productInfo.dbUnitMenoy);
		if(info->productInfo.dbSizeMoney>0.1)
			szSizePrice.Format(_T("(%.2lf)"), info->productInfo.dbSizeMoney);
		COrderItemVisualObject* pObject = new COrderItemVisualObject(info->productInfo.szSize+ szSizePrice, info->productInfo.dbMoney, gbChinese? info->productInfo.szProductNameCN+ szUnit :info->productInfo.szProductName + szUnit,
			info->productInfo.szProductThumbFile, info->productInfo.szICE, info->productInfo.bShowHoneyLevel? info->productInfo.szHoney:info->productInfo.szSugar, info->productInfo.szTopping, info->nProductCounts);
		pObject->SetOrderIndex(info->nOrderID);
		pContainer->Add(pObject);
	}
	pContainer->SetSquareSize(CBCGPSize(400, 450));
	pContainer->SetDirty();
	UpdateOrder();
}
void COrderContainerCtrl::UpdateCounts(CString szTotal)
{
	CBCGPVisualContainer* pContainer = GetVisualContainer();
	ASSERT_VALID(pContainer);

	CBCGPTextGaugeImpl* pText = DYNAMIC_DOWNCAST(CBCGPTextGaugeImpl, pContainer->GetByID(ID_ORDER_COUNTS));
	ASSERT_VALID(pText);
	CBCGPTextFormat  pFormat = pText->GetTextFormat();
	pText->SetText(szTotal);
	pText->Redraw();
}
void COrderContainerCtrl::UpdateTotalMoney(CString szMoney)
{
	CBCGPVisualContainer* pContainer = GetVisualContainer();
	ASSERT_VALID(pContainer);

	CBCGPTextGaugeImpl* pText = DYNAMIC_DOWNCAST(CBCGPTextGaugeImpl, pContainer->GetByID(ID_TOTAL_MONEY));
	ASSERT_VALID(pText);
	CBCGPTextFormat  pFormat = pText->GetTextFormat();
	pText->SetText(szMoney);
	pText->Redraw();
}
void COrderContainerCtrl::UpdateOrder()
{
	CString szMoney,szOrder;
	double  dbMoney = 0.;
	int nOrder = 0;
	for (POSITION pos = glstOrder.GetHeadPosition(); pos != NULL;)
	{
		LPORDERINFO info = glstOrder.GetNext(pos);
		dbMoney += info->nProductCounts* info->productInfo.dbMoney;
		nOrder += info->nProductCounts;
	}
	CString szTotalMoney,szTotalCups;
	szTotalMoney.LoadStringW(IDS_TOTAL_MONEY);
	szTotalCups.LoadStringW(IDS_TOTAL_CUPS);
	szMoney.Format(L" : $%.2lf",dbMoney);
	szOrder.Format(L" : %d",nOrder);
	UpdateCounts(szTotalCups+szOrder);
	UpdateTotalMoney(szTotalMoney+szMoney);
}
BOOL COrderContainerCtrl::OnMouseDown(int nButton, const CBCGPPoint& pt)
{
	CBCGPVisualContainer* pContainer = GetVisualContainer();
	COrderItemVisualObject * pObject = DYNAMIC_DOWNCAST(COrderItemVisualObject, pContainer->GetFromPoint(pt));
	if (pObject != NULL)
	{
		COrderItemVisualObject::CONTAINER_BASE_OBJECT nStatus;
		nStatus = pObject->GetFromPoint(pt);
		if (COrderItemVisualObject::CONTAINER_BASE_OBJECT_OTHER != nStatus)
		{
			if (nStatus == COrderItemVisualObject::CONTAINER_BASE_OBJECT_DEL_BUTTON)
			{
				for (POSITION pos = glstOrder.GetHeadPosition(); pos != NULL;)
				{
					int nIndex = pObject->GetOrderIndex();
					LPORDERINFO info = glstOrder.GetNext(pos);
					if (nIndex == info->nOrderID)
					{
						POSITION pos = glstOrder.Find(info);
						glstOrder.RemoveAt(pos);
						pContainer->Remove(pObject);
						break;
					}
				}
			}
			else if (nStatus == COrderItemVisualObject::CONTAINER_BASE_OBJECT_ADD_BUTTON)
			{
				for (POSITION pos = glstOrder.GetHeadPosition(); pos != NULL;)
				{
					int nIndex = pObject->GetOrderIndex();
					LPORDERINFO info = glstOrder.GetNext(pos);
					if (nIndex == info->nOrderID)
					{
						info->nProductCounts++;
						pObject->AddCount();
						break;
					}
				}
				pObject->SetDirty(TRUE, TRUE);
			}
			else if (nStatus == COrderItemVisualObject::CONTAINER_BASE_OBJECT_SUB_BUTTON)
			{

				for (POSITION pos = glstOrder.GetHeadPosition(); pos != NULL;)
				{
					int nIndex = pObject->GetOrderIndex();
					LPORDERINFO info = glstOrder.GetNext(pos);
					if (nIndex == info->nOrderID)
					{
						if (info->nProductCounts > 1)
						{
							info->nProductCounts--;
							pObject->SubCount();
						}
							
						break;
					}
				}
				pObject->SetDirty(TRUE, TRUE);
				
			}
			else if (nStatus == COrderItemVisualObject::CONTAINER_BASE_OBJECT_EDIT)
			{
				for (POSITION pos = glstOrder.GetHeadPosition(); pos != NULL;)
				{
					int nIndex = pObject->GetOrderIndex();
					LPORDERINFO info = glstOrder.GetNext(pos);
					if (nIndex == info->nOrderID)
					{
						gCurProduct = info->productInfo;
						info->bEdit = TRUE;
						break;
					}
				}
				::PostMessage(AfxGetMainWnd()->m_hWnd, WM_EDIT_ORDER, 0, 0);
				return FALSE;
			}
			UpdateOrder();
		}
	}
	CBCGPImageGaugeImpl * pImage = DYNAMIC_DOWNCAST(CBCGPImageGaugeImpl, pContainer->GetFromPoint(pt));
	if (pImage != NULL && pImage->GetID() == ID_CONTINUE_ORDERING)
	{
		::PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLICK_ORDER, 0, 0);
	}
	else if (pImage != NULL && pImage->GetID() == ID_PAY_BUTTON)
	{
		if(glstOrder.GetSize()>0)
			::PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLICK_PAY, 0, 0);
	}
	return FALSE;
}