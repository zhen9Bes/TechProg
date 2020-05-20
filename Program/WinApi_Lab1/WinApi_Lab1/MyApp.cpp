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
	catch (const std::exception& ex)
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
		if(!SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pApp)))
		{
			//�� ���� ��� ���. ���� ����� ������ ��� �� ���� ������, �� ��� ��������� ������ ��� �������� �� ����
			//� ����� �� ��� ���������� � ����, �� ������ �������� ����� ��������� ��� �� ��������� ����������
			//���� ��� �� ������ ������
			if(GetLastError() != 0)
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
	switch (uMsg)
	{
		//��� ����� ������������ ��� ������� ��������� ����������
		case WM_COMMAND:
		{
			switch (static_cast<MyApp::CTL_ID>(LOWORD(wParam)))
			{
			case MyApp::CTL_ID::OPENWINEXEC_ID:
			{
				////����� ����� ��������� �������� ����.

				//OPENFILENAME ofn{};	// ��������� ������������ ����������� ����
				//char szFile[260] = { 0 };	// ����� ��� ����� ����� (������ ���� �� ����� 256 ��������)
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
				//	ShellExecute(hWnd, NULL, reinterpret_cast<LPWSTR>(szFile), NULL, NULL, SW_RESTORE);


				///*hf = CreateFile(ofn.lpstrFile, GENERIC_READ,
				//	0, (LPSECURITY_ATTRIBUTES)NULL,
				//	OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
				//	(HANDLE)NULL);*/

				//	//MessageBox(hWnd, L"Do", L"Ne", MB_ICONINFORMATION);

			}
			return 0;
			case MyApp::CTL_ID::OPENSHELLFILE_ID:
			{

			}
			return 0;
			case MyApp::CTL_ID::OPENCREATEPROC_ID:
			{

			}
			return 0;
			case MyApp::CTL_ID::CLOSEPROC_ID:
			{

			}
			return 0;
			case MyApp::CTL_ID::FILEEDIT_ID:
			{

			}
			return 0;
			case MyApp::CTL_ID::EXITPROG_ID:
			{

			}
			return 0;
			}
		}
		return 0;
		//������� ������� �� �������
		case WM_DESTROY:
		{
			PostQuitMessage(EXIT_SUCCESS);
		}
		return 0;
	}
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
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
		20, 70, 250, 40, this->m_hWnd, reinterpret_cast<HMENU>(MyApp::CTL_ID::OPENWINEXEC_ID),nullptr, nullptr
	); !this->m_hWndButton)
		throw runtime_error("Can't create WinExec button!"s);
	//�������� ��������� c ������ WM_SETFONT, ������� ��������� ������ ��� ��������� ����������
	SendMessage(this->m_hWndButton, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), TRUE);

	if (this->m_hWndButton = CreateWindowEx(
		0,
		L"BUTTON",
		L"������� ����( ShellExecute() )",
		WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
		320, 70, 250, 40, this->m_hWnd, reinterpret_cast<HMENU>(MyApp::CTL_ID::OPENSHELLFILE_ID), nullptr, nullptr
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
