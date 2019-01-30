#pragma once


// CCashPop dialog

class CCashPop : public CBCGPPopupDlg
{
	DECLARE_SERIAL(CCashPop)
public:
	CCashPop();   // standard constructor

// Dialog Data
	enum { IDD = IDD_CASH_POP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CStatic m_wndTip1;
	CStatic m_wndTip2;
	CStatic m_wndTip3;
	CButton m_wndHome;
	CFont	m_fontBold;
	afx_msg void OnBnClickedButtonHome();
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
