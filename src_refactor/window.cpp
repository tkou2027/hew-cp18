#include "window.h"

#include <windows.h>

namespace
{
	// コンソールウィンドウを画面の中央に移動する関数
	void CenterConsoleWindow() {
		HWND hwnd = GetConsoleWindow();
		RECT rect;
		GetWindowRect(hwnd, &rect);
		int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = GetSystemMetrics(SM_CYSCREEN);
		int windowWidth = rect.right - rect.left;
		int windowHeight = rect.bottom - rect.top;
		int posX = (screenWidth - windowWidth) / 2;
		int posY = (screenHeight - windowHeight) / 2;
		MoveWindow(hwnd, posX, posY, windowWidth, windowHeight, TRUE);
	}

	// コンソールウィンドウポジションの変更
	void SetConsoleWindowPosition(int x, int y) {
		HWND hwnd = GetConsoleWindow();
		SetWindowPos(hwnd, 0, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	}

	// コンソールウィンドウポジションをデフォルト値に戻す
	void ResetConsoleWindowPosition() {
		HWND hwnd = GetConsoleWindow();
		SetWindowPos(hwnd, 0, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	}
}

namespace hoge
{
	void Window::Initialize(const WindowCreateInfo& create_info)
	{
		// save previous values
		GetConsoleWindowSize(m_default_config);
		GetConsoleWindowStyle(m_default_config);
		GetConsoleFont(m_default_config);
		GetConsoleInputMode(m_default_config);

		ConsoleWindowConfig config
		{
			{ create_info.width, create_info.height },
			{ 0, 0, create_info.width - 1, create_info.height - 1},
			L"MS Gothic",
			GetFullScreenFontSize(create_info.width, create_info.height),
			ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT | ENABLE_PROCESSED_INPUT
		};

		// Console Window Settings
		SetConsoleTitle(create_info.title);
		SetConsoleWindowSize(config);
		SetConsoleFont(config);
		SetConsoleInputMode(config);
		SetFullScreen();

		//// Unicode
		//SetConsoleOutputCP(65001);
	}

	void Window::Finalize()
	{
		SetConsoleWindowStyle(m_default_config);
		SetConsoleInputMode(m_default_config);
		SetConsoleFont(m_default_config);
		SetConsoleWindowSize(m_default_config);
	}


	// Windows Consol Utils
	void Window::GetConsoleWindowSize(ConsoleWindowConfig& config)
	{
		HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(handle_out, &csbi);
		config.buffer_size = csbi.dwSize;
		config.buffer_window = csbi.srWindow;
	}

	void  Window::SetConsoleWindowSize(const ConsoleWindowConfig& config)
	{
		HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
		// set buffer size first
		SetConsoleScreenBufferSize(handle_out, config.buffer_size);
		// then window coordinate (no larger than buffer size)
		SetConsoleWindowInfo(handle_out, TRUE, &config.buffer_window);
	}

	void Window::GetConsoleInputMode(ConsoleWindowConfig& config)
	{
		HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
		GetConsoleMode(handle_in, &config.console_mode);
	}

	void Window::SetConsoleInputMode(const ConsoleWindowConfig& config)
	{
		HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
		SetConsoleMode(handle_in, config.console_mode);
	}

	void Window::GetConsoleFont(ConsoleWindowConfig& config)
	{
		CONSOLE_FONT_INFOEX cfi;
		cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		GetCurrentConsoleFontEx(hOut, FALSE, &cfi);

		config.font_name = cfi.FaceName;
		config.font_size = cfi.dwFontSize.Y;
	}

	void Window::SetConsoleFont(const ConsoleWindowConfig& config)
	{
		CONSOLE_FONT_INFOEX cfi;
		cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
		HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
		GetCurrentConsoleFontEx(handle_out, FALSE, &cfi);
		// font size
		wcscpy_s(cfi.FaceName, LF_FACESIZE, config.font_name.c_str());
		cfi.dwFontSize.Y = config.font_size;
		SetCurrentConsoleFontEx(handle_out, FALSE, &cfi);
	}

	void Window::GetConsoleWindowStyle(ConsoleWindowConfig& config)
	{
		HWND hwnd = GetConsoleWindow();
		config.window_style = GetWindowLong(hwnd, GWL_STYLE);
		GetWindowRect(hwnd, &config.window_pos);
	}

	void Window::SetConsoleWindowStyle(const ConsoleWindowConfig& config)
	{
		HWND hwnd = GetConsoleWindow();
		SetWindowLong(hwnd, GWL_STYLE, config.window_style);
		SetWindowPos(hwnd, nullptr,
			config.window_pos.left,
			config.window_pos.top,
			config.window_pos.right - config.window_pos.left,
			config.window_pos.bottom - config.window_pos.top,
			SWP_FRAMECHANGED | SWP_NOZORDER | SWP_NOACTIVATE
		);
	}

	void Window::SetFullScreen()
	{
		HWND hwnd = GetConsoleWindow();
		LONG style = GetWindowLong(hwnd, GWL_STYLE);
		// remove title, thick frame, scroll
		style &= ~(WS_CAPTION | WS_THICKFRAME | WS_VSCROLL);
		SetWindowLong(hwnd, GWL_STYLE, style);

		// get screen size (primary monitor)
		int screen_width = GetSystemMetrics(SM_CXSCREEN);
		int screen_height = GetSystemMetrics(SM_CYSCREEN);
		// center window
		RECT rc;
		GetWindowRect(hwnd, &rc);
		int width = rc.right - rc.left;
		int height = rc.bottom - rc.top;
		int pos_x = (screen_width - width) / 2;
		int pos_y = (screen_height - height) / 2;
		SetWindowPos(hwnd, nullptr, pos_x, pos_y, 0, 0,
			SWP_FRAMECHANGED | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
	}
	
	int Window::GetFullScreenFontSize(int buffer_width, int buffer_height)
	{
		int screen_width = GetSystemMetrics(SM_CXSCREEN);
		int screen_height = GetSystemMetrics(SM_CYSCREEN);

		int horizontal_max = screen_width / buffer_width * 2; // font width : height = 1 : 2
		int vertical_max = screen_height / buffer_height;

		// return the smaller one
		return vertical_max < horizontal_max ? vertical_max : horizontal_max;
	}
}
