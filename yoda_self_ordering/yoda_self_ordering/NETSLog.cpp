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
	CString szPath;
	szPath+="LOG\\";
	CreateDirectory(szPath,NULL);
	CTime time = CTime::GetCurrentTime();
	CString szTime;
	szTime.Format(L"NETSLog%04d%02d%02d.log",time.GetYear(),time.GetMonth(),time.GetDay());
	szPath +=szTime;
	m_log.Open(szPath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite);
	m_log.SeekToEnd();

}

NETSLog::~NETSLog()
{
	m_log.Close();
}

BOOL NETSLog::WriteStringToLog(const char *pBuf)
{
	CTime time = CTime::GetCurrentTime();
	CString szTime;
	CString szTmp;
	//szDate.Format(L"%04d-%02d-%02d", time.GetYear(), time.GetMonth(), time.GetDay());
	szTime.Format(L"%02d:%02d:%02d ", time.GetHour(), time.GetMinute(), time.GetSecond());
	szTmp = pBuf;
	szTime += szTmp;
	szTime += "\n";
	char * szTemp = NULL;
	int length = WideCharToMultiByte(CP_ACP, 0, szTime, -1, NULL, 0, NULL, NULL);
	szTemp = new char[length + 1];
	WideCharToMultiByte(CP_ACP, 0, szTime, -1, szTemp, length, NULL, NULL);
	m_log.Write(szTemp, strlen(szTemp));
	delete szTemp;
	szTemp = NULL;
	return true;
}
BOOL NETSLog::WriteHexToLog(const char *pBuf,int nlen)
{
	CTime time = CTime::GetCurrentTime();
	CString szTime;
	CString szTmp,szData;
	char sBuf[256];
	for (int i = 0; i < nlen; i++)
	{
		szTmp.Format(L"%02x ", *(pBuf + i));
		szData += szTmp;
	}
	//szDate.Format(L"%04d-%02d-%02d", time.GetYear(), time.GetMonth(), time.GetDay());
	szTime.Format(L"%02d:%02d:%02d ", time.GetHour(), time.GetMinute(), time.GetSecond());
	szTime += szData;
	szTime += "\n";
	char * szTemp = NULL;
	int length = WideCharToMultiByte(CP_ACP, 0, szTime, -1, NULL, 0, NULL, NULL);
	szTemp = new char[length + 1];
	WideCharToMultiByte(CP_ACP, 0, szTime, -1, szTemp, length, NULL, NULL);
	m_log.Write(szTemp, strlen(szTemp));
	delete szTemp;
	szTemp = NULL;
	return true;
}

BOOL NETSLog::CloseLog()
{
	m_log.Close();
	return true;
}