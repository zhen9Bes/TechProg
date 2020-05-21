#include "MyApp.h"

MyApp::MyApp()
{
	using std::wstring;
	using std::string;

	try
	{
		this->init_native_window_obj();
		this->create_native_controls();
	}
	catch (const std::exception & ex)
	{
		string expt_data = ex.what();
		MessageBox(nullptr, wstring(begin(expt_data), end(expt_data)).c_str(), L"������!", MB_ICONERROR | MB_OK);
		ExitProcess(EXIT_FAILURE);
	}
}

MyApp::~MyApp()
{

}

int MyApp::Run()
{
	MSG _msg{};

	//������� ����������� ���� � ���������� ���������� ����� � ������ �����
	//��� ����� ��� ����, ����� �������� ����� ���� ���������, �� �������� ���������� ��� ������ �� ������ ������������
	ShowWindow(this->m_hWnd, SW_SHOWDEFAULT);
	UpdateWindow(this->m_hWnd);

	while (GetMessage(&_msg, nullptr, 0, 0))
	{
		TranslateMessage(&_msg);
		DispatchMessage(&_msg);
	}
	return static_cast<int>(_msg.wParam);
}

void MyApp::init_native_window_obj()
{
	using std::runtime_error;
	using namespace std::string_literals;

	WNDCLASSEX _wc{ sizeof(WNDCLASSEX) };
	_wc.cbClsExtra = 0;
	_wc.cbWndExtra = 0;
	_wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	_wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	_wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	_wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	_wc.hInstance = GetModuleHandle(nullptr);
	//����� ���� �������� ��������� �� ����������� ��������� ����, ������� � ���� ������� �������� ���������� ������������
	//��� ��� lpfnWndProc �� ����� ��������� ������ �� ������������� �����
	//��� ��� ����������� ��������� �� ����� ������ � ����� ������ - ��� �������
	_wc.lpfnWndProc = MyApp::application_proc;
	_wc.lpszClassName = this->m_szClassName.c_str();
	_wc.lpszMenuName = nullptr;
	_wc.style = CS_VREDRAW | CS_HREDRAW;

	if (!RegisterClassEx(&_wc))
		throw runtime_error("Error! Can't register main window class!"s);
	//��������� ��������������. ��� ����� ��� ���������� �������� ������ ���������� ������� ����
	RECT _windowRC{ 0,0,this->m_nAppWidth, this->m_nAppHeigth };
	//��� ������ ������� ���� � ������ ���������� �����, �� ���� � ����� ���� ����� ������ ��� ��������� ������
	//� ����� ������. �� ���� ��� ���������� ����� ���������� ������ ������� �������� ������� ��� �����
	AdjustWindowRect(&_windowRC, WS_OVERLAPPEDWINDOW, false);

	//��� ���� ������� �� ������������ �������� ����, ������ CreateWindow ������������ CreateWindowEx
	//��� ��� ��������� ���������� ��� ������� ��������� ��������� �� ����� ���� ���������� ������ ���������
	if (this->m_hWnd = CreateWindowEx(
		0,
		this->m_szClassName.c_str(),
		this->m_szAppName.c_str(),
		WS_DLGFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZE,
		(GetSystemMetrics(SM_CXSCREEN) - _windowRC.right) / 2,
		(GetSystemMetrics(SM_CYSCREEN) - _windowRC.bottom) / 2,
		_windowRC.right, _windowRC.bottom, nullptr, nullptr, nullptr, this //<- ��� ��� �����
	); !this->m_hWnd)
		throw runtime_error("Error! Can't create main window!"s);
}

LRESULT MyApp::application_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//������ ����� ������� =)

	//�������� ��������� �� ����� ������ ����
	MyApp* pApp;

	//������� ������� WM_NCCREATE(�������� ��� ������������ �������) ��� ���� �� ����� ������ ������� �������� ����
	if (uMsg == WM_NCCREATE)
	{
		//��� ������ ��� ��� ��������
		pApp = static_cast<MyApp*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
		//������������� ��� ������ � ����(������ �� ������ ����� ��� ���� ��� �� �� ����"� �����, ��� ���� ����� ��������� ������� ��������� ��� ������)
		//����� ��� � ����� ��� ����, ����� ��������� ��������� ����� ��������� �� ������������ ��������� �� �����
		SetLastError(0);
		//� ������ ������ ���� ���� 2 ��������� ���������� �� ���������������� ���������
		//�� ��������� ��� ����� ���� cbClsExtra = 0;cbWndExtra = 0;
		//��� ��� ���� GWLP_USERDATA � ���� ������� ��������� ����� ��������� �� ����� ������ ���� � �������� ��� ����
		if (!SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pApp)))
		{
			//�� ���� ��� ���. ���� ����� ������ ��� �� ���� ������, �� ��� ��������� ������ ��� �������� �� ����
			//� ����� �� ��� ���������� � ����, �� ������ �������� ����� ��������� ��� �� ��������� ����������
			//���� ��� �� ������ ������
			if (GetLastError() != 0)
				return false;
		}
	}
	else
	{
		//��� ���� ��������� �� ������, ������ ���� ��� ���� �������
		//����� � ��������� ������ ������ ���������, ������� ��� ����� � �������� ���� ������ ����
		pApp = reinterpret_cast<MyApp*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	}

	//���� ��� �� � ��������� �� �������
	if (pApp)
	{
		//�� � ���� ������ ���� ����� ������ ������ ����� ���������� ����
		pApp->m_hWnd = hWnd;
		//� ��� ��� �� ������ ���������� �������� ��������� ����
		//��� ��� ��� ����� ����������� ��� ������� � ����������� ���-�� �� ���
		return pApp->window_proc(hWnd, uMsg, wParam, lParam);
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT MyApp::window_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	this->m_Proc_Info.reserve(10);
	// ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
	switch (uMsg)
	{
		using std::runtime_error;
		using std::string;
		using namespace std::string_literals;
		//��� ����� ������������ ��� ������� ��������� ����������
	case WM_COMMAND:
	{
		switch (static_cast<MyApp::CTL_ID>(LOWORD(wParam)))
		{
		case MyApp::CTL_ID::OPENWINEXEC_ID:
		{
			try
			{
				//this->open_file_dialog(hWnd);
				std::wstring filePath;
				filePath.resize(MAX_PATH);
				GetWindowText(this->m_hWndEdit, &filePath[0], MAX_PATH);
				filePath.erase(remove(begin(filePath), end(filePath), 0), end(filePath));

				if (!filePath.empty())
				{
					//������ � ���� �� ����� ��������. � �������, ��� ��� ����� ��� ������������� �� ������ ��
					//16 ������. �������� ������ � ���� ��������
					//�� ������ �� �� ��� � ������� ��������������� � ��� ����� �����, ������� �� ����������� =(
					LPCSTR lpcStr = reinterpret_cast<LPCSTR>(filePath.c_str());
					LPCWSTR lpcwStr = reinterpret_cast<LPCWSTR>(filePath.c_str());
					DWORD iSizeStr = wcslen(lpcwStr) + 1;
					LPSTR lpStrTmp = (PCHAR)calloc(iSizeStr, sizeof(CHAR));

					DWORD iResult = WideCharToMultiByte(
						CP_ACP,
						WC_NO_BEST_FIT_CHARS,
						lpcwStr,
						INFINITE,
						lpStrTmp,
						iSizeStr,
						NULL,
						NULL
					);

					SetLastError(0);

					WinExec("word.doc", SW_RESTORE); //const_cast<LPCSTR>(lpStrTmp)

					int  i = GetLastError(); // ������ ���� ���� �� �����, ���� ERROR_BAD_EXE_FORMAT
					if (i > 0)
					{
						throw runtime_error("������ �������� �����! ShellExecute()"s);
					}
				}
			}
			catch (...)
			{
				MessageBox(nullptr, L"������ �������� �����", L"������", MB_ICONERROR | MB_OK);
			}
		}
		return 0;
		case MyApp::CTL_ID::OPENSHELLEXEC_ID:
		{
			try
			{
				this->open_file_dialog(hWnd);
				std::wstring filePath;
				filePath.resize(MAX_PATH);
				GetWindowText(this->m_hWndEdit, &filePath[0], MAX_PATH);
				filePath.erase(remove(begin(filePath), end(filePath), 0), end(filePath));

				if (!filePath.empty())
				{
					//��� ��� ������������ ShellExecute �� ���� ����� ������� �����, ���� ������� ������ ���������� � ���� ������
					ShellExecute(hWnd, NULL, reinterpret_cast<LPCWSTR>(filePath.c_str()), NULL, NULL, SW_RESTORE);
					//��������� ������ ��� ����������� ������
					int  i = GetLastError();
					if (i > 0)
					{
						throw runtime_error("������ �������� �����! ShellExecute()"s);
					}
				}
			}
			catch (...)
			{
				MessageBox(this->m_hWnd, L"������ �������� �����", L"������ ������", MB_ICONWARNING | MB_OK);
			}
			////����� ����� ��������� �������� ����.
		//OPENFILENAME ofn{};	// ��������� ������������ ����������� ����
		//char szFile[MAX_PATH] = { 0 };	// ����� ��� ����� ����� (������ ���� �� ����� 256 ��������)
		//HANDLE hf;         	// ���������� �����
		//// ������������� OPENFILENAME
		//ZeroMemory(&ofn, sizeof(OPENFILENAME));				//��� ������� �������� ��������� � ������(� ������� �� memset ��������� �� ������)
		//ofn.lStructSize = sizeof(OPENFILENAME);				//��������� ������ ���������
		//ofn.hwndOwner = hWnd;								//������� ���� ���������
		//ofn.lpstrFile = reinterpret_cast<LPWSTR>(szFile);	//�������� ���� ���� ��������� ��� �����
		//ofn.nMaxFile = sizeof(szFile);						//�������� ������ ��� ����� �����
		//ofn.lpstrFilter = L"All\0*.*\0Word\0*.doc;*.docx\0Text\0*.TXT\0\0"; //������ ��� ������. ���� � ����� ����� \0. 1(�� \0) - ��� �������� �������, 2(����� \0) - ��� ������� ���������� �����, ������� ����������� � ���������� ����� ����� � �������
		//ofn.nFilterIndex = 2;								//��������� ����� ������ ������ ������� ����� ������ ����������. ��������� ���� � 1
		//ofn.lpstrFileTitle = NULL;							//The file name and extension (without path information) of the selected file. This member can be NULL.
		//ofn.nMaxFileTitle = 0;								//The size, in characters, of the buffer pointed to by lpstrFileTitle. This member is ignored if lpstrFileTitle is NULL.
		//ofn.lpstrInitialDir = NULL;							//The initial directory. The algorithm for selecting the initial directory varies on different platforms.
		//ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;	//������������ �����, ��� ���� ����������� ������������� ������ ������������, � ��� �� ��� ����, ����� ��������� ��������������
		//// ������� ���������� ���� ������� (Open) � ���� ��� ����������� �������, �� ��������� �������� ����. 
		////��� ��� ������������ ShellExecute �� ���� ����� ������� �����, ���� ������� ������ ���������� � ���� ������
		//if (GetOpenFileName(&ofn) == TRUE)
		//{
		//	LPWSTR lpwstr = reinterpret_cast<LPWSTR>(szFile); //������ �������� �� ����� ������ LPWSTR
		//	SetWindowText(this->m_hWndEdit, lpwstr);
		//	ShellExecute(hWnd, NULL, ofn.lpstrFile, NULL, NULL, SW_RESTORE);
		//}
		//int  i = GetLastError();
		//if (i > 0)  MessageBox(nullptr, L"������ �������� �����", L"������", MB_ICONERROR | MB_OK);
		//MessageBox(hWnd, L"Do", L"Ne", MB_ICONINFORMATION);
		}
		return 0;
		case MyApp::CTL_ID::OPENCREATEPROC_ID:
		{
			
			this->open_file_dialog(hWnd);
			std::wstring filePath;
			filePath.resize(MAX_PATH);
			GetWindowText(this->m_hWndEdit, &filePath[0], MAX_PATH);
			filePath.erase(remove(begin(filePath), end(filePath), 0), end(filePath));

			this->init_new_proc(const_cast<LPWSTR>(filePath.c_str()));
		}
		return 0;
		case MyApp::CTL_ID::CLOSEPROC_ID:
		{
			PROCESS_INFORMATION pi;
			DWORD dwExitCode;			
			if (!this->m_Proc_Info.empty()) 
			{
				if (GetExitCodeProcess(pi.hProcess, &dwExitCode))
				{
					pi = this->m_Proc_Info.back();

					TerminateProcess(pi.hProcess, EXIT_SUCCESS);
					//SendMessage(reinterpret_cast<HWND>(pi.hProcess), WM_CLOSE, 0, 0);
					MessageBox(hWnd, L"��������� ���������� ������� ��������", L"��������!", MB_OK | MB_ICONINFORMATION);
					CloseHandle(pi.hProcess);
					if (!this->m_Proc_Info.empty())
						this->m_Proc_Info.pop_back();
				}
			}
			else
			{
				MessageBox(hWnd, L"�������� ��������� �� ����������! \n������ ��� �������, ����� ���-�� �������", L"��������!", MB_OK | MB_ICONINFORMATION);
			}
			
		}
		return 0;
		case MyApp::CTL_ID::EXITPROG_ID:
		{
			PROCESS_INFORMATION pi;
			DWORD dwExitCode;

			if (!this->m_Proc_Info.empty())
			{
				while (!this->m_Proc_Info.empty())
				{
					pi = this->m_Proc_Info.back();

					if (GetExitCodeProcess(pi.hProcess, &dwExitCode))
					{
						TerminateProcess(pi.hProcess, EXIT_SUCCESS);
						//SendMessage(reinterpret_cast<HWND>(pi.hProcess), WM_CLOSE, 0, 0);
						CloseHandle(pi.hProcess);
						if (!this->m_Proc_Info.empty())
							this->m_Proc_Info.pop_back();
					}
				}
				MessageBox(hWnd, L"��� �������� �������� ���������", L"��������!", MB_OK | MB_ICONINFORMATION);
			}
			PostQuitMessage(EXIT_SUCCESS);
		}
		return 0;
		}
	}
	return 0;

	//������� ������� �� �������
	case WM_DESTROY:
	{
		PROCESS_INFORMATION pi;
		DWORD dwExitCode;

		if (!this->m_Proc_Info.empty()) 
		{
			while (!this->m_Proc_Info.empty())
			{
				pi = this->m_Proc_Info.back();

				if (GetExitCodeProcess(pi.hProcess, &dwExitCode))
				{
					TerminateProcess(pi.hProcess, EXIT_SUCCESS);
					//SendMessage(reinterpret_cast<HWND>(pi.hProcess), WM_CLOSE, 0, 0);
					CloseHandle(pi.hProcess);
					if (!this->m_Proc_Info.empty())
						this->m_Proc_Info.pop_back();
				}
			}
			MessageBox(hWnd, L"��� �������� �������� ���������", L"��������!", MB_OK | MB_ICONINFORMATION);
		}		
		PostQuitMessage(EXIT_SUCCESS);
	}
	return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void MyApp::init_new_proc(LPWSTR filePath)
{
	STARTUPINFO si;			//�������� ��������� STARTUPINFO ������������ Windows - ��������� ��� �������� ������ ��������.
	PROCESS_INFORMATION pi;	//
	DWORD dwExitCode;
	
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	BOOL success = CreateProcess(NULL,		// No module name (use command line)
		filePath,							// Command line
		NULL,								// Process handle not inheritable
		NULL,								// Thread handle not inheritable
		FALSE,								// Set handle inheritance to FALSE
		CREATE_SUSPENDED,									// No creation flags CREATE_SUSPENDED
		NULL,								// Use parent's environment block
		NULL,								// Use parent's starting directory 
		&si,								// Pointer to "STARTUPINFO" structure
		&pi);								// Pointer to PROCESS_INFORMATION structure
	if (success)
	{
		this->m_Proc_Info.push_back(pi);
		ResumeThread(pi.hThread);
		CloseHandle(pi.hThread);		
		// ���������������� ���������� ������������� ��������, 
		// ���� �� ���������� �������� ������� 
		//WaitForSingleObject(pinfo.hProcess, INFINITE);
		// �������� ������� ����������; �������� ��� ��� ���������� 
		//GetExitCodeProcess(pi.hProcess, &dwExitCode);

		// ���������� ��������� ��������, ��� ������
		// ������������� � ��� ��������! 
		//������ �� ���� �� ������� ��� �����
		//�� ���� ��������� ����� �������� ��� ��� ����� ���� �� ��������� �� ������
		//����� ������ �� ��������
		//������������� ����� �������� �������� ���� ������� � �����
		//CloseHandle(pi.hProcess);
	}
}

LPWSTR MyApp::open_file_dialog(HWND hWnd)
{
	//����� ����� ��������� �������� ����.
	OPENFILENAME ofn{};	// ��������� ������������ ����������� ����
	char szFile[MAX_PATH] = { 0 };	// ����� ��� ����� ����� (������ ���� �� ����� 256 ��������)
	//HANDLE hf;         	// ���������� �����

	// ������������� OPENFILENAME
	ZeroMemory(&ofn, sizeof(OPENFILENAME));				//��� ������� �������� ��������� � ������(� ������� �� memset ��������� �� ������)
	ofn.lStructSize = sizeof(OPENFILENAME);				//��������� ������ ���������
	ofn.hwndOwner = hWnd;								//������� ���� ���������
	ofn.lpstrFile = reinterpret_cast<LPWSTR>(szFile);	//�������� ���� ���� ��������� ��� �����
	ofn.nMaxFile = sizeof(szFile);						//�������� ������ ��� ����� �����
	ofn.lpstrFilter = L"All\0*.*\0Word\0*.doc;*.docx\0EXE\0*.exe\0\0"; //������ ��� ������. ���� � ����� ����� \0. 1(�� \0) - ��� �������� �������, 2(����� \0) - ��� ������� ���������� �����, ������� ����������� � ���������� ����� ����� � �������
	ofn.nFilterIndex = 1;								//��������� ����� ������ ������ ������� ����� ������ ����������. ��������� ���� � 1
	ofn.lpstrFileTitle = NULL;							//The file name and extension (without path information) of the selected file. This member can be NULL.
	ofn.nMaxFileTitle = 0;								//The size, in characters, of the buffer pointed to by lpstrFileTitle. This member is ignored if lpstrFileTitle is NULL.
	ofn.lpstrInitialDir = NULL;							//The initial directory. The algorithm for selecting the initial directory varies on different platforms.
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;	//������������ �����, ��� ���� ����������� ������������� ������ ������������, � ��� �� ��� ����, ����� ��������� ��������������

	// ������� ���������� ���� ������� (Open) � ���� ��� ����������� �������, �� ������������� � �������� ������ ���� ����� � ����� ���������� ����. 
	if (GetOpenFileName(&ofn) == TRUE)
	{
		SetWindowText(this->m_hWndEdit, ofn.lpstrFile);
		return ofn.lpstrFile;
	}
	else
	{
		int  i = GetLastError(); //�� ������ ��������� ���� ������ �� ����� �������
		if (i > 0)  MessageBox(nullptr, L"������ �������� �����", L"������", MB_ICONERROR | MB_OK);
		return nullptr;
	}
}

void MyApp::create_native_controls()
{
	using std::runtime_error;
	using namespace std::string_literals;

	//�������� ���� ���������� �����
	HFONT hFont = CreateFont(18, 0, 0, 0, FW_REGULAR, 0, 0, 0,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH, L"Robo");

	//����� ������ ������� ����������� �������� ����������
	if (this->m_hWndButton = CreateWindowEx(
		0,
		L"BUTTON",
		L"������� ����( WinExec() )",
		WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
		20, 70, 250, 40, this->m_hWnd, reinterpret_cast<HMENU>(MyApp::CTL_ID::OPENWINEXEC_ID), nullptr, nullptr
	); !this->m_hWndButton)
		throw runtime_error("Can't create WinExec button!"s);
	//�������� ��������� c ������ WM_SETFONT, ������� ��������� ������ ��� ��������� ����������
	SendMessage(this->m_hWndButton, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), TRUE);

	if (this->m_hWndButton = CreateWindowEx(
		0,
		L"BUTTON",
		L"������� ����( ShellExecute() )",
		WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
		320, 70, 250, 40, this->m_hWnd, reinterpret_cast<HMENU>(MyApp::CTL_ID::OPENSHELLEXEC_ID), nullptr, nullptr
	); !this->m_hWndButton)
		throw runtime_error("Can't create ShellExecute button!"s);
	//�������� ��������� c ������ WM_SETFONT, ������� ��������� ������ ��� ��������� ����������
	SendMessage(this->m_hWndButton, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), TRUE);

	if (this->m_hWndButton = CreateWindowEx(
		0,
		L"BUTTON",
		L"������� ����( CreateProcces() )",
		WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
		20, 140, 550, 40, this->m_hWnd, reinterpret_cast<HMENU>(MyApp::CTL_ID::OPENCREATEPROC_ID), nullptr, nullptr
	); !this->m_hWndButton)
		throw runtime_error("Can't create CreateProcces button!"s);
	//�������� ��������� c ������ WM_SETFONT, ������� ��������� ������ ��� ��������� ����������
	SendMessage(this->m_hWndButton, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), TRUE);

	if (this->m_hWndButton = CreateWindowEx(
		0,
		L"BUTTON",
		L"������� ��������� �������( CreateProcces() )",
		WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
		20, 190, 550, 40, this->m_hWnd, reinterpret_cast<HMENU>(MyApp::CTL_ID::CLOSEPROC_ID), nullptr, nullptr
	); !this->m_hWndButton)
		throw runtime_error("Can't create TerminateProcces button!"s);
	//�������� ��������� c ������ WM_SETFONT, ������� ��������� ������ ��� ��������� ����������
	SendMessage(this->m_hWndButton, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), TRUE);

	if (this->m_hWndButton = CreateWindowEx(
		0,
		L"BUTTON",
		L"����� �� ���������",
		WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
		175, 250, 240, 40, this->m_hWnd, reinterpret_cast<HMENU>(MyApp::CTL_ID::EXITPROG_ID), nullptr, nullptr
	); !this->m_hWndButton)
		throw runtime_error("Can't create EXIT button!"s);
	//�������� ��������� c ������ WM_SETFONT, ������� ��������� ������ ��� ��������� ����������
	SendMessage(this->m_hWndButton, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), TRUE);

	if (this->m_hWndEdit = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		L"EDIT",
		L"No one procces didn't start",
		WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
		10, 20, 570, 25, this->m_hWnd, reinterpret_cast<HMENU>(MyApp::CTL_ID::FILEEDIT_ID), nullptr, nullptr
	); !this->m_hWndEdit)
		throw runtime_error("Can't create edit!"s);
	//�������� ��������� c ������ WM_SETFONT, ������� ��������� ������ ��� ��������� ����������
	SendMessage(this->m_hWndEdit, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), TRUE);

}
