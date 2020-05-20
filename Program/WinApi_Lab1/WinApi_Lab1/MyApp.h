#pragma once
//���� ��� �� ��� �������� - ��������� ������������� ������� ������ ��� ����� ��������� ���������� �� ����� �����������(������� �������� �� ����� �� ���� ����)
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <Windows.h>

#include <string>
#include <algorithm>
#include <execution>
#include <vector>

class MyApp 
{
	//�������������� ��� ������
	enum class CTL_ID
	{
		OPENWINEXEC_ID,
		OPENSHELLEXEC_ID,
		OPENCREATEPROC_ID,
		CLOSEPROC_ID,
		EXITPROG_ID,
		FILEEDIT_ID
	};

public:
	explicit MyApp();
	~MyApp();

	//����� ������� ����� ��������� ��� ������
	int Run();

private:
	//���� ����� ����� �������� �� ������������� ����
	void init_native_window_obj();
	//����������� ����� ���� ������ ����� ����� ���������� ��������� �� ������������ ����� ����
	static LRESULT CALLBACK application_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	//������������ ����� ���� ������� ����� ����� ������ � ����� ������
	LRESULT CALLBACK window_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	//����� ������� ����� ��������� �������� ����������
	void create_native_controls();
	LPWSTR open_file_dialog(HWND hWnd);
	void init_new_proc(LPWSTR filePath);
private:
	//���� ��� �������� �������� ���� � ������
	const std::wstring m_szAppName{ L"������������ ������ �1" }, m_szClassName{ L"WinApi_lab1" };
	//������ ��� ��������� ����������(��� �������� ��� � ������ ��� ������)
	HWND m_hWnd{}, m_hWndButton{}, m_hWndEdit{};
	//���� ��������� ���������� � ���� �������� ���������
	//�� ���� ���� ��! ���� � ������ ���� �� ���, ����� ����� ��������� ���� ������� �� ���� �������, �� �� ����� ���� ��������
	//��� ��� ������� ����� ���� ������� �������� ��� �� PID
	std::vector<PROCESS_INFORMATION> m_Proc_Info;
	//��������� ��� �������� ����
	const int m_nAppWidth{600}, m_nAppHeigth{350};

};