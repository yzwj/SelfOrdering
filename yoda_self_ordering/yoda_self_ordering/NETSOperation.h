#pragma once


// CNETSOperation dialog
#include "PictureEx.h"
#include "SerialPort.h"
#include "afxwin.h"
class CNETSOperation : public CDialog
{
	DECLARE_DYNAMIC(CNETSOperation)

public:
	CNETSOperation(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNETSOperation();

// Dialog Data
	enum { IDD = IDD_NETS_OPER };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonLogon();
	afx_msg void OnBnClickedButtonSettlement();
	CPictureEx m_staticLog;
	CBCGPProgressCtrl	m_wndProgress;
	CSerialPort m_Port;
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg LRESULT OnShowProgress(WPARAM wp, LPARAM lp);
	CStatic m_staticDesc;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClose();
};
