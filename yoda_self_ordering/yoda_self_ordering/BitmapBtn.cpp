// BitmapBtn.cpp : 实现文件
//

#include "stdafx.h"
#include "BitmapBtn.h"

// CBitmapBtn

IMPLEMENT_DYNAMIC(CBitmapBtn, CButton)

CBitmapBtn::CBitmapBtn()
{
    m_over=false;
	m_ResIDDown=0;
	m_ResIDForb=0;
	m_ResIDMove=0;
	m_ResIDNorm=0;
}

CBitmapBtn::~CBitmapBtn()
{
}

BEGIN_MESSAGE_MAP(CBitmapBtn, CButton)
	ON_MESSAGE ( BM_SETSTYLE, OnSetStyle )
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE,onmouseleave)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CBitmapBtn 消息处理程序

LRESULT CBitmapBtn::OnSetStyle ( WPARAM wParam, LPARAM lParam )
{
	return DefWindowProc ( BM_SETSTYLE,(wParam&~BS_TYPEMASK)|BS_OWNERDRAW, lParam );
}

//根据按钮不同状态对应的不同位图资源绘制按钮,没有相应状态的位图就正常状态的位图代替
void CBitmapBtn::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC *pdc;
	pdc=CDC::FromHandle (lpDrawItemStruct->hDC );
	UINT state=lpDrawItemStruct->itemState ;
	if(state&ODS_SELECTED)
	{
		if(this->m_ResIDDown)
			DrawIt(pdc,m_ResIDDown);//绘制按钮按下的状态
		else
			DrawIt(pdc,m_ResIDNorm);
	}
	else if(state&ODS_DISABLED)
	{
		if(this->m_ResIDForb)
			DrawIt(pdc,m_ResIDForb);//绘制按钮禁止的状态
		else
			DrawIt(pdc,m_ResIDNorm);
	}
	else
	{
		if(m_over)
		{
			if(this->m_ResIDMove)
				DrawIt(pdc,m_ResIDMove);//绘制鼠标在按钮上的状态
			else
				DrawIt(pdc,m_ResIDNorm);
		}
		else
		{
			if(this->m_ResIDNorm)
				DrawIt(pdc,m_ResIDNorm);//绘制按钮正常的状态
		}
	}
}

bool CBitmapBtn::DrawIt (CDC *pdc,UINT ResID)
{   
	CRect rect;
	GetClientRect(&rect);

	CDC dc;
	dc.CreateCompatibleDC (pdc);
	CBitmap bmp;
	bmp.LoadBitmap(ResID);
	BITMAP bm;
	bmp.GetBitmap(&bm);
	dc.SelectObject (&bmp);

    //pdc->BitBlt (0,0,rect.Width(),rect.Height(),&dc,0,0,SRCCOPY);
	pdc->TransparentBlt(0,0,rect.Width(),rect.Height(),&dc,0,0,bm.bmWidth,bm.bmHeight,RGB(255,255,255));
	DeleteObject(&pdc);
	return 0;
}
void CBitmapBtn::OnMouseMove(UINT nFlags, CPoint point)
{
	if(!m_over)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize =sizeof(tme);
		tme.hwndTrack =m_hWnd;
		tme.dwFlags =TME_LEAVE;
		tme.dwHoverTime =1;
		_TrackMouseEvent(&tme);//当鼠标离开按钮时，发送离开消息
		CRect rect;
		GetClientRect(&rect);
		InvalidateRect(&rect);//重绘按钮，使按钮处于鼠标在其上的状态

		m_over=true;//为了减少按钮重绘的次数
	}
	CButton::OnMouseMove(nFlags, point);
}

LPARAM CBitmapBtn::onmouseleave (WPARAM wm,LPARAM lm)
{
	m_over=false;
	CRect rect;
	GetClientRect(&rect);
	InvalidateRect(&rect);//重绘按钮，使按钮处于正常的状态
	return 0;
}

// 加载四幅位图，分别对应的是正常状态、按下状态、鼠标在其上移动状态和禁止状态
void CBitmapBtn::LoadFourBitmaps(unsigned int ResIDNorm,unsigned int ResIDDown, unsigned int ResIDMove,unsigned int ResIDForb)
{
	m_ResIDDown=ResIDDown;
	m_ResIDForb=ResIDForb;
	m_ResIDMove=ResIDMove;
	m_ResIDNorm=ResIDNorm;
}

void CBitmapBtn::OnLButtonUp(UINT nFlags, CPoint point)
{
	CRect rect;
	GetClientRect(&rect);
	InvalidateRect(&rect);//重绘按钮

	CButton::OnLButtonUp(nFlags, point);
}

void CBitmapBtn::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect rect;
	GetClientRect(&rect);
	InvalidateRect(&rect);//重绘按钮

	CButton::OnLButtonDown(nFlags, point);
}
