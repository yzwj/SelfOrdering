// SGoldLog.h: interface for the SGoldLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NETSLOG_H__6FE0FFBB_6226_4DD3_AAC5_2CA0718BABD8__INCLUDED_)
#define AFX_NETSLOG_H__6FE0FFBB_6226_4DD3_AAC5_2CA0718BABD8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <iostream>
#include <iosfwd>
using namespace std;
class NETSLog  
{
public:
	NETSLog();
	virtual ~NETSLog();
public:
	CStdioFile   m_log;
	BOOL CloseLog();
	BOOL WriteStringToLog(const char*pBuf);
	BOOL WriteHexToLog(const char *pBuf, int nlen);
};

#endif // !defined(AFX_NETSLOG_H__6FE0FFBB_6226_4DD3_AAC5_2CA0718BABD8__INCLUDED_)
