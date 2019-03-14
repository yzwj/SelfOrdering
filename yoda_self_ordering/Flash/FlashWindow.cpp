#include "StdAfx.h"
#include "FlashWindow.h"

CFlashWindow::CFlashWindow(void)
{
	this->m_bNeedBackBmp = true;
}

CFlashWindow::~CFlashWindow(void)
{
}
/// @brief �������� �� ��ʼ������
BOOL CFlashWindow::CreateDisplayWindow(HWND hWndParent,HINSTANCE hInst,CRect windowRec,CString filename,bool m_isChild)
{
	m_Rect=windowRec;
    BOOL success=Create(hWndParent, hInst,windowRec,filename,m_isChild);
	return success;
}
/// @brief ���٣����ţ����ڡ�ɾ��֮ǰ���øýӿڣ���ɾ�����ڡ�
void CFlashWindow::DestroyDisplayWindow()
{
    ::DestroyWindow(GetHWND());
}
/// @brief �������ţ�������Ϊ���
void CFlashWindow::SetWindowTopMost()
{
    ::SetWindowPos(GetHWND(),NULL,0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);
}
/// @brief ���ã����ţ����ڵĴ�С��λ��
void CFlashWindow::SetWindowArea(int left, int top, int width, int height)
{
    ::MoveWindow(GetHWND(), left, top, width, height,TRUE);
	m_Rect.left=left;
	m_Rect.top=top;
	m_Rect.right=left+width;
	m_Rect.bottom=top+height;
}
/// @brief ��ȡ���ڵľ�������
CRect CFlashWindow::GetWindowArea()
{
    return m_Rect;
}
/// @brief ��ʼ���š���������
void CFlashWindow::PlayFlash()
{
    m_lpControl->Play();
}
/// @brief ��ͣ����
void CFlashWindow::Pause()
{
    m_lpControl->StopPlay();
}
/// @brief �л�ѭ��״̬�����Ŀǰ����ѭ�����ţ��л���ѭ�����ţ������ѭ�����л�����ѭ����
void CFlashWindow::ToggleLoop()
{
    if(m_isloop)
		m_lpControl->PutLoop(FALSE);
	else
        m_lpControl->PutLoop(TRUE);
}
/// @brief ����������
void CFlashWindow::RewindFlash()
{
    m_lpControl->Rewind();
	m_lpControl->Play();
}
/// @brief ��ǰ���Ƿ����ڲ���
bool CFlashWindow::IsPlaying()
{
    return m_lpControl->IsPlaying();
}
/// @brief ��ǰ���Ƿ���ͣ����
bool CFlashWindow::IsPaused()
{
    return !(m_lpControl->IsPlaying());
}
/// @brief ��ǰ���Ƿ�ѭ������
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
