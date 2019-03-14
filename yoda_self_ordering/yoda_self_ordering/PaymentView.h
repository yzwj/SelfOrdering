#if !defined(AFX_PAYMENT_VIEW_H__5AACF30F_9829_40F8_82A6_1B90291E10BA__INCLUDED_)
#define AFX_PAYMENT_VIEW_H__5AACF30F_9829_40F8_82A6_1B90291E10BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "CDXGraph.h"
#include "../Flash/DisplayObject.h"
#include "SerialPort.h"
class CPayContainerCtrl : public CBCGPVisualContainerCtrl
{
	DECLARE_SERIAL(CPayContainerCtrl)
public:
	CPayContainerCtrl();
	void CommandInit();
	void SetTextInfo(UINT nID, CString szText);
	void InitImage();
	void SetImage(UINT nID, UINT nImageID);
	void SetImage(UINT nID, CString szImagePath);
	void StartGif(CString szFileName);
	void CloseGif();
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
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	
	bool						m_bShow;
	CBCGPTextFormat							m_textFormat;
	CBCGPBrush								m_brText;
	CBCGPPoint								m_ptText;
	CBCGPSize								m_sizeText;
	CDXGraph *mFilterGraph;
	
public:
	double						m_dbTotal;
	IDisplayObject              *m_flash;
	int							m_nPayTryTimes;
	CSerialPort					m_NETS;
};
#endif // !defined(AFX_PAYMENT_VIEW_H__5AACF30F_9829_40F8_82A6_1B90291E10BA__INCLUDED_)
