#ifndef DisplayObject_h__
#define DisplayObject_h__

/// ���յ�����
struct DisplayObjectSnapshot
{
	HBITMAP hbmp;  ///< ���յ��ڴ�λͼ
	HDC		hdc;   ///< ����ڴ�λͼ�ж�Ӧ���ڴ�DC�������ã�����ΪNULL
	int		width; ///< λͼ�Ŀ��
	int		height;///< λͼ�ĸ߶�

	DisplayObjectSnapshot()
		: hbmp(NULL), hdc(NULL), width(0), height(0)
	{}
};

class IDisplayObject
{
public:
	virtual ~IDisplayObject() {}

	/// @brief �������� �� ��ʼ������
	virtual BOOL CreateDisplayWindow(HWND hWndParent,HINSTANCE hInst,CRect windowRec,CString filename,bool m_isChild) = 0;

	/// @brief ���٣����ţ����ڡ�ɾ��֮ǰ���øýӿڣ���ɾ�����ڡ�
	virtual void DestroyDisplayWindow() = 0;

	/// @brief �������ţ�������Ϊ���
	virtual void SetWindowTopMost() = 0;

	/// @brief ���ã����ţ����ڵĴ�С��λ��
	virtual void SetWindowArea(int left, int top, int width, int height) = 0;

	/// @brief ��ȡ���ڵľ�������
	virtual CRect GetWindowArea() = 0;

	/// @brief ��ʼ���š���������
	virtual void PlayFlash() = 0;

	/// @brief ��ͣ����
	virtual void Pause() = 0;

	/// @brief �л�ѭ��״̬�����Ŀǰ����ѭ�����ţ��л���ѭ�����ţ������ѭ�����л�����ѭ����
	virtual void ToggleLoop() = 0;

	/// @brief ����������
	virtual void RewindFlash() = 0;

	/// @brief ��ǰ���Ƿ����ڲ���
	virtual bool IsPlaying() = 0;

	/// @brief ��ǰ���Ƿ���ͣ����
	virtual bool IsPaused() = 0;

	/// @brief ��ǰ���Ƿ�ѭ������
	virtual bool IsLoop() = 0;

	/// ��ȡ���գ�������˵������״̬�ĵ�ǰ֡��
	virtual bool GetSnapshot(DisplayObjectSnapshot& snapshot) = 0;

	/// �����Ҫ�ͷſ������ݣ�����д�ú������ͷ���Դ��Ĭ����ʲôҲ������
	virtual void ReleaseSnapshot(DisplayObjectSnapshot& snapshot)	{ }
};

#endif // DisplayObject_h__
