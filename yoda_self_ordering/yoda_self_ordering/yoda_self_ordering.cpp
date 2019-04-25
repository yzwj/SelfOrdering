// yoda_self_ordering.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "yoda_self_ordering.h"
#include "MainFrm.h"
#include "NetsOperation.h"
#include "yoda_self_orderingDoc.h"
#include "yoda_self_orderingView.h"
#include "qrencode.h"
#include "printerlibs.h"
#include "SerialPort.h"
#include <iostream>  
#include <vector>
#include "HttpClient.h"
#include "json/json.h"
using namespace std;
#pragma comment(lib,"printerlibs.lib")
#define  TOKEN _T("fa3c5db850ada70422a02e4638984dc08822f010")
#define TOKEN2 _T("1336161b67b86cf40faa3a86f8cb6b9e32dca5e3")
extern CList<LPORDERINFO, LPORDERINFO>	glstOrder;
CList<LPTOPPINGINFO, LPTOPPINGINFO>		glstTopping;
CList<LPSIZEINFO, LPSIZEINFO>		glstSize;
extern paymentModes payModes;
extern  int gCurSerialNO;
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
	HRESULT hr = CoInitialize(NULL);
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

	WM_SCAN_MESSAGE = ::RegisterWindowMessage(_T("WM_SCAN_MESSAGE"));
	WM_CLOSE_SON = ::RegisterWindowMessage(_T("WM_CLOSE_SON"));

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

	CNETSOperation op;
	op.DoModal();
	
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
	bool bSuccess = true;
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
		if (!bCounter)
		{
			CString szNo;
			szNo.Format(L"Queue NO:%04d\r\n", gCurSerialNO);
			POS_TextOut(h, szNo.GetBuffer(0), 0, 1, 0, 0, 0x08, ENCODING_UTF8);
		}
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
			szInfo.Format(L"%d", gCurSerialNO);
			//POS_SetBarcode(h, szInfo, -2, 2, 50, 0, 2, POS_BARCODE_TYPE_CODE128);
			POS_SetQRCode(h, W2A(szInfo), -2, 10, 0, 2);
			POS_TextOut(h, L"\n", -2, 0, 0, 0, 0, ENCODING_UTF8);
			POS_TextOut(h, L"Your Order has been Sent to the Counter.\n", -2, 0, 0, 0, 0, ENCODING_UTF8);
			POS_TextOut(h, L"Thank You.", -2, 0, 0, 0, 0, ENCODING_UTF8);
		}
		else
		{
			POS_TextOut(h, L"-------------------------------\n", 0, 0, 0, 0, 0, ENCODING_UTF8);
			double dbTotal = 0.;
			USES_CONVERSION;
			for (POSITION pos = glstOrder.GetHeadPosition(); pos != NULL;)
			{
				LPORDERINFO info = glstOrder.GetNext(pos);
				CString szInfo;
				szInfo.Format(L"%dX %s-%s %0.2lf  %0.2lf\n", info->nProductCounts, info->productInfo.szSize, gbChinese ? info->productInfo.szProductNameCN : info->productInfo.szProductName, info->productInfo.dbMoney, info->productInfo.dbMoney*info->nProductCounts);
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
			szInfo.Format(L"%d", gCurSerialNO);
			//POS_SetBarcode(h, szInfo, -2, 2, 50, 0, 2, POS_BARCODE_TYPE_CODE128);
			POS_SetQRCode(h, W2A(szInfo), 0, 10, 0, 2);
			POS_TextOut(h, L"\n", -2, 0, 0, 0, 0, ENCODING_UTF8);
			POS_TextOut(h, L"-------------------------------\n", 0, 0, 0, 0, 0, ENCODING_UTF8);
			POS_TextOut(h, L"Thank you! Welcome to come again.\n", -2, 0, 0, 0, 0, ENCODING_UTF8);
			POS_TextOut(h, L"-------------------------------\n", 0, 0, 0, 0, 0, ENCODING_UTF8);
		} 
		POS_FeedAndCutPaper(h);
		POS_KickOutDrawer(h, 0, 100, 100);
		POS_Beep(h, 1, 5);
		//bSuccess = POS_TicketSucceed(h, 0, 1000);
		Port_Close(h);
	}
	return bSuccess;
}
#define OUT_FILE_PIXEL_PRESCALER	8											// Prescaler (number of pixels in bmp file for each QRCode pixel, on each dimension)
#define PIXEL_COLOR_R				0											// Color of bmp pixels
#define PIXEL_COLOR_G				0
#define PIXEL_COLOR_B				0
BOOL GenerateQRCode(CString szText)
{
	USES_CONVERSION;
	char*			szSourceSring = W2A(szText);
	unsigned int	unWidth, x, y, l, n, unWidthAdjusted, unDataBytes;
	unsigned char*	pRGBData, *pSourceData, *pDestData;
	QRcode*			pQRC;
	FILE*			f;
	/*
	* Create a symbol from the string. The library automatically parses the input
	* string and encodes in a QR Code symbol.
	* @warning This function is THREAD UNSAFE when pthread is disabled.
	* @param string input string. It must be NUL terminated.
	* @param version version of the symbol. If 0, the library chooses the minimum
	*                version for the given input data.
	* @param level error correction level.
	* @param hint tell the library how non-alphanumerical characters should be
	*             encoded. If QR_MODE_KANJI is given, kanji characters will be
	*             encoded as Shif-JIS characters. If QR_MODE_8 is given, all of
	*             non-alphanumerical characters will be encoded as is. If you want
	*             to embed UTF-8 string, choose this.
	* @param casesensitive case-sensitive(1) or not(0).
	* @return an instance of QRcode class. The version of the result QRcode may
	*         be larger than the designated version. On error, NULL is returned,
	*         and errno is set to indicate the error. See Exceptions for the
	*         details.
	* @throw EINVAL invalid input object.
	* @throw ENOMEM unable to allocate memory for input objects.
	* @throw ERANGE input data is too large.
	*/

	// Compute QRCode

	if (pQRC = QRcode_encodeString(szSourceSring, 0, QR_ECLEVEL_H, QR_MODE_8, 1))
	{
		unWidth = pQRC->width;
		unWidthAdjusted = unWidth * OUT_FILE_PIXEL_PRESCALER * 3;
		if (unWidthAdjusted % 4)
			unWidthAdjusted = (unWidthAdjusted / 4 + 1) * 4;
		unDataBytes = unWidthAdjusted * unWidth * OUT_FILE_PIXEL_PRESCALER;

		// Allocate pixels buffer

		if (!(pRGBData = (unsigned char*)malloc(unDataBytes)))
		{
			return false;
		}
		// Preset to white
		memset(pRGBData, 0xff, unDataBytes);
		// Prepare bmp headers
		BITMAPFILEHEADER kFileHeader;
		kFileHeader.bfType = 0x4d42;  // "BM"
		kFileHeader.bfSize = sizeof(BITMAPFILEHEADER) +
			sizeof(BITMAPINFOHEADER) +
			unDataBytes;
		kFileHeader.bfReserved1 = 0;
		kFileHeader.bfReserved2 = 0;
		kFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) +
			sizeof(BITMAPINFOHEADER);

		BITMAPINFOHEADER kInfoHeader;
		kInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
		kInfoHeader.biWidth = unWidth * OUT_FILE_PIXEL_PRESCALER;
		kInfoHeader.biHeight = -((int)unWidth * OUT_FILE_PIXEL_PRESCALER);
		kInfoHeader.biPlanes = 1;
		kInfoHeader.biBitCount = 24;
		kInfoHeader.biCompression = BI_RGB;
		kInfoHeader.biSizeImage = 0;
		kInfoHeader.biXPelsPerMeter = 0;
		kInfoHeader.biYPelsPerMeter = 0;
		kInfoHeader.biClrUsed = 0;
		kInfoHeader.biClrImportant = 0;
		//
		// Convert QrCode bits to bmp pixels

		pSourceData = pQRC->data;
		for (y = 0; y < unWidth; y++)
		{
			pDestData = pRGBData + unWidthAdjusted * y * OUT_FILE_PIXEL_PRESCALER;
			for (x = 0; x < unWidth; x++)
			{
				if (*pSourceData & 1)
				{
					for (l = 0; l < OUT_FILE_PIXEL_PRESCALER; l++)
					{
						for (n = 0; n < OUT_FILE_PIXEL_PRESCALER; n++)
						{
							*(pDestData + n * 3 + unWidthAdjusted * l) = PIXEL_COLOR_B;
							*(pDestData + 1 + n * 3 + unWidthAdjusted * l) = PIXEL_COLOR_G;
							*(pDestData + 2 + n * 3 + unWidthAdjusted * l) = PIXEL_COLOR_R;
						}
					}
				}
				pDestData += 3 * OUT_FILE_PIXEL_PRESCALER;
				pSourceData++;
			}
		}
		// Output the bmp file
		if (!(fopen_s(&f, "d:\\QRCode.bmp", "wb")))
		{
			fwrite(&kFileHeader, sizeof(BITMAPFILEHEADER), 1, f);
			fwrite(&kInfoHeader, sizeof(BITMAPINFOHEADER), 1, f);
			fwrite(pRGBData, sizeof(unsigned char), unDataBytes, f);
			fclose(f);
		}
		else
		{
			return false;
		}
		// Free data
		free(pRGBData);
		QRcode_free(pQRC);
	}
	else
	{
		return false;
	}
	return true;
}

long getnewFreeID(CAdoConnection *pDB, const CString szTable, CString sColName)
{
	int nID = getFreeID(pDB, szTable);
	if (nID<1)
		return -1;
	CString szID;
	szID.Format(L"%d", nID);
	BOOL bHaveSame = IsHaveSameID(pDB, sColName, szTable, szID);
	while (bHaveSame)
	{
		nID = getFreeID(pDB, szTable);
		if (nID<1)
			return -1;
		szID.Format(L"%d", nID);
		bHaveSame = IsHaveSameID(pDB, sColName, szTable, szID);
	}
	return nID;
}
int	IsHaveSameID(CAdoConnection *pDB, CString sColName, CString sTableName, CString szValue)
{
	CAdoRecordSet recordset;
	recordset.SetAdoConnection(pDB);
	recordset.SetCursorLocation();
	CString sSQL, sTemp;
	//sSQL = _T("select " + sColName + " from " + sTableName + " where " + sColName + " = " + szValue);//
	sSQL.Format(_T("select '%s' from '%s' where '%s' = '%s' and Date = CURDATE()"), sColName, sTableName, sColName, szValue);
	int nID = 0;
	try {
		if (!recordset.Open(sSQL))
			return 0;
		short nCol = 0;
	}

	catch (...) {
		if (recordset.IsOpen())
			recordset.Close();
		return 0;
	}

	if (recordset.GetRecordCount() == 0) {
		recordset.Close();
		return 0;
	}

	recordset.Close();
	return 1;
}

BOOL AddLog(CAdoConnection *pDB, CString szAction, CString szStr, const CString&szUserTitle)
{
	CString szUser, szHostName, szA, szDes;
	//szLogID.Format("%d",nID);
	CTime time = CTime::GetCurrentTime();
	CString szDate = time.Format("%Y-%m-%d");
	CString szTime = time.Format("%H:%M:%S");
	char sBuf[255];
	gethostname(sBuf, 255);
	CString szSQL;
	szSQL.Format(_T("Insert into G_Log(log_Date,log_Time,User_Title,log_Action,Log_Des) values('%s','%s','%s','%s','%s')"),szDate,szTime,szUserTitle,szAction,szStr);
	return execSQL(pDB, szSQL);
}

long getFreeID(CAdoConnection *pDB, const CString szTable)
{
	CString szSQL;
	long var;
	long lID;

	if (szTable.IsEmpty())
		return -1;

	CAdoRecordSet RegSet;
	RegSet.SetAdoConnection(pDB);
	RegSet.SetCursorLocation();

	pDB->BeginTrans();

	szSQL.Format(_T("INSERT INTO y_quenextid (tableName, nextID,Date) VALUES('%s',0,CURDATE())"),szTable);
	pDB->Execute(szSQL);

	szSQL.Format(_T("UPDATE y_quenextid SET nextID = nextID+1 WHERE tableName = '%s' and Date = CURDATE()"), szTable);
	if (pDB->Execute(szSQL) == NULL)
	{
		pDB->RollbackTrans();
		AddLog(pDB, L"ERROR", szSQL, L"");
		return -1;
	}
	szSQL.Format(_T("SELECT nextID FROM y_quenextid WHERE tableName ='%s' and Date = CURDATE()"), szTable);
	try {
		if (!RegSet.Open(szSQL))
		{
			pDB->RollbackTrans();
			AddLog(pDB, L"ERROR", szSQL, L"");
			return -1;
		}
	}
	catch (_com_error e)
	{
		pDB->RollbackTrans();
		AddLog(pDB, L"ERROR", szSQL, L"");
		return -1;
	}
	if (RegSet.IsEOF())
	{
		pDB->RollbackTrans();
		AddLog(pDB, L"ERROR", szSQL, L"");
		return -1;
	}
	RegSet.GetFieldValue((long)0, var);
	lID = var;
	RegSet.Close();
	pDB->CommitTrans();
	return lID;
}
bool execSQL(CAdoConnection *pDB, CString &szSQL)
{
	bool b = true;
	int nCount = 0;
	int nTimes = 5;
	try
	{
		while (nCount < nTimes)
		{
			nCount++;
			if (pDB->Execute(szSQL) == NULL)
			{
				Sleep(100);
				if (nCount == nTimes)
				{
					return false;
				}
			}
			else
			{
				return b;
			}
		}
	}
	catch (_com_error e)
	{
		AfxMessageBox(L"sql Execute falied:" + szSQL);
		b = false;
	}

	return b;
}

BOOL ImageFromIDResource(UINT nID, LPCTSTR sTR, Image *&pImg)
{
	HINSTANCE hInst = AfxGetResourceHandle(); 
	HRSRC hRsrc = ::FindResource(hInst, MAKEINTRESOURCE(nID), sTR); // type
	if (!hRsrc)
		return FALSE;
	// load resource into memory
	DWORD len = SizeofResource(hInst, hRsrc);
	BYTE* lpRsrc = (BYTE*)LoadResource(hInst, hRsrc);
	if (!lpRsrc)
		return FALSE;
	// Allocate global memory on which to create stream
	HGLOBAL m_hMem = GlobalAlloc(GMEM_FIXED, len);
	BYTE* pmem = (BYTE*)GlobalLock(m_hMem);
	memcpy(pmem, lpRsrc, len);
	GlobalUnlock(m_hMem);
	IStream* pstm;
	CreateStreamOnHGlobal(m_hMem, FALSE, &pstm);
	// load from stream
	pImg = Gdiplus::Image::FromStream(pstm);
	// free/release stuff
	pstm->Release();
	FreeResource(lpRsrc);
	GlobalFree(m_hMem);
	return TRUE;
}
BOOL ExistOrder()
{
	CAdoRecordSet RegSet;
	RegSet.SetAdoConnection(gpDB);
	RegSet.SetCursorLocation();
	CString szSQL;
	szSQL.Format(L"select * from y_order_list where RIGHT(OrderID,4) = '%04d' and OrderDate = CURDATE()", gCurSerialNO);
	try {
		if (!RegSet.Open(szSQL))
			return FALSE;
	}

	catch (...) {
		if (RegSet.IsOpen())
			RegSet.Close();
		return FALSE;
	}
	if (RegSet.GetRecordCount() == 0) {
		RegSet.Close();
		return FALSE;
	}
	else return TRUE;
}
BOOL SaveOrder(ORDERSTATUS orderStatus)
{
	//CS-01TP1810040014
	CString szOrderID,szID,szOutlet,szDate;
	TCHAR sBuf[255];
	::GetPrivateProfileString(L"Kiosk", L"ID", L"", sBuf, 255, gIniFile);
	szID = sBuf;
	::GetPrivateProfileString(L"Kiosk", L"Outlet", L"", sBuf, 255, gIniFile);
	szOutlet = sBuf;
	CString szOutletID;
	::GetPrivateProfileString(L"Outlet", L"ID", L"", sBuf, 255, gIniFile);
	szOutletID = sBuf;
	CTime tm;
	CTime time = CTime::GetCurrentTime();
	szDate.Format(L"%04d%02d%02d", time.GetYear(), time.GetMonth(), time.GetDay());
	szOrderID.Format(L"SO|CS-%s%s%s%04d", szID,szOutlet, szDate.Right(6),gCurSerialNO);
	CString szSQL;
	int nIndex = 1;
	int nTotal = 0;
	for (POSITION pos = glstOrder.GetHeadPosition();pos!=NULL;)
	{
		LPORDERINFO info = glstOrder.GetNext(pos);
		nTotal += info->nProductCounts;
	}
	for (POSITION pos = glstOrder.GetHeadPosition(); pos != NULL;)
	{
		LPORDERINFO info = glstOrder.GetNext(pos);
		if (info != NULL)
		{
			CString szTopping = L"";
			if (info->productInfo.ToppingArr.size() > 0)
			{
				for (int i=0;i<info->productInfo.ToppingArr.size();i++)
				{
					szTopping += info->productInfo.ToppingArr.at(i);
					szTopping += "|";
				}
			//	szTopping = szTopping.Left(szTopping.GetLength() - 1);
			}
			szSQL.Format(_T("insert into y_order_list(OrderID,s_no,ItemIndex,ProductName,item_code,Size,Ice,SugarLevel,HoneyLevel,Topping,unit_price,\
							disc_amt,payable,OrderDate,OrderTime,OrderStatus,PayTimes,KioskID,isDelivery,printer,payModes,total_no,outlet_id) \
							VALUES('%s',%d,%d,'%s %s','%s','%s','%s','%s','%s','%s',%0.2lf,\
							%0.2lf,%0.2lf,CURDATE(),CURTIME(),%d,0,'%s','%s','%s',%d,%d,%s)"),
				szOrderID, info->nProductCounts,nIndex, info->productInfo.szProductName,info->productInfo.szProductNameCN,
				info->productInfo.szItemCode,info->productInfo.szSizeCode, info->productInfo.szICECode, info->productInfo.bShowSugarLevel ? info->productInfo.szSugarCode : L"", 
				info->productInfo.bShowHoneyLevel ? info->productInfo.szHoneyCode : L"", szTopping,
				info->productInfo.dbMoney*info->nProductCounts, info->productInfo.dbDiscount, info->productInfo.dbMoney*info->nProductCounts - info->productInfo.dbDiscount,
				orderStatus,szOutlet+szID,"N", info->productInfo.szPrinter, payModes,nTotal,szOutletID);
			if (gpDB->Execute(szSQL) == NULL)
			{
				AfxMessageBox(szSQL);
				return FALSE;
			}
			nIndex++;
		}
	}
	return TRUE;
}
int GetOrderCounts()
{
	int nCounts = 0;
	for (POSITION pos = glstOrder.GetHeadPosition(); pos != NULL;)
	{
		LPORDERINFO info = glstOrder.GetNext(pos);
		nCounts += info->nProductCounts;
	}
	return nCounts;
}
int GetCurOrderPayTimes()
{
	CAdoRecordSet RegSet;
	RegSet.SetAdoConnection(gpDB);
	RegSet.SetCursorLocation();
	CString szSQL;
	gpDB->BeginTrans();
	szSQL.Format(_T("UPDATE y_order_list SET PayTimes = PayTimes + 1 WHERE OrderID = %d and OrderDate = CURDATE()"), gCurSerialNO);
	if (gpDB->Execute(szSQL) == NULL)
	{
		gpDB->RollbackTrans();
		AddLog(gpDB, L"ERROR", szSQL, L"GetCurOrderPayTimes");
		return -1;
	}
	szSQL.Format(_T("select PayTimes from y_order_list where OrderID = %d and OrderDate = CURDATE()"), gCurSerialNO);
	try {
		if (!RegSet.Open(szSQL))
		{
			gpDB->RollbackTrans();
			AddLog(gpDB, L"ERROR", szSQL, L"GetCurOrderPayTimes");
			return -1;
		}
	}
	catch (_com_error e)
	{
		gpDB->RollbackTrans();
		AddLog(gpDB, L"ERROR", szSQL, L"GetCurOrderPayTimes");
		return -1;
	}
	if (RegSet.IsEOF())
	{
		gpDB->RollbackTrans();
		AddLog(gpDB, L"ERROR", szSQL, L"GetCurOrderPayTimes");
		return -1;
	}
	int nTimes = 0;
	RegSet.GetFieldValue((long)0, nTimes);
	RegSet.Close();
	gpDB->CommitTrans();
	return nTimes;
}
BOOL UpdateOrderStatus(ORDERSTATUS orderStatus)
{
	CAdoRecordSet RegSet;
	RegSet.SetAdoConnection(gpDB);
	RegSet.SetCursorLocation();
	CString szSQL;
	gpDB->BeginTrans();
	szSQL.Format(_T("UPDATE y_order_list SET OrderStatus = %d WHERE OrderID = %d and OrderDate = CURDATE()"), orderStatus,gCurSerialNO);
	if (gpDB->Execute(szSQL) == NULL)
	{
		AddLog(gpDB, L"ERROR", szSQL, L"UpdateOrderStatus");
		return FALSE;
	}
	return TRUE;
}
double GetToppingPrice(CString szItemCode)
{
	for (POSITION pos = glstTopping.GetHeadPosition(); pos != NULL;)
	{
		LPTOPPINGINFO top = glstTopping.GetNext(pos);
		if (top != NULL && !top->szItemCode.CompareNoCase(szItemCode))
		{
			return top->ItemPrice;
		}
	}
	return 0.0;
}
int GetToppingID(CString szItemCode)
{
	for (POSITION pos = glstTopping.GetHeadPosition(); pos != NULL;)
	{
		LPTOPPINGINFO top = glstTopping.GetNext(pos);
		if (top != NULL && !top->szItemCode.CompareNoCase(szItemCode))
		{
			return top->nItemId;
		}
	}
	return -1;
}
CString GetToppingName(CString szItemCode)
{
	for (POSITION pos = glstTopping.GetHeadPosition(); pos != NULL;)
	{
		LPTOPPINGINFO top = glstTopping.GetNext(pos);
		if (top != NULL && !top->szItemCode.CompareNoCase(szItemCode))
		{
			return top->szItemName;
		}
	}
	return _T("");
}
double GetSizePrice(CString szItemCode)
{
	for (POSITION pos = glstSize.GetHeadPosition(); pos != NULL;)
	{
		LPSIZEINFO top = glstSize.GetNext(pos);
		if (top != NULL && !top->szItemCode.CompareNoCase(szItemCode))
		{
			return top->ItemPrice;
		}
	}
	return 0.0;
}
int GetSizeID(CString szItemCode)
{
	for (POSITION pos = glstSize.GetHeadPosition(); pos != NULL;)
	{
		LPSIZEINFO top = glstSize.GetNext(pos);
		if (top != NULL && !top->szItemCode.CompareNoCase(szItemCode))
		{
			return top->nItemId;
		}
	}
	return -1;
}

bool NeedPrint(CString szPrinter)
{
	for (POSITION pos = glstOrder.GetHeadPosition(); pos != NULL;)
	{
		LPORDERINFO info = glstOrder.GetNext(pos);
		if (!szPrinter.CompareNoCase(info->productInfo.szPrinter))
			return true;
	}
	return false;
}
bool printTicket(CString szPrinter)
{
	if (!NeedPrint(szPrinter))
		return true;
	DOCINFO     di;
	CDC             dc;
	HDC             hDC;
	ZeroMemory(&di, sizeof(DOCINFO));
	di.cbSize = sizeof(DOCINFO);
	di.lpszDocName = _T("order");
	di.fwType = DI_APPBANDING;
	hDC = CreateDC(NULL, szPrinter, NULL, NULL);
	dc.Attach(hDC);
	int nPageX, nPageY;
	nPageX = dc.GetDeviceCaps(HORZRES);
	nPageY = dc.GetDeviceCaps(VERTRES);
	nPageY -= 20;
	CRect rc;
	dc.GetBoundsRect(rc, 0);
	CFont* pFont = dc.GetCurrentFont();
	LOGFONT logFont;
	pFont->GetLogFont(&logFont);
	if (dc.StartDoc(&di) > 0)
	{
		int nOrderCounts = GetOrderCounts();
		int nCurOrder = 0;
		for (POSITION pos = glstOrder.GetHeadPosition(); pos != NULL;)
		{
			LPORDERINFO info = glstOrder.GetNext(pos);
			for (int i = 0; i<info->nProductCounts; i++)
			{
				nCurOrder++;
				if (!szPrinter.CompareNoCase(info->productInfo.szPrinter) && dc.StartPage() > 0)
				{
					int nHeight = 0, nSecondHeight;
					CString szOrderID, szTitle;
					memcpy(logFont.lfFaceName, L"Arial Black", 24);
					logFont.lfWeight = FW_HEAVY;
					logFont.lfHeight = 25;
					logFont.lfWidth = 11;
					CFont newFont, Font2,FontProduct,FontDesc;
					CFont *oldFont;
					newFont.CreateFontIndirect(&logFont);
					oldFont = dc.SelectObject(&newFont);
					CRect rc(5, nHeight, 170, 30);
					szOrderID.Format(_T("RCT# %04d"), gCurSerialNO);
					dc.TextOutW(5, nHeight, szOrderID);
					//dc.DrawText(szOrderID, &rc, DT_NOCLIP | DT_NOPREFIX | DT_EXPANDTABS | DT_LEFT);
					dc.SelectObject(oldFont);
					DeleteObject(oldFont);
					memcpy(logFont.lfFaceName, L"Arial", 12);
					logFont.lfWeight = FW_NORMAL;
					logFont.lfHeight = 25;
					logFont.lfWidth = 10;
					Font2.CreateFontIndirect(&logFont);
					oldFont = dc.SelectObject(&Font2);
					CTime tm = CTime::GetCurrentTime();
					szTitle.Format(_T("%d / %d   %02d/%02d/%04d %02d:%02d"), nCurOrder, nOrderCounts, tm.GetMonth(), tm.GetDay(), tm.GetYear(), tm.GetHour(), tm.GetMinute());
					dc.TextOut(170, nHeight, szTitle);
					CPen pen(PS_SOLID, 3, RGB(0, 0, 0));
					CPen* pOldPen = dc.SelectObject(&pen);
					nHeight += 28;
					dc.MoveTo(0, nHeight);
					dc.LineTo(nPageX, nHeight);
					dc.SelectObject(oldFont);
					DeleteObject(oldFont);
					//
					logFont.lfWeight = FW_BOLD;
					logFont.lfHeight = 25;
					logFont.lfWidth = 10;
					FontProduct.CreateFontIndirect(&logFont);
					oldFont = dc.SelectObject(&FontProduct);
					nHeight += 5;
					dc.TextOut(5, nHeight, info->productInfo.szProductName + info->productInfo.szProductNameCN);
					dc.SelectObject(oldFont);
					DeleteObject(oldFont);
					//
					logFont.lfWeight = FW_NORMAL;
					logFont.lfHeight = 25;
					logFont.lfWidth = 10;
					FontDesc.CreateFontIndirect(&logFont);
					oldFont = dc.SelectObject(&FontDesc);
					nSecondHeight = nHeight + 30;
					if (!info->productInfo.szSize.IsEmpty())
					{
						CString szSize;
						if (!info->productInfo.szSize.CompareNoCase(L"M"))
							szSize = "MEDIUM";
						else if (!info->productInfo.szSize.CompareNoCase(L"L"))
							szSize = "LARGE";
						nHeight += 30;
						dc.TextOut(20, nHeight, L"* " + szSize);
					}
					if (info->productInfo.bShowHoneyLevel)
					{
						nHeight += 30;
						dc.TextOut(20, nHeight, L"* HONEY " + info->productInfo.szHoney);
					}
					else if (info->productInfo.bShowSugarLevel)
					{
						nHeight += 30;
						dc.TextOut(20, nHeight, L"* SUGAR " + info->productInfo.szSugar);
					}
					if (!info->productInfo.szICE.IsEmpty())
					{
						nHeight += 30;
						dc.TextOut(20, nHeight, L"* " + info->productInfo.szICE);
					}
					if (!info->productInfo.szTopping.IsEmpty())
					{

						for (int i = 0; i<info->productInfo.ToppingArr.size(); i++)
						{
							CString szCode = info->productInfo.ToppingArr[i];
							CString szTopping = GetToppingName(szCode);
							nHeight += 30;
							if (nPageY >= nHeight)
								dc.TextOut(20, nHeight, L"* " + szTopping);
							else
							{
								dc.TextOut(nPageX / 2, nSecondHeight, L"* " + szTopping);
								nSecondHeight += 30;
							}
						}
					}
					dc.SelectObject(pOldPen);
					dc.SelectObject(oldFont);
					DeleteObject(oldFont);
				}
				dc.EndPage();
			}
		}
	}
	dc.EndDoc();
	dc.Detach();
	dc.DeleteDC();
	return TRUE;
}

CList<LPVOUCHERINFO> voucherlist;

int GetVoucherID(CString szCode)
{
	std::string strResponse;
	CString szMsg;
	CHttpClient http;
	int bResult = http.HttpPost(_T("https://crm.itea.sg/api/v1/invicode/check/?code=") + szCode, _T(""), strResponse, TOKEN);
	Json::Reader reader;
	Json::Value root;
	if (!bResult)
	{
		//strResponse = "{\"voucher_id\":57,\"outlet_limit\":[\"Clementi Central\",\"Elias Mall\"],\"outlet\":[[31,57,10],[32,57,15]],\"user\":\"root\",\"voucher\":\"Free Medium Size Milk Tea Drink\",\"status\":\"valid\"}"
		if (reader.parse(strResponse, root))
		{
			CString szStatus;
			int szID;
			szStatus = root["status"].asCString();
			if (!szStatus.CompareNoCase(_T("valid")))
			{
				szID = root["voucher_id"].asInt();
				return szID;
			}
		}
	}
	return -1;
}
int GetVoucherInfo(CString szCode)
{
	int nID =  GetVoucherID(szCode);
	if (nID == -1)
		return -1;
	CString szID;
	szID.Format(L"%d", nID);
	/*
	 "outlet_limit": [],
	"status": "valid",
	"user": "root",
	"outlet": "",
	"voucher": "1 Free M Size Drink(TCC Student Welfare Pack Giveaway II)",
	"voucher_id": 56
	*/
	if (voucherlist.GetCount() > 0)
	{
		for (POSITION pos = voucherlist.GetHeadPosition(); pos != NULL;)
		{
			LPVOUCHERINFO voucher = voucherlist.GetNext(pos);
			if (!voucher->szInvicode.CompareNoCase(szCode))
				return -1;
		}
	}
	std::string strResponse;
	CString szMsg;
	CHttpClient http;
	int bResult = http.HttpGet(_T("https://crm.itea.sg/api/v1/voucher/")+szID+_T("/"), _T(""), strResponse, TOKEN);
	Json::Reader reader;
	Json::Value root;
	if (!bResult)
	{
		if (reader.parse(strResponse, root)&& strResponse.find("detail",6)>0)
		{
			LPVOUCHERINFO voucher = new VOUCHERINFO();
			voucher->VoucherID = root["id"].asInt();
			voucher->szName = root["name"].asCString();
			voucher->type = (voucherType)root["type"].asInt();
			voucher->szDescription = root["description"].asCString();
			voucher->szInvicode = szCode;
			voucher->szEffectiveDate = root["effective_date"].asCString();
			voucher->ExpiringDate = root["expiring_date"].asCString();
			voucher->nToppings = root["toppings_number"].asInt();
			if(root["product_size"].size()>0)
				voucher->nProductSize = root["product_size"][0].asInt();
			else voucher->nProductSize = -1;
			voucher->limitProductNumber = root["product_number"].asInt();
			voucher->size_upgrade = root["size_upgrade"].asBool();
			voucher->discount_percent = root["discount_percent"].asDouble();
			voucher->discount_price = root["discount_price"].asDouble();
			voucher->number_purchase = root["number_purchase"].asInt();
			voucher->number_complimentary_drinks = root["number_complimentary_drinks"].asInt();
			voucher->status = (voucherStatus)root["state"].asInt();

			if (!root["unparticipated_outlets"].isNull() && root["unparticipated_outlets"].size() > 0)
			{
				int nCount = root["unparticipated_outlets"].size();
				for (int i = 0; i < nCount; i++)
				{
					voucher->limitRedeemOutlet.push_back(root["unparticipated_outlets"][i].asInt());
				}
			}
			if (!root["redemption_toppings"].isNull() && root["redemption_toppings"].size() > 0)
			{
				int nCount = root["redemption_toppings"].size();
				for (int i = 0; i < nCount; i++)
				{
					voucher->limitRedeemTopping.push_back(root["redemption_toppings"][i].asInt());
				}
			}
			if (!root["redemption_products"].isNull() && root["redemption_products"].size() > 0)
			{
				int nCount = root["redemption_products"].size();
				for (int i = 0; i < nCount; i++)
				{
					voucher->limitRedeemProduct.push_back(root["redemption_products"][i].asInt());
				}
			}
			if (!root["redemption_products_category"].isNull() && root["redemption_products_category"].size() > 0)
			{
				int nCount = root["redemption_products_category"].size();
				for (int i = 0; i < nCount; i++)
				{
					voucher->limitRedeemCategroy.push_back(root["redemption_products_category"][i].asInt());
				}
			}
			voucherlist.AddHead(voucher);
		}
		else return 0;
	}
	return 1;
}

BOOL VoucherEnable(LPVOUCHERINFO voucher)
{
	//status
	if (voucher->status == archived
		|| voucher->status == completed
		|| voucher->status == waiting)
		return false;
	//outlet
	if (voucher->limitRedeemOutlet.size() > 0)
	{
		bool bbSpecifiedOutlet = false;
		int OutletID = -1;
		OutletID = ::GetPrivateProfileInt(L"Outlet", L"ID", OutletID, gIniFile);
		for (int i = 0; i < voucher->limitRedeemOutlet.size(); i++)
		{
			if (voucher->limitRedeemOutlet[i] == OutletID)
			{
				bbSpecifiedOutlet = true;
				break;
			}
		}
		if (!bbSpecifiedOutlet)
			return false;
	}
	//type
	switch (voucher->type)
	{
	case Upgrade:
	{
		for (POSITION pos = glstOrder.GetHeadPosition(); pos != NULL;)
		{
			LPORDERINFO order = glstOrder.GetNext(pos);
			if (voucher->nProductSize >= GetSizeID(order->productInfo.szSizeCode))
			{
				return true;
			}
		}
	}
	break;
	case Toppings:
		//topping 仅限一种 TODO:可以添加多种
		for (POSITION pos = glstOrder.GetHeadPosition(); pos != NULL;)
		{
			LPORDERINFO order = glstOrder.GetNext(pos);
			for (int i = 0; i < voucher->limitRedeemCategroy.size(); i++)
			{
				for (int j = 0; j < order->productInfo.ToppingArr.size(); j++)
				{
					if (GetToppingID(order->productInfo.ToppingArr[j]) == voucher->limitRedeemCategroy[i])
						return true;
				}
			}
		}
		break;
	case Cash:
	case Discount:
		return true;
		break;
	case Product:
	{
		//size 0 normal/1 large/2 tall
		//1.case 类别，产品限制
		if (!voucher->limitRedeemCategroy.empty())
		{
			for (POSITION pos = glstOrder.GetHeadPosition(); pos != NULL;)
			{
				LPORDERINFO order = glstOrder.GetNext(pos);
				for (int i = 0; i < voucher->limitRedeemCategroy.size(); i++)
				{
					if (order->productInfo.nCategroyID == voucher->limitRedeemCategroy[i] && voucher->nProductSize >= GetSizeID(order->productInfo.szSizeCode))
					{
						return true;
					}
				}
			}
			return false;
		}
		/*
		if (!voucher->limitRedeemCategroy.empty() && !voucher->limitRedeemProduct.empty())
		{
			bool bPassCate = false;
			bool bPassProduct = false;
			//类别限制
			for (POSITION pos = glstOrder.GetHeadPosition(); pos != NULL;)
			{
				LPORDERINFO order = glstOrder.GetNext(pos);
				for (int i = 0; i < voucher->limitRedeemCategroy.size(); i++)
				{
					if (order->productInfo.nCategroyID == voucher->limitRedeemCategroy[i] && voucher->nProductSize >= GetSizeID(order->productInfo.szSizeCode))
					{
						bPassCate = true;
						break;
					}
				}
				if (bPassCate)
					break;
			}
			if (!bPassCate)
				return false;
			//产品限制
			for (POSITION pos = glstOrder.GetHeadPosition(); pos != NULL;)
			{
				LPORDERINFO order = glstOrder.GetNext(pos);
				for (int i = 0; i < voucher->limitRedeemCategroy.size(); i++)
				{
					if (order->productInfo.nCategroyID == voucher->limitRedeemCategroy[i] && voucher->nProductSize >= GetSizeID(order->productInfo.szSizeCode))
					{
						bPassProduct = true;
						break;
					}
				}
				if (bPassProduct)
					break;
			}
			if (!bPassProduct)
				return false;
			return true;
		}*/
		//2.case 产品限制 
		if (!voucher->limitRedeemProduct.empty())
		{
			for (POSITION pos = glstOrder.GetHeadPosition(); pos != NULL;)
			{
				LPORDERINFO order = glstOrder.GetNext(pos);
				for (int i = 0; i < voucher->limitRedeemProduct.size(); i++)
				{
					if (order->productInfo.nProductID == voucher->limitRedeemProduct[i] && voucher->nProductSize >= GetSizeID(order->productInfo.szSizeCode))
					{
						return true;
					}
				}
			}
			return false;
		}
	}
	break;
	case BuyOneGetOne:
		break;
	default:
		break;
	}
	return false;
}

double RedeemVoucherIncomePrice(int nVoucherID,double Amount)
{
	LPVOUCHERINFO voucher;
	for (POSITION pos=voucherlist.GetHeadPosition();pos!=NULL;)
	{
		voucher = voucherlist.GetNext(pos);
		if (voucher->VoucherID == nVoucherID)
			break;
	}
	//type
	switch (voucher->type)
	{
	case Upgrade:
	{
		for (POSITION pos = glstOrder.GetHeadPosition(); pos != NULL;)
		{
			LPORDERINFO order = glstOrder.GetNext(pos);
			if (GetSizeID(order->productInfo.szSizeCode) == voucher->nProductSize)
			{
				return GetSizePrice(order->productInfo.szSizeCode);
			}
		}
	}
	break;
	case Toppings:
	{
		double dbPrice =.0;
		for (POSITION pos = glstOrder.GetHeadPosition(); pos != NULL;)
		{
			LPORDERINFO order = glstOrder.GetNext(pos);
			for (int i = 0; i < voucher->limitRedeemCategroy.size(); i++)
			{
				for (int j=0;j<order->productInfo.ToppingArr.size();j++)
				{
					if (GetToppingID(order->productInfo.ToppingArr[j]) == voucher->limitRedeemCategroy[i])
					{
						dbPrice += GetToppingPrice(order->productInfo.ToppingArr[j]);
					}
				}
			}
		}
		return dbPrice;
	}
		
		break;
	case Cash:
	case Discount:
		return true;
		break;
	case Product:
	{
		//1.case 类别限制 
		if (!voucher->limitRedeemCategroy.empty())
		{
			for (POSITION pos = glstOrder.GetHeadPosition(); pos != NULL;)
			{
				LPORDERINFO order = glstOrder.GetNext(pos);
				for (int i = 0; i < voucher->limitRedeemCategroy.size(); i++)
				{
					if (order->productInfo.nCategroyID == voucher->limitRedeemCategroy[i] && voucher->nProductSize >= GetSizeID(order->productInfo.szSizeCode))
					{
						//TODO 判断价格最高的或价格最低的返回
						voucher->discount_price = order->productInfo.dbUnitMenoy + GetSizePrice(order->productInfo.szSizeCode);
						return voucher->discount_price;
					}
				}
			}
		}
		//2.case 产品限制
		if (!voucher->limitRedeemProduct.empty())
		{
			for (POSITION pos = glstOrder.GetHeadPosition(); pos != NULL;)
			{
				LPORDERINFO order = glstOrder.GetNext(pos);
				for (int i = 0; i < voucher->limitRedeemProduct.size(); i++)
				{
					if (order->productInfo.nProductID == voucher->limitRedeemProduct[i] && voucher->nProductSize >= GetSizeID(order->productInfo.szSizeCode))
					{
						voucher->discount_price = order->productInfo.dbUnitMenoy + GetSizePrice(order->productInfo.szSizeCode);
						return voucher->discount_price;
					}
				}
			}
		}
	}
	break;
	case BuyOneGetOne:
		break;
	default:
		break;
	}
	return .0;
}