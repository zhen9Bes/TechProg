#include <Windows.h>
#define button1 1

int CALLBACK wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR szCmdLine, int nCmdShow) 
{
	MSG msg{};
	HWND hwnd{};
	WNDCLASSEX wc{ sizeof(WNDCLASSEX) };
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = [](HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)->LRESULT
	{
		switch (uMsg)
		{
			case WM_CREATE: 
			{
				CreateWindow(TEXT("STATIC"), TEXT("Привет!!! Здесь оформление как в WIN98 наверное :D"), // Label tipo 
					WS_VISIBLE | WS_CHILD,
					80, 100, 250, 50,
					hwnd, (HMENU)3, NULL, NULL);

				CreateWindow(TEXT("button"), TEXT("Обычная кнопка"),
					WS_VISIBLE | WS_CHILD,
					100, 250, 200, 30,
					hwnd, (HMENU)button1, NULL, NULL);

				CreateWindow(TEXT("edit"), TEXT("Типо лайнэдит"),
					WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
					100, 400, 200, 30, hwnd, (HMENU)2, NULL, NULL);

				
			}
			return 0;

			case WM_COMMAND:
			{
				if (LOWORD(wParam) == button1) {
					MessageBox(0, L"Кнопка сработала", L"Сообщение", MB_OK | MB_ICONINFORMATION);
				}
			}
			return 0;	

			case WM_DESTROY:
			{
				PostQuitMessage(EXIT_FAILURE);
			}
			return 0;
		}
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	};
	wc.lpszClassName = L"MyAppClass";
	wc.lpszMenuName = nullptr;
	wc.style = CS_VREDRAW | CS_HREDRAW;

	if (!RegisterClassEx(&wc))
		return EXIT_FAILURE;

	if (hwnd = CreateWindow(
		wc.lpszClassName,
		L"Лабораторная работа №1 Дочерный процесс \"Привет\"", WS_OVERLAPPEDWINDOW,
		600, 200, 400, 600, nullptr, nullptr, hInstance, nullptr);
		hwnd == INVALID_HANDLE_VALUE)
		return EXIT_FAILURE;

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return static_cast<int>(msg.wParam);
}