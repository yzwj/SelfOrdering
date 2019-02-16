// Float.cpp : implementation file
//

#include "stdafx.h"
#include "yoda_self_ordering.h"
#include "Float.h"
#include <locale.h>
// CFloat dialog
IMPLEMENT_DYNAMIC(CFloat, CDialog)

CFloat::CFloat(CWnd* pParent /*=NULL*/)
	: CDialog(CFloat::IDD, pParent)
{

}

CFloat::~CFloat()
{
}

void CFloat::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
void CFloat::ShowFloatWindow(CWnd* pParentWnd)
{
	if(c_pFloat == NULL)
	{
		c_pFloat =  new CFloat;
		if (!c_pFloat->Create(CFloat::IDD, pParentWnd))
		{
			delete c_pFloat;
		}
		else
		{
			c_pFloat->ShowWindow(SW_SHOW);
			c_pFloat->UpdateWindow();
		}
	}
	else
	{
		c_pFloat->ShowWindow(SW_SHOW);
		c_pFloat->UpdateWindow();
	}
}

void CFloat::HideFloatWindow()
{
	if(c_pFloat != NULL)
	{	
		c_pFloat->ShowWindow(SW_HIDE);
		c_pFloat->UpdateWindow();
	}
}

BEGIN_MESSAGE_MAP(CFloat, CDialog)
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_NCHITTEST()
	ON_WM_NCRBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_NCLBUTTONDBLCLK()
	ON_WM_NCLBUTTONUP()
END_MESSAGE_MAP()

HBITMAP KCreateCompatibleBitmap(int width, int height)
{
	BYTE                      * pBits;
	BITMAPINFOHEADER          bmih;
	ZeroMemory(&bmih, sizeof(BITMAPINFOHEADER));

	bmih.biSize = sizeof(BITMAPINFOHEADER);
	bmih.biWidth = width;
	bmih.biHeight = height;
	bmih.biPlanes = 1;
	bmih.biBitCount = 32;        //这里一定要是32    
	bmih.biCompression = BI_RGB;
	bmih.biSizeImage = 0;
	bmih.biXPelsPerMeter = 0;
	bmih.biYPelsPerMeter = 0;
	bmih.biClrUsed = 0;
	bmih.biClrImportant = 0;

	HBITMAP hBitMap = CreateDIBSection(NULL, (BITMAPINFO *)&bmih, 0, (VOID**)&pBits, NULL, 0);
	return hBitMap;
}
// CFloat message handlers
extern BLENDFUNCTION gBlendFun;
typedef BOOL (WINAPI *FunTUpdateLayeredWindow)(HWND,HDC,POINT*,SIZE*,HDC,POINT*,COLORREF,BLENDFUNCTION*,DWORD);          
BOOL CFloat::OnInitDialog()
{
	CDialog::OnInitDialog();
	int x=GetSystemMetrics(SM_CXSCREEN);
	CRect rt;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rt,0);
	int y = rt.Height();
	//int y=GetSystemMetrics(SM_CYSCREEN);
	SIZE startSize={x/3 , (4*x)/15};

	//load g_pfnUpdateLayeredWindow DLL
	FunTUpdateLayeredWindow g_pfnUpdateLayeredWindow=NULL;
	if (g_pfnUpdateLayeredWindow == NULL) {
		HINSTANCE hFuncInst = LoadLibrary(_T("User32.DLL")); 
		BOOL bRet=FALSE;
		if(hFuncInst) 
			g_pfnUpdateLayeredWindow = (FunTUpdateLayeredWindow)GetProcAddress(hFuncInst, "UpdateLayeredWindow");
		else
			return false;
	}

	SetWindowLong(this->m_hWnd, GWL_EXSTYLE, GetWindowLong(this->m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);

	BLENDFUNCTION gBlendFun = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

	CDC* dc=GetDC();
	CDC cmemDC;
	cmemDC.CreateCompatibleDC(dc);

	HBITMAP hbmp;
	hbmp = KCreateCompatibleBitmap(startSize.cx , startSize.cy);
	CBitmap*   pcBmp   =   CBitmap::FromHandle(hbmp); 
	CBitmap* pOldBitmap = cmemDC.SelectObject(pcBmp);

	Graphics startGraphics(cmemDC);
	SolidBrush solidBrush(Color(0, 0, 0, 0));
	
	Bitmap pngImage(gExePath+L"cart.png");
	int height =pngImage.GetHeight();
	int width = pngImage.GetWidth();
	startGraphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);
	startGraphics.FillRectangle(&solidBrush, 0, 0, width , height);
	//startGraphics.DrawImage(&pngImage,Rect(0, 0, width, height), 0, 0, pngImage.GetWidth(), pngImage.GetHeight(), UnitPixel);
	SolidBrush sBrush(RGB(255,0,0));
	static REAL const fontWeightOfDigit = 5.5f;
	static WCHAR const * fontFamilyName = L"Arial";
	static Color const fontColorOfDigit(255, 0, 0, 0);
	Font fontDigit(fontFamilyName, fontWeightOfDigit, 10, 10, NULL);
	startGraphics.DrawString(L"Hello World!!!", 20, &fontDigit, PointF(100, 200), &sBrush);
	//startGraphics.DrawString("Hello World!!!",20,)
	SetWindowPos(&CWnd::wndTopMost, x-width*2, y/5, startSize.cx, startSize.cy,
		0/*SWP_NOSIZE|SWP_NOMOVE*/); //要使该函数能成功设置尺寸和位置，不能使用参数 SWP_NOSIZE|SWP_NOMOVE
	HDC   hWinDC   =   ::GetDC(NULL);  
	CRect rc;
	GetWindowRect(&rc);
	POINT pt={ rc.left, rc.top };
	SIZE size = {startSize.cx,startSize.cy};
	POINT ptBmp={ 0,0 };
	g_pfnUpdateLayeredWindow(this->m_hWnd, hWinDC,&pt ,&size , cmemDC, &ptBmp, 0, &gBlendFun ,ULW_ALPHA);

	ReleaseDC(dc);
	::ReleaseDC(this->m_hWnd,hWinDC);
	cmemDC.DeleteDC();
	DeleteObject(hbmp);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
BOOL CFloat::PreTranslateMessage(MSG* pMsg)
{
	if (c_pFloat == NULL)
	{
		return false;
	}
	return false;
}

BOOL CFloat::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	GetClientRect(rect); 

	CDC   *pMemDC=new   CDC; 
	CBitmap   bmpTitle; 
	CBitmap   *poldBitmap; 
	BITMAP   bmpTemp; 

	if   (pMemDC) 
	{ 
		bmpTitle.LoadBitmap(IDB_SPLASH); 
		bmpTitle.GetObject(sizeof(BITMAP),&bmpTemp); 
		pMemDC-> CreateCompatibleDC(pDC); 
		poldBitmap=pMemDC-> SelectObject(&bmpTitle); 
		pDC->StretchBlt(0,0,rect.right,rect.bottom,pMemDC,0,0,bmpTemp.bmWidth,bmpTemp.bmHeight,SRCCOPY); 
		pMemDC-> SelectObject(poldBitmap); 
		bmpTitle.DeleteObject();
		delete   pMemDC; 
	}
	return true;
	//return CDialog::OnEraseBkgnd(pDC);
}

LRESULT CFloat::OnNcHitTest(CPoint pt)
{
	LRESULT nHitTest = CDialog::OnNcHitTest(pt);
	if(nHitTest == HTCLIENT)
	{
		nHitTest = HTCAPTION;
	}
	return nHitTest;
}
void CFloat::OnNcRButtonDown(UINT nHitTest, CPoint point)
{
	CDialog::OnNcRButtonDown(nHitTest, point);
}

void CFloat::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnRButtonDown(nFlags, point);
}
void CFloat::ShowImage(CString szImageDir)
{
	
}
void CFloat::OnNcLButtonDblClk(UINT nHitTest, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnNcLButtonDblClk(nHitTest, point);
}


void CFloat::OnNcLButtonUp(UINT nHitTest, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	::PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLICK_CART, 0, 0);
	CDialog::OnNcLButtonUp(nHitTest, point);
}
