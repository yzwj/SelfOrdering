////////////////////////////////////////////////////////////////
// 2000 VCKBASE Online Journal. 
// This program compiles with Visual C++ 6.0 on Windows 98
//
#define DLLIMPORT __declspec(dllimport)

// IdleUI 是一个动态链接库，用来确定用户界面处于空闲状态的时间。
// 工作原理是安装键盘钩子和鼠标钩子监控输入，动态库记录输入事件发生时
// 系统时钟值。
//
// 动态库的使用说明：
// - 应用启动时调用 IdleUIInit 
// - 应用终止时调用 IdleUITerm 
// - 调用 IdleUIGetLastInputTime 
// - 获取时间, 然后与当前的 GetTickCount()进行比较;
//
// 详细使用方法参见 TestIdleUI 例子。
// 

DLLIMPORT BOOL IdleUIInit();
DLLIMPORT void IdleUITerm();
DLLIMPORT void SetLastIdleTime();
DLLIMPORT DWORD IdleUIGetLastInputTime();
