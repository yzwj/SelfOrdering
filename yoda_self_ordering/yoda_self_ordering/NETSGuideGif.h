#pragma once
// CNETSGuideGif dialog
class CNETSGuideGif : public CDialogEx
{
	DECLARE_DYNAMIC(CNETSGuideGif)

public:
	CNETSGuideGif(CWnd* pParent = NULL);   // standard constructor
	CNETSGuideGif(int nResID,CWnd* pParent = NULL);   // standard constructor
	virtual ~CNETSGuideGif();

// Dialog Data
	enum { IDD = IDD_DIALOG_GIF };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	
public:
	afx_msg void OnClose();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	int		m_nResID;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
