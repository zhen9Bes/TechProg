#include <iostream>
//#include <thread>
//#include <chrono>
//#include <ShellApi.h>
//#include <WinUser.h>
#include <Windows.h>

void PrintMenu();
int ReadIn();
int main() {

	setlocale(LC_ALL, "ru");
	PrintMenu();	
	int in;
		
	in = ReadIn();

	switch (in)
	{
	case 1:
		/*BOOL CreateProcess(
			LPCSTR lpApplicationName,
			LPSTR  lpCommandLine,
			LPSECURITY_ATTRIBUTES lpProcessAttributes,
			LPSECURITY_ATTRIBUTES lpThreadAttributes,
			BOOL bInheritHandles,
			DWORD dwCreationFlags,
			LPVOID lpEnvironment,
			LPCSTR lpCurrentDirectory,
			LPSTARTUPINFO lpStartupInfo,
			LPPROCESS_INFORMATION lpProcessInformation
		);*/

		STARTUPINFO          sinfo;
		PROCESS_INFORMATION  pinfo;
		memset(&pinfo, 0, sizeof(pinfo));
		memset(&sinfo, 0, sizeof(sinfo));
		sinfo.cb = sizeof(sinfo);

		if (CreateProcess(L"C:\\Users\\Jacky\\Desktop\\Evgeniy Kochetkov\\4semak\\OperSys\\Hello\\Debug\\Hello.exe", NULL,
			NULL, NULL, FALSE, 0, NULL, NULL, &sinfo, &pinfo) == TRUE)
		{
			std::cout << "Process" << std::endl;
			std::cout << "Handle " << pinfo.hProcess << std::endl;
		}
		break;
	case 2:
		
		break;
	case 3:

		break;
	}
	system("pause");
	return 0; 
}

void PrintMenu() {
	system("cls");
	std::cout << "1 - запустить дочерний процесс, используя функцию CreateProcess()" << std::endl;
	std::cout << "2 - закрыть дочерний процесс" << std::endl;
	std::cout << "3 - завершение программы" << std::endl;
	std::cout << "Введите номер команды: ";	
}
int ReadIn() {
	int in;
	char ch;
	std::cin >> ch;
	if (ch < '1' || ch > '3') {
		system("cls");
		std::cout << "Неопознанная команда. Попробуйте снова..." << std::endl;		
		system("pause");
		PrintMenu();
		ReadIn();
	}
	in = atoi(&ch);
	return in;
}