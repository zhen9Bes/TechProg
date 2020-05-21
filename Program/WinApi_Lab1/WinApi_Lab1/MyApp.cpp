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
		MessageBox(nullptr, wstring(begin(expt_data), end(expt_data)).c_str(), L"Ошибка!", MB_ICONERROR | MB_OK);
		ExitProcess(EXIT_FAILURE);
	}
}

MyApp::~MyApp()
{

}

int MyApp::Run()
{
	MSG _msg{};

	//Функция отображения окна и обновления вызывается здесь и больше нигде
	//Это нужно для того, чтобы избежать когда окно появилось, но элементы управления еще просто не успели отрисоваться
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
	//здесь надо передать указатель на статическую процедуру окна, которая в свою очередь передаст управление динамический
	//Так как lpfnWndProc не может содержать ссылку на нестатический метод
	//так как статические процедуры не имеют доступ к полям класса - это логично
	_wc.lpfnWndProc = MyApp::application_proc;
	_wc.lpszClassName = this->m_szClassName.c_str();
	_wc.lpszMenuName = nullptr;
	_wc.style = CS_VREDRAW | CS_HREDRAW;

	if (!RegisterClassEx(&_wc))
		throw runtime_error("Error! Can't register main window class!"s);
	//структура прямоугольника. мне нудна для правильных размеров именно КЛИЕНТСКОЙ области окна
	RECT _windowRC{ 0,0,this->m_nAppWidth, this->m_nAppHeigth };
	//эта фукция создаст окно с учетом клиентской части, то есть в целом окно будет больше чем указанные размер
	//в полях класса. но зато для клиенсткой части выделиться именно столько пикселей сколько мне нужно
	AdjustWindowRect(&_windowRC, WS_OVERLAPPEDWINDOW, false);

	//тут есть отличия от стандартного создания окна, вместе CreateWindow используется CreateWindowEx
	//так как последним параметром эта функция принимает указатель на класс окна содержащий нужные процедуры
	if (this->m_hWnd = CreateWindowEx(
		0,
		this->m_szClassName.c_str(),
		this->m_szAppName.c_str(),
		WS_DLGFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZE,
		(GetSystemMetrics(SM_CXSCREEN) - _windowRC.right) / 2,
		(GetSystemMetrics(SM_CYSCREEN) - _windowRC.bottom) / 2,
		_windowRC.right, _windowRC.bottom, nullptr, nullptr, nullptr, this //<- вот эта шляпа
	); !this->m_hWnd)
		throw runtime_error("Error! Can't create main window!"s);
}

LRESULT MyApp::application_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//дальше самое вкусное =)

	//создадим указатель на класс нашего окна
	MyApp* pApp;

	//смотрим событие WM_NCCREATE(создание его неклиентской области) это одно из самых первых событий создания окна
	if (uMsg == WM_NCCREATE)
	{
		//тут первый раз при создании
		pApp = static_cast<MyApp*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
		//устанавливаем код ошибки в ноль(честно не совсем понял для чего это но на мсдн"е пишут, что надо перед созданием оконной процедуры это делать)
		//вроде как я понял это надо, чтобы корректно проверить потом правильно ли скопировался указатель на класс
		SetLastError(0);
		//в классе самого окна если 2 параметра отвечающие за пользовательскую информаци
		//по умолчанию они равны нулю cbClsExtra = 0;cbWndExtra = 0;
		//так вот флаг GWLP_USERDATA в этой функции позволяет взять указатель на класс нашего окна и запихать его туды
		if (!SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pApp)))
		{
			//то есть вот тут. если вдруг раньше где то были ошибки, то код последний ошибки был априории не ноль
			//а когда мы его установили в ноль, то теперь спокойно можем проверить все ли корректно записалось
			//если нет то кидаем ошибку
			if (GetLastError() != 0)
				return false;
		}
	}
	else
	{
		//это если сообщение не первое, значит окно уже было создано
		//тогда в указатель класса кидаем указатель, который уже лежит в юзерской дате класса окна
		pApp = reinterpret_cast<MyApp*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	}

	//если все ок и указатель не нулевой
	if (pApp)
	{
		//то в поле хэндла окна моего класса пихаем хэндл созданного окна
		pApp->m_hWnd = hWnd;
		//и вот тут мы отдаем управление главноей процедуре окна
		//она уже там будет отлавливать все события и реагировать как-то на них
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
		//тут будем обрабатывать все события элементов управления
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
					//Вообще у меня не хочет работать. Я понимаю, что она нужна для совместимости со старым ОС
					//16 битная. Работает только с АНСИ строками
					//Не смотря на то что я пытаюсь преобразовывать и все вроде верно, процесс не запускается =(
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

					int  i = GetLastError(); // выдает либо Путь не найде, либо ERROR_BAD_EXE_FORMAT
					if (i > 0)
					{
						throw runtime_error("Ошибка открытия файла! ShellExecute()"s);
					}
				}
			}
			catch (...)
			{
				MessageBox(nullptr, L"Ошибка открытия файла", L"Ошибка", MB_ICONERROR | MB_OK);
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
					//Так как используется ShellExecute то файл можно открыть любой, если система найдет ассоциацию с этим файлом
					ShellExecute(hWnd, NULL, reinterpret_cast<LPCWSTR>(filePath.c_str()), NULL, NULL, SW_RESTORE);
					//Обработка ошибок для отладочного режима
					int  i = GetLastError();
					if (i > 0)
					{
						throw runtime_error("Ошибка открытия файла! ShellExecute()"s);
					}
				}
			}
			catch (...)
			{
				MessageBox(this->m_hWnd, L"Ошибка открытия файла", L"Ошибка чтения", MB_ICONWARNING | MB_OK);
			}
			////Здесь будем запускать открытие окна.
		//OPENFILENAME ofn{};	// структура стандартного диалогового окна
		//char szFile[MAX_PATH] = { 0 };	// буфер для имени файла (должен быть не менее 256 символов)
		//HANDLE hf;         	// дескриптор файла
		//// Инициализация OPENFILENAME
		//ZeroMemory(&ofn, sizeof(OPENFILENAME));				//Эта функция обнуляет структуру в памяти(в отличие от memset заполняет ее нулями)
		//ofn.lStructSize = sizeof(OPENFILENAME);				//Указываем размер структуры
		//ofn.hwndOwner = hWnd;								//передаю окно владельца
		//ofn.lpstrFile = reinterpret_cast<LPWSTR>(szFile);	//указываю куда буду сохранять имя файла
		//ofn.nMaxFile = sizeof(szFile);						//указываю размер для имени файла
		//ofn.lpstrFilter = L"All\0*.*\0Word\0*.doc;*.docx\0Text\0*.TXT\0\0"; //фильты для поиска. Идут в парах через \0. 1(до \0) - это название фильтра, 2(после \0) - это фильтры разрешения фалов, которые указываются в звездочках через точку с запятой
		//ofn.nFilterIndex = 2;								//Указываем какой именно индекс фильтра будет выбран изначально. Нумерация идет с 1
		//ofn.lpstrFileTitle = NULL;							//The file name and extension (without path information) of the selected file. This member can be NULL.
		//ofn.nMaxFileTitle = 0;								//The size, in characters, of the buffer pointed to by lpstrFileTitle. This member is ignored if lpstrFileTitle is NULL.
		//ofn.lpstrInitialDir = NULL;							//The initial directory. The algorithm for selecting the initial directory varies on different platforms.
		//ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;	//Устанавливаю флаги, что путь указываемый пользователем должен существовать, а так же сам файл, иначе выскочить предупреждение
		//// Покажем диалоговое окно Открыть (Open) и если все выполнилось успешно, то открываем выбраный файл. 
		////Так как используется ShellExecute то файл можно открыть любой, если система найдет ассоциацию с этим файлом
		//if (GetOpenFileName(&ofn) == TRUE)
		//{
		//	LPWSTR lpwstr = reinterpret_cast<LPWSTR>(szFile); //просто проверял чо лежит внутри LPWSTR
		//	SetWindowText(this->m_hWndEdit, lpwstr);
		//	ShellExecute(hWnd, NULL, ofn.lpstrFile, NULL, NULL, SW_RESTORE);
		//}
		//int  i = GetLastError();
		//if (i > 0)  MessageBox(nullptr, L"Ошибка открытия файла", L"Ошибка", MB_ICONERROR | MB_OK);
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
					MessageBox(hWnd, L"Последний запуженный процесс завершен", L"Внимание!", MB_OK | MB_ICONINFORMATION);
					CloseHandle(pi.hProcess);
					if (!this->m_Proc_Info.empty())
						this->m_Proc_Info.pop_back();
				}
			}
			else
			{
				MessageBox(hWnd, L"Дочерних процессов не существует! \nПрежде чем закрыть, нужно что-то создать", L"Внимание!", MB_OK | MB_ICONINFORMATION);
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
				MessageBox(hWnd, L"Все дочерние процессы завершены", L"Внимание!", MB_OK | MB_ICONINFORMATION);
			}
			PostQuitMessage(EXIT_SUCCESS);
		}
		return 0;
		}
	}
	return 0;

	//событие нажатия на крестик
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
			MessageBox(hWnd, L"Все дочерние процессы завершены", L"Внимание!", MB_OK | MB_ICONINFORMATION);
		}		
		PostQuitMessage(EXIT_SUCCESS);
	}
	return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void MyApp::init_new_proc(LPWSTR filePath)
{
	STARTUPINFO si;			//Элементы структуры STARTUPINFO используются Windows - функциями при создании нового процесса.
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
		// приостанавливаем выполнение родительского процесса, 
		// пока не завершится дочерний процесс 
		//WaitForSingleObject(pinfo.hProcess, INFINITE);
		// дочерний процесс завершился; получаем код его завершения 
		//GetExitCodeProcess(pi.hProcess, &dwExitCode);

		// закрывайте описатель процесса, как только
		// необходимость в нем отпадает! 
		//Честно не знаю на сколько это верно
		//Не стал закрывать хэндл процесса так как потом буду их закрывать по кнопке
		//иначе просто не работало
		//соответсвенно после удаление процесса буду удалять и хэндл
		//CloseHandle(pi.hProcess);
	}
}

LPWSTR MyApp::open_file_dialog(HWND hWnd)
{
	//Здесь будем запускать открытие окна.
	OPENFILENAME ofn{};	// структура стандартного диалогового окна
	char szFile[MAX_PATH] = { 0 };	// буфер для имени файла (должен быть не менее 256 символов)
	//HANDLE hf;         	// дескриптор файла

	// Инициализация OPENFILENAME
	ZeroMemory(&ofn, sizeof(OPENFILENAME));				//Эта функция обнуляет структуру в памяти(в отличие от memset заполняет ее нулями)
	ofn.lStructSize = sizeof(OPENFILENAME);				//Указываем размер структуры
	ofn.hwndOwner = hWnd;								//передаю окно владельца
	ofn.lpstrFile = reinterpret_cast<LPWSTR>(szFile);	//указываю куда буду сохранять имя файла
	ofn.nMaxFile = sizeof(szFile);						//указываю размер для имени файла
	ofn.lpstrFilter = L"All\0*.*\0Word\0*.doc;*.docx\0EXE\0*.exe\0\0"; //фильты для поиска. Идут в парах через \0. 1(до \0) - это название фильтра, 2(после \0) - это фильтры разрешения фалов, которые указываются в звездочках через точку с запятой
	ofn.nFilterIndex = 1;								//Указываем какой именно индекс фильтра будет выбран изначально. Нумерация идет с 1
	ofn.lpstrFileTitle = NULL;							//The file name and extension (without path information) of the selected file. This member can be NULL.
	ofn.nMaxFileTitle = 0;								//The size, in characters, of the buffer pointed to by lpstrFileTitle. This member is ignored if lpstrFileTitle is NULL.
	ofn.lpstrInitialDir = NULL;							//The initial directory. The algorithm for selecting the initial directory varies on different platforms.
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;	//Устанавливаю флаги, что путь указываемый пользователем должен существовать, а так же сам файл, иначе выскочить предупреждение

	// Покажем диалоговое окно Открыть (Open) и если все выполнилось успешно, то устанавливаем в лайнэдит полный путь файла и после возвращаем путь. 
	if (GetOpenFileName(&ofn) == TRUE)
	{
		SetWindowText(this->m_hWndEdit, ofn.lpstrFile);
		return ofn.lpstrFile;
	}
	else
	{
		int  i = GetLastError(); //на случай просмотра кода ошибки во время отладки
		if (i > 0)  MessageBox(nullptr, L"Ошибка открытия файла", L"Ошибка", MB_ICONERROR | MB_OK);
		return nullptr;
	}
}

void MyApp::create_native_controls()
{
	using std::runtime_error;
	using namespace std::string_literals;

	//свистнул один прикольный шрифт
	HFONT hFont = CreateFont(18, 0, 0, 0, FW_REGULAR, 0, 0, 0,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH, L"Robo");

	//далее просто создаем необходимые элементы управления
	if (this->m_hWndButton = CreateWindowEx(
		0,
		L"BUTTON",
		L"Открыть файл( WinExec() )",
		WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
		20, 70, 250, 40, this->m_hWnd, reinterpret_cast<HMENU>(MyApp::CTL_ID::OPENWINEXEC_ID), nullptr, nullptr
	); !this->m_hWndButton)
		throw runtime_error("Can't create WinExec button!"s);
	//отправим сообщения c флагом WM_SETFONT, которые установят шрифры для элементов управления
	SendMessage(this->m_hWndButton, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), TRUE);

	if (this->m_hWndButton = CreateWindowEx(
		0,
		L"BUTTON",
		L"Открыть файл( ShellExecute() )",
		WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
		320, 70, 250, 40, this->m_hWnd, reinterpret_cast<HMENU>(MyApp::CTL_ID::OPENSHELLEXEC_ID), nullptr, nullptr
	); !this->m_hWndButton)
		throw runtime_error("Can't create ShellExecute button!"s);
	//отправим сообщения c флагом WM_SETFONT, которые установят шрифры для элементов управления
	SendMessage(this->m_hWndButton, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), TRUE);

	if (this->m_hWndButton = CreateWindowEx(
		0,
		L"BUTTON",
		L"Открыть файл( CreateProcces() )",
		WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
		20, 140, 550, 40, this->m_hWnd, reinterpret_cast<HMENU>(MyApp::CTL_ID::OPENCREATEPROC_ID), nullptr, nullptr
	); !this->m_hWndButton)
		throw runtime_error("Can't create CreateProcces button!"s);
	//отправим сообщения c флагом WM_SETFONT, которые установят шрифры для элементов управления
	SendMessage(this->m_hWndButton, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), TRUE);

	if (this->m_hWndButton = CreateWindowEx(
		0,
		L"BUTTON",
		L"Закрыть последний процесс( CreateProcces() )",
		WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
		20, 190, 550, 40, this->m_hWnd, reinterpret_cast<HMENU>(MyApp::CTL_ID::CLOSEPROC_ID), nullptr, nullptr
	); !this->m_hWndButton)
		throw runtime_error("Can't create TerminateProcces button!"s);
	//отправим сообщения c флагом WM_SETFONT, которые установят шрифры для элементов управления
	SendMessage(this->m_hWndButton, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), TRUE);

	if (this->m_hWndButton = CreateWindowEx(
		0,
		L"BUTTON",
		L"Выйти из программы",
		WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
		175, 250, 240, 40, this->m_hWnd, reinterpret_cast<HMENU>(MyApp::CTL_ID::EXITPROG_ID), nullptr, nullptr
	); !this->m_hWndButton)
		throw runtime_error("Can't create EXIT button!"s);
	//отправим сообщения c флагом WM_SETFONT, которые установят шрифры для элементов управления
	SendMessage(this->m_hWndButton, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), TRUE);

	if (this->m_hWndEdit = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		L"EDIT",
		L"No one procces didn't start",
		WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
		10, 20, 570, 25, this->m_hWnd, reinterpret_cast<HMENU>(MyApp::CTL_ID::FILEEDIT_ID), nullptr, nullptr
	); !this->m_hWndEdit)
		throw runtime_error("Can't create edit!"s);
	//отправим сообщения c флагом WM_SETFONT, которые установят шрифры для элементов управления
	SendMessage(this->m_hWndEdit, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), TRUE);

}
