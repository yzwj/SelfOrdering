#ifndef DisplayObject_h__
#define DisplayObject_h__

/// 快照的数据
struct DisplayObjectSnapshot
{
	HBITMAP hbmp;  ///< 快照的内存位图
	HDC		hdc;   ///< 如果内存位图有对应的内存DC，则设置；否则为NULL
	int		width; ///< 位图的宽度
	int		height;///< 位图的高度

	DisplayObjectSnapshot()
		: hbmp(NULL), hdc(NULL), width(0), height(0)
	{}
};

class IDisplayObject
{
public:
	virtual ~IDisplayObject() {}

	/// @brief 创建窗口 和 初始化数据
	virtual BOOL CreateDisplayWindow(HWND hWndParent,HINSTANCE hInst,CRect windowRec,CString filename,bool m_isChild) = 0;

	/// @brief 销毁（播放）窗口。删除之前调用该接口，来删除窗口。
	virtual void DestroyDisplayWindow() = 0;

	/// @brief 将（播放）窗口设为最顶层
	virtual void SetWindowTopMost() = 0;

	/// @brief 设置（播放）窗口的大小、位置
	virtual void SetWindowArea(int left, int top, int width, int height) = 0;

	/// @brief 获取窗口的矩形区域
	virtual CRect GetWindowArea() = 0;

	/// @brief 开始播放、继续播放
	virtual void PlayFlash() = 0;

	/// @brief 暂停播放
	virtual void Pause() = 0;

	/// @brief 切换循环状态：如果目前不是循环播放，切换到循环播放；如果是循环，切换到不循环。
	virtual void ToggleLoop() = 0;

	/// @brief 倒带、后退
	virtual void RewindFlash() = 0;

	/// @brief 当前，是否正在播放
	virtual bool IsPlaying() = 0;

	/// @brief 当前，是否暂停播放
	virtual bool IsPaused() = 0;

	/// @brief 当前，是否循环播放
	virtual bool IsLoop() = 0;

	/// 获取快照，（或者说，播放状态的当前帧）
	virtual bool GetSnapshot(DisplayObjectSnapshot& snapshot) = 0;

	/// 如果需要释放快照数据，则重写该函数来释放资源。默认是什么也不做。
	virtual void ReleaseSnapshot(DisplayObjectSnapshot& snapshot)	{ }
};

#endif // DisplayObject_h__
