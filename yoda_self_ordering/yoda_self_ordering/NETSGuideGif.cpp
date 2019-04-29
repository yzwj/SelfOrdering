// NETSGuideGif.cpp : implementation file
//

#include "stdafx.h"
#include "yoda_self_ordering.h"
#include "NETSGuideGif.h"
TRANS_STATUS gTransStatus;

// CNETSGuideGif dialog

IMPLEMENT_DYNAMIC(CNETSGuideGif, CDialogEx)

CNETSGuideGif::CNETSGuideGif(CWnd* pParent /*=NULL*/)
	:CDialogEx(CNETSGuideGif::IDD, pParent)
{
	//mFilterGraph = NULL;
}

CNETSGuideGif::CNETSGuideGif(int nResID, CWnd * pParent)
{
	m_nResID = nResID;
}

CNETSGuideGif::~CNETSGuideGif()
{
	//DestroyGraph();
}

void CNETSGuideGif::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNETSGuideGif, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CNETSGuideGif message handlers


BOOL CNETSGuideGif::OnInitDialog()
{
	CDialogEx::OnInitDialog();
#ifndef DEBUG
	SetWindowPos(&wndTopMost, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_SHOWWINDOW);
#else
	SetWindowPos(NULL, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_SHOWWINDOW);
#endif // !DEBUG
	SetTimer(1001, 100, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CNETSGuideGif::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	CDialogEx::OnClose();
}

BOOL CNETSGuideGif::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return CDialogEx::OnEraseBkgnd(pDC);
}
void CNETSGuideGif::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CDialog::OnPaint() for painting messages
	if (gTransStatus == TRANS_START || gTransStatus == TRANS_PROCESSING)
	{
		HRSRC hResource = ::FindResource(AfxGetInstanceHandle(), MAKEINTRESOURCE(m_nResID), _T("GIF"));
		if (!hResource)
			return;

		DWORD imageSize = ::SizeofResource(AfxGetInstanceHandle(), hResource);
		if (!imageSize)
			return;

		const void* pResourceData = ::LockResource(::LoadResource(AfxGetInstanceHandle(), hResource));
		if (!pResourceData)
			return;

		HGLOBAL hBuffer = ::GlobalAlloc(GMEM_FIXED, imageSize);
		if (NULL == hBuffer)
			return;

		void* pBuffer = ::GlobalLock(hBuffer);
		if (pBuffer)
		{
			CopyMemory(pBuffer, pResourceData, imageSize);
			IStream* pStream = NULL;
			if (::CreateStreamOnHGlobal(hBuffer, FALSE, &pStream) == S_OK)
			{
				Image* image = Image::FromStream(pStream);
				pStream->Release();
				//获得有多少个维度，对于gif就一个维度
				UINT count = image->GetFrameDimensionsCount();
				GUID *pDimensionIDs = (GUID*)new GUID[count];
				image->GetFrameDimensionsList(pDimensionIDs, count);
				TCHAR strGuid[39];
				StringFromGUID2(pDimensionIDs[0], strGuid, 39);
				UINT frameCount = image->GetFrameCount(&pDimensionIDs[0]);
				delete[]pDimensionIDs;
				//获得各帧之间的时间间隔
				//先获得有多少个时间间隔，PropertyTagFrameDelay是GDI+中预定义的一个GIG属性ID值，表示标签帧数据的延迟时间
				UINT   FrameDelayNums = image->GetPropertyItemSize(PropertyTagFrameDelay);
				PropertyItem *  lpPropertyItem = new  PropertyItem[FrameDelayNums];
				image->GetPropertyItem(PropertyTagFrameDelay, FrameDelayNums, lpPropertyItem);
				//Guid的值在显示GIF为FrameDimensionTime，显示TIF时为FrameDimensionPage
				int    FrameCount = 0;
				GUID    Guid = FrameDimensionTime;
				image->SelectActiveFrame(&Guid, FrameCount);
				while (gTransStatus == TRANS_PROCESSING || gTransStatus == TRANS_START)
				{
					Graphics   gh(dc);
					CRect rc;
					GetWindowRect(&rc);
					int XSpace = rc.Width() - image->GetWidth();
					int YSpace = rc.Height() - image->GetHeight();
					//显示当前帧
					gh.DrawImage(image, XSpace / 2, YSpace / 2, image->GetWidth(), image->GetHeight());

					//时间间隔
					long lPause = ((long*)lpPropertyItem->value)[FrameCount] * 10;
					Sleep(lPause);

					//设置当前需要显示的帧数
					if ((FrameCount + 1) == frameCount)
					{   //如果已经显示到最后一帧，那么重新开始显示
						FrameCount = 0;
						image->SelectActiveFrame(&Guid, 0);
					}
					else
					{
						image->SelectActiveFrame(&Guid, ++FrameCount);
					}
				}
			}
		}
	}
}
void CNETSGuideGif::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here
}


void CNETSGuideGif::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 1001 && (gTransStatus == TRANS_END || gTransStatus == TRANS_INIT))
	{
		KillTimer(nIDEvent);
		SendMessage(WM_CLOSE);
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CNETSGuideGif::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	CDialogEx::PostNcDestroy();
	delete this;
}
