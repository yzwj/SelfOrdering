// yoda_self_orderingView.h : interface of the Cyoda_self_orderingView class
//


#pragma once

#include "yoda_self_orderingDoc.h"

class CShowHomeContainerCtrl : public CBCGPVisualContainerCtrl
{
};
class Cyoda_self_orderingView : public CView
{
	friend class CMainFrame;
protected: // create from serialization only
	Cyoda_self_orderingView();
	DECLARE_DYNCREATE(Cyoda_self_orderingView)

// Attributes
public:
	Cyoda_self_orderingDoc* GetDocument() const;
	void AddTile(CBCGPWinUITile* pTile, UINT nImageResID, UINT nFullColorImageResID, int nGroup, UINT nCmdID = 0,
		CBCGPWinUITile::BCGP_WINUI_BADGE_GLYPH badgeGlyph = CBCGPWinUITile::BCGP_NONE,
		BOOL bIsFullSizeImage = FALSE);
	void SetThemeColors();
	void EnableBackButton();
	void EnableHomeButton();
	void EnableNextButton();
	void EnableCartButton();
	void EnablePaymentButton();
	bool printTicket();
// Operations
public:
	void InitUITiles();
	BOOL InitCategroy();
	BOOL InitProduct();
	void CreateProduct(int nCategroy);
	void CreateCategory();
	void SetCurProduct(int nCategroy,int nProductID);
	void ShowView(int nViewID);
	void ShowHomePage();
// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~Cyoda_self_orderingView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CBCGPWinUITilesCtrl	m_wndUITiles;
	CBCGPBrush					m_brCurrView;
	CBCGPBrush					m_brCurrViewFrame;
	
	CList<LPCATEGROYINFO,LPCATEGROYINFO>		m_lstCategroy;
	CList<PRODUCTINFO*, PRODUCTINFO*>		m_lstProduct;
	BOOL								m_bEditOrder;
	CBCGPVisualContainerCtrl              m_wndContainerCtrl;
public:
	
// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg LRESULT OnPrintClient(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnChangeVisualManager(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnClickCaptionButton(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnCreateView(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnCloseView(WPARAM, LPARAM);
	afx_msg LRESULT OnClickTile(WPARAM, LPARAM);
};

#ifndef _DEBUG  // debug version in yoda_self_orderingView.cpp
inline Cyoda_self_orderingDoc* Cyoda_self_orderingView::GetDocument() const
   { return reinterpret_cast<Cyoda_self_orderingDoc*>(m_pDocument); }
#endif

