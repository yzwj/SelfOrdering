////////////////////////////////////////////////////////////////
// 2000 VCKBASE Online Journal. 
// This program compiles with Visual C++ 6.0 on Windows 98
//
#define DLLIMPORT __declspec(dllimport)

// IdleUI ��һ����̬���ӿ⣬����ȷ���û����洦�ڿ���״̬��ʱ�䡣
// ����ԭ���ǰ�װ���̹��Ӻ���깳�Ӽ�����룬��̬���¼�����¼�����ʱ
// ϵͳʱ��ֵ��
//
// ��̬���ʹ��˵����
// - Ӧ������ʱ���� IdleUIInit 
// - Ӧ����ֹʱ���� IdleUITerm 
// - ���� IdleUIGetLastInputTime 
// - ��ȡʱ��, Ȼ���뵱ǰ�� GetTickCount()���бȽ�;
//
// ��ϸʹ�÷����μ� TestIdleUI ���ӡ�
// 

DLLIMPORT BOOL IdleUIInit();
DLLIMPORT void IdleUITerm();
DLLIMPORT void SetLastIdleTime();
DLLIMPORT DWORD IdleUIGetLastInputTime();
