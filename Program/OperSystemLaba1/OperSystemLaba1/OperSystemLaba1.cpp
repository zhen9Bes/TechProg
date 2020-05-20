#include <iostream>
#include <Windows.h>
#include "WinUser.h"
#include "ShellAPI.h"


int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "ru");
	std::cout << "1 - Открыть файл \"1.docx\"" << std::endl;
	char* str = new char[256];
	std::cin >> str;
	if (str[0] == '1')
		ShellExecute(0, L"open", L"1.docx", NULL, NULL, SW_RESTORE);
	else
		std::cout << "Error! Incorrect input";	


	return 0;
}
