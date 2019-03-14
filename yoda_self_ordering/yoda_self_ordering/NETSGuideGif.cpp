// NETSGuideGif.cpp : implementation file
//

#include "stdafx.h"
#include "yoda_self_ordering.h"
#include "NETSGuideGif.h"


// CNETSGuideGif dialog

IMPLEMENT_DYNAMIC(CNETSGuideGif, CDialog)

CNETSGuideGif::CNETSGuideGif(CWnd* pParent /*=NULL*/)
	:CDialog(CNETSGuideGif::IDD, pParent)
{
	//mFilterGraph = NULL;
}

CNETSGuideGif::~CNETSGuideGif()
{
	//DestroyGraph();
}

void CNETSGuideGif::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_GIF, m_staticGif);
}


BEGIN_MESSAGE_MAP(CNETSGuideGif, CDialog)
	ON_WM_CLOSE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CNETSGuideGif message handlers


BOOL CNETSGuideGif::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  Add extra initialization here
	CenterWindow();
	HRESULT hr = CoInitialize(NULL);
	if (FAILED(hr)) {
		AfxMessageBox(_T("Error - Can't init COM."));
		return FALSE;
	}
	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **)&pGraph);
	if (FAILED(hr)) {
		AfxMessageBox(_T("Error - Can't create Filter Graph."));
		return FALSE;
	}
	//  Query Interface
	hr |= pGraph->QueryInterface(IID_IMediaControl, (void **)&pControl);
	hr |= pGraph->QueryInterface(IID_IMediaEvent, (void **)&pEvent);
	hr |= pGraph->QueryInterface(IID_IBasicVideo, (void **)&pVideo);
	hr |= pGraph->QueryInterface(IID_IBasicAudio, (void **)&pAudio);
	hr |= pGraph->QueryInterface(IID_IVideoWindow, (void **)&pWindow);
	hr |= pGraph->QueryInterface(IID_IMediaSeeking, (void **)&pSeeking);
	if (FAILED(hr)) {
		AfxMessageBox(_T("Error - Can't Query Interface."));
		return FALSE;
	}
	hr = pGraph->RenderFile(L"c:\\animation ICT250 GIF\\I'm With You.avi", NULL);
	if (FAILED(hr)) {
		AfxMessageBox(_T("Can't open input file!"));
		return 0;
	}

	//Set Window
	HWND screen_hwnd = NULL;
	RECT windowRect;
	screen_hwnd = this->GetDlgItem(IDC_STATIC_GIF)->GetSafeHwnd();
	::GetClientRect(screen_hwnd, &windowRect);

	pWindow->put_Visible(OAFALSE);
	pWindow->put_Owner((OAHWND)m_hWnd);
	pWindow->put_Left(0);
	pWindow->put_Top(0);
	pWindow->put_Width(windowRect.right - windowRect.left);
	pWindow->put_Height(windowRect.bottom - windowRect.top);
	pWindow->put_WindowStyle(WS_CHILD | WS_CLIPCHILDREN);
	pWindow->put_MessageDrain((OAHWND)m_hWnd);//Receive Message
	pWindow->put_Visible(OAFALSE);
	RECT grc;
	GetClientRect(&grc);
	pWindow->SetWindowPosition(0, 0, grc.right, grc.bottom);
	HWND dlg_hwnd = NULL;
	dlg_hwnd = this->GetSafeHwnd();
	pEvent->SetNotifyWindow((OAHWND)dlg_hwnd, NULL, 0);

	// Run
	hr = pControl->Run();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
void CNETSGuideGif::CreateGraph()
{
	
}
void CNETSGuideGif::DestroyGraph()
{
}

void CNETSGuideGif::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnClose();
}

BOOL CNETSGuideGif::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return CDialog::OnEraseBkgnd(pDC);
}


void CNETSGuideGif::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CDialog::OnPaint() for painting messages
}


void CNETSGuideGif::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
}
