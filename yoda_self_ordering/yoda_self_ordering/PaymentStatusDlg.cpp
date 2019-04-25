// PaymentStatusDlg.cpp : implementation file
//

#include "stdafx.h"
#include "yoda_self_ordering.h"
#include "PaymentStatusDlg.h"


// CPaymentStatusDlg dialog

IMPLEMENT_DYNAMIC(CPaymentStatusDlg, CDialog)

CPaymentStatusDlg::CPaymentStatusDlg(CWnd* pParent /*=NULL*/)
	:CDialog(CPaymentStatusDlg::IDD, pParent)
{
	m_bPaymentSuccess = TRUE;
}

CPaymentStatusDlg::CPaymentStatusDlg(BOOL bStatus, CString szMsg, CWnd* pParent /*=NULL*/)
	: CDialog(CPaymentStatusDlg::IDD, pParent)
{
	m_bPaymentSuccess = bStatus;
	m_szMsg = szMsg;
}


CPaymentStatusDlg::~CPaymentStatusDlg()
{
}

void CPaymentStatusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_ICON, m_staticIcon);
	DDX_Control(pDX, IDC_BUTTON_TRY, m_BtnPay);
}


BEGIN_MESSAGE_MAP(CPaymentStatusDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_TRY, &CPaymentStatusDlg::OnBnClickedButtonTry)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CPaymentStatusDlg message handlers


BOOL CPaymentStatusDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_BtnPay.LoadFourBitmaps(IDB_PAY);
	CenterWindow();
	if (!m_bPaymentSuccess)
	{
		m_szMsg.Replace(L"  ", L"");
		GetDlgItem(IDC_STATIC_DESC)->SetWindowText(m_szMsg+L". PLEASE PAY AGAIN.");
		((CButton*)GetDlgItem(IDC_BUTTON_TRY))->ShowWindow(SW_SHOW);
	}
	else
	{
		SetTimer(1001, 5000, NULL);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CPaymentStatusDlg::OnBnClickedButtonTry()
{
	// TODO: Add your control notification handler code here
	
	if(!m_bPaymentSuccess)
		OnOK();
	else
	{
		KillTimer(1001);
		PostMessage(WM_CLOSE);
		::PostMessage(AfxGetMainWnd()->m_hWnd, WM_HOME_VIEW, 0, 0);
	}
}

int nCounts = 0;
void CPaymentStatusDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	OnBnClickedButtonTry();
	CDialog::OnTimer(nIDEvent);
}


void CPaymentStatusDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CDialog::OnPaint() for painting messages
	CRect rc;
	m_staticIcon.GetWindowRect(&rc);
	ScreenToClient(&rc);
	if (!m_bPaymentSuccess)
	{
		Graphics graphics(dc); // Create a GDI+ graphics object
		Image *pimage; // Construct an image
		ImageFromIDResource(IDB_PAY_FAILED, _T("PNG"), pimage);
		graphics.DrawImage(pimage, rc.left,rc.top,rc.Width(),rc.Height());
	}
	else
	{
		Graphics graphics(dc); // Create a GDI+ graphics object
		Image *pimage; // Construct an image
		ImageFromIDResource(IDB_PAY_SUCCESS, _T("PNG"), pimage);
		graphics.DrawImage(pimage, rc.left, rc.top, rc.Width(), rc.Height());
	}
}


HBRUSH CPaymentStatusDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	TCHAR czType[255];
	GetClassName(pWnd->m_hWnd, czType, 255);
	if (memcmp(czType, L"Static", 7) == 0)
	{
		if (m_bPaymentSuccess)
			pDC->SetTextColor(RGB(0, 255, 0));
		else
		{
			pDC->SetTextColor(RGB(255, 0, 0));
		}
	}
	// TODO:  Return a different brush if the default is not desired
	return hbr;
}


void CPaymentStatusDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnClose();
}
