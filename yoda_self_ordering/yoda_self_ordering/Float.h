#pragma once


// CFloat dialog

class CFloat : public CDialog
{
	DECLARE_DYNAMIC(CFloat)

public:
	CFloat(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFloat();
	static CFloat* c_pFloat;
	CStringArray m_patharray;
// Dialog Data
	enum { IDD = IDD_FLOAT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CRect   rect; //启动画面大小
	DECLARE_MESSAGE_MAP()
public:
	static void ShowFloatWindow(CWnd* pParentWnd);
	void HideFloatWindow();
	//afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
	//	static virtual BOOL PreTranslateMessage(MSG* pMsg);
	static BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnNcHitTest(CPoint pt);
	afx_msg void OnNcRButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
public:
	void ShowImage(CString szImageDir);
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
};
