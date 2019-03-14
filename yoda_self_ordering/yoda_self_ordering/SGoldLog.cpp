// SGoldLog.cpp: implementation of the SGoldLog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SGoldLog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SGoldLog::SGoldLog()
{
	CString szPath;
	CheckINIFileExist(szPath);
	szPath+="LOG\\";
	CreateDirectory(szPath,NULL);
	CTime time = CTime::GetCurrentTime();
	CString szTime;
	szTime.Format("Service%04d%02d%02d.log",time.GetYear(),time.GetMonth(),time.GetDay());
	szPath +=szTime;
	m_log = new ofstream;
	m_log->open(szPath,ios::app,filebuf::sh_read||filebuf::sh_write);

}

SGoldLog::~SGoldLog()
{
	if(m_log != NULL)
		delete m_log;
}
//获取本程序所在的目录
void SGoldLog::CheckINIFileExist(CString &szPath)
{
		//寻找数据源，连接数据库
	char sBuf[MAX_PATH];
	int dwCount = GetModuleFileName(AfxGetApp()->m_hInstance, sBuf, MAX_PATH);
	szPath = ".\\";
	if (dwCount > 0){
		int iSepPos = -1;
		for (int i = 0; i < dwCount; i++)
			if (sBuf[i] == '\\')
				iSepPos = i;
			if (iSepPos >= 0){
				sBuf[iSepPos] = '\0';
				szPath = sBuf;
				szPath += "\\";
			}
	}


}

BOOL SGoldLog::WriteStringToLog(const char *pBuf)
{
	CTime time = CTime::GetCurrentTime();
	CString szTime;
	CString szDate;
	szDate.Format("%04d-%02d-%02d",time.GetYear(),time.GetMonth(),time.GetDay());
	szTime.Format("%02d:%02d:%02d",time.GetHour(),time.GetMinute(),time.GetSecond());
    *m_log << szDate << " "<< szTime << "   "<< pBuf << endl;
    return true;

}
