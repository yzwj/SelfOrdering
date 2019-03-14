#pragma once
#include "BitmapBtn.h"
#include "PngButton.h"
#include "afxwin.h"
// CPaymentStatusDlg dialog
class CPaymentStatusDlg : public CDialog
{
	DECLARE_DYNAMIC(CPaymentStatusDlg)

public:
	CPaymentStatusDlg(CWnd* pParent = NULL);   // standard constructor
	CPaymentStatusDlg(BOOL bStatus,CString szMsg=_T(""),CWnd* pParent = NULL);   // standard constructor
	virtual ~CPaymentStatusDlg();

// Dialog Data
	enum { IDD = IDD_PAYMENT_STATUS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bPaymentSuccess;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonTry();
	CStatic m_staticIcon;
	CString m_szMsg;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CBitmapBtn m_BtnPay;
	afx_msg void OnClose();
};
