#include "buffer.h"

#include <sstream>

namespace hoge
{
	void Buffer::Initialize(int width, int height)
	{
		m_width = width;
		m_height = height;
		m_size = width * height;

		// create buffers
		// console screen buffers
		for (auto& handle : m_console_buffers) {
			handle = InitializeConsoleBuffer();
		}
		// pixel double buffers
		for (auto& buffer : m_display_buffers)
		{
			buffer = std::make_unique<Pixel[]>(m_size);
			// init first frame
			std::fill_n(buffer.get(), m_size, Pixel{ Color{255, 255, 255} });
		}
		m_draw_buffer = std::make_unique<Pixel[]>(m_size);

		// z buffer
		m_z_buffer = std::make_unique<float[]>(m_size);
	}

	void Buffer::Finalize()
	{
		// moved to smart pointers thus no need to release
		// just as a record of the orignal design...
		for (auto& buffer : m_display_buffers)
		{
			buffer.reset(); // delete []buffer;
		}
		m_draw_buffer.reset();
		m_z_buffer.reset();
	}

	void Buffer::PresentSwap()
	{
		// write to console buffer when changed
		for (int i = 0; i < m_height; ++i)
		{
			for (int j = 0; j < m_width; ++j)
			{
				int offset = i * m_width + j;
				const auto& pixel_draw = m_draw_buffer[offset];
				auto& pixel_display = m_display_buffers[m_buffer_index][offset];
				if (pixel_draw == pixel_display)
				{
					continue;
				}
				// copy value
				pixel_display = Pixel{ pixel_draw };
				// previous pixel is fullwith, skip
				if (pixel_draw.IfSkipped())
				{
					continue;
				}
				const std::wstring pixel_output = GetPixelOutput(pixel_draw, i, j);
				WriteConsole(m_console_buffers[m_buffer_index],
					pixel_output.c_str(), pixel_output.length(), NULL, NULL);
			}
		}
		// swap console buffers
		SetConsoleActiveScreenBuffer(m_console_buffers[m_buffer_index]);
		// clear draw buffer
		std::fill_n(m_draw_buffer.get(), m_size, Pixel{});
		m_buffer_index = 1 - m_buffer_index;
	}

	void Buffer::DrawPixel(int x, int y, const Pixel& pixel)
	{
		if (x >= 0 && x < m_width && y >= 0 && y < m_height)
		{
			m_draw_buffer[y * m_width + x] = Pixel{ pixel };
		}
	}

	void Buffer::DrawString(int x, int y, const std::wstring text, const Color& color_fore, const Color& color_back)
	{
		for (int i = 0; i < text.length(); ++i) {
			DrawPixel(x + i, y, Pixel{ text[i], color_fore, color_back });
		}
	}

	void Buffer::DrawStringFullwidth(int x, int y, const std::wstring text, const Color& color_fore, const Color& color_back)
	{
		for (int i = 0; i < text.length(); ++i) {
			DrawPixel(x + i * 2, y, Pixel{ text[i], color_fore, color_back });
			DrawPixel(x + i * 2 + 1, y, Pixel::CreateSkipped());
		}
	}

	void Buffer::SetZBuffer(int x, int y, float value)
	{
		if (x >= 0 && x < m_width && y >= 0 && y < m_height)
		{
			m_z_buffer[y * m_width + x] = value;
		}
	}

	void Buffer::ClearZBuffer()
	{
		std::fill_n(m_z_buffer.get(), m_size, 0);
	}

	// Buffer Utils ====
	HANDLE Buffer::InitializeConsoleBuffer()
	{
		HANDLE handle = CreateConsoleScreenBuffer(
			GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		// Enable virtual terminal processing
		DWORD mode;
		GetConsoleMode(handle, &mode);
		SetConsoleMode(handle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
		// make cursor invisible
		const std::wstring escape_hide_cursor{ L"\033[?25l" };
		WriteConsoleW(handle, escape_hide_cursor.c_str(), escape_hide_cursor.length(), NULL, NULL);
		return handle;
	}

	std::wstring Buffer::GetPixelOutput(const Pixel& pixel, int row, int col)
	{
		std::wstringstream ss;
		ss << L"\033[" << row + 1 << L";" << col + 1 << L"H"
			<< L"\033[38;2;" << pixel.m_color_fore.m_r << L";" << pixel.m_color_fore.m_g << L";" << pixel.m_color_fore.m_b << L"m"
			<< L"\033[48;2;" << pixel.m_color_back.m_r << L";" << pixel.m_color_back.m_g << L";" << pixel.m_color_back.m_b << L"m"
			<< pixel.m_text;
		return ss.str();
	}
}

