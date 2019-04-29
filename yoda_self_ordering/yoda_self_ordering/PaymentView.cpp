#include "stdafx.h"
#include "yoda_self_ordering.h"
#include "PaymentView.h"
#include "CashPop.h"
#include "QRCodeDlg.h"
#include "NetsGuideGif.h"
#include "PaymentStatusDlg.h"
#include "NETSLog.h"
#include "VoucherList.h"
//#include "FileUtils.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif
paymentModes payModes;
extern UINT	gCurViewID;
int gCurSerialNO;
bool gNETSLogon = false;
UINT gSerialPortComm = 42;
extern TRANS_STATUS gTransStatus;
//FileSession *gpFileSession = NULL;
extern 	CList<LPORDERINFO, LPORDERINFO>	glstOrder;
CString szHint;
int nRedrawCounts = 0;
int nPaymentTimers = 0;
#define TIMER_REDRAW					1001
#define TIMER_TEXT						1002
#define TIMER_NETS						1003
#define ID_VOUCHER_TEXT					151
#define ID_TEXT_PAYABLE					152
#define ID_TEXT_VOUCHER_PRICE			153
#define ID_TEXT_PAYABLE_PRICE			154
#define ID_TEXT_TOTAL					160
#define ID_TEXT_TIP						161
#define ID_TEXT_AMOUNT					163
#define ID_IMAGE_IPAY					170
#define ID_IMAGE_NETS					171
#define ID_IMAGE_NETS_FLASHPAY			172
#define ID_IMAGE_NETS_QR				173
#define ID_IMAGE_COUNTER				174
#define ID_IMAGE_VOUCHER				175
#define ID_IMAGE_FREE					176
IMPLEMENT_SERIAL(CPayContainerCtrl, CBCGPVisualContainerCtrl, 1)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPayContainerCtrl::CPayContainerCtrl()
{
	CBCGPVisualContainer* pContainer = GetVisualContainer();
	ASSERT_VALID(pContainer);
	CBCGPVisualScrollBarColorTheme theme(CBCGPColor::SteelBlue);
	m_bShow = TRUE;
	LOGFONT lf;
	globalData.fontRegular.GetLogFont(&lf);
	memcpy(lf.lfFaceName,L"Arial",5);
	lf.lfHeight *= 3;
	lf.lfWidth *= 3;
	m_brText.SetColor(CBCGPColor::LightGreen);
	m_textFormat.CreateFromLogFont(lf);
	m_textFormat.SetTextAlignment(CBCGPTextFormat::BCGP_TEXT_ALIGNMENT_CENTER);
	m_textFormat.SetTextVerticalAlignment(CBCGPTextFormat::BCGP_TEXT_ALIGNMENT_CENTER);
	//m_textFormat.SetWordWrap();
	gSerialPortComm = ::GetPrivateProfileInt(L"NETS", L"Port", 0, gIniFile);
	gNETSLogon = ::GetPrivateProfileInt(L"NETS", L"Logon", 1, gIniFile);
	m_nPayTryTimes = 0;
	gTransStatus = TRANS_INIT;
	m_bConnNETS = FALSE;
	m_dbDiscount = .0;
	m_dbPayable = .0;
	m_InvicodeID = -1;
}
CPayContainerCtrl::~CPayContainerCtrl()
{
}
BEGIN_MESSAGE_MAP(CPayContainerCtrl, CBCGPVisualContainerCtrl)
	//{{AFX_MSG_MAP(CPayContainerCtrl)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_MESSAGE(WM_VOUCHER_REDEEM,OnVoucherRedeem)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
int CPayContainerCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPVisualContainerCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	CBCGPVisualContainer* pContainer = GetVisualContainer();
	ASSERT_VALID(pContainer);
	return 0;
}

void CPayContainerCtrl::OnTimer(UINT_PTR nIDEvent)
{
	CBCGPVisualContainer* pContainer = GetVisualContainer();
	ASSERT_VALID(pContainer);
	if (nIDEvent == TIMER_NETS)
	{
		switch (m_NETS.m_bRecvFlag)
		{
			case TRANSACTION_ERROR_STATUS:
				{
				    gTransStatus = TRANS_INIT;
					USES_CONVERSION;
					CPaymentStatusDlg *pTD = new CPaymentStatusDlg(FALSE,A2W(m_NETS.m_ResponseMsg),this);
					pTD->Create(IDD_PAYMENT_STATUS);
					pTD->ShowWindow(SW_SHOWNORMAL);
					KillTimer(TIMER_NETS);
				}
				break;
			case TRANSACTION_COMPLATED_STATUS:
				{
					gTransStatus = TRANS_INIT;
					CPaymentStatusDlg *pTD = new CPaymentStatusDlg(TRUE);
					pTD->Create(IDD_PAYMENT_STATUS);
					pTD->ShowWindow(SW_SHOWNORMAL);
					KillTimer(TIMER_NETS);
					
				}
			break;
			case NO_RESPONSE_STATUS:
			{
				gTransStatus = TRANS_INIT;
				CPaymentStatusDlg *pTD = new CPaymentStatusDlg(FALSE,L"Terminal No Response.",this);
				pTD->Create(IDD_PAYMENT_STATUS);
				pTD->ShowWindow(SW_SHOWNORMAL);
				KillTimer(TIMER_NETS);
			}
			break;
			default:
				break;
		}
	}
	else if (nIDEvent == TIMER_REDRAW)
	{
		if (m_bShow)
		{
			if (gbChinese)
				SetTextInfo(ID_TEXT_TIP, L"请选择您的支付方式");
			else
				SetTextInfo(ID_TEXT_TIP, L"PLEASE SELECT PAYMENT");
		}
		else
		{
			SetTextInfo(ID_TEXT_TIP, L"");
		}
		m_bShow = !m_bShow;
	}
	else if (nIDEvent == TIMER_TEXT)
	{
		if (nRedrawCounts < 40)
			m_ptText.Offset(0,-0.1);
		nRedrawCounts++;
		if (nRedrawCounts >=40 || szHint.IsEmpty())
		{
			if (nRedrawCounts >= 50)
			{
				CRect rectClient;
				GetClientRect(rectClient);
				nRedrawCounts = 0;
				m_ptText = m_ptText = rectClient.CenterPoint();
				szHint = "";
				KillTimer(TIMER_TEXT);
			}
			else
			{
				switch (nRedrawCounts)
				{
				case 40:
					m_brText.SetOpacity(.9);
					break;
				case 41:
					m_brText.SetOpacity(.8);
					break;
				case 42:
					m_brText.SetOpacity(.7);
					break;
				case 43:
					m_brText.SetOpacity(.6);
					break;
				case 44:
					m_brText.SetOpacity(.5);
					break;
				case 45:
					m_brText.SetOpacity(.4);
					break;
				case 46:
					m_brText.SetOpacity(.3);
					break;
				case 47:
					m_brText.SetOpacity(.2);
					break;
				case 48:
					m_brText.SetOpacity(.1);
					break;
				case 49:
					m_brText.SetOpacity(0);
					break;
				default:
					break;
				}
			}
		}
		RedrawWindow();
	}
}

void CPayContainerCtrl::InitImage()
{
	SetImage(ID_IMAGE_IPAY, IDB_IPAY);
	SetImage(ID_IMAGE_NETS, m_bConnNETS ?IDB_NETS:IDB_NETS_DISABLE);
	SetImage(ID_IMAGE_COUNTER, IDB_COUNTER);
	SetImage(ID_IMAGE_NETS_FLASHPAY, m_bConnNETS ? IDB_NETS_FLASHPAY: IDB_NETS_FLASHPAY_DISABLE);
	SetImage(ID_IMAGE_NETS_QR, m_bConnNETS ? IDB_NETS_QR : IDB_NETS_QR_DISABLE);
	SetImage(ID_IMAGE_VOUCHER, IDB_VOUCHER);
	SetImage(ID_IMAGE_FREE, IDB_FREE_DISABLE);
}

void CPayContainerCtrl::SetImage(UINT nID, CString szImagePath)
{
	CBCGPVisualContainer* pContainer = GetVisualContainer();
	ASSERT_VALID(pContainer);
	CBCGPImageGaugeImpl* pImage = DYNAMIC_DOWNCAST(CBCGPImageGaugeImpl, pContainer->GetByID(nID));
	ASSERT_VALID(pImage);
	pImage->SetImage(CBCGPImage(szImagePath), CBCGPSize(pImage->GetRect().Width(), pImage->GetRect().Height()));
}

void CPayContainerCtrl::SetImage(UINT nID, UINT nImageID)
{
	CBCGPVisualContainer* pContainer = GetVisualContainer();
	ASSERT_VALID(pContainer);
	CBCGPImageGaugeImpl* pImage = DYNAMIC_DOWNCAST(CBCGPImageGaugeImpl, pContainer->GetByID(nID));
	ASSERT_VALID(pImage);
	pImage->SetImage(CBCGPImage(nImageID), CBCGPSize(pImage->GetRect().Width(), pImage->GetRect().Height()));
	pImage->SetRoundedShapes();
}

void CPayContainerCtrl::CommandInit()
{
	m_dbPayable = m_dbTotal = 0.;
	CBCGPVisualContainer* pContainer = GetVisualContainer();
	ASSERT_VALID(pContainer);
	m_bConnNETS = m_NETS.IsConn(gSerialPortComm);
	InitImage();
	for (POSITION pos = glstOrder.GetHeadPosition(); pos!=NULL;)
	{
		LPORDERINFO order = glstOrder.GetNext(pos);
		m_dbTotal += order->productInfo.dbMoney * order->nProductCounts;
	}
	m_dbPayable = m_dbTotal;
	CString szMoney;
	szMoney.Format(L"$%0.2lf", m_dbTotal);
	SetTextInfo(ID_TEXT_AMOUNT, szMoney);
	SetTextEnable(ID_VOUCHER_TEXT, FALSE);
	SetTextEnable(ID_TEXT_PAYABLE, FALSE);
	SetTextEnable(ID_TEXT_VOUCHER_PRICE, FALSE);
	SetTextEnable(ID_TEXT_PAYABLE_PRICE, FALSE);
	//SetTextInfo(ID_TEXT_TIP, L"PLEASE SELECT PAYMENT");
	SetTimer(TIMER_REDRAW,600,NULL);
}

void CPayContainerCtrl::SetTextInfo(UINT nID, CString szText)
{
	CBCGPVisualContainer* pContainer = GetVisualContainer();
	ASSERT_VALID(pContainer);
	CBCGPTextGaugeImpl* pText = DYNAMIC_DOWNCAST(CBCGPTextGaugeImpl, pContainer->GetByID(nID));
	ASSERT_VALID(pText);
	pText->SetText(szText);
	pText->Redraw();
}
void CPayContainerCtrl::SetTextEnable(UINT nID, BOOL bEnable)
{
	CBCGPVisualContainer* pContainer = GetVisualContainer();
	ASSERT_VALID(pContainer);
	CBCGPTextGaugeImpl* pText = DYNAMIC_DOWNCAST(CBCGPTextGaugeImpl, pContainer->GetByID(nID));
	ASSERT_VALID(pText);
	pText->SetVisible(bEnable);
}
DWORD WINAPI VOUCHER_THREAD(LPVOID lpParam)
{
	CPayContainerCtrl * pThis = (CPayContainerCtrl*)lpParam;
	CVoucherList dlg(pThis);
	dlg.DoModal();
	return 1;
}
DWORD WINAPI DRAW_GIF(LPVOID lpParam)
{
	CPayContainerCtrl * pThis = (CPayContainerCtrl*)lpParam;
	return 1;
}
DWORD WINAPI RECEIPT_THREAD(LPVOID lpParam)
{
	CPayContainerCtrl * pThis = (CPayContainerCtrl*)lpParam;
	nPaymentTimers = 0;
	while(1)
	{
		if (PrintReceipt(false))
		{
			TCHAR sBuf[255];
			CString szPrinter1,szPrinter2;
			::GetPrivateProfileString(L"PRINTER", L"Printer1", L"", sBuf, 255, gIniFile);
			szPrinter1 = sBuf;
			if(!szPrinter1.IsEmpty())
				printTicket(szPrinter1);
			::GetPrivateProfileString(L"PRINTER", L"Printer2", L"", sBuf, 255, gIniFile);
			szPrinter2 = sBuf;
			if(!szPrinter2.IsEmpty())
				printTicket(szPrinter2);
			break;
		}
		else
			Sleep(1000);
	}
	gCurSerialNO = 0;
	//::PostMessage(AfxGetApp()->m_pMainWnd->m_hWnd, WM_HOME_VIEW, 0, 0);
	return 1;
}
DWORD WINAPI ThreadNETSDebit(LPVOID lpParam)
{
	CPayContainerCtrl * pThis = (CPayContainerCtrl*)lpParam;
	if (gNETSLogon)
	{
		USES_CONVERSION;
		CString strDate, ECN, strSerialNO,strTryTimes,TransECN;
		CTime tm; tm = CTime::GetCurrentTime();
		strDate = tm.Format("%Y%m%d");
		strDate = strDate.Right(6);
		strSerialNO.Format(_T("%04d%02d"), gCurSerialNO, nPaymentTimers++);
		ECN = strDate + strSerialNO;
		if (!pThis->m_NETS.InitPort(gSerialPortComm))
		{
			gTransStatus = TRANS_INIT;
			return 0;
		}
		gTransStatus = TRANS_PROCESSING;
		TransECN = ECN;
		CString szLog;
		NETSLog log;
		szLog.Format(L"########\t %s  ######## ", TransECN);
		log.WriteStringToLog(W2A(szLog));
		int bResult = pThis->m_NETS.TransactionNETSDebit(W2A(ECN), pThis->m_dbPayable);
		if (bResult == TRANSACTION_COMPLATED_STATUS)
		{
			NETSLog log;
			log.WriteStringToLog("Transaction successful.");
			log.CloseLog();
			//success
			gTransStatus = TRANS_END;
			if (!SaveOrder(ORDER_PAID, pThis->m_InvicodeID))
			{
				CString szTip, szError;
				szTip.LoadStringW(IDS_SAVE_ORDER_ERROR);
				szError.LoadStringW(IDS_ERROR_CAPTION);
				MessageBox(pThis->m_hWnd, szTip, szError, 0);
				return 1;
			}
			CreateThread(NULL, 0, RECEIPT_THREAD, pThis, 0, NULL);
		}
		else
		{
			if (bResult == TRANSACTION_ERROR_STATUS)
			{
				NETSLog log;
				log.WriteStringToLog("Transaction failed.");
				log.CloseLog();
				gTransStatus = TRANS_END;
			}
			else if (bResult == NO_RESPONSE_STATUS)
			{
				gTransStatus = TRANS_END;
			}
			else if (bResult == TIME_OUT_STATUS)
			{
				NETSLog log;
				CString szLog;
				szLog = L"Transaction TimeOut & GetLastApproved:" + TransECN;
				log.WriteStringToLog(W2A(szLog));
				log.CloseLog();
				pThis->m_NETS.ClosePort();
				if (pThis->m_NETS.InitPort(gSerialPortComm))
				{
					//strSerialNO.Format(_T("%04d%02d"), gCurSerialNO, nPaymentTimers++);
					//ECN = strDate + strSerialNO;
					strcpy(pThis->m_NETS.m_lastApprovedECN, W2A(TransECN));
					pThis->m_NETS.GetLastApproved(W2A(ECN));
					
					if (pThis->m_NETS.m_bRecvFlag == TRANSACTION_COMPLATED_STATUS)
					{
						NETSLog log;
						log.WriteStringToLog("LastApproved Attach successful");
						log.CloseLog();
						//success
					//	pThis->m_flash->DestroyDisplayWindow();
						gTransStatus = TRANS_END;
						if (!SaveOrder(ORDER_PAID, pThis->m_InvicodeID))
						{
							CString szTip, szError;
							szTip.LoadStringW(IDS_SAVE_ORDER_ERROR);
							szError.LoadStringW(IDS_ERROR_CAPTION);
							MessageBox(pThis->m_hWnd, szTip, szError, 0);

							return 1;
						}
						CreateThread(NULL, 0, RECEIPT_THREAD, pThis, 0, NULL);
					//	CPaymentStatusDlg *dlg = new CPaymentStatusDlg(true);
					//	dlg->Create(IDD_PAYMENT_STATUS);
					//	dlg->ShowWindow(SW_SHOW);
						
					}
					else
					{
						NETSLog log;
						log.WriteStringToLog("LastApproved Attach failed");
						log.CloseLog();
						gTransStatus = TRANS_END;
						//pThis->m_flash->DestroyDisplayWindow();
						//CPaymentStatusDlg dlg(false);
						//dlg.DoModal();
					}

				}
			}
		}

	}
	else
	{
		CString szTip, szError;
		szTip.LoadStringW(IDS_SAVE_ORDER_ERROR);
		szError.LoadStringW(IDS_ERROR_CAPTION);
		MessageBox(pThis->m_hWnd, szTip, szError, 0);
		return 1;
	}
	pThis->m_NETS.ClosePort();
	return 1;
}
DWORD WINAPI ThreadNETSFlashPay(LPVOID lpParam)
{
	CPayContainerCtrl * pThis = (CPayContainerCtrl*)lpParam;
	if (gNETSLogon)
	{
		USES_CONVERSION;
		CString strDate, ECN, strSerialNO, strTryTimes, TransECN;
		CTime tm; tm = CTime::GetCurrentTime();
		strDate = tm.Format("%Y%m%d");
		strDate = strDate.Right(6);
		strSerialNO.Format(_T("%04d%02d"), gCurSerialNO, nPaymentTimers++);
		ECN = strDate + strSerialNO;
		if (!pThis->m_NETS.InitPort(gSerialPortComm))
		{
			gTransStatus = TRANS_INIT;
			return 0;
		}
		gTransStatus = TRANS_PROCESSING;
		TransECN = ECN;
		CString szLog;
		NETSLog log;
		
		szLog.Format(L"########\t %s  ######## ", TransECN);
		log.WriteStringToLog(W2A(szLog));
		int bResult = pThis->m_NETS.TransactionNETSFlashPay(W2A(ECN), pThis->m_dbPayable);
		if (bResult == TRANSACTION_COMPLATED_STATUS)
		{
			NETSLog log;
			log.WriteStringToLog("Transaction successful.");
			log.CloseLog();
			gTransStatus = TRANS_END;
			//success
			if (!SaveOrder(ORDER_PAID, pThis->m_InvicodeID))
			{
				CString szTip, szError;
				szTip.LoadStringW(IDS_SAVE_ORDER_ERROR);
				szError.LoadStringW(IDS_ERROR_CAPTION);
				MessageBox(pThis->m_hWnd, szTip, szError, 0);
				return 1;
			}
			CreateThread(NULL, 0, RECEIPT_THREAD, pThis, 0, NULL);
		}
		else
		{
		
			if (bResult == TRANSACTION_ERROR_STATUS)
			{
				NETSLog log;
				log.WriteStringToLog("Transaction failed");
				log.CloseLog();
				gTransStatus = TRANS_END;
			}
			else if (bResult == NO_RESPONSE_STATUS)
			{
				gTransStatus = TRANS_END;
			}
			else if (bResult == TIME_OUT_STATUS)
			{
				NETSLog log;
				CString szLog;
				szLog = L"Transaction TimeOut & GetLastApproved:" + TransECN;
				log.WriteStringToLog(W2A(szLog));
				log.CloseLog();
				pThis->m_NETS.ClosePort();
				if (pThis->m_NETS.InitPort(gSerialPortComm))
				{
					//strSerialNO.Format(_T("%04d%02d"), gCurSerialNO, nPaymentTimers++);
					//ECN = strDate + strSerialNO;
					
					strcpy(pThis->m_NETS.m_lastApprovedECN, W2A(TransECN));
					pThis->m_NETS.GetLastApproved(W2A(ECN));

					if (pThis->m_NETS.m_bRecvFlag == TRANSACTION_COMPLATED_STATUS)
					{
						NETSLog log;
						log.WriteStringToLog("LastApproved Attach successful");
						log.CloseLog();
						gTransStatus = TRANS_END;
						//success
						//	pThis->m_flash->DestroyDisplayWindow();
						if (!SaveOrder(ORDER_PAID, pThis->m_InvicodeID))
						{
							CString szTip, szError;
							szTip.LoadStringW(IDS_SAVE_ORDER_ERROR);
							szError.LoadStringW(IDS_ERROR_CAPTION);
							MessageBox(pThis->m_hWnd, szTip, szError, 0);
							return 1;
						}
						CreateThread(NULL, 0, RECEIPT_THREAD, pThis, 0, NULL);
						//	CPaymentStatusDlg *dlg = new CPaymentStatusDlg(true);
						//	dlg->Create(IDD_PAYMENT_STATUS);
						//	dlg->ShowWindow(SW_SHOW);

					}
					else
					{
						NETSLog log;
						log.WriteStringToLog("LastApproved Attach failed");
						log.CloseLog();
						gTransStatus = TRANS_END;
						//pThis->m_flash->DestroyDisplayWindow();
						//CPaymentStatusDlg dlg(false);
						//dlg.DoModal();
					}
				}
			}
			
		}
		
	}
	else
	{
		CString szTip, szError;
		szTip.LoadStringW(IDS_SAVE_ORDER_ERROR);
		szError.LoadStringW(IDS_ERROR_CAPTION);
		MessageBox(pThis->m_hWnd, szTip, szError, 0);
		return 1;
	}
	
	pThis->m_NETS.ClosePort();
	return 1;
}
DWORD WINAPI ThreadNETSQRCode(LPVOID lpParam)
{
	CPayContainerCtrl * pThis = (CPayContainerCtrl*)lpParam;
	if (gNETSLogon )
	{
		USES_CONVERSION;
		CString strDate, ECN, strSerialNO, strTryTimes, TransECN;
		CTime tm; tm = CTime::GetCurrentTime();
		strDate = tm.Format("%Y%m%d");
		strDate = strDate.Right(6);
		strSerialNO.Format(_T("%04d%02d"), gCurSerialNO, nPaymentTimers++);
		ECN = strDate + strSerialNO;
		if (!pThis->m_NETS.InitPort(gSerialPortComm))
		{
			gTransStatus = TRANS_INIT;
			return 0;
		}
		gTransStatus = TRANS_PROCESSING;
		TransECN = ECN;
		CString szLog;
		NETSLog log;
		szLog.Format(L"########\t %s  ######## ", TransECN);
		log.WriteStringToLog(W2A(szLog));
		int bResult = pThis->m_NETS.TransactionNETSQRCode(W2A(ECN), pThis->m_dbPayable);
		if (bResult == TRANSACTION_COMPLATED_STATUS)
		{
			NETSLog log;
			log.WriteStringToLog("Transaction successful.");
			log.CloseLog();
			//success
			gTransStatus = TRANS_END;
			if (!SaveOrder(ORDER_PAID, pThis->m_InvicodeID))
			{
				CString szTip, szError;
				szTip.LoadStringW(IDS_SAVE_ORDER_ERROR);
				szError.LoadStringW(IDS_ERROR_CAPTION);
				MessageBox(pThis->m_hWnd, szTip, szError, 0);
				return 1;
			}
			CreateThread(NULL, 0, RECEIPT_THREAD, pThis, 0, NULL);
		}
		else
		{
			if (bResult == TRANSACTION_ERROR_STATUS)
			{
				NETSLog log;
				log.WriteStringToLog("Transaction failed");
				log.CloseLog();
				gTransStatus = TRANS_END;
			}
			else if (bResult == NO_RESPONSE_STATUS)
			{
				gTransStatus = TRANS_END;
			}
			else if (bResult == TIME_OUT_STATUS)
			{
				NETSLog log;
				CString szLog;
				szLog = L"Transaction TimeOut & GetLastApproved:" + TransECN;
				log.WriteStringToLog(W2A(szLog));
				log.CloseLog();
				pThis->m_NETS.ClosePort();
				if (pThis->m_NETS.InitPort(gSerialPortComm))
				{
					//strSerialNO.Format(_T("%04d%02d"), gCurSerialNO, nPaymentTimers++);
					//ECN = strDate + strSerialNO;
					strcpy(pThis->m_NETS.m_lastApprovedECN, W2A(TransECN));

					pThis->m_NETS.GetLastApproved(W2A(ECN));

					if (pThis->m_NETS.m_bRecvFlag == TRANSACTION_COMPLATED_STATUS)
					{
						NETSLog log;
						log.WriteStringToLog("LastApproved Attach successful");
						log.CloseLog();
						gTransStatus = TRANS_END;
						//success
						//	pThis->m_flash->DestroyDisplayWindow();
						if (!SaveOrder(ORDER_PAID, pThis->m_InvicodeID))
						{
							CString szTip, szError;
							szTip.LoadStringW(IDS_SAVE_ORDER_ERROR);
							szError.LoadStringW(IDS_ERROR_CAPTION);
							MessageBox(pThis->m_hWnd, szTip, szError, 0);
							return 1;
						}
						CreateThread(NULL, 0, RECEIPT_THREAD, pThis, 0, NULL);
						//	CPaymentStatusDlg *dlg = new CPaymentStatusDlg(true);
						//	dlg->Create(IDD_PAYMENT_STATUS);
						//	dlg->ShowWindow(SW_SHOW);
					}
					else
					{
						NETSLog log;
						log.WriteStringToLog("LastApproved Attach failed");
						log.CloseLog();
						gTransStatus = TRANS_END;
						//pThis->m_flash->DestroyDisplayWindow();
						//CPaymentStatusDlg dlg(false);
						//dlg.DoModal();
					}
					
				}
			}
			
		}
	}
	else
	{
		CString szTip, szError;
		szTip.LoadStringW(IDS_SAVE_ORDER_ERROR);
		szError.LoadStringW(IDS_ERROR_CAPTION);
		MessageBox(pThis->m_hWnd, szTip, szError, 0);
		return 1;
	}
	pThis->m_NETS.ClosePort();
	return 1;
}
BOOL CPayContainerCtrl::OnMouseDown(int nButton, const CBCGPPoint& pt)
{
	USES_CONVERSION;
	CBCGPVisualContainer* pContainer = GetVisualContainer();
	ASSERT_VALID(pContainer);
	if (pContainer != NULL && !pContainer->IsSingleSel())
	{
		CBCGPBaseVisualObject* pObject = pContainer->GetFromPoint(pt);
		CBCGPImageGaugeImpl* pImage = DYNAMIC_DOWNCAST(CBCGPImageGaugeImpl, pObject);
		if (pImage != NULL && pImage->GetID() == ID_IMAGE_VOUCHER)
		{
			CreateThread(NULL, 0, VOUCHER_THREAD, this, 0, NULL);
		}
		if (pImage != NULL && m_dbPayable >0.0 && pImage->GetID() == ID_IMAGE_IPAY)
		{
			payModes = EWALLET;
			CString szText;
			szText.Format(L"%d", gCurSerialNO);
			if (GenerateQRCode(szText))
			{
				CQRCodeDlg *pTD = new CQRCodeDlg(this);
				pTD->Create(IDD_SHOW_QRCODE); 
				pTD->ShowWindow(SW_SHOWNORMAL);
			}
		}
		else if (m_bConnNETS && m_dbPayable >0.0 && pImage != NULL && pImage->GetID() == ID_IMAGE_NETS)
		{	
			gTransStatus = TRANS_START;
			CNETSGuideGif *gif = new CNETSGuideGif(IDR_GIF_NETSDEBIT);
			gif->Create(IDD_DIALOG_GIF);
			gif->ShowWindow(SW_SHOW);
			CreateThread(NULL, 0, ThreadNETSDebit, this, 0, NULL);
			SetTimer(TIMER_NETS, 1000, NULL);
			payModes = NETS;
		}
		else if (m_bConnNETS && m_dbPayable >0.0 && pImage != NULL && pImage->GetID() == ID_IMAGE_NETS_FLASHPAY)
		{
			gTransStatus = TRANS_START;
			CNETSGuideGif *gif = new CNETSGuideGif(IDR_GIF_NETSFLASHPAY);
			gif->Create(IDD_DIALOG_GIF);
			gif->ShowWindow(SW_SHOW);
			//CreateThread(NULL, 0, DRAW_GIF, this, 0, NULL);
			CreateThread(NULL, 0, ThreadNETSFlashPay, this, 0, NULL);	
			SetTimer(TIMER_NETS, 1000, NULL);
			payModes = NETS;
		}
		else if (m_bConnNETS && m_dbPayable >0.0 && pImage != NULL && pImage->GetID() == ID_IMAGE_NETS_QR)
		{
			payModes = NETS;
			gTransStatus = TRANS_START;
			CNETSGuideGif *gif = new CNETSGuideGif(IDR_GIF_NETSQRCODE);
			gif->Create(IDD_DIALOG_GIF);
			gif->ShowWindow(SW_SHOW);
			//CreateThread(NULL, 0, DRAW_GIF, this, 0, NULL);
			CreateThread(NULL, 0, ThreadNETSQRCode, this, 0, NULL);
			SetTimer(TIMER_NETS, 1000, NULL);
			/*if (!szHint.IsEmpty())
			{
				CRect rectClient;
				GetClientRect(rectClient);
				nRedrawCounts = 0;
				m_ptText = m_ptText = rectClient.CenterPoint();
				szHint = "Hello world!";
			}
			else
				szHint = "Hello world!";
			SetTimer(TIMER_TEXT, 50, NULL);*/
		}
		else if (m_dbPayable >0.0 && pImage != NULL && pImage->GetID() == ID_IMAGE_COUNTER)
		{
			payModes = UNKNOW;
			if (SaveOrder(ORDER_COUNTER))
			{
				CreateThread(NULL, 0, RECEIPT_THREAD, this, 0, NULL);
				CCashPop *pCash = new CCashPop();
				pCash->Create(IDD_CASH_POP, this);
				pCash->ShowWindow(SW_SHOWNORMAL);
				pCash->CenterWindow();
				/*CBCGPPopupWindow* pPopup = new CBCGPPopupWindow;
				CBCGPPopupWindowColors customColors;
				customColors.clrFill = RGB(103, 109, 134);
				customColors.clrText = RGB(255, 255, 255);
				customColors.clrBorder = RGB(103, 109, 134);
				customColors.clrLink = RGB(0, 255, 0);
				customColors.clrHoverLink = RGB(254, 233, 148);
				pPopup->SetCustomTheme(customColors);
				pPopup->SetAnimationType(
					CBCGPPopupMenu::SYSTEM_DEFAULT_ANIMATION);
				pPopup->SetTransparency((BYTE)255);
				pPopup->SetSmallCaption(FALSE);
				pPopup->SetSmallCaptionGripper(FALSE);
				pPopup->SetAutoCloseTime(5000);
				pPopup->SetRoundedCorners();
				pPopup->SetShadow();
				pPopup->EnablePinButton(FALSE);
				CPoint ptLocation(-1, -1);
				ptLocation.x = GetSystemMetrics(SM_CYSCREEN) / 2;
				ptLocation.y = GetSystemMetrics(SM_CXSCREEN) / 2;
				pPopup->EnableCloseButton(FALSE);
				pPopup->Create(this, IDD_CASH_POP,
					NULL, ptLocation, RUNTIME_CLASS(CCashPop));
				pPopup->CenterWindow();*/
			}
		}
		else if (m_dbPayable <= 0.0 && pImage != NULL && pImage->GetID() == ID_IMAGE_FREE)
		{
			if (SaveOrder(ORDER_COUNTER,m_InvicodeID))
			{
				CPaymentStatusDlg *pTD = new CPaymentStatusDlg(TRUE);
				pTD->Create(IDD_PAYMENT_STATUS);
				pTD->ShowWindow(SW_SHOWNORMAL);
				CreateThread(NULL, 0, RECEIPT_THREAD, this, 0, NULL);
			}
			

		}
	}
	return FALSE;
}

void CPayContainerCtrl::OnDraw(CBCGPGraphicsManager * pGM, const CBCGPRect & rectClip)
{
	CBCGPVisualContainer* pVisualContainer = GetVisualContainer();
	if (pVisualContainer != NULL)
	{
		pVisualContainer->OnDraw(pGM, rectClip);
	}
	if (!szHint.IsEmpty())
	{
		CRect rectClient;
		GetClientRect(rectClient);

		const double dblTextWidth = 500;

		if (m_sizeText.IsEmpty())
		{
			m_sizeText = m_pGM->GetTextSize(szHint, m_textFormat, dblTextWidth);
			m_ptText = rectClient.CenterPoint();
			m_sizeText.cx = dblTextWidth + 10;
		}
		CBCGPRect rectText(CBCGPPoint(m_ptText.x - .5 * m_sizeText.cx, m_ptText.y - .5 * m_sizeText.cy), m_sizeText);
		m_brText.SetPenAttributes(4, PS_SOLID);
		pGM->DrawText(szHint, rectText, m_textFormat, m_brText);
	}
}

LRESULT CPayContainerCtrl::OnVoucherRedeem(WPARAM wParam, LPARAM lpParam)
{
	m_InvicodeID = lpParam;
	m_dbDiscount = RedeemVoucherIncomePrice(m_InvicodeID, m_dbTotal);
	CString szDiscount,szPayable;
	szDiscount.Format(L"-$%0.2lf", m_dbDiscount);
	m_dbPayable = m_dbTotal - m_dbDiscount;
	if (m_dbPayable <= 0.000001)
		m_dbPayable = 0.0;
	szPayable.Format(L"$%0.2lf", m_dbPayable);
	SetTextInfo(ID_TEXT_VOUCHER_PRICE, szDiscount);
	SetTextInfo(ID_TEXT_PAYABLE_PRICE, szPayable);
	SetTextEnable(ID_VOUCHER_TEXT, TRUE);
	SetTextEnable(ID_TEXT_PAYABLE, TRUE);
	SetTextEnable(ID_TEXT_VOUCHER_PRICE, TRUE);
	SetTextEnable(ID_TEXT_PAYABLE_PRICE, TRUE);
	if (m_dbPayable <= 0.0)
	{

		SetImage(ID_IMAGE_NETS, IDB_NETS_DISABLE);
		SetImage(ID_IMAGE_NETS_FLASHPAY, IDB_NETS_FLASHPAY_DISABLE);
		SetImage(ID_IMAGE_NETS_QR,IDB_NETS_QR_DISABLE);
		SetImage(ID_IMAGE_IPAY,IDB_IPAY_DISABLE);
		SetImage(ID_IMAGE_COUNTER,IDB_COUNTER_DISABLE);
		SetImage(ID_IMAGE_FREE, IDB_FREE);
	}
	RedrawWindow();
	return 0;
}