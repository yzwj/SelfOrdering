// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "yoda_self_ordering.h"
#include "yoda_self_orderingView.h"
#include "MainFrm.h"
#include "TipInfo.h"

CTipInfo* CTipInfo::c_pTip;
UINT	gCurViewID;
extern PRODUCTINFO gCurProduct;
extern 	CList<LPORDERINFO, LPORDERINFO>	glstOrder;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_MESSAGE(WM_CLICK_ORDER, OnCreateCategroy)
	ON_MESSAGE(WM_EDIT_ORDER, OnEditOrder)
	ON_MESSAGE(WM_CLICK_CART,OnOrderView)
	ON_MESSAGE(WM_CLICK_HOME, OnCategroyView)
	ON_MESSAGE(WM_HOME_VIEW, OnHomeView)
	ON_MESSAGE(ID_SHOWTOOLTIP, &CMainFrame::ShowBalloonTip)
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	m_cx = GetSystemMetrics(SM_CXSCREEN);
	m_cy = GetSystemMetrics(SM_CYSCREEN);
	m_hLangDLL = NULL;
}

CMainFrame::~CMainFrame()
{
#ifndef _DEBUG
	HWND hWnd = ::FindWindow(_T("Shell_TrayWnd"), NULL);
	HWND   hStar = ::FindWindow(_T("Button"), NULL);
	::ShowWindow(hWnd, SW_SHOW);
	::ShowWindow(hStar, SW_SHOW);
#endif
}
int ShouDown(DWORD verInfo)
{
	try
	{
		//如果获取到的系统dwMajorVersion值大于等于,表示为NT以上操作系统,需提升进程权限
		if (verInfo >= 5)
		{
			HANDLE ToHandle;
			TOKEN_PRIVILEGES tkp;
			//打开本进程访问信令
			if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &ToHandle))
			{
				//修改本进程权限
				LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
				tkp.PrivilegeCount = 1;
				tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
				//通知系统已修改
				AdjustTokenPrivileges(ToHandle, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
				//获得权限后关闭计算机
				ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE, 0);
			}
		}
		//否则为以下,直接调用ExitWindowsEx函数即可
		else
		{
			ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE, 0);
		}
		return 0;
	}
	catch (...)
	{
		//出现任何错误则返回
		//printf("关机失败,请确认程序是否被拦截或具有管理权限!");
		return -1;
	}
}
DWORD WINAPI GetVerInfo()
{
	OSVERSIONINFO osver = { sizeof(OSVERSIONINFO) };
	GetVersionEx(&osver);
	return osver.dwMajorVersion;
}
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	SetMenu(NULL);
#ifndef _DEBUG
	FullScreen();
#endif
	m_Tip.Create(this);
	SetTimer(1001,1000,NULL);
	//SetTimer(1002, 60*1000, NULL);
	UpdateData(false);
	// Menu will not take the focus on activation:
	return 0;
}

void CMainFrame::FullScreen()
{
	ModifyStyle(WS_CAPTION | WS_THICKFRAME, 0, 0);
	ModifyStyleEx(WS_EX_CLIENTEDGE, 0, 0);
	LONG style = ::GetWindowLong(m_hWnd, GWL_STYLE);
	style &= ~WS_CAPTION;
	::SetWindowLong(m_hWnd, GWL_STYLE, style);
	CRect rt;
	//SystemParametersInfo(SPI_GETWORKAREA,0,&rt,0);
	SetFocus();
#ifndef _DEBUG
	HWND hWnd = ::FindWindow(_T("Shell_TrayWnd"), NULL);
	HWND   hStar = ::FindWindow(_T("Button"), NULL);
	::ShowWindow(hWnd, SW_HIDE);
	::ShowWindow(hStar, SW_HIDE);
#endif
	SetWindowPos(&CWnd::wndTopMost, 0, 0, m_cx, m_cy, SWP_SHOWWINDOW);//窗口置顶
}
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CFrameWnd::PreCreateWindow(cs))
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	WNDCLASS   wndclass;
	::GetClassInfo(AfxGetInstanceHandle(), cs.lpszClass, &wndclass);

	VERIFY(AfxRegisterClass(&wndclass));
	//cs.hMenu=NULL; 
	cs.lpszClass = wndclass.lpszClassName;
#ifndef _DEBUG
	cs.style &= ~WS_SYSMENU;//去掉标题栏系统菜单
#endif
	return TRUE;
}
LRESULT CMainFrame::ShowBalloonTip(WPARAM wParam, LPARAM lParam)
{
	CString strCfg;
	Cyoda_self_orderingView* pView = (Cyoda_self_orderingView*)GetActiveView();
	CRect rectClient;
	pView->GetWindowRect(&rectClient);
	m_Tip.SetClientRect(rectClient);
	strCfg.LoadStringW((UINT)wParam);
	m_Tip.Show(strCfg, NULL);
	return 0;
}
LRESULT CMainFrame::OnCreateCategroy(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	Cyoda_self_orderingView * pView = (Cyoda_self_orderingView*)GetActiveView();
	pView->CreateCategory();
	
	return 0;
}
LRESULT CMainFrame::OnEditOrder(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	Cyoda_self_orderingView * pView = (Cyoda_self_orderingView*)GetActiveView();
	if (gCurProduct.bShowSugarLevel)
	{
		pView->ShowView(ID_VIEW_ICE_SUGAR_TOPPING);
	}
	else if (gCurProduct.bShowHoneyLevel)
	{
		pView->ShowView(ID_VIEW_ICE_HONEY_TOPPING);

	}
	else if (!gCurProduct.bShowIce && !gCurProduct.bShowSize)
	{
		pView->ShowView(ID_VIEW_TOPPING);
	}
	else if (!gCurProduct.bShowIce && gCurProduct.bShowSize)
	{
		pView->ShowView(ID_VIEW_SIZE_TOPPING);
	}
	else if (gCurProduct.bShowIce && gCurProduct.bShowSize)
	{
		pView->ShowView(ID_VIEW_ICE_TOPPING);
	}
	return 0;
}
LRESULT CMainFrame::OnOrderView(WPARAM wp, LPARAM lp)
{
	Cyoda_self_orderingView * pView = (Cyoda_self_orderingView*)GetActiveView();
	pView->ShowView(ID_VIEW_CHECK_ORDERING);
	return LRESULT();
}
LRESULT CMainFrame::OnCategroyView(WPARAM wp, LPARAM lp)
{
	if (gbChinese)
	{
		AfxEnableControlContainer();
		CString szLanguageDll;
		szLanguageDll = gExePath + L"yoda_self_ordering_cn.dll";
		m_hLangDLL = ::LoadLibrary(szLanguageDll);
		if (m_hLangDLL == NULL)
		{
			m_hLangDLL = LoadLibraryEx(szLanguageDll, NULL, LOAD_LIBRARY_AS_DATAFILE);
			if (m_hLangDLL == NULL)
			{
				return FALSE;
			}
		}
		AfxSetResourceHandle(m_hLangDLL);
	}
	else
	{
		AfxSetResourceHandle(AfxGetInstanceHandle());
	}
	Cyoda_self_orderingView * pView = (Cyoda_self_orderingView*)GetActiveView();
	pView->CreateCategory();
	return LRESULT();
}
LRESULT CMainFrame::OnHomeView(WPARAM wp, LPARAM lp)
{
	Cyoda_self_orderingView * pView = (Cyoda_self_orderingView*)GetActiveView();
	if (glstOrder.GetSize() > 0)
		glstOrder.RemoveAll();
	pView->ShowView(ID_VIEW_HOME);
	return LRESULT();
}
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers



void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (IdleUIGetLastInputTime()>1)
	{
		int nIdleTime = (GetTickCount() - IdleUIGetLastInputTime()) / 1000;
		if (nIdleTime>300)
		{
			if (gCurViewID != ID_VIEW_HOME)
			{
				Cyoda_self_orderingView * pView = (Cyoda_self_orderingView*)GetActiveView();
				pView->ShowView(ID_VIEW_HOME);
				glstOrder.RemoveAll();
			}
			
		}
	}
	/*if (nIDEvent == 1002)
	{
		COleDateTime;
		COleDateTime datetime;
		datetime = COleDateTime::GetCurrentTime();
		CString str2 = datetime.Format(_T("%H:%M"));
		TCHAR sBuf[255];
		::GetPrivateProfileString(L"AUTOSHUTDOWN", L"TIME", L"21:30", sBuf, 255, gIniFile);
		COleDateTime t;
		t.ParseDateTime(sBuf);
		COleDateTime p;
		p.ParseDateTime(str2);
		if (p >=t )
		{
			KillTimer(1002);
			ShouDown(GetVerInfo());
		}
	}*/
	CFrameWnd::OnTimer(nIDEvent);
}
