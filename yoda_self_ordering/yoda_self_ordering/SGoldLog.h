/ SGoldLog.h: interface for the SGoldLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SGOLDLOG_H__6FE0FFBB_6226_4DD3_AAC5_2CA0718BABD8__INCLUDED_)
#define AFX_SGOLDLOG_H__6FE0FFBB_6226_4DD3_AAC5_2CA0718BABD8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <fstream.h>
class SGoldLog  
{
public:
	SGoldLog();
	virtual ~SGoldLog();
public:
	ofstream   *m_log;
	void CheckINIFileExist(CString &szPath);
public:
	BOOL WriteStringToLog(const char*pBuf);
};

#endif // !defined(AFX_SGOLDLOG_H__6FE0FFBB_6226_4DD3_AAC5_2CA0718BABD8__INCLUDED_)
