#include "Game.hpp"
#include "Core.hpp"
#include "AssetManager.hpp"
#include <windows.h>
#include <thread>

#pragma comment(lib, "comctl32.lib")

wchar_t* winChar(const char* charArray)
{
	wchar_t* wString = new wchar_t[2048];
	MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 2048);
	return wString;
}

LRESULT CALLBACK WindowProcessMessages(HWND hwnd, UINT msg, WPARAM param, LPARAM lparam);

int WINAPI WinMain(HINSTANCE currentInstance, HINSTANCE previousInstance, PSTR cmdLine, INT cmdCount)
{
	new Game();
	return 0;
	// Register the window class
	const char* CLASS_NAME = "MainWClass";
	WNDCLASS wc{};
	wc.hInstance = currentInstance;
	wc.lpszClassName = winChar(CLASS_NAME);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpfnWndProc = WindowProcessMessages;
	RegisterClass(&wc);

	// Create the window
	CreateWindow(winChar(CLASS_NAME), winChar("Level Editor"),
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,			// Window style
		CW_USEDEFAULT, CW_USEDEFAULT,				// Window initial position
		800, 600,						// Window size
		nullptr, nullptr, nullptr, nullptr);

	// Window loop
	MSG msg{};
	while (GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

Game* mainGame = nullptr;
void createGame()
{
	new Game();
}

LRESULT CALLBACK WindowProcessMessages(HWND hwnd, UINT msg, WPARAM param, LPARAM lparam) {
	switch (msg) {
		case WM_CREATE:
		{
			HMENU hMenuBar = CreateMenu();

			HMENU hFile = CreateMenu();
			AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR) hFile, winChar("File"));
			AppendMenu(hFile, MF_STRING, 1, winChar("Run"));
			AppendMenu(hFile, MF_SEPARATOR, 1, NULL);
			AppendMenu(hFile, MF_STRING, 2, winChar("Exit"));

			HMENU hEdit = CreateMenu();
			AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR) hEdit, winChar("Edit"));

			HMENU hOptions = CreateMenu();
			AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR) hOptions, winChar("Options"));

			SetMenu(hwnd, hMenuBar);
			std::thread gameThread(createGame);
			gameThread.detach();
			break;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
			break;
		}
		case WM_COMMAND:
		{
			switch (LOWORD(param))
			{
				case (1):
					break;
				case (2):
					exit(0);
					break;
			}
		}
		default:
		{
			return DefWindowProc(hwnd, msg, param, lparam);
		}
	}
}