// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� PKB_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// PKB_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef PKB_EXPORTS
#define PKB_API __declspec(dllexport)
#else
#define PKB_API __declspec(dllimport)
#endif


extern "C" PKB_API void fnInitDll(HWND hWnd);
extern "C" PKB_API void fnCreateWndContainer(HINSTANCE hInstance, HWND hWnd, DWORD i);
extern "C" PKB_API void fnReSize(HWND hWndContainer);