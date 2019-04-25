// SGoldLog.h: interface for the SGoldLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NETSLOG_H__6FE0FFBB_6226_4DD3_AAC5_2CA0718BABD8__INCLUDED_)
#define AFX_NETSLOG_H__6FE0FFBB_6226_4DD3_AAC5_2CA0718BABD8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#include <iostream>
//#include <iosfwd>
//#include <fstream>
using namespace std;
class AutoLockCS
{
private:
	LPCRITICAL_SECTION _cs;
public:
	explicit AutoLockCS(LPCRITICAL_SECTION cs)
	{
		_cs = cs;
		EnterCriticalSection(_cs);
	}
	~AutoLockCS()
	{
		LeaveCriticalSection(_cs);
	}
};
class NETSLog  
{
public:
	NETSLog();
	virtual ~NETSLog();
public:
	CRITICAL_SECTION m_cs;
	CStdioFile   m_log;
	//ofstream   *m_log;
	CString m_strFile;
	BOOL CloseLog();
	BOOL WriteStringToLog(const char*pBuf);
	BOOL WriteHexToLog(const char *pBuf, int nlen);
};
class LOG
{
public:

	LOG();


	virtual ~LOG();


public:


	//-日志文件-


	//----如果没有指定，则为exe所在路径下的log.log文件----


	static CString  GetLogFile();


	static short  SetLogFile(LPCTSTR strPath);


	static short  ViewLogFile();





	//-前缀-


	//----如果多个进程往同一个文件输出日志，可以为每个进程设置一个前缀----


	//----前缀出现在日期时间之前----


	static short  SetPrefix(LPCTSTR strPrefix);


	//-日志信息-


	//-获取日志字符串,可以另外-


	static CString  sOutV(LPCTSTR strType, LPCTSTR strFormat = NULL, va_list valist = NULL);


	static CString  sOut0(LPCTSTR strType, LPCTSTR strFormat = NULL, ...);


	static CString  sOut(LPCTSTR strFormat = NULL, ...);

	//-将日志信息输出到文件-


	static short  OutV(LPCTSTR strType, LPCTSTR strFormat = NULL, va_list valist = NULL);


	static short  Out0(LPCTSTR strType, LPCTSTR strFormat = NULL, ...);


	static short  Out(LPCTSTR strFormat = NULL, ...);

protected:


	static CString  s_strLogFile;


	static CString  s_strLogPrefix;


	static HANDLE  s_hWriteEvent;


};
#endif // !defined(AFX_NETSLOG_H__6FE0FFBB_6226_4DD3_AAC5_2CA0718BABD8__INCLUDED_)
