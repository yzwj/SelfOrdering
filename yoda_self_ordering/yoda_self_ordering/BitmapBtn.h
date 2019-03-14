#pragma once

// CBitmapBtn

class CBitmapBtn : public CButton
{
	DECLARE_DYNAMIC(CBitmapBtn)

public:
	CBitmapBtn();
	virtual ~CBitmapBtn();
    bool m_over;//��ʾ����Ƿ�������
	UINT m_ResIDDown;//��ʾ��ť����״̬λͼ��Դ
	UINT m_ResIDForb;//��ʾ��ť��ֹ״̬λͼ��Դ
	UINT m_ResIDMove;//��ʾ����ڰ�ť��ʱ��λͼ��Դ
	UINT m_ResIDNorm;//��ʾ�������״̬λͼ��Դ
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnSetStyle ( WPARAM wParam, LPARAM lParam );
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	bool DrawIt (CDC *pdc,UINT ResID);//���ݲ�ͬλͼ��Դ�ػ水ť
	afx_msg LPARAM onmouseleave(WPARAM wm, LPARAM lm);
public:
	// �����ķ�λͼ
void LoadFourBitmaps(unsigned int ResIDNorm,unsigned int ResIDDown=0, unsigned int ResIDMove=0,unsigned int ResIDForb=0);
afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


