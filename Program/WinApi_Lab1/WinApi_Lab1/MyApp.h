#pragma once
//≈сли што то это манифест - директива препроцессора котора€ мен€ет нам стиль элементов управлени€ на более современный(надыбал ссылочку на ютубе на сайт мсдн)
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
	//итендификаторы дл€ кнопок
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

	//ћетод который будет запускать мне окошко
	int Run();

private:
	//Ётот метод будет отвечать за инициализацию окна
	void init_native_window_obj();
	//статический метод окна которй потом будет передавать указатель на динамический метод окна
	static LRESULT CALLBACK application_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	//динамический метод окна который будет иметь доступ к пол€м класса
	LRESULT CALLBACK window_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	//метод который будет создавать элементы управлени€
	void create_native_controls();
	LPWSTR open_file_dialog(HWND hWnd);
	void init_new_proc(LPWSTR filePath);
private:
	//пол€ дл€ хранени€ названи€ окна и класса
	const std::wstring m_szAppName{ L"Ћабораторна€ работа є1" }, m_szClassName{ L"WinApi_lab1" };
	//хендлы дл€ элементов управлени€(все элементы упр в винапи €вл окнами)
	HWND m_hWnd{}, m_hWndButton{}, m_hWndEdit{};
	//Ѕуду сохран€ть информацию о всех дочерних процессах
	//Ќо есть одно но! ≈сли € закрою один из них, затем начну провер€ть есть активен ли этот процесс, то он может быть активным
	//так как система может дать другому процессу тот же PID
	std::vector<PROCESS_INFORMATION> m_Proc_Info;
	//константы дл€ размеров окна
	const int m_nAppWidth{600}, m_nAppHeigth{350};

};