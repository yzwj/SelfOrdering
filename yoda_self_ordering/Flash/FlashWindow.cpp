#include "StdAfx.h"
#include "FlashWindow.h"

CFlashWindow::CFlashWindow(void)
{
	this->m_bNeedBackBmp = true;
}

CFlashWindow::~CFlashWindow(void)
{
}
/// @brief 创建窗口 和 初始化数据
BOOL CFlashWindow::CreateDisplayWindow(HWND hWndParent,HINSTANCE hInst,CRect windowRec,CString filename,bool m_isChild)
{
	m_Rect=windowRec;
    BOOL success=Create(hWndParent, hInst,windowRec,filename,m_isChild);
	return success;
}
/// @brief 销毁（播放）窗口。删除之前调用该接口，来删除窗口。
void CFlashWindow::DestroyDisplayWindow()
{
    ::DestroyWindow(GetHWND());
}
/// @brief 将（播放）窗口设为最顶层
void CFlashWindow::SetWindowTopMost()
{
    ::SetWindowPos(GetHWND(),NULL,0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);
}
/// @brief 设置（播放）窗口的大小、位置
void CFlashWindow::SetWindowArea(int left, int top, int width, int height)
{
    ::MoveWindow(GetHWND(), left, top, width, height,TRUE);
	m_Rect.left=left;
	m_Rect.top=top;
	m_Rect.right=left+width;
	m_Rect.bottom=top+height;
}
/// @brief 获取窗口的矩形区域
CRect CFlashWindow::GetWindowArea()
{
    return m_Rect;
}
/// @brief 开始播放、继续播放
void CFlashWindow::PlayFlash()
{
    m_lpControl->Play();
}
/// @brief 暂停播放
void CFlashWindow::Pause()
{
    m_lpControl->StopPlay();
}
/// @brief 切换循环状态：如果目前不是循环播放，切换到循环播放；如果是循环，切换到不循环。
void CFlashWindow::ToggleLoop()
{
    if(m_isloop)
		m_lpControl->PutLoop(FALSE);
	else
        m_lpControl->PutLoop(TRUE);
}
/// @brief 倒带、后退
void CFlashWindow::RewindFlash()
{
    m_lpControl->Rewind();
	m_lpControl->Play();
}
/// @brief 当前，是否正在播放
bool CFlashWindow::IsPlaying()
{
    return m_lpControl->IsPlaying();
}
/// @brief 当前，是否暂停播放
bool CFlashWindow::IsPaused()
{
    return !(m_lpControl->IsPlaying());
}
/// @brief 当前，是否循环播放
bool CFlashWindow::IsLoop()
{
    m_isloop=m_lpControl->GetLoop();
	return m_isloop;
}

bool CFlashWindow::GetSnapshot( DisplayObjectSnapshot& snapshot )
{
	HWND hwnd = GetHWND();
	CRect rc;
	::GetWindowRect(hwnd, rc);

	snapshot.width = rc.Width();
	snapshot.height = rc.Height();

	snapshot.hdc = m_hdcBack;//::GetDC(hwnd);
	snapshot.hbmp = m_bmpBack;//NULL;
	return true;
}

void CFlashWindow::ReleaseSnapshot( DisplayObjectSnapshot& snapshot )
{
//	::ReleaseDC(GetHWND(), snapshot.hdc);
}

void CFlashWindow::handleUpdating()
{
	m_bNeedBackBmp = false;
}
