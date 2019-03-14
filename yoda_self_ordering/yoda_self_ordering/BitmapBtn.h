#pragma once

// CBitmapBtn

class CBitmapBtn : public CButton
{
	DECLARE_DYNAMIC(CBitmapBtn)

public:
	CBitmapBtn();
	virtual ~CBitmapBtn();
    bool m_over;//表示鼠标是否在其上
	UINT m_ResIDDown;//表示按钮按下状态位图资源
	UINT m_ResIDForb;//表示按钮禁止状态位图资源
	UINT m_ResIDMove;//表示鼠标在按钮上时的位图资源
	UINT m_ResIDNorm;//表示鼠标正常状态位图资源
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnSetStyle ( WPARAM wParam, LPARAM lParam );
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	bool DrawIt (CDC *pdc,UINT ResID);//根据不同位图资源重绘按钮
	afx_msg LPARAM onmouseleave(WPARAM wm, LPARAM lm);
public:
	// 加载四幅位图
void LoadFourBitmaps(unsigned int ResIDNorm,unsigned int ResIDDown=0, unsigned int ResIDMove=0,unsigned int ResIDForb=0);
afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


