#define _CRT_SECURE_NO_WARNINGS
#include "sprite.h"

#include "global_context.h"
#include "buffer.h"

namespace hoge
{
	Sprite::~Sprite()
	{
		delete[] m_pixels;
	}

	void Sprite::Initialize(int width, int height)
	{
		m_width = width;
		m_height = height;
		m_pixels = new Pixel[width * height];
	}

	void Sprite::Load(const std::wstring& filename)
	{
		FILE* fp;
		fp = _wfopen(filename.c_str(), L"r, ccs=UTF-8");

		if (!fp) {
			// TODO: Error Log
			return;
		}

		const int line_buffer_size = 256;
		wchar_t line[line_buffer_size];
		// read first two line for metadata
		if (!fgetws(line, line_buffer_size, fp)
			|| !fgetws(line, line_buffer_size, fp)) {
			// TODO: Error Log
			return;
		}
		// split second line
		wchar_t seps[] = L" ,\t\n";
		wchar_t* token;
		wchar_t* next_token = NULL;

		// width
		token = wcstok_s(line, seps, &next_token);
		int width = _wtoi(token);
		// height
		token = wcstok_s(NULL, seps, &next_token);
		int height = _wtoi(token);

		Initialize(width, height);

		for (int i = 0; i < height; ++i) {
			// skip
			fgetws(line, line_buffer_size, fp);
		}
		for (int i = 0; i < width * height; ++i) {
			fgetws(line, line_buffer_size, fp);
			wchar_t text = line[0];
			int colors[6];
			for (int i = 0; i < 6; ++i) {
				token = wcstok_s(i == 0 ? line + 2 : NULL, seps, &next_token);
				colors[i] = _wtoi(token);
			}
			m_pixels[i] = Pixel{
				text,
				Color{ colors[0], colors[1], colors[2] },
				Color{ colors[3], colors[4], colors[5] }
			};
		}
	}

	void Sprite::Draw(int x, int y) const
	{
		for (int row = 0; row < m_height; ++row) {
			for (int col = 0; col < m_width; ++col) {
				g_global_context.m_buffer->DrawPixel(
					col + x, row + y, m_pixels[row * m_width + col]);
			}
		}
	}
}