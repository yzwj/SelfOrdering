#if !defined(AFX_HONEY_VIEW_H__5AACF30F_9829_40F8_82A6_1B90291E10BA__INCLUDED_)
#define AFX_HONEY_VIEW_H__5AACF30F_9829_40F8_82A6_1B90291E10BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class Cyoda_self_orderingView;
class CHoneyContainerCtrl : public CBCGPVisualContainerCtrl
{
	DECLARE_SERIAL(CHoneyContainerCtrl)
public:
	CHoneyContainerCtrl();
	void CommandInit(const CString & szProduct);
	void SetSwitch(UINT nID, BOOL bMedium, UINT nLabelID, LPCTSTR lpszLabelOn, LPCTSTR lpszLabelOff);
	void SetTextInfo(UINT nID, CString szText);
	void InitImage();
	void SetImage(UINT nID, UINT nImageID);
	void SetImage(UINT nID, CString szImagePath);
	void CleanAllTopping();
	void SelectedTopping();
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNormalContainerCtrl)
	//}}AFX_VIRTUAL
	virtual BOOL OnMouseDown(int nButton, const CBCGPPoint& pt);
	virtual void OnDraw(CBCGPGraphicsManager* pGM, const CBCGPRect& rectClip);
	// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CNormalContainerCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:

protected:
	CBCGPColorIndicatorImpl*	m_pLastColorIndicator;
	CBCGPImageGaugeImpl*        m_pLastImageGauge;
	CBCGPImageGaugeImpl*        m_pLastImageSize;
	CBCGPBrush					m_brFill;
	CString						m_szProduct;
	CString						m_szSize;
	double						m_dbUnitMenoy;
	CString						m_szIce;
	CString						m_szHoney;
	std::vector<CString>		m_szTopping;
	CString						m_szToppingMoney;
	double						m_dbSizeMoney;
	double						m_dbToppingMoney;
	double						m_dbUnit;
};
#endif // !defined(AFX_TEAVIEW_H__5AACF30F_9829_40F8_82A6_1B90291E10BA__INCLUDED_)
