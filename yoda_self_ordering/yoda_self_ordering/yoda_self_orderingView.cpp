// yoda_self_orderingView.cpp : implementation of the Cyoda_self_orderingView class
//

#include "stdafx.h"
#include "yoda_self_ordering.h"

#include "yoda_self_orderingDoc.h"
#include "yoda_self_orderingView.h"
#include "ItemTile.h"
#include "TeaView.h"
#include "HoneyView.h"
#include "OrderView.h"
#include "IceToppingView.h"
#include "sizeToppingView.h"
#include "ToppingView.h"
#include "CashPop.h"
#include "PaymentView.h"
#include "HomePage.h"
#include  <winspool.h>
extern UINT	gCurViewID;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

PRODUCTINFO gCurProduct ;
extern 	CList<LPORDERINFO, LPORDERINFO>	glstOrder;
extern  CList<LPTOPPINGINFO, LPTOPPINGINFO>	glstTopping;
extern  CList<LPSIZEINFO, LPSIZEINFO>	glstSize;
extern int gCurOrderIndex;

// Cyoda_self_orderingView
IMPLEMENT_DYNCREATE(Cyoda_self_orderingView, CView)

BEGIN_MESSAGE_MAP(Cyoda_self_orderingView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_MESSAGE(WM_PRINTCLIENT, OnPrintClient)
	ON_REGISTERED_MESSAGE(BCGM_CHANGEVISUALMANAGER, OnChangeVisualManager)
	ON_REGISTERED_MESSAGE(BCGM_ON_CLICK_WINUI_CAPTION_BUTTON, OnClickCaptionButton)
	ON_REGISTERED_MESSAGE(BCGM_ON_CREATE_WINUI_VIEW, OnCreateView)
	ON_REGISTERED_MESSAGE(BCGM_ON_CLOSE_WINUI_VIEW, OnCloseView)
	ON_REGISTERED_MESSAGE(BCGM_ON_CLICK_WINUI_UI_TILE, OnClickTile)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

void Cyoda_self_orderingView::AddTile(CBCGPWinUITile* pTile, UINT nImageResID, UINT nFullColorImageResID, int nGroup, UINT nCmdID,
	CBCGPWinUITile::BCGP_WINUI_BADGE_GLYPH badgeGlyph, BOOL bIsFullSizeImage)
{
	ASSERT_VALID(pTile);

	CBCGPWinUITiles* pUITiles = m_wndUITiles.GetWinUITiles();
	ASSERT_VALID(pUITiles);

	CBCGPColor color = pTile->GetBackgroundBrush().GetColor();

	CBCGPColor colorBorder = color;
	colorBorder.MakeLighter(.15);

	pTile->SetUserData(nCmdID);
	pTile->SetBorderColor(colorBorder);

	pTile->SetImage(CBCGPImage(nImageResID), CBCGPWinUITile::BCGP_ANIMATION_NONE, 1000, bIsFullSizeImage);

	pTile->SetBadgeGlyph(badgeGlyph);

	pTile->SetToolTipText(pTile->GetName());

	CFrameWnd* pParent = GetParentFrame();
	if (pParent->GetSafeHwnd() != NULL && nCmdID != 0)
	{
		CString strDescr;
		pParent->GetMessageString(nCmdID, strDescr);

		pTile->SetToolTipDescription(strDescr);
	}

	pUITiles->Add(pTile, nGroup);
	pTile->SetView(NULL);	// Generic view
}
// Cyoda_self_orderingView construction/destruction

Cyoda_self_orderingView::Cyoda_self_orderingView()
{
	// TODO: add construction code here
	gCurViewID = 0;
	m_bEditOrder = FALSE;
	m_bClickCart = FALSE;
}

Cyoda_self_orderingView::~Cyoda_self_orderingView()
{

}

BOOL Cyoda_self_orderingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style &= ~WS_BORDER;
	return CView::PreCreateWindow(cs);
}

// Cyoda_self_orderingView drawing

void Cyoda_self_orderingView::OnDraw(CDC* /*pDC*/)
{
	Cyoda_self_orderingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}


// Cyoda_self_orderingView printing

void Cyoda_self_orderingView::OnFilePrintPreview()
{
	BCGPPrintPreview (this);
}

BOOL Cyoda_self_orderingView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void Cyoda_self_orderingView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void Cyoda_self_orderingView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// Cyoda_self_orderingView diagnostics

#ifdef _DEBUG
void Cyoda_self_orderingView::AssertValid() const
{
	CView::AssertValid();
}

void Cyoda_self_orderingView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Cyoda_self_orderingDoc* Cyoda_self_orderingView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cyoda_self_orderingDoc)));
	return (Cyoda_self_orderingDoc*)m_pDocument;
}
#endif //_DEBUG


// Cyoda_self_orderingView message handlers

LRESULT Cyoda_self_orderingView::OnPrintClient(WPARAM wp, LPARAM lp)
{
	if ((lp & PRF_CLIENT) == PRF_CLIENT)
	{
		CDC* pDC = CDC::FromHandle((HDC) wp);
		ASSERT_VALID(pDC);
		
		OnDraw(pDC);
	}
	
	return 0;
}

LRESULT Cyoda_self_orderingView::OnChangeVisualManager(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	return 0;
}
void Cyoda_self_orderingView::SetThemeColors()
{
	CBCGPWinUITiles* pUITiles = m_wndUITiles.GetWinUITiles();
	ASSERT_VALID(pUITiles);

	CBCGPColor colorTheme = theApp.GetBrushColor(pUITiles->GetFillBrush());

	CBCGPVisualScrollBarColorTheme theme(colorTheme);
	pUITiles->SetScrollBarColorTheme(theme);

	if (colorTheme.IsPale())
	{
		colorTheme.MakeDarker(.05);
	}

#ifndef _BCGSUITE_INC_
	CBCGPVisualManager2013::SetAccentColorRGB(colorTheme);

	CBCGPColor colorText = pUITiles->GetCaptionForegroundColor();
	CBCGPColor colorInaciveText = colorText; colorInaciveText.MakeDarker();

	CBCGPVisualManager2013::SetFrameColor(colorTheme, colorText, colorInaciveText);
#endif
}

void Cyoda_self_orderingView::CreateCategory()
{
	CBCGPWinUITiles* pUITiles = m_wndUITiles.GetWinUITiles();
	ASSERT_VALID(pUITiles);
	pUITiles->RemoveCaptionButtons();
	if (pUITiles->GetCurrentView() != NULL)
	{
		pUITiles->OnCloseView();
	}
	pUITiles->RemoveAll();
	pUITiles->SetGroupCaption(0, gbChinese?L"选择一种饮品":L"CHOOSE A CATEGROY", CBCGPColor::White);
	CBCGPTextFormat format = pUITiles->GetGroupCaptionTextFormat();
	format.SetTextAlignment(CBCGPTextFormat::BCGP_TEXT_ALIGNMENT::BCGP_TEXT_ALIGNMENT_CENTER);
	pUITiles->SetGroupCaptionTextFormat(format);
	for (POSITION pos = m_lstCategroy.GetHeadPosition(); pos != NULL;)
	{
		LPCATEGROYINFO pCategroy = m_lstCategroy.GetNext(pos);
		CCategoryTile * pItem = new CCategoryTile(pCategroy->nCategroyID, gbChinese? pCategroy->szCategroyNameCN:pCategroy->szCategroyName, gbChinese ? pCategroy->szCateGroyFileNameCN:pCategroy->szCateGroyFileName,
			pCategroy->text_clr,pCategroy->background_clr);
		AddTile(pItem, 0, 0, 0);
	}
	SetThemeColors();
	pUITiles->SetScrollBarStyle(CBCGPVisualScrollBar::BCGP_VISUAL_SCROLLBAR_STYLE::BCGP_VISUAL_SCROLLBAR_3D_ROUNDED);
	EnableHomeButton();
	CBCGPImage image(L"C:\\background.png");
	pUITiles->SetFillBrush(CBCGPBrush(image, CBCGPColor::SteelBlue, TRUE), FALSE);
	if (glstOrder.GetSize() > 0)
		EnableCartButton();
	pUITiles->SetDirty(TRUE, TRUE);
	gCurViewID = ID_VIEW_CATEGORY;
}
void Cyoda_self_orderingView::SetCurProduct(int nCategroy, int nProductID)
{
	for (POSITION pos = m_lstProduct.GetHeadPosition(); pos != NULL;)
	{
		PRODUCTINFO* pProduct = m_lstProduct.GetNext(pos);
		if (pProduct->nCategroyID == nCategroy &&
			pProduct->nProductID == nProductID)
		{	
			gCurProduct = *pProduct;
			if (gbChinese)
			{
				gCurProduct.szICE = L"正常冰";
				gCurProduct.szSize = L"中杯";
			}
			return;
		}
	}
}
void Cyoda_self_orderingView::ShowView(int nViewID)
{
	CBCGPWinUITiles* pUITiles = m_wndUITiles.GetWinUITiles();
	CBCGPRect rcTiles = pUITiles->GetTilesArea();
	CBCGPRect rcContainer = CBCGPRect(0, 0, GetSystemMetrics(SM_CXSCREEN),rcTiles.bottom);
	switch (nViewID)
	{
	case ID_VIEW_ICE_SUGAR_TOPPING:
		pUITiles->OnShowView(RUNTIME_CLASS(CNormalContainerCtrl), IDR_BCGP_VISUAL_XML1, rcContainer,true);
		break;
	case ID_VIEW_ICE_HONEY_TOPPING:
		pUITiles->OnShowView(RUNTIME_CLASS(CHoneyContainerCtrl), IDR_BCGP_VISUAL_XML3, rcContainer, true);
		break;
	case ID_VIEW_ICE_TOPPING:
		pUITiles->OnShowView(RUNTIME_CLASS(CIceToppingContainerCtrl), IDR_BCGP_VISUAL_XML4, rcContainer, true);
		break;
	case ID_VIEW_SIZE_TOPPING:
		pUITiles->OnShowView(RUNTIME_CLASS(CSizeToppingContainerCtrl), IDR_BCGP_VISUAL_XML5, rcContainer, true);
		break;
	case ID_VIEW_TOPPING:
		pUITiles->OnShowView(RUNTIME_CLASS(CToppingContainerCtrl), IDR_BCGP_VISUAL_XML6, rcContainer, true);
		break;
	case ID_VIEW_CHECK_ORDERING:
		pUITiles->OnShowView(RUNTIME_CLASS(COrderContainerCtrl), IDR_BCGP_VISUAL_XML2, rcContainer, true);
		break;
	case ID_VIEW_PAYMENT:
		pUITiles->OnShowView(RUNTIME_CLASS(CPayContainerCtrl), IDR_BCGP_VISUAL_XML7, rcContainer, true);
		break;
	case ID_VIEW_HOME:
		pUITiles->OnShowView(RUNTIME_CLASS(CHomeContainerCtrl), IDR_BCGP_VISUAL_XML8, CBCGPRect(0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)), false);
		break;
	}	
}
BOOL Cyoda_self_orderingView::InitCfg()
{
	glstTopping.RemoveAll();
	glstSize.RemoveAll();
	CString szSQL;
	szSQL.Format(L"select * from y_Item where cat_no = 11");
	CAdoRecordSet recordset;
	recordset.SetAdoConnection(gpDB);
	recordset.SetCursorLocation();
	try
	{
		if (!recordset.Open(szSQL))
		{
			return FALSE;
		}
	}
	catch (_com_error e)
	{
		return FALSE;
	}

	int nCount = recordset.GetRecordCount();
	if (nCount <= 0)
	{
		recordset.Close();
		return FALSE;
	}
	CString szName, szNameCN,szCode;
	int  nID;
	double dbPrice;
	while (!recordset.IsEOF())
	{
		LPTOPPINGINFO pTopping = new TOPPINGINFO();
		recordset.GetFieldValue("item_code", szCode);
		recordset.GetFieldValue("item_no", nID);
		recordset.GetFieldValue("item_desc", szName);
		recordset.GetFieldValue("zh_name", szNameCN);
		recordset.GetFieldValue("unit_price", dbPrice);
		pTopping->szItemName = szName;
		pTopping->szItemNameCN = szNameCN;
		pTopping->szItemCode = szCode;
		pTopping->nItemId = nID;
		pTopping->ItemPrice = dbPrice;
		glstTopping.AddTail(pTopping);
		recordset.MoveNext();
	}
	recordset.Close();
	szSQL.Format(L"select * from y_Item where cat_no = 10");
	recordset.SetAdoConnection(gpDB);
	recordset.SetCursorLocation();
	try
	{
		if (!recordset.Open(szSQL))
		{
			return FALSE;
		}
	}
	catch (_com_error e)
	{
		return FALSE;
	}

	nCount = recordset.GetRecordCount();
	if (nCount <= 0)
	{
		recordset.Close();
		return FALSE;
	}
	while (!recordset.IsEOF())
	{
		LPSIZEINFO pSize = new SIZEINFO();
		recordset.GetFieldValue("item_code", szCode);
		recordset.GetFieldValue("item_no", nID);
		recordset.GetFieldValue("item_desc", szName);
		recordset.GetFieldValue("zh_name", szNameCN);
		recordset.GetFieldValue("unit_price", dbPrice);
		pSize->szItemName = szName;
		pSize->szItemNameCN = szNameCN;
		pSize->szItemCode = szCode;
		pSize->nItemId = nID;
		pSize->ItemPrice = dbPrice;
		glstSize.AddTail(pSize);
		recordset.MoveNext();
	}
	recordset.Close();
	return TRUE;
}
BOOL Cyoda_self_orderingView::InitCategroy()
{
	m_lstCategroy.RemoveAll();
	CString szSQL;
	szSQL.Format(L"select * from y_category where isShow = 'Y'");
	CAdoRecordSet recordset;
	recordset.SetAdoConnection(gpDB);
	recordset.SetCursorLocation();
	try
	{
		if (!recordset.Open(szSQL))
		{
			return FALSE;
		}
	}
	catch (_com_error e)
	{
		return FALSE;
	}

	int nCount = recordset.GetRecordCount();
	if (nCount <= 0)
	{
		recordset.Close();
		return FALSE;
	}
	CString szName, szPath, szFile,szNameCN,szFileCN;
	int  nCategroy, nTextColor, nBackgroundColor;
	while (!recordset.IsEOF())
	{
		LPCATEGROYINFO pCategroy = new CATEGROYINFO();
		recordset.GetFieldValue("categroy_no", nCategroy);
		recordset.GetFieldValue("categroy_name", szName);
		recordset.GetFieldValue("categroy_zh_name", szNameCN);
		recordset.GetFieldValue("categroy_imagepath", szPath);
		recordset.GetFieldValue("categroy_filename", szFile);
		recordset.GetFieldValue("categroy_zh_filename", szFileCN);
		recordset.GetFieldValue("text_color", nTextColor);
		recordset.GetFieldValue("background_color", nBackgroundColor);
		pCategroy->nCategroyID = nCategroy;
		pCategroy->szCategroyName = szName;
		pCategroy->szCateGroyFileName = szPath + szFile;
		pCategroy->szCategroyNameCN = szNameCN;
		pCategroy->szCateGroyFileNameCN = szPath + szFileCN;
		pCategroy->text_clr = CBCGPColor(nTextColor);
		pCategroy->background_clr = CBCGPColor(nBackgroundColor);
		m_lstCategroy.AddTail(pCategroy);
		recordset.MoveNext();
	}
	recordset.Close();
	return TRUE;
}

BOOL Cyoda_self_orderingView::InitProduct()
{
	m_lstProduct.RemoveAll();
	CString szSQL;
	szSQL.Format(L"select * from y_item  where CURDATE() >= st_date and CURDATE() <ed_date order by item_desc");
	CAdoRecordSet recordset;
	recordset.SetAdoConnection(gpDB);
	recordset.SetCursorLocation();
	try
	{
		if (!recordset.Open(szSQL))
		{
			return FALSE;
		}
	}
	catch (_com_error e)
	{
		return FALSE;
	}

	int nCount = recordset.GetRecordCount();
	if (nCount <= 0)
	{
		recordset.Close();
		return FALSE;
	}
	CString szName, szPath, szFile,szThumbFile, szNameCN,szFileCN,szCode,szPrinter;
	int  nCategroy,nProduct, nTextColor, nBackgroundColor;
	BOOL bShowSugar, bShowHoney,bShowIce,bShowSize;
	double dbMoney;
	while (!recordset.IsEOF())
	{
		PRODUCTINFO* pProduct = new PRODUCTINFO();
		recordset.GetFieldValue("cat_no", nCategroy);
		recordset.GetFieldValue("item_no", nProduct);
		recordset.GetFieldValue("item_code", szCode);
		recordset.GetFieldValue("item_desc", szName);
		recordset.GetFieldValue("zh_name", szNameCN);
		recordset.GetFieldValue("image_path", szPath);
		recordset.GetFieldValue("image_file_name", szFile);
		recordset.GetFieldValue("image_zh_file_name", szFileCN);
		recordset.GetFieldValue("thumb_file_name", szThumbFile);
		recordset.GetFieldValue("text_color", nTextColor);
		recordset.GetFieldValue("background_color", nBackgroundColor);
		recordset.GetFieldValue("show_sugar_level", bShowSugar);
		recordset.GetFieldValue("show_honey_level", bShowHoney);
		recordset.GetFieldValue("show_ice", bShowIce);
		recordset.GetFieldValue("show_size", bShowSize);
		recordset.GetFieldValue("unit_price", dbMoney);
		recordset.GetFieldValue("printer_name", szPrinter);

		pProduct->nCategroyID = nCategroy;
		pProduct->nProductID = nProduct;
		pProduct->szItemCode = szCode;
		pProduct->szProductName = szName;
		pProduct->szProductNameCN = szNameCN;
		pProduct->szProductFileName = szPath+szFile;
		pProduct->szProductFileNameCN = szPath + szFileCN;
		pProduct->text_clr = CBCGPColor(nTextColor);
		pProduct->background_clr = CBCGPColor(nBackgroundColor);
		pProduct->bShowSugarLevel = bShowSugar;
		pProduct->bShowHoneyLevel = bShowHoney;
		pProduct->bShowIce = bShowIce;
		pProduct->bShowSize = bShowSize;
		pProduct->szPrinter = szPrinter;
		if (szThumbFile.IsEmpty())
			pProduct->szProductThumbFile = pProduct->szProductFileName;
		else pProduct->szProductThumbFile = szPath+szThumbFile;
		pProduct->dbUnitMenoy = pProduct->dbMoney = dbMoney;
		m_lstProduct.AddTail(pProduct);
		recordset.MoveNext();
	}
	recordset.Close();
	return TRUE;
}

void Cyoda_self_orderingView::CreateProduct(int nCategroy)
{
	CBCGPWinUITiles* pUITiles = m_wndUITiles.GetWinUITiles();
	ASSERT_VALID(pUITiles);
	if (m_lstProduct.GetSize() <= 0)
		return;
	if (pUITiles->GetCurrentView() != NULL)
	{
		pUITiles->OnCloseView();
	}
	pUITiles->RemoveCaptionButtons();
	pUITiles->RemoveAll();
	for (POSITION pos = m_lstCategroy.GetHeadPosition(); pos != NULL;)
	{
		LPCATEGROYINFO pCategroy = m_lstCategroy.GetNext(pos);
		if (pCategroy->nCategroyID == nCategroy)
		{
			pUITiles->SetGroupCaption(0, gbChinese? pCategroy->szCategroyNameCN:pCategroy->szCategroyName, CBCGPColor::White);
			CBCGPTextFormat format = pUITiles->GetGroupCaptionTextFormat();
			format.SetTextAlignment(CBCGPTextFormat::BCGP_TEXT_ALIGNMENT::BCGP_TEXT_ALIGNMENT_CENTER);
			pUITiles->SetGroupCaptionTextFormat(format);
			break;
		}
	}
	for (POSITION pos = m_lstProduct.GetHeadPosition(); pos != NULL;)
	{
		PRODUCTINFO* pProduct = m_lstProduct.GetNext(pos);
		if (pProduct->nCategroyID == nCategroy)
		{
			CProductTile * pItem = new CProductTile(pProduct->nCategroyID,pProduct->nProductID,gbChinese? pProduct->szProductNameCN:pProduct->szProductName,
				gbChinese ? pProduct->szProductFileNameCN:pProduct->szProductFileName, pProduct->text_clr, pProduct->background_clr);
			AddTile(pItem, 0, 0, 0);
			if(pProduct->bShowHoneyLevel)
				pItem->SetView(RUNTIME_CLASS(CHoneyContainerCtrl), IDR_BCGP_VISUAL_XML3);
			else if(pProduct->bShowSugarLevel)
				pItem->SetView(RUNTIME_CLASS(CNormalContainerCtrl), IDR_BCGP_VISUAL_XML1);
			else if(!pProduct->bShowIce && pProduct->bShowSize)
				pItem->SetView(RUNTIME_CLASS(CSizeToppingContainerCtrl), IDR_BCGP_VISUAL_XML5);
			else if (!pProduct->bShowIce && !pProduct->bShowSize)
				pItem->SetView(RUNTIME_CLASS(CToppingContainerCtrl), IDR_BCGP_VISUAL_XML6);
			else 
				pItem->SetView(RUNTIME_CLASS(CIceToppingContainerCtrl), IDR_BCGP_VISUAL_XML4);
		}
	}
	SetThemeColors();
	pUITiles->SetScrollBarStyle(CBCGPVisualScrollBar::BCGP_VISUAL_SCROLLBAR_STYLE::BCGP_VISUAL_SCROLLBAR_3D_ROUNDED);
	EnableBackButton();
	EnableHomeButton();
	CBCGPImage image(L"C:\\background.png");
	pUITiles->SetFillBrush(CBCGPBrush(image, CBCGPColor::SteelBlue, TRUE), FALSE);
	pUITiles->SetDirty(TRUE, TRUE);
	gCurViewID = ID_VIEW_PRODUCT;
}
void Cyoda_self_orderingView::EnableBackButton()
{
	CBCGPWinUITiles* pUITiles = m_wndUITiles.GetWinUITiles();
	ASSERT_VALID(pUITiles);
	CBCGPWinUITilesCaptionButton* pCaptionButtonBack = new CBCGPWinUITilesCaptionButton(ID_BACK);
	pCaptionButtonBack->SetName(gbChinese?_T("返回"):_T("BACK"));
	CBCGPImage backimage(IDB_BACK);
	pCaptionButtonBack->SetImage(backimage);
	pUITiles->AddCaptionButton(pCaptionButtonBack);
}
void Cyoda_self_orderingView::EnableHomeButton()
{
	CBCGPWinUITiles* pUITiles = m_wndUITiles.GetWinUITiles();
	ASSERT_VALID(pUITiles);
	CBCGPWinUITilesCaptionButton* pCaptionButtonHome = new CBCGPWinUITilesCaptionButton(ID_HOME);
	pCaptionButtonHome->SetName(gbChinese ? _T("主页") : _T("HOME"));
	pCaptionButtonHome->SetCenterAligned();
	CBCGPImage image1(IDB_HOME);
	pCaptionButtonHome->SetImage(image1);
	pUITiles->AddCaptionButton(pCaptionButtonHome);
}
void Cyoda_self_orderingView::EnableCartButton()
{
	CBCGPWinUITiles* pUITiles = m_wndUITiles.GetWinUITiles();
	ASSERT_VALID(pUITiles);
	CBCGPWinUITilesCaptionButton* pCaptionButtonCart = new CBCGPWinUITilesCaptionButton(ID_CART);
	pCaptionButtonCart->SetName(gbChinese ? _T("订单页") : _T("CART"));
	CBCGPImage image1(IDB_BACK);
	pCaptionButtonCart->SetImage(image1);
	pUITiles->AddCaptionButton(pCaptionButtonCart);
}
void Cyoda_self_orderingView::EnableNextButton()
{
	CBCGPWinUITiles* pUITiles = m_wndUITiles.GetWinUITiles();
	ASSERT_VALID(pUITiles);
	CBCGPWinUITilesCaptionButton* pCaptionButtonNext= new CBCGPWinUITilesCaptionButton(ID_NEXT);
	pCaptionButtonNext->SetName(gbChinese ? _T("继续") : _T("Next"));
	pCaptionButtonNext->SetRightAligned();
	pCaptionButtonNext->SetView(RUNTIME_CLASS(COrderContainerCtrl), IDR_BCGP_VISUAL_XML2);
	CBCGPImage image1(IDB_NEXT);
	pCaptionButtonNext->SetImage(image1);
	pUITiles->AddCaptionButton(pCaptionButtonNext);
}
void Cyoda_self_orderingView::EnablePaymentButton()
{
	CBCGPWinUITiles* pUITiles = m_wndUITiles.GetWinUITiles();
	ASSERT_VALID(pUITiles);
	CBCGPWinUITilesCaptionButton* pCaptionButtonPayment = new CBCGPWinUITilesCaptionButton(ID_PAYMENT);
	pCaptionButtonPayment->SetName(gbChinese ? _T("付款") : _T("PAY"));
	pCaptionButtonPayment->SetRightAligned();
	CBCGPImage image1(IDB_CAPTION_PAY_BUTTON);
	pCaptionButtonPayment->SetImage(image1);
	pUITiles->AddCaptionButton(pCaptionButtonPayment);
}
void Cyoda_self_orderingView::InitUITiles()
{
	CBCGPWinUITiles* pUITiles = m_wndUITiles.GetWinUITiles();
	ASSERT_VALID(pUITiles);
	pUITiles->SetRect(CBCGPRect(0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)),1);
	pUITiles->SetSquareSize(CBCGPSize(315, 405));
	pUITiles->SetHorizontalLayout(0);
	pUITiles->SetVertMargin(30);
	CBCGPTextFormat* pFormat = new CBCGPTextFormat(_T("Arial"), 25, FW_EXTRABOLD);
	pFormat->SetTextAlignment(CBCGPTextFormat::BCGP_TEXT_ALIGNMENT::BCGP_TEXT_ALIGNMENT_CENTER);
	pFormat->SetWordWrap();
	pUITiles->SetNameTextFormat(*pFormat);
	delete pFormat;
	pUITiles->EnableIconsScaleByDPI();
	pUITiles->SetHorzMargin(20);
	pUITiles->SetRoundedShapes();
}
LRESULT Cyoda_self_orderingView::OnClickCaptionButton(WPARAM /*wp*/, LPARAM lp)
{
	CBCGPWinUITilesCaptionButton* pButton = (CBCGPWinUITilesCaptionButton*)lp;
	ASSERT_VALID(pButton);
	if (pButton->GetCommandID() == ID_BACK)
	{
		if (gCurViewID == ID_VIEW_PRODUCT)
		{
			CreateCategory();
		}
		else if (gCurViewID == ID_VIEW_ICE_SUGAR_TOPPING ||
				 gCurViewID == ID_VIEW_ICE_HONEY_TOPPING ||
				 gCurViewID == ID_VIEW_ICE_TOPPING		  ||
				 gCurViewID == ID_VIEW_SIZE_TOPPING	      ||
				 gCurViewID == ID_VIEW_TOPPING
			   )
		{
			CreateProduct(gCurProduct.nCategroyID);
		}
	}
	else if (pButton->GetCommandID() == ID_HOME)
	{
		/*if (glstOrder.GetSize() > 0)
		{
			BCGP_MSGBOXPARAMS params;
			LOGFONT lf;
			globalData.fontRegular.GetLogFont(&lf);
			memcpy(lf.lfFaceName, L"Arial",5);
			lf.lfHeight = 2 * lf.lfHeight;
			lf.lfWeight = FW_BOLD;
			HFONT hFont = ::CreateFontIndirect(&lf);
			params.hfontText = hFont;
			params.lpszCaption = L"";
			params.lpszText = L"If jump to home Page,current order will be cancel.\nWill you continue!\n";
			params.bUseNativeControls = 0;
			params.dwStyle = MB_OKCANCEL;
			params.dwStyle |= MB_ICONWARNING;
			params.bIgnoreStandardButtons = FALSE;
			params.bDrawButtonsBanner = 0;
			
			params.hInstance = ::GetModuleHandle(NULL);
			
			if (IDYES == BCGPMessageBoxIndirect(&params))
			{
				ShowView(ID_VIEW_HOME);
				
			}
		}
		else*/
		{
			ShowView(ID_VIEW_HOME);
		}
	}
	else if (pButton->GetCommandID() == ID_CART)
	{
		m_bClickCart = TRUE;
		CBCGPWinUITiles* pUITiles = m_wndUITiles.GetWinUITiles();
		ASSERT_VALID(pUITiles);
		pUITiles->RemoveTiles();
		pUITiles->RemoveCaptionButtons();
		EnableBackButton();
		EnableHomeButton();
		EnableNextButton();
		pUITiles->SetFillBrush(CBCGPBrush(CBCGPColor::Black));

		ShowView(ID_VIEW_CHECK_ORDERING);
	}
	else if (pButton->GetCommandID() == ID_PAYMENT)
	{
		ShowView(ID_VIEW_PAYMENT);
	}
	return 0;
}

LRESULT Cyoda_self_orderingView::OnCreateView(WPARAM, LPARAM lp)
{
	CWnd* pView = (CWnd*)lp;
	CNormalContainerCtrl* pTeaView = DYNAMIC_DOWNCAST(CNormalContainerCtrl, pView);
	if (pTeaView != NULL)
	{
		CBCGPWinUITiles* pUITiles = m_wndUITiles.GetWinUITiles();
		ASSERT_VALID(pUITiles);
		CBCGPWinUITile* pTile = DYNAMIC_DOWNCAST(CBCGPWinUITile, pUITiles->GetViewParent());
		CProductTile* pProduct = DYNAMIC_DOWNCAST(CProductTile, pTile);
		if (pTile != NULL)
		{
			SetCurProduct(pProduct->GetCategroyID(), pProduct->GetProductID());
			pTeaView->CommandInit(pProduct->GetName());
			pUITiles->RemoveTiles();
			pUITiles->RemoveCaptionButtons();
			EnableBackButton();
			EnableHomeButton();
			EnableNextButton();
			pUITiles->SetFillBrush(CBCGPBrush(CBCGPColor::Black));
		}
		else
		{
			pTeaView->CommandInit(gCurProduct.szProductName);
			pUITiles->RemoveTiles();
			pUITiles->RemoveCaptionButtons();
			m_bEditOrder = TRUE;
			EnableNextButton();
		}
		pUITiles->SetDirty(TRUE, TRUE);
		gCurViewID = ID_VIEW_ICE_SUGAR_TOPPING;
		
	}
	CHoneyContainerCtrl* pHoneyView = DYNAMIC_DOWNCAST(CHoneyContainerCtrl, pView);
	if (pHoneyView != NULL)
	{
		CBCGPWinUITiles* pUITiles = m_wndUITiles.GetWinUITiles();
		ASSERT_VALID(pUITiles);
		CBCGPWinUITile* pTile = DYNAMIC_DOWNCAST(CBCGPWinUITile, pUITiles->GetViewParent());
		CProductTile* pProduct = DYNAMIC_DOWNCAST(CProductTile, pTile);
		if (pTile != NULL)
		{
			SetCurProduct(pProduct->GetCategroyID(), pProduct->GetProductID());
			pHoneyView->CommandInit(pProduct->GetName());
			pUITiles->RemoveTiles();
			pUITiles->RemoveCaptionButtons();
			EnableBackButton();
			EnableHomeButton();
			EnableNextButton();
			pUITiles->SetFillBrush(CBCGPBrush(CBCGPColor::Black));
		}
		else
		{
			pHoneyView->CommandInit(gCurProduct.szProductName);
			pUITiles->RemoveTiles();
			pUITiles->RemoveCaptionButtons();
			m_bEditOrder = TRUE;
			EnableNextButton();
		}
		pUITiles->SetDirty(TRUE, TRUE);
		gCurViewID = ID_VIEW_ICE_HONEY_TOPPING;
	}
	CIceToppingContainerCtrl* pIceToppingView = DYNAMIC_DOWNCAST(CIceToppingContainerCtrl, pView);
	if (pIceToppingView != NULL)
	{
		CBCGPWinUITiles* pUITiles = m_wndUITiles.GetWinUITiles();
		ASSERT_VALID(pUITiles);
		CBCGPWinUITile* pTile = DYNAMIC_DOWNCAST(CBCGPWinUITile, pUITiles->GetViewParent());
		CProductTile* pProduct = DYNAMIC_DOWNCAST(CProductTile, pTile);
		if (pTile != NULL)
		{
			SetCurProduct(pProduct->GetCategroyID(), pProduct->GetProductID());
			pIceToppingView->CommandInit(pProduct->GetName());
			pUITiles->RemoveTiles();
			pUITiles->RemoveCaptionButtons();
			EnableBackButton();
			EnableHomeButton();
			EnableNextButton();
			pUITiles->SetFillBrush(CBCGPBrush(CBCGPColor::Black));
		}
		else
		{
			pIceToppingView->CommandInit(gCurProduct.szProductName);
			pUITiles->RemoveTiles();
			pUITiles->RemoveCaptionButtons();
			m_bEditOrder = TRUE;
			EnableNextButton();
		}
		pUITiles->SetDirty(TRUE, TRUE);
		gCurViewID = ID_VIEW_ICE_TOPPING;
	}
	CSizeToppingContainerCtrl* pSizeToppingView = DYNAMIC_DOWNCAST(CSizeToppingContainerCtrl, pView);
	if (pSizeToppingView != NULL)
	{
		CBCGPWinUITiles* pUITiles = m_wndUITiles.GetWinUITiles();
		ASSERT_VALID(pUITiles);
		CBCGPWinUITile* pTile = DYNAMIC_DOWNCAST(CBCGPWinUITile, pUITiles->GetViewParent());
		CProductTile* pProduct = DYNAMIC_DOWNCAST(CProductTile, pTile);
		if (pTile != NULL)
		{
			SetCurProduct(pProduct->GetCategroyID(), pProduct->GetProductID());
			pSizeToppingView->CommandInit(pProduct->GetName());
			pUITiles->RemoveTiles();
			pUITiles->RemoveCaptionButtons();
			EnableBackButton();
			EnableHomeButton();
			EnableNextButton();
			pUITiles->SetFillBrush(CBCGPBrush(CBCGPColor::Black));
		}
		else
		{
			pSizeToppingView->CommandInit(gCurProduct.szProductName);
			pUITiles->RemoveTiles();
			pUITiles->RemoveCaptionButtons();
			m_bEditOrder = TRUE;
			EnableNextButton();
		}
		pUITiles->SetDirty(TRUE, TRUE);
		gCurViewID = ID_VIEW_SIZE_TOPPING;
	}
	CToppingContainerCtrl* pToppingView = DYNAMIC_DOWNCAST(CToppingContainerCtrl, pView);
	if (pToppingView != NULL)
	{
		CBCGPWinUITiles* pUITiles = m_wndUITiles.GetWinUITiles();
		ASSERT_VALID(pUITiles);
		CBCGPWinUITile* pTile = DYNAMIC_DOWNCAST(CBCGPWinUITile, pUITiles->GetViewParent());
		CProductTile* pProduct = DYNAMIC_DOWNCAST(CProductTile, pTile);
		if (pTile != NULL)
		{
			SetCurProduct(pProduct->GetCategroyID(), pProduct->GetProductID());
			pToppingView->CommandInit(pProduct->GetName());
			pUITiles->RemoveTiles();
			pUITiles->RemoveCaptionButtons();
			EnableBackButton();
			EnableHomeButton();
			EnableNextButton();
			pUITiles->SetFillBrush(CBCGPBrush(CBCGPColor::Black));
		}
		else
		{
			pToppingView->CommandInit(gCurProduct.szProductName);
			pUITiles->RemoveTiles();
			pUITiles->RemoveCaptionButtons();
			m_bEditOrder = TRUE;
			EnableNextButton();
		}
		pUITiles->SetDirty(TRUE, TRUE);
		gCurViewID = ID_VIEW_TOPPING;
	}
	COrderContainerCtrl* pOrderView = DYNAMIC_DOWNCAST(COrderContainerCtrl, pView);
	if (pOrderView != NULL)
	{
		CBCGPWinUITiles* pUITiles = m_wndUITiles.GetWinUITiles();
		ASSERT_VALID(pUITiles);
		if(!gCurProduct.szProductName.IsEmpty())
		{
			if (!m_bEditOrder && !m_bClickCart)
			{
				LPORDERINFO orderinfo = new ORDERINFO();
				orderinfo->productInfo = gCurProduct;
				orderinfo->nOrderID = gCurOrderIndex++;
				glstOrder.AddTail(orderinfo);
			}
			else
			{
				for (POSITION pos = glstOrder.GetHeadPosition(); pos != NULL;)
				{
					LPORDERINFO info = glstOrder.GetNext(pos);
					if (info->bEdit)
					{
						info->productInfo = gCurProduct;
						info->bEdit = FALSE;
					}
				}
				m_bEditOrder = FALSE;
			}
			
			gCurProduct.clear();
		}
		m_bClickCart = FALSE;
		pOrderView->OnInitOrder();
		pUITiles->RemoveTiles();
		pUITiles->RemoveCaptionButtons();
		EnableHomeButton();
		//EnablePaymentButton();
		pUITiles->SetDirty(TRUE, TRUE);
		gCurViewID = ID_VIEW_CHECK_ORDERING;
	}
	CPayContainerCtrl* pPayView = DYNAMIC_DOWNCAST(CPayContainerCtrl, pView);
	if (pPayView)
	{
		CBCGPWinUITiles* pUITiles = m_wndUITiles.GetWinUITiles();
		ASSERT_VALID(pUITiles);
		pPayView->CommandInit();
		pUITiles->RemoveTiles();
		pUITiles->RemoveCaptionButtons();
		EnableHomeButton();
		EnableCartButton();
		pUITiles->SetDirty(TRUE, TRUE);
		gCurViewID = ID_VIEW_PAYMENT;
	}
	CHomeContainerCtrl* pHomeView = DYNAMIC_DOWNCAST(CHomeContainerCtrl, pView);
	if (pHomeView)
	{
		CBCGPWinUITiles* pUITiles = m_wndUITiles.GetWinUITiles();
		ASSERT_VALID(pUITiles);
		pHomeView->CommandInit();
		pUITiles->RemoveTiles();
		pUITiles->RemoveCaptionButtons();
		pUITiles->SetDirty(TRUE, TRUE);
		gCurViewID = ID_VIEW_HOME;
	}
	return 0;
}

LRESULT Cyoda_self_orderingView::OnCloseView(WPARAM, LPARAM)
{
	CBCGPWinUITiles* pUITiles = m_wndUITiles.GetWinUITiles();
	ASSERT_VALID(pUITiles);
	return 0;
}
LRESULT Cyoda_self_orderingView::OnClickTile(WPARAM, LPARAM lp)
{
	CWnd* pView = (CWnd*)lp;

	CCategoryTile* pCateView = DYNAMIC_DOWNCAST(CCategoryTile, pView);
	if (pCateView != NULL && gCurViewID == ID_VIEW_CATEGORY)
	{
		CreateProduct(pCateView->GetCategroyID());
	}
	return 0;
}
int Cyoda_self_orderingView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	theApp.SetVisualTheme(CBCGPWinApp::BCGP_VISUAL_THEME_OFFICE_2016_BLACK);
	// TODO:  Add your specialized creation code here
	if (!m_wndUITiles.Create(CRect(0, 0, 0, 0), this, 1))
	{
		return -1;
	}
	CBCGPWinUITiles* pUITiles = m_wndUITiles.GetWinUITiles();
	pUITiles->SetFillBrush(CBCGPBrush(CBCGPColor::Black));
	if (!InitCategroy())
	{
		MessageBox(L"INIT MENU ERROR.", L"ERROR", 0);
		exit(0);
	}	
	if (!InitProduct())
	{
		MessageBox(L"INIT MENU ERROR.", L"ERROR", 0);
		exit(0);
	}
	InitCfg();
	InitUITiles();
	//CreateCategory();
	ShowView(ID_VIEW_HOME);
	//RedrawWindow();
	return 0;
}
//获得默认打印机名称和默认的纸张
BOOL GetDefaultPrinterAndPaper(LPTSTR szPrinterName, int nPrintNameBufferLen, LPTSTR szPaperName)
{
	*szPaperName = 0;
	CPrintDialog pd(FALSE);
	if (pd.GetDefaults())
	{
		if (pd.m_pd.hDC) DeleteDC(pd.m_pd.hDC);
		if (pd.m_pd.hDevMode)
		{
			LPDEVMODE pdm = (LPDEVMODE)GlobalLock(pd.m_pd.hDevMode);
			*(szPaperName + 63) = 0;
			_tcsncpy(szPaperName, (LPCTSTR)pdm->dmFormName, 63); //打印纸张名称
			GlobalUnlock(pd.m_pd.hDevMode);
			GlobalFree(pd.m_pd.hDevMode);
		}
		if (pd.m_pd.hDevNames)
		{
			LPDEVNAMES pdn = (LPDEVNAMES)GlobalLock(pd.m_pd.hDevNames);
			nPrintNameBufferLen--;
			*(szPrinterName + nPrintNameBufferLen) = 0;
			_tcsncpy(szPrinterName, (LPTSTR)pdn + pdn->wDeviceOffset, nPrintNameBufferLen); //打印机名称
			GlobalUnlock(pd.m_pd.hDevNames);
			GlobalFree(pd.m_pd.hDevNames);
		}
	}
	return (*szPrinterName && *szPaperName);
}

void Cyoda_self_orderingView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	m_wndUITiles.SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	// TODO: Add your message handler code here
}

CString Cyoda_self_orderingView::GetProductName(std::vector<int> product_list)
{
	CString szPorduct;
	for (POSITION pos = m_lstProduct.GetHeadPosition();pos!=NULL;)
	{
		PRODUCTINFO* product = m_lstProduct.GetNext(pos);
		for (int i = 0; i < product_list.size(); i++)
		{
			if (product->nProductID == product_list[i])
			{
				szPorduct += product->szProductName + L",";
			}
		}
	}
	return szPorduct.TrimRight(',');
}
CString Cyoda_self_orderingView::GetCategroyName(std::vector<int> category_id)
{
	CString szCategory;
	for (POSITION pos = m_lstCategroy.GetHeadPosition(); pos != NULL;)
	{
		LPCATEGROYINFO categroy = m_lstCategroy.GetNext(pos);
		for (int i = 0; i < category_id.size(); i++)
		{
			if (categroy->nCategroyID == category_id[i])
			{
				szCategory += categroy->szCategroyName + L",";
			}
		}
	}
	return szCategory.TrimRight(',');
}