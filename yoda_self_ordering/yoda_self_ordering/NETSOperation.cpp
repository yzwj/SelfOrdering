// NETSOperation.cpp : implementation file
//

#include "stdafx.h"
#include "yoda_self_ordering.h"
#include "NETSOperation.h"

UINT WM_CLOSE_SON;
// CNETSOperation dialog

IMPLEMENT_DYNAMIC(CNETSOperation, CDialog)

CNETSOperation::CNETSOperation(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_NETS_OPER, pParent)
{

}

CNETSOperation::~CNETSOperation()
{
}

void CNETSOperation::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_LOG, m_staticLog);
	DDX_Control(pDX, IDC_PROGRESS, m_wndProgress);
	DDX_Control(pDX, IDC_STATIC_DESC, m_staticDesc);
}


BEGIN_MESSAGE_MAP(CNETSOperation, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_LOGON, &CNETSOperation::OnBnClickedButtonLogon)
	ON_BN_CLICKED(IDC_BUTTON_SETTLEMENT, &CNETSOperation::OnBnClickedButtonSettlement)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CLOSE, &CNETSOperation::OnBnClickedClose)
END_MESSAGE_MAP()


// CNETSOperation message handlers

DWORD WINAPI LogonThread(LPVOID lpParam)
{
	CNETSOperation* pThis = (CNETSOperation*)lpParam;
	int nPort = ::GetPrivateProfileInt(L"NETS", L"Port", 0, gIniFile);
	if (pThis->m_Port.InitPort(nPort))
	{
		CString ECN;
		CTime tm; tm = CTime::GetCurrentTime();
		ECN = tm.Format("S%Y%m%d002");
		USES_CONVERSION;
		int result = pThis->m_Port.LogonNETS(W2A(ECN));
		if (result == TRANSACTION_COMPLATED_STATUS)
		{
			::WritePrivateProfileString(L"NETS", L"Logon", L"1", gIniFile);
			::WritePrivateProfileString(L"NETS", L"Settlement", L"0", gIniFile);
			pThis->GetDlgItem(IDC_STATIC_DESC)->SetWindowText(L"Logon Success!");
		}
		else if(result == NO_RESPONSE_STATUS)
		{
			USES_CONVERSION;
			pThis->GetDlgItem(IDC_STATIC_DESC)->SetWindowText(L"Terminal no response.");
		}
		else
		{
			USES_CONVERSION;
			pThis->GetDlgItem(IDC_STATIC_DESC)->SetWindowText(A2W(pThis->m_Port.m_ResponseMsg));
		}
	}
	else
	{
		pThis->GetDlgItem(IDC_STATIC_DESC)->SetWindowText(L"NETS Termianl Comm failed");
	}
	pThis->m_wndProgress.PostMessage(PBM_SETMARQUEE, FALSE, 0);
	pThis->m_wndProgress.ShowWindow(SW_HIDE);
	pThis->m_Port.ClosePort();
	return 0;
}
DWORD WINAPI SettlementThread(LPVOID lpParam)
{
	CNETSOperation* pThis = (CNETSOperation*)lpParam;
	int nPort = ::GetPrivateProfileInt(L"NETS", L"Port", 0, gIniFile);
	if (pThis->m_Port.InitPort(nPort))
	{
		CString ECN;
		CTime tm; tm = CTime::GetCurrentTime();
		ECN = tm.Format("S%Y%m%d000");
		USES_CONVERSION;
		int result = pThis->m_Port.SettlementNETS(W2A(ECN));
		if (result == TRANSACTION_COMPLATED_STATUS)
		{
			::WritePrivateProfileString(L"NETS", L"Settlement", L"1", gIniFile);
			::WritePrivateProfileString(L"NETS", L"Logon", L"0", gIniFile);
			pThis->GetDlgItem(IDC_STATIC_DESC)->SetWindowText(L"Settlement Success!");
		}
		else
		{
			CString szMsg;
			szMsg = pThis->m_Port.m_ResponseMsg;
			pThis->GetDlgItem(IDC_STATIC_DESC)->SetWindowText(szMsg);
		}
	}
	else
	{
		pThis->GetDlgItem(IDC_STATIC_DESC)->SetWindowText(L"NETS Termianl Comm failed");
	}
	pThis->m_wndProgress.PostMessage(PBM_SETMARQUEE, FALSE, 0);
	pThis->m_wndProgress.ShowWindow(SW_HIDE);
	pThis->m_Port.ClosePort();
	return 0;
}
void CNETSOperation::OnBnClickedButtonLogon()
{
	// TODO: Add your control notification handler code here
	m_wndProgress.ShowWindow(SW_SHOWNORMAL);
	m_wndProgress.PostMessage(PBM_SETMARQUEE, 1, 0);
	GetDlgItem(IDC_STATIC_DESC)->SetWindowText(L"Processing");
	CreateThread(NULL, 4096, LogonThread, this,0, NULL);
}

void CNETSOperation::OnBnClickedButtonSettlement()
{
	// TODO: Add your control notification handler code here
	m_wndProgress.ShowWindow(SW_SHOWNORMAL);
	m_wndProgress.PostMessage(PBM_SETMARQUEE, 1, 0);
	GetDlgItem(IDC_STATIC_DESC)->SetWindowText(L"Processing");
	CreateThread(NULL, 0, SettlementThread, this, 0, NULL);
}
BOOL CNETSOperation::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  Add extra initialization here
	m_wndProgress.m_MarqueeStyle = CBCGPProgressCtrl::BCGP_MARQUEE_DOTS;
	m_wndProgress.ShowWindow(SW_HIDE);
	m_wndProgress.ModifyStyle(0, PBS_MARQUEE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CNETSOperation::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CDialog::OnPaint() for painting messages
	CClientDC *pDC = new CClientDC(GetDlgItem(IDC_STATIC_LOG));
	CRect rect;
	GetDlgItem(IDC_STATIC_LOG)->GetWindowRect(&rect);
	Graphics graphics(pDC->m_hDC); // Create a GDI+ graphics object
	Image *pimage; // Construct an image
	ImageFromIDResource(IDB_NETSLOG, _T("PNG"), pimage);
	graphics.DrawImage(pimage, 0, 0, rect.Width(), rect.Height());
	delete pDC;
}


HBRUSH CNETSOperation::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	TCHAR czType[255];
	GetClassName(pWnd->m_hWnd, czType, 255);
	if (memcmp(czType, L"Static",7) == 0)
	{
		if(m_Port.m_bRecvFlag == TRANSACTION_COMPLATED_STATUS )
			pDC->SetTextColor(RGB(0, 255, 0));
		else if (m_Port.m_bRecvFlag == TRANSACTION_ERROR_STATUS)
		{
			pDC->SetTextColor(RGB(255, 0, 0));
		}
	}
	// TODO:  Return a different brush if the default is not desired
	return hbr;
}


void CNETSOperation::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnClose();
}


void CNETSOperation::OnBnClickedClose()
{
	// TODO: Add your control notification handler code here
	::PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE_SON,NULL, NULL);
}
