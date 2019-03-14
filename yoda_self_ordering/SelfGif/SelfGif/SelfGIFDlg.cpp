// SelfGIFDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SelfGif.h"
#include "SelfGIFDlg.h"


// CSelfGIFDlg dialog

IMPLEMENT_DYNAMIC(CSelfGIFDlg, CDialog)

CSelfGIFDlg::CSelfGIFDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_GIF, pParent)
{
	m_nCX = GetSystemMetrics(SM_CXSCREEN);
	m_nCY = GetSystemMetrics(SM_CYSCREEN);
	mFilterGraph = NULL;
	m_szFileName = L"";
}

CSelfGIFDlg::~CSelfGIFDlg()
{
}

void CSelfGIFDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_GIF, m_staticGif);
}


BEGIN_MESSAGE_MAP(CSelfGIFDlg, CDialog)
	ON_WM_CREATE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CSelfGIFDlg message handlers


BOOL CSelfGIFDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	//m_staticGif.MoveWindow(0, 0, m_nCX, m_nCY, 1);
	CreateGraph();
	if (mFilterGraph)
	{
		mFilterGraph->Run();
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
int CSelfGIFDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	CRect   rcTemp;
	rcTemp = CRect(0, 0, m_nCX, m_nCY);
	m_rcWnd = rcTemp;
	MoveWindow(&m_rcWnd);
	SetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE,
		GetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE) | 0x80000);
	
	SetWindowPos(&wndTopMost, 0, 0, m_nCX, m_nCY, SWP_NOSIZE);
	return 0;
}
void CSelfGIFDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	DestroyGraph();
	CDialog::OnClose();
}

void CSelfGIFDlg::CreateGraph(void)
{
	DestroyGraph();
	mFilterGraph = new CDXGraph();
	if (mFilterGraph->Create())
	{
		m_szFileName = "c:\\animation ICT250 GIF\\ICT250 NETS.gif";
		USES_CONVERSION;
		char *temp = T2A(m_szFileName);

		// Render the source clip
		mFilterGraph->RenderFile(temp);
		// Set video window and notification window
		mFilterGraph->SetDisplayWindow(m_staticGif.GetSafeHwnd());
		mFilterGraph->SetNotifyWindow(this->GetSafeHwnd());
		// Show the first frame
		mFilterGraph->Pause();
	}
}
void CSelfGIFDlg::DestroyGraph(void)
{
	if (mFilterGraph)
	{
		//Stop the filter graph first
		mFilterGraph->Stop();
		mFilterGraph->SetNotifyWindow(NULL);//²¥·Å´°¿ÚÖÃ¿Õ
		delete mFilterGraph;
		mFilterGraph = NULL;
	}
}