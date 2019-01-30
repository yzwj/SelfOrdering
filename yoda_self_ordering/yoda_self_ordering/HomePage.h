#if !defined(AFX_HOME_PAGE_H__5AACF30F_9829_40F8_82A6_1B90291E10BA__INCLUDED_)
#define AFX_HOME_PAGE_H__5AACF30F_9829_40F8_82A6_1B90291E10BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class CHomeContainerCtrl : public CBCGPVisualContainerCtrl
{
	DECLARE_SERIAL(CHomeContainerCtrl)
public:
	CHomeContainerCtrl();
	void CommandInit();
	void InitImage();
	void SetImage(UINT nID, UINT nImageID);
	void SetImage(UINT nID, CString szImagePath);
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
};
#endif // !defined(AFX_HOME_PAGE_H__5AACF30F_9829_40F8_82A6_1B90291E10BA__INCLUDED_)
