#pragma once
#include "afxwin.h"
#include "CDXGraph.h"

// CSelfGIFDlg dialog

class CSelfGIFDlg : public CDialog
{
	DECLARE_DYNAMIC(CSelfGIFDlg)

public:
	CSelfGIFDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSelfGIFDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_GIF };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	void CreateGraph(void);
	void DestroyGraph(void);
public:
	int m_nCX;
	int m_nCY;
	CRect m_rcWnd;
	CStatic m_staticGif;
	CDXGraph * mFilterGraph;
	CString m_szFileName;
};
