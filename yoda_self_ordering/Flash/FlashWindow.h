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
	/// @brief 创建窗口 和 初始化数据
	BOOL CreateDisplayWindow(HWND hWndParent,HINSTANCE hInst,CRect windowRec,CString filename,bool m_isChild);

	/// @brief 销毁（播放）窗口。删除之前调用该接口，来删除窗口。
	void DestroyDisplayWindow();

	/// @brief 将（播放）窗口设为最顶层
	void SetWindowTopMost();

	/// @brief 设置（播放）窗口的大小、位置
	void SetWindowArea(int left, int top, int width, int height);

	/// @brief 获取窗口的矩形区域
	CRect GetWindowArea();

	/// @brief 开始播放、继续播放
	void PlayFlash();

	/// @brief 暂停播放
	void Pause();

	/// @brief 切换循环状态：如果目前不是循环播放，切换到循环播放；如果是循环，切换到不循环。
	void ToggleLoop();

	/// @brief 倒带、后退
	void RewindFlash();

	/// @brief 当前，是否正在播放
	bool IsPlaying();

	/// @brief 当前，是否暂停播放
	bool IsPaused();

	/// @brief 当前，是否循环播放
	bool IsLoop();

	/// 获取快照，（或者说，播放状态的当前帧）
	virtual bool GetSnapshot(DisplayObjectSnapshot& snapshot);

	virtual void ReleaseSnapshot(DisplayObjectSnapshot& snapshot);
};
