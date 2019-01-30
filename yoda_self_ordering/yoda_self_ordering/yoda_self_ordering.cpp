// yoda_self_ordering.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "yoda_self_ordering.h"
#include "MainFrm.h"

#include "yoda_self_orderingDoc.h"
#include "yoda_self_orderingView.h"
#include "printerlibs.h"
#pragma comment(lib,"printerlibs.lib")
extern 	CList<LPORDERINFO, LPORDERINFO>	glstOrder;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
const int  iMaxUserToolbars		= 10;
const UINT uiFirstUserToolBarId	= AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId	= uiFirstUserToolBarId + iMaxUserToolbars - 1;
CAdoConnection	*gpDB;
CString gIniFile;
CString gExePath;
// Cyoda_self_orderingApp

BEGIN_MESSAGE_MAP(Cyoda_self_orderingApp, CBCGPWinApp)
	
END_MESSAGE_MAP()


// Cyoda_self_orderingApp construction

Cyoda_self_orderingApp::Cyoda_self_orderingApp()
{
#ifndef _BCGSUITE_INC_
	globalData.m_bUseVisualManagerInBuiltInDialogs = TRUE;
	SetVisualTheme(CBCGPWinApp::BCGP_VISUAL_THEME_OFFICE_2013_GRAY);
#else
	BCGPSetVisualManagerSuite(CBCGPVisualManager::BCGPVM_WINXP);
#endif
}


// The one and only Cyoda_self_orderingApp object

Cyoda_self_orderingApp theApp;


// Cyoda_self_orderingApp initialization

BOOL Cyoda_self_orderingApp::InitInstance()
{

	CBCGPWinApp::InitInstance();
	if (!IdleUIInit()) {
		AfxMessageBox(L"fail to init keymouse hook");

	}
	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();

	if (!ConnectDB())
	{
		AfxMessageBox(L"Connection server failed");
		return false;
	}

	
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

	
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	SetRegistryBase(_T("Settings200"));

	// Initialize all Managers for usage. They are automatically constructed
	// if not yet present
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(Cyoda_self_orderingDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(Cyoda_self_orderingView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	
	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	return TRUE;
}

// Cyoda_self_orderingApp message handlers

int Cyoda_self_orderingApp::ExitInstance() 
{
	GdiplusShutdown(m_gdiplusToken);
	return CBCGPWinApp::ExitInstance();
}
CBCGPColor Cyoda_self_orderingApp::GetBrushColor(const CBCGPBrush& br)
{
	if (br.HasTextureImage() || br.GetGradientType() == CBCGPBrush::BCGP_NO_GRADIENT)
	{
		return br.GetColor();
	}

	return br.GetColor() < br.GetGradientColor() ? br.GetColor() : br.GetGradientColor();
}
BOOL Cyoda_self_orderingApp::IsConnection()
{
	CString szSQL;
	szSQL.Format(L"select * from y_category");
	CAdoRecordSet recordset;
	recordset.SetAdoConnection(gpDB);
	recordset.SetCursorLocation();
	try
	{
		if (!recordset.Open(szSQL))
		{
			return FALSE;
		}
	}
	catch (_com_error e)
	{
		return FALSE;
	}
	if (recordset.GetRecordCount()>0)
	{
		recordset.Close();
		return TRUE;
	}
	else return FALSE;
}
BOOL Cyoda_self_orderingApp::ConnectDB()
{
	//寻找数据源，连接数据库
	TCHAR sBuf[MAX_PATH];
	int dwCount = GetModuleFileName(m_hInstance, sBuf, MAX_PATH);
	CString szPath;
	szPath = ".\\";
	if (dwCount > 0) {
		int iSepPos = -1;
		for (int i = 0; i < dwCount; i++)
			if (sBuf[i] == '\\')
				iSepPos = i;
		if (iSepPos >= 0) {
			sBuf[iSepPos] = '\0';
			szPath = sBuf;
			szPath += '\\';
		}
	}
	CString sDriver;
	CString sServerName;
	CString sDataBase;
	CString sLogId;
	CString sLogPass;
	CString sDataBaseType;
	gExePath = szPath;
	CString sFullPath = szPath + "YodaOrdering.ini";
	//全局的ini文件路径变量，
	gIniFile = sFullPath;

	// 判断这个配置文件是否存在,不存在整个程序结束
	WIN32_FIND_DATA pdata;
	HANDLE hFind = FindFirstFile(sFullPath, &pdata);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		FindClose(hFind);
		return false;
	}
	FindClose(hFind);

	if (gpDB)
	{
		if (gpDB->IsOpen())
			gpDB->Close();
		delete gpDB;
		gpDB = NULL;
	}
	gpDB = new CAdoConnection;

	if (gpDB == NULL)
	{
		return false;
	}
	::GetPrivateProfileString(L"ServerDB", L"Driver", L"", sBuf, 255, sFullPath);
	sDriver = sBuf;
	::GetPrivateProfileString(L"ServerDB", L"ServerName", L"", sBuf, 255, sFullPath);
	sServerName = sBuf;
	::GetPrivateProfileString(L"ServerDB", L"DataBase", L"", sBuf, 255, sFullPath);
	sDataBase = sBuf;
	::GetPrivateProfileString(L"ServerDB", L"LogId", L"", sBuf, 255, sFullPath);
	sLogId = sBuf;
	::GetPrivateProfileString(L"ServerDB", L"LogPass", L"", sBuf, 255, sFullPath);
	sLogPass = sBuf;
	//sLogPass = StringUnCode(sLogPass);
	if (!gpDB->ConnectMySQL(sDriver, sServerName, sDataBase, sLogId, sLogPass))
	{
		Sleep(3000);
		if (!gpDB->ConnectMySQL(sDriver, sServerName, sDataBase, sLogId, sLogPass))
		{
			delete gpDB;
			gpDB = NULL;
			return false;
		}
	}
	return true;
}
char device_id[] = "vid_0fe6&pid_811e";
char gDeviceID[255];
void EnumUsbToComboBox()
{
	memset(gDeviceID, 0, 255);
	int cbNeeded = 0;
	int cnReturned = 0;
	char sBuf[255] = { 0 };
	Port_EnumUSB(NULL, 0, &cbNeeded, &cnReturned);
	if (cbNeeded)
	{
		CHAR * pBuf = (CHAR *)malloc(cbNeeded);
		if (pBuf)
		{
			Port_EnumUSB(pBuf, cbNeeded, &cbNeeded, &cnReturned);
			CHAR * pDevice = pBuf;
			for (int i = 0; i < cnReturned; ++i)
			{
				if (strstr(pDevice, device_id))
				{
					strcpy(gDeviceID, pDevice);
					break;
				}
				pDevice += strlen(pDevice) + 1;
			}
			free(pBuf);
		}
	}
}
BOOL PrintReceipt(bool bCounter)
{
	EnumUsbToComboBox();
	int h = Port_OpenUsb(gDeviceID);
	if (h) 
	{
		//printer header
		TCHAR sBuf[255];
		TCHAR Buffer[255];
		POS_TextOut(h, L"\n", -2, 0, 0, 0, 0, ENCODING_UTF8);
		::GetPrivateProfileString(L"Receipt", L"Company", L"iTea", sBuf, 255, gIniFile);
		wcscat_s(sBuf, L"\n");
		POS_TextOut(h, sBuf, -2, 1, 0, 0, 0x08, ENCODING_UTF8);
		::GetPrivateProfileString(L"Receipt", L"Outlet", L"", sBuf, 255, gIniFile);
		wcscat_s(sBuf, L"\n");
		POS_TextOut(h, sBuf, -2, 0, 0, 0, 0, ENCODING_UTF8);
		::GetPrivateProfileString(L"Receipt", L"Doorplate", L"", sBuf, 255, gIniFile);
		wcscat_s(sBuf, L"\n");
		POS_TextOut(h, sBuf, -2, 0, 0, 0, 0, ENCODING_UTF8);
		::GetPrivateProfileString(L"Receipt", L"PostalCode", L"", sBuf, 255, gIniFile);
		wcscat_s(sBuf, L"\n");
		POS_TextOut(h, sBuf, -2, 0, 0, 0, 0, ENCODING_UTF8);
		POS_TextOut(h, L"\n", -2, 0, 0, 0, 0, ENCODING_UTF8);
		COleDateTime datetime;
		datetime = COleDateTime::GetCurrentTime();
		CString str2 = datetime.Format(L"%Y-%m-%d %H:%M:%S");
		wsprintfW(sBuf, L"Printed : %s\r\n", str2.GetBuffer(0));
		POS_TextOut(h, sBuf,-1, 0, 0, 0, 0, ENCODING_UTF8);
		::GetPrivateProfileString(L"Receipt", L"Termial", L"", sBuf, 255, gIniFile);
		wsprintfW(Buffer, L"Termial : %s\r\n", sBuf);
		POS_TextOut(h, Buffer, -1, 0, 0, 0, 0, ENCODING_UTF8);
	
		//printer Counter
		if (bCounter)
		{
			POS_TextOut(h, L"-------------------------------\n", 0, 0, 0, 0, 0, ENCODING_UTF8);
			double dbTotal = 0.;
			USES_CONVERSION;
			for (POSITION pos = glstOrder.GetHeadPosition(); pos != NULL;)
			{
				LPORDERINFO info = glstOrder.GetNext(pos);
				CString szInfo;
				szInfo.Format(L"%dX %s-%s %0.2lf  %0.2lf\n", info->nProductCounts, info->productInfo.szSize, gbChinese? info->productInfo.szProductNameCN:info->productInfo.szProductName, info->productInfo.dbMoney, info->productInfo.dbMoney*info->nProductCounts);
				POS_TextOut(h, szInfo.GetBuffer(0), -1, 0, 0, 0, 0, ENCODING_GBK);
				dbTotal += info->productInfo.dbMoney * info->nProductCounts;
			}
			POS_TextOut(h, L"-------------------------------\n", 0, 0, 0, 0, 0, ENCODING_UTF8);
			CString szInfo;
			szInfo.Format(L"Total :    %0.2lf\n", dbTotal);
			POS_TextOut(h, szInfo.GetBuffer(0), 0, 1, 1, 0, 0x08, ENCODING_UTF8);
			POS_TextOut(h, L"", 150, 1, 1, 0, 0x08, ENCODING_UTF8);
			//wsprintfW(sBuf, L"%.2f\n", 2.8);
			POS_TextOut(h, L"\n", -2, 0, 0, 0, 0, ENCODING_UTF8);
			POS_SetBarcode(h, "No.123456", -2, 2, 50, 0, 2, POS_BARCODE_TYPE_CODE128);
			POS_TextOut(h, L"\n", -2, 0, 0, 0, 0, ENCODING_UTF8);
			POS_TextOut(h, L"Your Order has been Sent to the Counter.\n", -2, 0, 0, 0, 0, ENCODING_UTF8);
			POS_TextOut(h, L"Thank You.", -2, 0, 0, 0, 0, ENCODING_UTF8);
		}
		else
		{

		}
		POS_FeedAndCutPaper(h);
		POS_KickOutDrawer(h, 0, 100, 100);
		POS_Beep(h, 1, 5);
		bool bSuccess = POS_TicketSucceed(h, 0, 1000);
		Port_Close(h);
	}
	return true;
}