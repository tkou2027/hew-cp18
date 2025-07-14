#pragma once

#include <string>
#include <cstddef> // size_t
#include <windows.h> // HANDLE
#include <array>
#include <memory>

#include "pixel.h"

namespace hoge
{
	class Buffer
	{
	public:
		void Initialize(int width, int height);
		void Finalize();
		void PresentSwap();
		// Draw Functions
		void DrawPixel(int x, int y, const Pixel& pixel);
		void DrawString(int x, int y, const std::wstring text, const Color& color_fore, const Color& color_back);
		void DrawStringFullwidth(int x, int y, const std::wstring text, const Color& color_fore, const Color& color_back);
		void SetZBuffer(int x, int y, float value);
		void ClearZBuffer();
	private:
		constexpr static std::size_t BUFFER_CNT{ 2 };

		static HANDLE InitializeConsoleBuffer();
		std::wstring GetPixelOutput(const Pixel& pixel, int row, int col);

		std::size_t m_width{ 0 };
		std::size_t m_height{ 0 };
		std::size_t m_size{ 0 };

		std::size_t m_buffer_index{ 0 };
		// use console screen buffer to display all pixel at once
		std::array<HANDLE, BUFFER_CNT> m_console_buffers;
		// stores pixels written to each console screen buffer
		// to be compared with m_draw_buffer,
		// only pixel that changed will be written to console screen buffer
		std::array<std::unique_ptr<Pixel[]>, BUFFER_CNT> m_display_buffers;
		// draw functions will modify this buffer
		std::unique_ptr<Pixel[]> m_draw_buffer;
		std::unique_ptr<float[]> m_z_buffer;
	};
}