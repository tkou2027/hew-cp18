#pragma once
#include <string>
#include <windows.h>

namespace hoge
{
	struct WindowCreateInfo
	{
		int         width{ 80 };
		int         height{ 25 };
		const wchar_t* title{ L"hoge" };
	};

	class Window
	{
	public:
		void Initialize(const WindowCreateInfo& create_info);
		void Finalize();
	private:
		struct ConsoleWindowConfig
		{
			// buffer and window size
			COORD buffer_size{0, 0};
			SMALL_RECT buffer_window{0, 0, 0, 0};
			// font
			std::wstring font_name;
			SHORT font_size{0};
			// stdin console mode
			DWORD console_mode{0};
			// window style and position
			LONG window_style{ 0 };
			RECT window_pos{ 0, 0, 0, 0 };
		};

		ConsoleWindowConfig m_default_config{};

		// windows console utils
		static void GetConsoleWindowSize(ConsoleWindowConfig& config);
		static void SetConsoleWindowSize(const ConsoleWindowConfig& config);

		static void GetConsoleFont(ConsoleWindowConfig& config);
		static void SetConsoleFont(const ConsoleWindowConfig& config);

		static void GetConsoleInputMode(ConsoleWindowConfig& config);
		static void SetConsoleInputMode(const ConsoleWindowConfig& config);

		static void GetConsoleWindowStyle(ConsoleWindowConfig& config);
		static void SetConsoleWindowStyle(const ConsoleWindowConfig& config);

		static void SetFullScreen();
		static int GetFullScreenFontSize(int buffer_width, int buffer_height);
	};
}