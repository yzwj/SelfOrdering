// SGoldLog.cpp: implementation of the SGoldLog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NETSLog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NETSLog::NETSLog()
{
	CString szPath = gExePath;
	szPath+="LOG\\";
	CreateDirectory(szPath,NULL);
	CTime time = CTime::GetCurrentTime();
	CString szTime;
	szTime.Format(L"NETSLog%04d%02d%02d.log",time.GetYear(),time.GetMonth(),time.GetDay());
	szPath +=szTime;
	m_strFile = szPath;
	//bool bOpen = m_log.Open(szPath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite);
	//m_log.SeekToEnd();
}

NETSLog::~NETSLog()
{
}

BOOL NETSLog::WriteStringToLog(const char *pBuf)
{
	CTime time = CTime::GetCurrentTime();
	CString szTime;
	CString szTmp;
	//szDate.Format(L"%04d-%02d-%02d", time.GetYear(), time.GetMonth(), time.GetDay());
	szTime.Format(L"#%02d:%02d:%02d #", time.GetHour(), time.GetMinute(), time.GetSecond());
	szTmp = pBuf;
	szTime += szTmp;
	szTime += "\n";
	char * szTemp = NULL;
	int length = WideCharToMultiByte(CP_ACP, 0, szTime, -1, NULL, 0, NULL, NULL);
	szTemp = (char*)malloc(length + 1);
	WideCharToMultiByte(CP_ACP, 0, szTime, -1, szTemp, length, NULL, NULL);
	//m_log.Write(szTemp, strlen(szTemp));
	USES_CONVERSION;
	FILE *fp = fopen(W2A(m_strFile), "a+t");
	if (fp)
	{
		fwrite((LPCTSTR)szTemp, strlen(szTemp), sizeof(char), fp);
		fclose(fp);
	}
	free(szTemp);
	szTemp = NULL;
	return true;
}
BOOL NETSLog::WriteHexToLog(const char *pBuf,int nlen)
{
	CTime time = CTime::GetCurrentTime();
	CString szTime;
	CString szTmp,szData;
	char sBuf[1024];
	for (int i = 0; i < nlen; i++)
	{
		szTmp.Format(L"%02x ", *(pBuf + i));
		szData += szTmp;
	}
	//szDate.Format(L"%04d-%02d-%02d", time.GetYear(), time.GetMonth(), time.GetDay());
	szTime.Format(L"#%02d:%02d:%02d #", time.GetHour(), time.GetMinute(), time.GetSecond());
	szTime += szData;
	szTime += "\n";
	char * szTemp = NULL;
	int length = WideCharToMultiByte(CP_ACP, 0, szTime, -1, NULL, 0, NULL, NULL);
	szTemp = (char*)malloc(length + 1);
	WideCharToMultiByte(CP_ACP, 0, szTime, -1, szTemp, length, NULL, NULL);
	USES_CONVERSION;
	FILE *fp = fopen(W2A(m_strFile), "a+t");
	if (fp)
	{
		fwrite((LPCTSTR)szTemp, strlen(szTemp), sizeof(char), fp);
		fclose(fp);
	}
	free(szTemp);
	szTemp = NULL;
	return true;
}

BOOL NETSLog::CloseLog()
{
	
	return true;
}


CString GetExePath(BOOL bIncludeSep)
{
	// 得到当前的文件名
	CString strFileName;
	GetModuleFileName(AfxGetInstanceHandle(), strFileName.GetBuffer(_MAX_PATH), _MAX_PATH);
	strFileName.ReleaseBuffer();
	// 得到当前目录
	strFileName = strFileName.Left(strFileName.ReverseFind(_T('\\')) + 1);
	if (bIncludeSep)
		return strFileName;
	else
		return strFileName.Left(strFileName.GetLength() - 1);
}


//-获取最后的文件名 如果给定文件不是全路径，就是相对于exe-


CString GetFileForExePath(LPCTSTR strCurFileName)
{
	CString strPath = strCurFileName;
	if (!strPath.IsEmpty())
	{
		//-相对路径-
		if (strPath.Find(_T(":")) <= 0)
		{
			strPath.Format(_T("%s%s"), GetExePath(FALSE), strCurFileName);
		}
	}
	return strPath;
}

#define LOG_EVENT _T("ChyLogWrite")  

CString LOG::s_strLogFile = _T("");

CString LOG::s_strLogPrefix = _T("");

HANDLE LOG::s_hWriteEvent = NULL;

LOG::LOG()
{

}

LOG::~LOG()
{

}
short LOG::SetLogFile(LPCTSTR strPath)
{
	s_strLogFile = gExePath;
	s_strLogFile += "LOG\\";
	CreateDirectory(s_strLogFile, NULL);
	CTime time = CTime::GetCurrentTime();
	CString szTime;
	szTime.Format(L"NETSLog%04d%02d%02d.log", time.GetYear(), time.GetMonth(), time.GetDay());
	s_strLogFile += szTime;
	return 1;
}


CString LOG::GetLogFile()
{
	return s_strLogFile;
}


short LOG::ViewLogFile()
{
	CString strLogFile = GetLogFile();
	ShellExecute(NULL, _T("open"), strLogFile, NULL, NULL, SW_SHOW);
	return strLogFile.IsEmpty() ? 0 : 1;
}


short LOG::SetPrefix(LPCTSTR strPrefix)
{
	if (strPrefix && strPrefix[0])
	{
		s_strLogPrefix = strPrefix;
	}
	return 1;
}

CString LOG::sOutV(LPCTSTR strType, LPCTSTR strFormat, va_list valist)
{
	CString   strPart_Prefix;
	if (!s_strLogPrefix.IsEmpty())
	{
		strPart_Prefix.Format(_T("[%s]"), s_strLogPrefix);
	}
	CString   strPart_Time;
	{

		SYSTEMTIME sysTime = { 0 };


		GetLocalTime(&sysTime);


		strPart_Time.Format(_T("[%2d-%2d %2d:%2d:%2d_%3d]"),


			sysTime.wMonth, sysTime.wDay,


			sysTime.wHour, sysTime.wMinute, sysTime.wSecond,


			sysTime.wMilliseconds);


	}

	CString   strPart_Type;
	if (strType && strType[0])
	{
		strPart_Type.Format(_T("[%s]"), strType);

	}

	CString   strPart_Info;
	{
		strPart_Info.FormatV(strFormat, valist);
	}
	CString str = strPart_Prefix + strPart_Time + strPart_Type + strPart_Info;
	return str;
}

CString LOG::sOut0(LPCTSTR strType, LPCTSTR strFormat, ...)
{
	va_list  valist;


	va_start(valist, strFormat);


	CString strInfo = sOutV(strType, strFormat, valist);


	va_end(valist);

	return strInfo;
}


CString LOG::sOut(LPCTSTR strFormat, ...)
{
	va_list  valist;
	va_start(valist, strFormat);
	CString strInfo = sOutV(NULL, strFormat, valist);
	va_end(valist);
	return strInfo;
}


short LOG::OutV(LPCTSTR strType, LPCTSTR strFormat, va_list valist)
{
	//--
	if (s_hWriteEvent == NULL)
	{

		s_hWriteEvent = OpenEvent(0, FALSE, LOG_EVENT);

		if (s_hWriteEvent == NULL)

			s_hWriteEvent = CreateEvent(NULL, FALSE, TRUE, LOG_EVENT);

	}
	WaitForSingleObject(s_hWriteEvent, INFINITE);

	//-打开关闭文件-
	if (s_strLogFile.IsEmpty())
		SetLogFile(NULL);

	CStdioFile file;
	if (file.Open(s_strLogFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite))
	{

		CString  strPart_NewLine = _T("\n");


		CString  strInfo = sOutV(strType, strFormat, valist);


		CString  str = strPart_NewLine + strInfo;

		file.SeekToEnd();


		file.WriteString(str);


		file.Close();


	}

	SetEvent(s_hWriteEvent);

	return 1;
}

short LOG::Out0(LPCTSTR strType, LPCTSTR strFormat, ...)
{

	va_list  valist;

	va_start(valist, strFormat);

	short rtn = OutV(strType, strFormat, valist);

	va_end(valist);

	return rtn;
}

short LOG::Out(LPCTSTR strFormat, ...)
{
	va_list  valist;
	va_start(valist, strFormat);
	short rtn = OutV(NULL, strFormat, valist);
	va_end(valist);
	return rtn;
}