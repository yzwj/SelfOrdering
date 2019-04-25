// CashPop.cpp : implementation file
//

#include "stdafx.h"
#include "yoda_self_ordering.h"
#include "CashPop.h"


// CCashPop dialog

IMPLEMENT_SERIAL(CCashPop, CBCGPPopupDlg, 1)

CCashPop::CCashPop()
{

}

void CCashPop::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPopupDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TIP1, m_wndTip1);
	DDX_Control(pDX, IDC_STATIC_TIP2, m_wndTip2);
	DDX_Control(pDX, IDC_STATIC_TIP3, m_wndTip3);
	DDX_Control(pDX, IDC_BUTTON_HOME, m_wndHome);
}


BEGIN_MESSAGE_MAP(CCashPop, CBCGPPopupDlg)
	ON_BN_CLICKED(IDC_BUTTON_HOME, &CCashPop::OnBnClickedButtonHome)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CCashPop message handlers


BOOL CCashPop::OnInitDialog()
{
	CBCGPPopupDlg::OnInitDialog();
	// TODO:  Add extra initialization here
	SetMenu(NULL);
	LOGFONT lf;
	m_wndTip1.GetFont()->GetLogFont(&lf);
	lf.lfWeight = FW_BOLD;
	lf.lfHeight = lf.lfHeight*1.5;
	m_fontBold.CreateFontIndirect(&lf);
	m_wndTip1.SetFont(&m_fontBold);
	m_wndTip2.SetFont(&m_fontBold);
	m_wndTip3.SetFont(&m_fontBold);
	SetTimer(1000, 5000, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CCashPop::OnBnClickedButtonHome()
{
	// TODO: Add your control notification handler code here
	KillTimer(1000);
	SendMessage(WM_CLOSE);
	::PostMessage(AfxGetMainWnd()->m_hWnd, WM_HOME_VIEW, 0, 0);
}

void CCashPop::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	CBCGPPopupDlg::OnClose();
}


void CCashPop::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default	
	OnBnClickedButtonHome();
	CBCGPPopupDlg::OnTimer(nIDEvent);
}


void CCashPop::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CBCGPPopupDlg::OnPaint() for painting messages
	CRect rc;
	GetDlgItem(IDC_STATIC_LOGO)->GetWindowRect(&rc);
	ScreenToClient(&rc);
	Graphics graphics(dc); // Create a GDI+ graphics object
	Image *pimage; // Construct an image
	ImageFromIDResource(IDB_PAY_SUCCESS, _T("PNG"), pimage);
	graphics.DrawImage(pimage, rc.left, rc.top, rc.Width(), rc.Height());
}
