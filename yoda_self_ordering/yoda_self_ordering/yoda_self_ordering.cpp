// yoda_self_ordering.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "yoda_self_ordering.h"
#include "MainFrm.h"

#include "yoda_self_orderingDoc.h"
#include "yoda_self_orderingView.h"
#include "qrencode.h"
#include "printerlibs.h"
#pragma comment(lib,"printerlibs.lib")
extern 	CList<LPORDERINFO, LPORDERINFO>	glstOrder;
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
	bool bSuccess = false;
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
			szInfo.Format(L"%d", gCurSerialNO);
			//POS_SetBarcode(h, szInfo, -2, 2, 50, 0, 2, POS_BARCODE_TYPE_CODE128);
			POS_SetQRCode(h, W2A(szInfo), 0, 10, 0, 2);
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
		bSuccess = POS_TicketSucceed(h, 0, 1000);
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
	sSQL = _T("select " + sColName + " from " + sTableName + " where " + sColName + " = " + szValue);//
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

	szSQL = "INSERT INTO y_quenextid (tableName, nextID) VALUES('";
	szSQL += szTable;
	szSQL += "', 0)";
	pDB->Execute(szSQL);
	//execSQL(szSQL);

	szSQL = "UPDATE y_quenextid SET nextID = nextID+1 WHERE tableName = '";
	szSQL += szTable;
	szSQL += "'";
	if (pDB->Execute(szSQL) == NULL)
	{
		pDB->RollbackTrans();
		AddLog(pDB, L"ERROR", szSQL, L"");
		return -1;
	}
	szSQL = "SELECT nextID FROM y_quenextid WHERE tableName = '";
	szSQL += szTable;
	szSQL += "'";
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
