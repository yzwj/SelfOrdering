#include "StdAfx.h"  
#include "SerialPort.h"  
#include <process.h>  
#include <iostream>  
#include <iomanip>
#define STX_INDEX 0
#define LEN_INDEX 1
#define ECN_INDEX 3
#define FUN_INDEX 15
#define BODY_INDEX 21
#define STX 0x02
#define ETX 0x03
#define ACK 0x06
#define NCK 0x15
#define SEP 0x1C
#define RFU 0x30 
NETSLog gNETSLog;
std::vector<MsgData>MsgList;
int strTobcd(unsigned char *dest, const char *src)
{
	int i;
	unsigned char hbit, lbit;
	int len = strlen(src);
	for (i = 0; i < len; i += 2)
	{
		hbit = (src[i] > '9') ? ((src[i] & 0x0F) + 9) : (src[i] & 0x0F);
		lbit = (src[i + 1] > '9') ? ((src[i + 1] & 0x0F) + 9) : (src[i + 1] & 0x0F);
		dest[i / 2] = (hbit << 4) | lbit;
	}
	return 0;
}
int BcdToAsc(char *pcBCD, char *pcASC, int iLength)
{
	int i;
	for (i = 0; i<iLength; i++) {
		unsigned char ch;
		ch = (unsigned char)pcBCD[i];
		ch = ch >> 4;
		if (ch >= 10) {
			pcASC[2 * i] = ch - 10 + 'A';
		}
		else {
			pcASC[2 * i] = ch + '0';
		}
		ch = (unsigned char)pcBCD[i];
		ch = ch & 0x0f;
		if (ch >= 10) {
			pcASC[2 * i + 1] = ch - 10 + 'A';
		}
		else {
			pcASC[2 * i + 1] = ch + '0';
		}
	}
	return atoi(pcASC);
}
bool VerifyLRC(unsigned char * data)
{
	char bcdlen[8] = { 0 };
	char len[2] = { 0 };
	long  nlen;
	len[0] = data[1];
	len[1] = data[2];
	nlen = BcdToAsc(len, bcdlen, 2);
	data++;//0x02
	data++;//len
	data++;//len
	BYTE LRC = 0x00;
	for (int i = 0; i < nlen; i++)
	{
		LRC ^= *data++;
	}
	if (0x03 == *data++ && LRC == *data)
		return true;
	return false;
}
/** 线程退出标志 */
bool CSerialPort::s_bExit = false;
/** 当串口无数据时,sleep至下次查询间隔的时间,单位:毫秒 */
const UINT SLEEP_TIME_INTERVAL = 100;
#define RECEIVED_DATA_DEFAULT_LENGHTH 4096
CSerialPort::CSerialPort(void)
	: m_hListenThread(INVALID_HANDLE_VALUE)
{
	m_hComm = INVALID_HANDLE_VALUE;
	m_hListenThread = INVALID_HANDLE_VALUE;
	InitializeCriticalSection(&m_csCommunicationSync);
	m_recvData = (unsigned char *)malloc(RECEIVED_DATA_DEFAULT_LENGHTH);
	m_ResponseMsg = (char *)malloc(128);
}

CSerialPort::~CSerialPort(void)
{
	CloseListenTread();
	ClosePort();
	DeleteCriticalSection(&m_csCommunicationSync);
	free(m_recvData);
	m_recvData = NULL;
}

bool CSerialPort::InitPort(UINT portNo /*= 1*/, UINT baud /*= CBR_9600*/, char parity /*= 'N'*/,
	UINT databits /*= 8*/, UINT stopsbits /*= 1*/, DWORD dwCommEvents /*= EV_RXCHAR*/)
{

	/** 临时变量,将制定参数转化为字符串形式,以构造DCB结构 */
	char szDCBparam[50];
	sprintf_s(szDCBparam, "baud=%d parity=%c data=%d stop=%d", baud, parity, databits, stopsbits);

	/** 打开指定串口,该函数内部已经有临界区保护,上面请不要加保护 */
	if (!openPort(portNo))
	{
		return false;
	}

	/** 进入临界段 */
	EnterCriticalSection(&m_csCommunicationSync);

	/** 是否有错误发生 */
	BOOL bIsSuccess = TRUE;

	/** 在此可以设置输入输出的缓冲区大小,如果不设置,则系统会设置默认值.
	*  自己设置缓冲区大小时,要注意设置稍大一些,避免缓冲区溢出
	*/
	if (bIsSuccess )
	{
		bIsSuccess = SetupComm(m_hComm,1024,1024);
	}

	/** 设置串口的超时时间,均设为0,表示不使用超时限制 */
	COMMTIMEOUTS  CommTimeouts;
	CommTimeouts.ReadIntervalTimeout = 0;
	CommTimeouts.ReadTotalTimeoutMultiplier = 0;
	CommTimeouts.ReadTotalTimeoutConstant = 0;
	CommTimeouts.WriteTotalTimeoutMultiplier = 0;
	CommTimeouts.WriteTotalTimeoutConstant = 0;
	if (bIsSuccess)
	{
		bIsSuccess = SetCommTimeouts(m_hComm, &CommTimeouts);
	}

	DCB  dcb;
	if (bIsSuccess)
	{
		// 将ANSI字符串转换为UNICODE字符串  
		DWORD dwNum = MultiByteToWideChar(CP_ACP, 0, szDCBparam, -1, NULL, 0);
		wchar_t *pwText = new wchar_t[dwNum];
		if (!MultiByteToWideChar(CP_ACP, 0, szDCBparam, -1, pwText, dwNum))
		{
			bIsSuccess = TRUE;
		}

		/** 获取当前串口配置参数,并且构造串口DCB参数 */
		bIsSuccess = GetCommState(m_hComm, &dcb) && BuildCommDCB(pwText, &dcb);
		/** 开启RTS flow控制 */
		dcb.fRtsControl = RTS_CONTROL_ENABLE;

		/** 释放内存空间 */
		delete[] pwText;
	}

	if (bIsSuccess)
	{
		/** 使用DCB参数配置串口状态 */
		bIsSuccess = SetCommState(m_hComm, &dcb);
	}

	/**  清空串口缓冲区 */
	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
	OpenListenThread();
	/** 离开临界段 */
	LeaveCriticalSection(&m_csCommunicationSync);
	return bIsSuccess == TRUE;
}

bool CSerialPort::InitPort(UINT portNo, const LPDCB& plDCB)
{
	/** 打开指定串口,该函数内部已经有临界区保护,上面请不要加保护 */
	if (!openPort(portNo))
	{
		return false;
	}

	/** 进入临界段 */
	EnterCriticalSection(&m_csCommunicationSync);

	/** 配置串口参数 */
	if (!SetCommState(m_hComm, plDCB))
	{
		return false;
	}
	OpenListenThread();
	/**  清空串口缓冲区 */
	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

	/** 离开临界段 */
	LeaveCriticalSection(&m_csCommunicationSync);

	return true;
}

void CSerialPort::ClosePort()
{
	/** 如果有串口被打开，关闭它 */
	if (m_hComm != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hComm);
		m_hComm = INVALID_HANDLE_VALUE;
	}
	CloseListenTread();
}

bool CSerialPort::openPort(UINT portNo)
{
	/** 进入临界段 */
	EnterCriticalSection(&m_csCommunicationSync);

	/** 把串口的编号转换为设备名 */
	WCHAR szPort[50];
	wsprintf(szPort, _T("\\\\.\\COM%d"), portNo);
	/** 打开指定的串口 */
	m_hComm = CreateFile(szPort,  /** 设备名,COM1,COM2等 */
		GENERIC_READ | GENERIC_WRITE, /** 访问模式,可同时读写 */
		0,                            /** 共享模式,0表示不共享 */
		NULL,                         /** 安全性设置,一般使用NULL */
		OPEN_EXISTING,                /** 该参数表示设备必须存在,否则创建失败 */
		0,
		0);

	/** 如果打开失败，释放资源并返回 */
	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		LeaveCriticalSection(&m_csCommunicationSync);
		return false;
	}

	/** 退出临界区 */
	LeaveCriticalSection(&m_csCommunicationSync);

	return true;
}

bool CSerialPort::OpenListenThread()
{
	/** 检测线程是否已经开启了 */
	if (m_hListenThread != INVALID_HANDLE_VALUE)
	{
		/** 线程已经开启 */
		return false;
	}

	s_bExit = false;
	/** 线程ID */
	UINT threadId;
	/** 开启串口数据监听线程 */
	m_hListenThread = (HANDLE)_beginthreadex(NULL, 0, ListenThread, this, 0, &threadId);
	if (!m_hListenThread)
	{
		return false;
	}
	/** 设置线程的优先级,高于普通线程 */
	if (!SetThreadPriority(m_hListenThread, THREAD_PRIORITY_ABOVE_NORMAL))
	{
		return false;
	}

	return true;
}

bool CSerialPort::CloseListenTread()
{
	if (m_hListenThread != INVALID_HANDLE_VALUE)
	{
		/** 通知线程退出 */
		s_bExit = true;

		/** 等待线程退出 */
		Sleep(10);

		/** 置线程句柄无效 */
		CloseHandle(m_hListenThread);
		m_hListenThread = INVALID_HANDLE_VALUE;
	}
	return true;
}

UINT CSerialPort::GetBytesInCOM()
{
	DWORD dwError = 0;  /** 错误码 */
	COMSTAT  comstat;   /** COMSTAT结构体,记录通信设备的状态信息 */
	memset(&comstat, 0, sizeof(COMSTAT));

	UINT BytesInQue = 0;
	/** 在调用ReadFile和WriteFile之前,通过本函数清除以前遗留的错误标志 */
	if (ClearCommError(m_hComm, &dwError, &comstat))
	{
		BytesInQue = comstat.cbInQue; /** 获取在输入缓冲区中的字节数 */
	}

	return BytesInQue;
}
unsigned char * __cdecl strncat_com(
	unsigned char * front,
	size_t front_len,
	const unsigned char * back,
	size_t count
)
{
	unsigned char *start = front;
	while (front_len--)
		*front++;
	while (count--)
		*front++ = *back++;
	*front = '\0';
	return(start);
}
UINT WINAPI CSerialPort::ListenThread(void* pParam)
{
	/** 得到本类的指针 */
	CSerialPort *pSerialPort = reinterpret_cast<CSerialPort*>(pParam);
	unsigned char data[4096] = {0};
	bool bWhole = true;
	UINT nCounts = 0;
	// 线程循环,轮询方式读取串口数据  
	while (!pSerialPort->s_bExit)
	{
		UINT BytesInQue = pSerialPort->GetBytesInCOM();
		/** 如果串口输入缓冲区中无数据,则休息一会再查询 */
		if (BytesInQue == 0)
		{
			Sleep(SLEEP_TIME_INTERVAL);
			pSerialPort->m_bReceived = FALSE;
			continue;
		}
		
		/** 读取输入缓冲区中的数据并输出显示 */
		if (pSerialPort->ReadChar(pSerialPort->m_recvData,BytesInQue) == true)
		{
			if (BytesInQue == 1)
			{
				if (*pSerialPort->m_recvData== 0x06)
				{
					pSerialPort->m_bRecvFlag = ACK_STATUS;
					gNETSLog.WriteStringToLog("recv:ACK");
					nCounts = 0;
				}
				else if (*pSerialPort->m_recvData == 0x015)
				{
					pSerialPort->m_bRecvFlag = NACK_STATUS;
					gNETSLog.WriteStringToLog("recv:NACK");
					nCounts = 0;
				}
				continue;
			}
			else if (BytesInQue > 1 && *pSerialPort->m_recvData == 0x06)
			{
				pSerialPort->m_bRecvFlag = ACK_STATUS;
				unsigned char sBuf[512] = { 0 };
				memcpy(sBuf,data,BytesInQue);
				memcpy(data, sBuf + 1, BytesInQue - 1);
			}
			pSerialPort->m_bReceived = TRUE;
			if (bWhole)
			{
				memcpy(data, pSerialPort->m_recvData, BytesInQue);
				nCounts += BytesInQue;
			}
			else
			{
				strncat_com(data, nCounts,pSerialPort->m_recvData,BytesInQue);
				nCounts += BytesInQue;
			}
			
			if (data[0] == 0x02)
			{
				char bcdlen[8] = { 0 };
				char len[2] = { 0 };
				long  nRecvCounts;
				len[0] = data[1];
				len[1] = data[2];
				nRecvCounts = BcdToAsc(len, bcdlen, 2);
				if (nRecvCounts + 5 != nCounts)
				{
					bWhole = FALSE;
					continue;
				}
				else
				{
					bWhole = TRUE;
					nCounts = 0;
				}
				char ECN[12] = { 0 };
				for (int i = 3; i < 15; i++)
				{
					ECN[i - 3] = data[i];
				}
				char funcode[3];
				sprintf_s(funcode, "%c%c", data[FUN_INDEX], data[FUN_INDEX + 1]);
				if (!strcmp(funcode, "80"))//logon
				{
					gNETSLog.WriteStringToLog("recv:logon");
					gNETSLog.WriteHexToLog((char*)data, nRecvCounts+5);
					int index = BODY_INDEX;
					int nlen = 0;
					for (int j = index; j < nRecvCounts && index <= nRecvCounts; j++)
					{
						//fieldCode
						//printf("FieldCode:%c%c -> ", data[index], data[index + 1]);
						if (data[index] == '0' && data[index + 1] == '2')
						{
							index += 2;
							//len
							len[0] = data[index++];
							len[1] = data[index++];
							nlen = BcdToAsc(len, bcdlen, 2);
							//Text
							char sBuf[64] = { 0 };
							int k = 0;
							for (int i = index; i < index + nlen; i++, k++)
								sBuf[k] = data[i];
							if (!strncmp(pSerialPort->m_curECN, ECN, 12) && !strncmp(sBuf, "APPROVED", 8))
								pSerialPort->m_bRecvFlag = TRANSACTION_COMPLATED_STATUS;
							else
							{
								pSerialPort->m_bRecvFlag = TRANSACTION_ERROR_STATUS;
								pSerialPort->m_ResponseMsg = sBuf;
							}
							index += nlen;
							index += 1;//0x1c
						}
					}
					//returen 0x06
					char ack = 0x06;
					pSerialPort->WriteData(&ack, 1);
				}
				if (!strcmp(funcode, "81"))//settlement
				{
					gNETSLog.WriteStringToLog("recv:settlement");
					gNETSLog.WriteHexToLog((char*)data, nRecvCounts + 5);
					int index = BODY_INDEX;
					int nlen = 0;
					for (int j = index; j < nRecvCounts && index <= nRecvCounts; j++)
					{
						//fieldCode
						//printf("FieldCode:%c%c -> ", data[index], data[index + 1]);
						if (data[index] == '0' && data[index + 1] == '2')
						{
							index += 2;
							//len
							len[0] = data[index++];
							len[1] = data[index++];
							nlen = BcdToAsc(len, bcdlen, 2);
							//Text
							char sBuf[64] = { 0 };
							int k = 0;
							for (int i = index; i < index + nlen; i++, k++)
								sBuf[k] = data[i];
							if (!strncmp(pSerialPort->m_curECN, ECN, 12) && !strncmp(sBuf, "Settlement Complete", 19))
								pSerialPort->m_bRecvFlag = TRANSACTION_COMPLATED_STATUS;
							else
							{
								pSerialPort->m_bRecvFlag = TRANSACTION_ERROR_STATUS;
								pSerialPort->m_ResponseMsg = sBuf;
							}
							index += nlen;
							index += 1;//0x1c
						}
					}
					//returen 0x06
					char ack = 0x06;
					pSerialPort->WriteData(&ack, 1);
				}
				else if (!strcmp(funcode, "84"))//TMS
				{
					int index = BODY_INDEX;
					int nlen = 0;
					for (int j = index; j < nRecvCounts && index <= nRecvCounts; j++)
					{
						//fieldCode
						printf("FieldCode:%c%c -> ", data[index], data[index + 1]);
						index += 2;
						//len
						len[0] = data[index++];
						len[1] = data[index++];
						nlen = BcdToAsc(len, bcdlen, 2);
						//Text
						for (int i = index; i < index + nlen; i++)
							printf("%c", data[i]);
						printf("\n");
						index += nlen;
						index += 1;//0x1c
					}
					//returen 0x06
					pSerialPort->m_bRecvFlag = TRANSACTION_COMPLATED_STATUS;
					char ack = 0x06;
					pSerialPort->WriteData(&ack, 1);
				}
				else if (!strcmp(funcode, "24"))//flashpay
				{
					gNETSLog.WriteStringToLog("recv:flashpay");
					gNETSLog.WriteHexToLog((char*)data, nRecvCounts + 5);
					int index = BODY_INDEX;
					int nlen = 0;
					for (int j = index; j < nRecvCounts && index <= nRecvCounts; j++)
					{
						//fieldCode
						if (data[index] == '0' && data[index + 1] == '2')
						{
							printf("FieldCode:%c%c -> ", data[index], data[index + 1]);
							index += 2;
							//len
							len[0] = data[index++];
							len[1] = data[index++];
							nlen = BcdToAsc(len, bcdlen, 2);
							char sBuf[64] = { 0 };
							int j = 0;
							for (int i = index; i < index + nlen; i++, j++)
								sBuf[j] = data[i];
							printf(sBuf);
							printf("\n");
							if(!strncmp(pSerialPort->m_curECN, ECN, 12) && !strncmp(sBuf,"APPROVED",8))
								pSerialPort->m_bRecvFlag = TRANSACTION_COMPLATED_STATUS;
							else
							{
								pSerialPort->m_bRecvFlag = TRANSACTION_ERROR_STATUS;
								printf(sBuf);
								printf("\n");
							}
							memcpy(pSerialPort->m_ResponseMsg, sBuf, 64);
							index += nlen;
							index += 1;//0x1c
						}
						else if (data[index] == 'C' && data[index + 1] == '0')
						{
							printf("FieldCode:%c%c -> ", data[index], data[index + 1]);
							index += 2;
							//len
							len[0] = data[index++];
							len[1] = data[index++];
							nlen = BcdToAsc(len, bcdlen, 2);
							char sBuf[64] = { 0 };
							int j = 0;
							for (int i = index; i < index + nlen; i++, j++)
								sBuf[j] = data[i];
							j = 0;
							printf("\n{\nTransaction Type:%c%c\n", sBuf[j], sBuf[j + 1]);
							j += 2;
							//printf("Transaction Amount:");
							char amounts[12];
							for (int k = 0; k < 12; k++)
							{
								//printf("%c", sBuf[j + k]);
								amounts[k] = sBuf[j + k];
							}
							int nAmounts = atoi(amounts);
							pSerialPort->m_dbTransAmounts = (float)nAmounts / 100;
							printf("Translation Amounts:%f\n", pSerialPort->m_dbTransAmounts);
							j += 12;
							printf("Transaction Date:");
							for (int k = 0; k < 6; k++)
							{
								printf("%c", sBuf[j + k]);
							}
							j += 6;
							printf("\n");
							printf("Transaction Time:");
							for (int k = 0; k < 6; k++)
							{
								printf("%c", sBuf[j + k]);
							}
							j += 6;
							printf("\n");
							printf("Prior Card Balance");
							for (int k = 0; k < 12; k++)
							{
								printf("%c", sBuf[j + k]);
							}
							j += 12;
							printf("\n");
							printf("Post Card Balance");
							for (int k = 0; k < 12; k++)
							{
								printf("%c", sBuf[j + k]);
							}
							j += 12;
							printf("\n");
							printf("Post Auto load Amount");
							for (int k = 0; k < 12; k++)
							{
								printf("%c", sBuf[j + k]);
							}
							j += 12;
							printf("\n");
							printf("Post Card Status:%c%c\n}\n", sBuf[j], sBuf[j + 1]);
							index += nlen;
							index += 1;//0x1c
						}
						else
						{
							printf("FieldCode:%c%c -> ", data[index], data[index+1]);
							index += 2;
							//len
							len[0] = data[index++];
							len[1] = data[index++];
							nlen = BcdToAsc(len, bcdlen, 2);
							//Text
							for (int i = index; i < index + nlen; i++)
								printf("%c", data[i]);
							printf("\n");
							index += nlen;
							index += 1;//0x1c
						}
					}
					char ack = 0x06;
					pSerialPort->WriteData(&ack, 1);
				}
				else if (!strcmp(funcode, "30"))//
				{
					gNETSLog.WriteStringToLog("recv:NETSDebit/QRCode");
					gNETSLog.WriteHexToLog((char*)data, nRecvCounts + 5);
					int index = BODY_INDEX;
					int nlen = 0;
					for (int j = index; j < nRecvCounts && index <= nRecvCounts; j++)
					{
						if (data[index] == '0' && data[index + 1] == '2')
						{
							printf("FieldCode:%c%c -> ", data[index + 1], data[index + 2]);
							index += 2;
							//len
							len[0] = data[index++];
							len[1] = data[index++];
							nlen = BcdToAsc(len, bcdlen, 2);
							char sBuf[64] = { 0 };
							int j = 0;
							for (int i = index; i < index + nlen; i++, j++)
								sBuf[j] = data[i];
							printf(sBuf);
							printf("\n");
							if (!strncmp(pSerialPort->m_curECN, ECN, 12) && !strncmp(sBuf, "APPROVED", 8))
								pSerialPort->m_bRecvFlag = TRANSACTION_COMPLATED_STATUS;
							else
							{
								pSerialPort->m_bRecvFlag = TRANSACTION_ERROR_STATUS;
								printf(sBuf);
								printf("\n");
							}
							memcpy(pSerialPort->m_ResponseMsg, sBuf, 64);
							index += nlen;
							index += 1;//0x1c
						}
						else
						{
							//fieldCode
							printf("FieldCode:%c%c -> ", data[index], data[index + 1]);
							index += 2;
							//len
							len[0] = data[index++];
							len[1] = data[index++];
							nlen = BcdToAsc(len, bcdlen, 2);
							//Text
							for (int i = index; i < index + nlen; i++)
								printf("%c", data[i]);
							printf("\n");
							index += nlen;
							index += 1;//0x1c
						}
					}
					char ack = 0x06;
					pSerialPort->WriteData(&ack, 1);
				}
				else if (!strcmp(funcode, "56"))//Last Approved
				{
					int index = BODY_INDEX;
					int nlen = 0;
					for (int j = index; j < nRecvCounts && index <= nRecvCounts; j++)
					{
						if (data[index] == 'F' && data[index + 1] == 'C')
						{
							//printf("FieldCode:%c%c -> \n", data[index], data[index+1]);
							index += 2;
							//len
							len[0] = data[index++];
							len[1] = data[index++];
							nlen = BcdToAsc(len, bcdlen, 2);
							char sBuf[64] = { 0 };
							int j = 0;
							for (int i = index; i < index + nlen; i++, j++)
								pSerialPort->m_lastApprovedFun[j]= data[i];
						}
						else if (data[index] == 'T' && data[index + 1] == 'D')
						{
							//printf("FieldCode:%c%c -> \n", data[index], data[index + 1]);
							index += 2;
							//len
							len[0] = data[index++];
							len[1] = data[index++];
							nlen = BcdToAsc(len, bcdlen, 2);
							char sBuf[64] = { 0 };
							int j = 0;
							memcpy(pSerialPort->m_lastApprovedECN, data + index, 12);
						}
						index += nlen;
						index += 1;//0x1c
					}
					pSerialPort->m_bRecvFlag = TRANSACTION_COMPLATED_STATUS;
					char ack = 0x06;
					pSerialPort->WriteData(&ack, 1);
				}
			}
		}
	}
	return 0;
}

bool CSerialPort::ReadChar(unsigned char* cRecved,UINT nLen)
{
	BOOL  bResult = TRUE;
	DWORD BytesRead = 0;
	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	/** 临界区保护 */
	EnterCriticalSection(&m_csCommunicationSync);

	/** 从缓冲区读取一个字节的数据 */
	bResult = ReadFile(m_hComm, cRecved, nLen, &BytesRead, NULL);
	if ((!bResult))
	{
		/** 获取错误码,可以根据该错误码查出错误原因 */
		DWORD dwError = GetLastError();

		/** 清空串口缓冲区 */
		PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_RXABORT);
		LeaveCriticalSection(&m_csCommunicationSync);

		return false;
	}

	/** 离开临界区 */
	LeaveCriticalSection(&m_csCommunicationSync);

	return (bResult == 1);

}

bool CSerialPort::WriteData(char* pData, unsigned int length)
{
	//PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_RXABORT);
	//COMSTAT ComStat;
	//DWORD dwErrorFlags;
	//ClearCommError(m_hComm, &dwErrorFlags, &ComStat);
	BOOL   bResult = TRUE;
	DWORD  BytesToSend = 0;
	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	/** 临界区保护 */
	EnterCriticalSection(&m_csCommunicationSync);

	/** 向缓冲区写入指定量的数据 */
	bResult = WriteFile(m_hComm, pData, length, &BytesToSend, NULL);
	if (!bResult)
	{
		DWORD dwError = GetLastError();
		/** 清空串口缓冲区 */
		PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_RXABORT);
		LeaveCriticalSection(&m_csCommunicationSync);

		return false;
	}

	/** 离开临界区 */
	LeaveCriticalSection(&m_csCommunicationSync);

	return true;
}


long CSerialPort::GetFuntionData(char* data, char* ECN, char* FunCode, char* version, std::vector<MsgData>&msgData)
{
	unsigned char index = 0;
	char *tmp = data;
	*data++ = STX;
	long length = 18;
	if (msgData.size() > 0)
	{
		for (int i = 0; i < msgData.size(); i++)
		{
			MsgData data = msgData[i];
			length += 2;
			length += 2;
			char bcdlen[4] = { 0 };
			int nLen = BcdToAsc(data.len, bcdlen, 2);
			length += nLen;
			length += 1;
		}
	}
	char slen[32];
	sprintf_s(slen, "%04d", length);
	unsigned char dest[2];
	strTobcd(dest, slen);
	*data++ = dest[0];
	*data++ = dest[1];
	for (int i = 0; i < 12; i++)
		*data++ = *ECN++;
	index += 12;
	*data++ = *FunCode++;
	*data++ = *FunCode++;
	*data++ = *version++;
	*data++ = *version++;
	*data++ = RFU;
	*data++ = SEP;
	if (msgData.size() > 0)
	{
		char bcdlen[4];
		for (int i = 0; i < msgData.size(); i++)
		{
			MsgData msg = msgData[i];
			*data++ = msg.FieldCode[0];
			*data++ = msg.FieldCode[1];
			*data++ = msg.len[0];
			*data++ = msg.len[1];
			int nlen = BcdToAsc(msg.len, bcdlen, 2);
			for (int j = 0; j < nlen; j++)
			{
				*data++ = msg.data[j];
			}
			*data++ = msg.sep[0];
		}
	}
	*data++ = ETX;
	*data = 0xFF;
	unsigned char LRC;
	LRC = 0x00 ^ tmp[1];
	int i = 2;
	for (; i <= length + 3; i++)
	{
		if (tmp[i] == 0xff)
			break;
		LRC ^= tmp[i];
	}
	*data = LRC;
	return length + 4;
}

RECV_STATUS CSerialPort::LogonNETS(char * ECN)
{
	char data[255] = { 0 };
	m_curECN = ECN;
	m_bRecvFlag = INIT_STATUS;
	long length = GetFuntionData(data, ECN, "80", "01", MsgList);
	gNETSLog.WriteStringToLog("Data sent:Logon");
	gNETSLog.WriteHexToLog(data,length+1);
	WriteData(data, length + 1);
	int looptimes = 0;
	int retrytimes = 2;
	while (1)
	{
		if (m_bRecvFlag == NACK_STATUS && retrytimes > 0)
		{
			m_bRecvFlag = INIT_STATUS;
			WriteData(data, length + 1);
			retrytimes--;
		}
		else if (m_bRecvFlag == ACK_STATUS || m_bRecvFlag == INIT_STATUS)
		{
			if (m_bRecvFlag == INIT_STATUS && looptimes >= 1 && retrytimes >= 1)
				m_bRecvFlag == NACK_STATUS;
			Sleep(1000);
			looptimes++;
			if (looptimes >= 60)
				return TIME_OUT_STATUS;
		}
		else if (retrytimes == 0 && m_bRecvFlag == INIT_STATUS)
			return TIME_OUT_STATUS;
		else return m_bRecvFlag;
	}
	return m_bRecvFlag;
}

RECV_STATUS CSerialPort::SettlementNETS(char * ECN)
{

	char data[255] = { 0 };
	m_curECN = ECN;
	m_bRecvFlag = INIT_STATUS;
	long length = GetFuntionData(data, ECN, "81", "01", MsgList);
	gNETSLog.WriteStringToLog("Data sent:Logon");
	gNETSLog.WriteHexToLog(data, length + 1);
	WriteData(data, length + 1);
	int looptimes = 0;
	int retrytimes = 2;
	while (1)
	{
		if (m_bRecvFlag == NACK_STATUS && retrytimes > 0)
		{
			m_bRecvFlag = INIT_STATUS;
			WriteData(data, length + 1);
			retrytimes--;
		}
		else if (m_bRecvFlag == ACK_STATUS || m_bRecvFlag == INIT_STATUS)
		{
			if (m_bRecvFlag == INIT_STATUS && looptimes >= 1 && retrytimes >= 1)
				m_bRecvFlag == NACK_STATUS;
			Sleep(1000);
			looptimes++;
			if (looptimes >= 60)
				return TIME_OUT_STATUS;
		}
		else if (retrytimes == 0 && m_bRecvFlag == INIT_STATUS)
			return TIME_OUT_STATUS;
		else return m_bRecvFlag;
	}
	return m_bRecvFlag;
}

RECV_STATUS CSerialPort::TransactionNETSDebit(char* ECN, float Amounts)
{
	MsgList.clear();
	m_curECN = ECN;
	m_bRecvFlag = INIT_STATUS;
	char data[255] = { 0 };
	int amount_i = Amounts * 100;
	MsgData Transmsg, loyaltymsg, TransAmountMsg;
	Transmsg.FieldCode[0] = 'T';
	Transmsg.FieldCode[1] = '2';
	char slen[16];
	sprintf_s(slen, "%04d", 2);
	unsigned char dest[2];
	strTobcd(dest, slen);
	Transmsg.len[0] = dest[0];
	Transmsg.len[1] = dest[1];
	Transmsg.data[0] = '0';
	Transmsg.data[1] = '1';
	Transmsg.sep[0] = 0x1c;
	MsgList.push_back(Transmsg);
	loyaltymsg.FieldCode[0] = '4';
	loyaltymsg.FieldCode[1] = '3';
	sprintf_s(slen, "%04d", 1);
	strTobcd(dest, slen);
	memset(loyaltymsg.len, 0, 128);
	memset(loyaltymsg.data, 0, 128);
	loyaltymsg.len[0] = dest[0];
	loyaltymsg.len[1] = dest[1];
	loyaltymsg.data[0] = '0';
	loyaltymsg.sep[0] = 0x1c;
	MsgList.push_back(loyaltymsg);
	TransAmountMsg.FieldCode[0] = '4';
	TransAmountMsg.FieldCode[1] = '0';
	sprintf_s(slen, "%04d", 12);
	strTobcd(dest, slen);
	TransAmountMsg.len[0] = dest[0];
	TransAmountMsg.len[1] = dest[1];
	memset(TransAmountMsg.data, 0, 128);
	sprintf_s(slen, "%012d", amount_i);
	memcpy(TransAmountMsg.data, slen, 12);
	TransAmountMsg.sep[0] = 0x1C;
	MsgList.push_back(TransAmountMsg);
	long length = GetFuntionData(data, ECN, "30", "01", MsgList);
	gNETSLog.WriteStringToLog("Data sent:NETSDebit");
	gNETSLog.WriteHexToLog(data, length + 1);
	WriteData(data, length + 1);
	int looptimes = 0;
	int retrytimes = 2;
	while (1)
	{
		if (m_bRecvFlag == NACK_STATUS && retrytimes > 0)
		{
			m_bRecvFlag = INIT_STATUS;
			WriteData(data, length + 1);
			retrytimes--;
		}
		else if (m_bRecvFlag == ACK_STATUS || m_bRecvFlag == INIT_STATUS)
		{
			if (m_bRecvFlag == INIT_STATUS && looptimes >= 1 && retrytimes>=1)
				m_bRecvFlag == NACK_STATUS;
			Sleep(1000);
			looptimes++;
			if (looptimes >= 120)
				return TIME_OUT_STATUS;
		}
		else if(retrytimes == 0 && m_bRecvFlag == INIT_STATUS)
			return TIME_OUT_STATUS;
		else return m_bRecvFlag;
	}
	return m_bRecvFlag;
}

RECV_STATUS CSerialPort::TransactionNETSQRCode(char* ECN, float Amounts)
{
	MsgList.clear();
	m_curECN = ECN;
	m_bRecvFlag = INIT_STATUS;
	char data[255] = { 0 };
	int amount_i = Amounts * 100;
	MsgData Transmsg, loyaltymsg, TransAmountMsg;
	Transmsg.FieldCode[0] = 'T';
	Transmsg.FieldCode[1] = '2';
	char slen[16];
	sprintf_s(slen, "%04d", 2);
	unsigned char dest[2];
	strTobcd(dest, slen);
	Transmsg.len[0] = dest[0];
	Transmsg.len[1] = dest[1];
	Transmsg.data[0] = '0';
	Transmsg.data[1] = '4';
	Transmsg.sep[0] = 0x1c;
	MsgList.push_back(Transmsg);
	loyaltymsg.FieldCode[0] = '4';
	loyaltymsg.FieldCode[1] = '3';
	sprintf_s(slen, "%04d", 1);
	strTobcd(dest, slen);
	memset(loyaltymsg.len, 0, 128);
	memset(loyaltymsg.data, 0, 128);
	loyaltymsg.len[0] = dest[0];
	loyaltymsg.len[1] = dest[1];
	loyaltymsg.data[0] = '0';
	loyaltymsg.sep[0] = 0x1c;
	MsgList.push_back(loyaltymsg);
	TransAmountMsg.FieldCode[0] = '4';
	TransAmountMsg.FieldCode[1] = '0';
	sprintf_s(slen, "%04d", 12);
	strTobcd(dest, slen);
	TransAmountMsg.len[0] = dest[0];
	TransAmountMsg.len[1] = dest[1];
	memset(TransAmountMsg.data, 0, 128);
	sprintf_s(slen, "%012d", amount_i);
	memcpy(TransAmountMsg.data, slen, 12);
	TransAmountMsg.sep[0] = 0x1C;
	MsgList.push_back(TransAmountMsg);
	long length = GetFuntionData(data, ECN, "30", "01", MsgList);
	gNETSLog.WriteStringToLog("Data sent:NETSQRCode");
	gNETSLog.WriteHexToLog(data, length + 1);
	WriteData(data, length + 1);
	int looptimes = 0;
	int retrytimes = 2;
	while (1)
	{
		if (m_bRecvFlag == NACK_STATUS && retrytimes > 0)
		{
			m_bRecvFlag = INIT_STATUS;
			WriteData(data, length + 1);
			retrytimes--;
		}
		else if (m_bRecvFlag == ACK_STATUS || m_bRecvFlag == INIT_STATUS)
		{
			if (m_bRecvFlag == INIT_STATUS && looptimes >= 1 && retrytimes >= 1)
				m_bRecvFlag == NACK_STATUS;
			Sleep(1000);
			looptimes++;
			if (looptimes >= 120)
				return TIME_OUT_STATUS;
		}
		else if (retrytimes == 0 && m_bRecvFlag == INIT_STATUS)
			return TIME_OUT_STATUS;
		else return m_bRecvFlag;
	}
	return m_bRecvFlag;
}

RECV_STATUS CSerialPort::TransactionNETSFlashPay(char* ECN, float Amounts)
{
	MsgList.clear();
	m_curECN = ECN;
	m_bRecvFlag = INIT_STATUS;
	char data[255] = { 0 };
	int amount_i = Amounts * 100;
	MsgData msg;
	msg.FieldCode[0] = '4';
	msg.FieldCode[1] = '0';
	char slen[16];
	sprintf_s(slen, "%04d", 12);
	unsigned char dest[2];
	strTobcd(dest, slen);
	msg.len[0] = dest[0];
	msg.len[1] = dest[1];
	memset(msg.data, 0, 128);
	sprintf_s(slen, "%012d", amount_i);
	memcpy(msg.data, slen, 12);
	msg.sep[0] = 0x1c;
	MsgList.push_back(msg);
	long length = GetFuntionData(data, ECN, "24", "01", MsgList);
	gNETSLog.WriteStringToLog("Data sent:NETSFlashPay");
	gNETSLog.WriteHexToLog(data, length + 1);
	if (WriteData(data, length + 1))
	{
		printf("Write Data Successful");
	}
	int looptimes = 0;
	int retrytimes = 2;
	while (1)
	{
		if (m_bRecvFlag == NACK_STATUS && retrytimes > 0)
		{
			m_bRecvFlag = INIT_STATUS;
			WriteData(data, length + 1);
			retrytimes--;
		}
		else if (m_bRecvFlag == ACK_STATUS || m_bRecvFlag == INIT_STATUS)
		{
			if (m_bRecvFlag == INIT_STATUS && looptimes >= 1 && retrytimes >= 1)
				m_bRecvFlag == NACK_STATUS;
			Sleep(1000);
			looptimes++;
			if (looptimes >= 60)
				return TIME_OUT_STATUS;
		}
		else if (retrytimes == 0 && m_bRecvFlag == INIT_STATUS)
			return TIME_OUT_STATUS;
		else return m_bRecvFlag;
	}
	return m_bRecvFlag;
}
RECV_STATUS CSerialPort::GetLastApproved(char* ECN)
{
	char data[255] = { 0 };
	m_curECN = ECN;
	m_bRecvFlag = INIT_STATUS;
	long length = GetFuntionData(data, ECN, "56", "01", MsgList);
	gNETSLog.WriteStringToLog("Data sent:Logon");
	gNETSLog.WriteHexToLog(data, length + 1);
	if (WriteData(data, length + 1))
	{
		printf("Write Data Successful");
	}
	int looptimes = 0;
	int retrytimes = 2;
	while (1)
	{
		if (m_bRecvFlag == NACK_STATUS && retrytimes > 0)
		{
			m_bRecvFlag = INIT_STATUS;
			WriteData(data, length + 1);
			retrytimes--;
		}
		else if (m_bRecvFlag == ACK_STATUS || m_bRecvFlag == INIT_STATUS)
		{
			if (m_bRecvFlag == INIT_STATUS && looptimes >= 1 && retrytimes >= 1)
				m_bRecvFlag == NACK_STATUS;
			Sleep(1000);
			looptimes++;
			if (looptimes >= 60)
				return TIME_OUT_STATUS;
		}
		else if (retrytimes == 0 && m_bRecvFlag == INIT_STATUS)
			return TIME_OUT_STATUS;
		else
		{
			return m_bRecvFlag;
		}
	}
	return m_bRecvFlag;
}
RECV_STATUS CSerialPort::TMS()
{
	char data[255] = { 0 };
	m_bRecvFlag = INIT_STATUS;
	long length = GetFuntionData(data, "000000000001", "84", "01", MsgList);
	WriteData(data, length + 1);
	int looptimes = 0;
	int retrytimes = 2;
	while (1)
	{
		if (m_bRecvFlag == NACK_STATUS && retrytimes > 0)
		{
			m_bRecvFlag = INIT_STATUS;
			WriteData(data, length + 1);
			retrytimes--;
		}
		else if (m_bRecvFlag == ACK_STATUS || m_bRecvFlag == INIT_STATUS)
		{
			if (m_bRecvFlag == INIT_STATUS && looptimes >= 1 && retrytimes >= 1)
				m_bRecvFlag == NACK_STATUS;
			Sleep(1000);
			looptimes++;
			if (looptimes >= 60)
				return TIME_OUT_STATUS;
		}
		else if (retrytimes == 0 && m_bRecvFlag == INIT_STATUS)
			return TIME_OUT_STATUS;
		else
		{
			return m_bRecvFlag;
		}
	}
	return m_bRecvFlag;
}
