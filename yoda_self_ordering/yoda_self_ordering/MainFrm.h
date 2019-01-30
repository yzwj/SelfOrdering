// MainFrm.h : interface of the CMainFrame class
//


#pragma once

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)
	// Attributes
public:
	int m_cx;
	int m_cy;
	// Operations
public:
	void FullScreen();

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	HINSTANCE m_hLangDLL;
protected:  // control bar embedded members
// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnCreateCategroy(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnEditOrder(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnOrderView(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnCategroyView(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnHomeView(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
