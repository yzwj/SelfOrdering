#pragma once
#include "DisplayObject.h"
#include "FlashWnd.h"
class AFX_EXT_CLASS CFlashWindow:public CFlashWnd,public IDisplayObject
{
public:
	CRect m_Rect;
	bool m_isloop;

	virtual void handleUpdating();

	CFlashWindow(void);

	virtual ~CFlashWindow(void);
	/// @brief �������� �� ��ʼ������
	BOOL CreateDisplayWindow(HWND hWndParent,HINSTANCE hInst,CRect windowRec,CString filename,bool m_isChild);

	/// @brief ���٣����ţ����ڡ�ɾ��֮ǰ���øýӿڣ���ɾ�����ڡ�
	void DestroyDisplayWindow();

	/// @brief �������ţ�������Ϊ���
	void SetWindowTopMost();

	/// @brief ���ã����ţ����ڵĴ�С��λ��
	void SetWindowArea(int left, int top, int width, int height);

	/// @brief ��ȡ���ڵľ�������
	CRect GetWindowArea();

	/// @brief ��ʼ���š���������
	void PlayFlash();

	/// @brief ��ͣ����
	void Pause();

	/// @brief �л�ѭ��״̬�����Ŀǰ����ѭ�����ţ��л���ѭ�����ţ������ѭ�����л�����ѭ����
	void ToggleLoop();

	/// @brief ����������
	void RewindFlash();

	/// @brief ��ǰ���Ƿ����ڲ���
	bool IsPlaying();

	/// @brief ��ǰ���Ƿ���ͣ����
	bool IsPaused();

	/// @brief ��ǰ���Ƿ�ѭ������
	bool IsLoop();

	/// ��ȡ���գ�������˵������״̬�ĵ�ǰ֡��
	virtual bool GetSnapshot(DisplayObjectSnapshot& snapshot);

	virtual void ReleaseSnapshot(DisplayObjectSnapshot& snapshot);
};
