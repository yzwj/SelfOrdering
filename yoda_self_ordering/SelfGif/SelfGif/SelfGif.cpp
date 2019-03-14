// SelfGif.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "SelfGif.h"
#include "SelfGIFDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// CSelfGifApp

BEGIN_MESSAGE_MAP(CSelfGifApp, CWinApp)
END_MESSAGE_MAP()


// CSelfGifApp construction

CSelfGifApp::CSelfGifApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CSelfGifApp object

CSelfGifApp theApp;

CWnd* g_pWnd = NULL;

// CSelfGifApp initialization

BOOL CSelfGifApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
CSelfGIFDlg g_GifDlg;
extern "C"
__declspec(dllexport) void ShowGif(CWnd* pWnd,CString szFileName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	g_pWnd = pWnd;
	if (!::IsWindow(g_GifDlg.GetSafeHwnd()))
	{
		g_GifDlg.Create(IDD_DIALOG_GIF, g_pWnd);
	}
	g_GifDlg.ShowWindow(SW_SHOW);
}

extern "C"
__declspec(dllexport) void CloseGif()
{
	g_GifDlg.OnClose();
}
