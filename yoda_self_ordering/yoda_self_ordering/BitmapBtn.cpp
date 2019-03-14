// BitmapBtn.cpp : ʵ���ļ�
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

// CBitmapBtn ��Ϣ�������

LRESULT CBitmapBtn::OnSetStyle ( WPARAM wParam, LPARAM lParam )
{
	return DefWindowProc ( BM_SETSTYLE,(wParam&~BS_TYPEMASK)|BS_OWNERDRAW, lParam );
}

//���ݰ�ť��ͬ״̬��Ӧ�Ĳ�ͬλͼ��Դ���ư�ť,û����Ӧ״̬��λͼ������״̬��λͼ����
void CBitmapBtn::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC *pdc;
	pdc=CDC::FromHandle (lpDrawItemStruct->hDC );
	UINT state=lpDrawItemStruct->itemState ;
	if(state&ODS_SELECTED)
	{
		if(this->m_ResIDDown)
			DrawIt(pdc,m_ResIDDown);//���ư�ť���µ�״̬
		else
			DrawIt(pdc,m_ResIDNorm);
	}
	else if(state&ODS_DISABLED)
	{
		if(this->m_ResIDForb)
			DrawIt(pdc,m_ResIDForb);//���ư�ť��ֹ��״̬
		else
			DrawIt(pdc,m_ResIDNorm);
	}
	else
	{
		if(m_over)
		{
			if(this->m_ResIDMove)
				DrawIt(pdc,m_ResIDMove);//��������ڰ�ť�ϵ�״̬
			else
				DrawIt(pdc,m_ResIDNorm);
		}
		else
		{
			if(this->m_ResIDNorm)
				DrawIt(pdc,m_ResIDNorm);//���ư�ť������״̬
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
		_TrackMouseEvent(&tme);//������뿪��ťʱ�������뿪��Ϣ
		CRect rect;
		GetClientRect(&rect);
		InvalidateRect(&rect);//�ػ水ť��ʹ��ť������������ϵ�״̬

		m_over=true;//Ϊ�˼��ٰ�ť�ػ�Ĵ���
	}
	CButton::OnMouseMove(nFlags, point);
}

LPARAM CBitmapBtn::onmouseleave (WPARAM wm,LPARAM lm)
{
	m_over=false;
	CRect rect;
	GetClientRect(&rect);
	InvalidateRect(&rect);//�ػ水ť��ʹ��ť����������״̬
	return 0;
}

// �����ķ�λͼ���ֱ��Ӧ��������״̬������״̬������������ƶ�״̬�ͽ�ֹ״̬
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
	InvalidateRect(&rect);//�ػ水ť

	CButton::OnLButtonUp(nFlags, point);
}

void CBitmapBtn::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect rect;
	GetClientRect(&rect);
	InvalidateRect(&rect);//�ػ水ť

	CButton::OnLButtonDown(nFlags, point);
}
