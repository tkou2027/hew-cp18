#include "draw.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

static Pixel* buffer_draw;
static Pixel* buffer_display[2];
static float* buffer_z;
//static Pixel* buffer_display1;
static int buffer_width = 0;
static int buffer_height = 0;
static int buffer_size = 0;
// console buffers
static HANDLE console_buffer_default;
static HANDLE console_buffers[2];
static int console_buffer_current = 0;

// layers
#define MAX_LAYERS 5
static Pixel* layers[MAX_LAYERS];

static int fps = 0;

// utils pixel
bool pixel_if_equal(const Pixel* b1, const Pixel* b2) {
	return if_color_equal(b1->color_fore, b2->color_fore)
		&& if_color_equal(b1->color_back, b2->color_back)
		&& b1->text == b2->text
        && b1->fullwidth == b2->fullwidth;
}

// utils buffer
bool buffer_if_inside(int x, int y) {
    return x >= 0 && x < buffer_width
        && y >= 0 && y < buffer_height;
}

bool buffer_if_inside(Coord2 coord) {
	return coord.x >= 0 && coord.x < buffer_width
		&& coord.y >= 0 && coord.y < buffer_height;
}

void buffer_fill(Pixel* buffer, int width, int height) {
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			*(buffer + i * width + j) = Pixel{
				create_color(255, 255, 255),
				create_color(0, 0, 0),
				' '
			};
		}
	}
}

int buffer_get_width() {
	return buffer_width;
}

int buffer_get_height() {
	return buffer_height;
}

int buffer_get_size() {
    return buffer_size;
}

float *buffer_get_z_buffer() {
    return buffer_z;
}

// utils

// console utils
// @param x X座標(1～)
// @param y Y座標(1～)
void gotoxy(int x, int y)
{
    COORD c;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    c.X = x - 1;
    c.Y = y - 1;
    if (GetConsoleScreenBufferInfo(h, &csbi))
    {
        c.Y += csbi.srWindow.Top;
    }
    SetConsoleCursorPosition(h, c);
}

// コンソールウィンドウの表示色をRGB形式で変更
void set_color_foreground(int r, int g, int b) {
	wprintf(L"\x1b[38;2;%d;%d;%dm", r, g, b);
}

void set_color_foreground(Color c) {
	set_color_foreground(c.r, c.g, c.b);
}

// コンソールウィンドウの表示色をデフォルト値に戻す
void reset_color_foreground() {
    wprintf(L"\x1b[39m");
}

// コンソールウィンドウの背景色をRGB形式で変更
void set_color_background(int r, int g, int b) {
    wprintf(L"\x1b[48;2;%d;%d;%dm", r, g, b);
}

void set_color_background(Color c) {
	set_color_background(c.r, c.g, c.b);
}

// コンソールウィンドウの背景色をデフォルト値に戻す
void reset_color_background() {
    wprintf(L"\x1b[49m");
}


// mhoj buffer utils
void enableVirtualTerminalProcessing(HANDLE hConsole) {
    DWORD dwMode;
    GetConsoleMode(hConsole, &dwMode);
    SetConsoleMode(hConsole, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}

void setConsoleFontType(HANDLE hConsole, const wchar_t* fontName) {
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
    GetCurrentConsoleFontEx(hConsole, FALSE, &cfi);
    wcscpy_s(cfi.FaceName, LF_FACESIZE, fontName);
    SetCurrentConsoleFontEx(hConsole, FALSE, &cfi);
}
// mhoj buffer utils


//void copyBuffer(HANDLE hSrcBuffer, HANDLE hDestBuffer) {
//    CONSOLE_SCREEN_BUFFER_INFO srcInfo, destInfo;
//    COORD bufferSize;
//    CHAR_INFO* buffer;
//    SMALL_RECT readRegion, writeRegion;
//    COORD bufferCoord;
//
//    // Get buffer size and console screen info
//    GetConsoleScreenBufferInfo(hSrcBuffer, &srcInfo);
//    GetConsoleScreenBufferInfo(hDestBuffer, &destInfo);
//    bufferSize.X = srcInfo.dwSize.X;
//    bufferSize.Y = srcInfo.dwSize.Y;
//
//    // Allocate buffer for reading from source
//    buffer = (CHAR_INFO*)malloc(bufferSize.X * bufferSize.Y * sizeof(CHAR_INFO));
//    if (!buffer) {
//        fprintf(stderr, "Error: Memory allocation failed.\n");
//        return;
//    }
//
//    // Set read and write regions
//    readRegion.Top = 0;
//    readRegion.Left = 0;
//    readRegion.Bottom = bufferSize.Y - 1;
//    readRegion.Right = bufferSize.X - 1;
//    writeRegion = readRegion;  // Same for writing
//
//    // Read from source buffer
//    bufferCoord.X = 0;
//    bufferCoord.Y = 0;
//    ReadConsoleOutput(hSrcBuffer, buffer, bufferSize, bufferCoord, &readRegion);
//
//    // Write to destination buffer
//    bufferCoord.X = 0;
//    bufferCoord.Y = 0;
//    WriteConsoleOutput(hDestBuffer, buffer, bufferSize, bufferCoord, &writeRegion);
//
//    free(buffer);
//}
//void fastCopyBuffer(HANDLE hSrc, HANDLE hDest) {
//    CONSOLE_SCREEN_BUFFER_INFO info;
//    GetConsoleScreenBufferInfo(hSrc, &info);
//
//    COORD bufferSize = info.dwSize;
//    CHAR_INFO* buffer = (CHAR_INFO*)malloc(bufferSize.X * bufferSize.Y * sizeof(CHAR_INFO));
//
//    if (buffer == NULL) {
//        fprintf(stderr, "Memory allocation failed.\n");
//        return;
//    }
//
//    SMALL_RECT readRegion = { 0, 0, bufferSize.X - 1, bufferSize.Y - 1 };
//    SMALL_RECT writeRegion = readRegion;
//
//    COORD bufferCoord = { 0, 0 };
//    ReadConsoleOutput(hSrc, buffer, bufferSize, bufferCoord, &readRegion);
//    WriteConsoleOutput(hDest, buffer, bufferSize, bufferCoord, &writeRegion);
//
//    free(buffer);
//}

// buffer life
void buffer_init(int width, int height) {
	buffer_width = width;
	buffer_height = height;
    buffer_size = width * height;
	size_t buffer_size_pixel = sizeof(Pixel) * width * height;
	buffer_draw = (Pixel*)malloc(buffer_size_pixel);
	buffer_display[0] = (Pixel*)malloc(buffer_size_pixel);
    buffer_display[1] = (Pixel*)malloc(buffer_size_pixel);
	buffer_fill(buffer_draw, width, height);
    buffer_z = (float*)calloc(buffer_size, sizeof(float));
	// buffer_fill(buffer_display, width, height);
	 //reset_color_foreground();
	 //reset_color_background();
	 //clrscr();
    for (int i = 0; i < 2; ++i) {
        console_buffers[i] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
        enableVirtualTerminalProcessing(console_buffers[i]);
        // setConsoleFontType(console_buffers[i], L"Courier New");
        WriteConsoleW(console_buffers[i], L"\x1b[?25l", 7, NULL, NULL);
    }
    
    //if (hBuffer1 == INVALID_HANDLE_VALUE || hBuffer2 == INVALID_HANDLE_VALUE) {
    //    fprintf(stderr, "Error: Unable to create screen buffers.\n");
    //    return 1;
    //}
}

void buffer_clear() {
	buffer_fill(buffer_draw, buffer_width, buffer_height);
}

void buffer_destroy() {
	free(buffer_draw);
	free(buffer_display[0]);
    free(buffer_display[1]);
	buffer_draw = NULL;
	buffer_display[0] = NULL;
    buffer_display[1] = NULL;
}

void buffer_draw_update() {
    // int prev_ouput_mode = _setmode(_fileno(stdout), _O_U8TEXT);
	for (int i = 0; i < buffer_height; ++i) {
		for (int j = 0; j < buffer_width; ++j) {
			int offset = i * buffer_width + j;
			Pixel *pixel_draw = buffer_draw + offset;
			Pixel *pixel_display = buffer_display[0] + offset;
			if (!pixel_if_equal(pixel_draw, pixel_display)) {
				*pixel_display = Pixel{
					pixel_draw->color_fore,
					pixel_draw->color_back,
					pixel_draw->text
				};
				gotoxy(j + 1, i + 1);
				set_color_foreground(pixel_draw->color_fore.r, pixel_draw->color_fore.g, pixel_draw->color_fore.b);
				set_color_background(pixel_draw->color_back.r, pixel_draw->color_back.g, pixel_draw->color_back.b);
                printf("%c", pixel_draw->text);
			}
		}
	}
	buffer_clear();
	reset_color_foreground();
	reset_color_background();
    // (void)_setmode(_fileno(stdout), prev_ouput_mode);
}

void buffer_draw_swap() {
    // fastCopyBuffer(console_buffers[1 - console_buffer_current], console_buffers[console_buffer_current]);
    // copyBuffer(console_buffers[1 - console_buffer_current], console_buffers[console_buffer_current]);
    for (int i = 0; i < buffer_height; ++i) {
        for (int j = 0; j < buffer_width; ++j) {
            int offset = i * buffer_width + j;
            Pixel* pixel_draw = buffer_draw + offset;
            // Pixel* pixel_display = buffer_display + offset;
            Pixel* pixel_display = buffer_display[console_buffer_current] + offset;
            if (!pixel_if_equal(pixel_draw, pixel_display)) {
                *pixel_display = Pixel{
                    pixel_draw->color_fore,
                    pixel_draw->color_back,
                    pixel_draw->text,
                    pixel_draw->fullwidth
                };
                if (pixel_draw->fullwidth) {
                    continue;
                }
                const wchar_t* pixel_print_template = L"\x1b[%d;%dH\x1b[38;2;%d;%d;%dm\x1b[48;2;%d;%d;%dm%lc";
                wchar_t pixel_print[256];
                wsprintfW(
                    pixel_print,
                    pixel_print_template,
                    i + 1,
                    j + 1,
                    pixel_draw->color_fore.r,
                    pixel_draw->color_fore.g,
                    pixel_draw->color_fore.b,
                    pixel_draw->color_back.r,
                    pixel_draw->color_back.g,
                    pixel_draw->color_back.b,
                    pixel_draw->text
                );
                DWORD written;
                WriteConsoleW(console_buffers[console_buffer_current], pixel_print, wcslen(pixel_print), NULL, NULL);
            }
            // full width texts
            //if (pixel_draw->text == L'\u03c9') {
            //    j++;
            //}
        }
    }
    #ifdef _DEBUG
    wchar_t debug_print[256];
    wsprintfW(debug_print, L"\x1b[1;1H\x1b[31m\x1b[40mFPS:%d", fps);
    WriteConsoleW(console_buffers[console_buffer_current], debug_print, wcslen(debug_print), NULL, NULL);
    #endif
    buffer_clear();
    SetConsoleActiveScreenBuffer(console_buffers[console_buffer_current]);
    console_buffer_current = 1 - console_buffer_current;
}

#ifdef _DEBUG
// =============================================================================
// デバッグ表示
// =============================================================================
void DispFPSBuffer(int fpsCounter)
{
    fps = fpsCounter;
    // fprintf(stderr, "FPS:%d\n", fpsCounter);
}
#endif


// draw

// pixel
void draw_pixel(int x, int y, Pixel pixel) {
    if (buffer_if_inside(x, y)) {
        *(buffer_draw + y * buffer_width + x) = Pixel{
            pixel.color_fore,
            pixel.color_back,
            pixel.text,
            pixel.fullwidth
        };
    }
}

void draw_pixel(Coord2 pos, Pixel pixel) {
	if (buffer_if_inside(pos)) {
		*(buffer_draw + pos.y * buffer_width + pos.x) = Pixel{
			pixel.color_fore,
			pixel.color_back,
			pixel.text,
            pixel.fullwidth
		};
	}
}

Pixel* buffer_get_pixel_at(int x, int y) {
    if (buffer_if_inside(x, y)) {
        return buffer_draw + y * buffer_width + x;
    }
    return NULL;
}

Pixel *buffer_get_pixel_at(Coord2 pos) {
    if (buffer_if_inside(pos)) {
        return buffer_draw + pos.y * buffer_width + pos.x;
    }
    return NULL;
}

float buffer_z_get_at(int x, int y) {
    if (buffer_if_inside(x, y)) {
        return buffer_z[y * buffer_width + x];
    }
    return NULL;
}

void buffer_z_set_at(int x, int y, float value) {
    if (buffer_if_inside(x, y)) {
        buffer_z[y * buffer_width + x] = value;
    }
}

void buffer_z_clear() {
    memset(buffer_z, 0, sizeof(float) * buffer_size);
    //for (int i = 0; i < buffer_size; ++i) {
    //    *(buffer_z + i) = -1;
    //}
}

// pixel
void draw_pixel_alpha(int x, int y, Pixel pixel) {
    if (pixel.text != L' ' && buffer_if_inside(x, y)) {
        Pixel* p_target = buffer_draw + y * buffer_width + x;
        p_target->color_fore = pixel.color_fore;
        p_target->text = pixel.text;
    }
}

void draw_pixel_alpha(Coord2 pos, Pixel pixel) {
	if (pixel.text != L' ' && buffer_if_inside(pos)) {
		Pixel* p_target = buffer_draw + pos.y * buffer_width + pos.x;
		p_target->color_fore = pixel.color_fore;
		p_target->text = pixel.text;
	}
}

void draw_fill(Coord2 pos, Pixel pixel, int width, int height) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            draw_pixel(Coord2{ j + pos.x, i + pos.y }, pixel);
        }
    }
}

void draw_string(int x, int y, const wchar_t* s, Color color_fore, Color color_back) {
    int len = wcslen(s);
    for (int i = 0; i < len; ++i) {
        draw_pixel(x + i, y, create_pixel(s[i], color_fore, color_back));
    }
}

void draw_string_fullwidth(int x, int y, const wchar_t* s, Color color_fore, Color color_back) {
    int len = wcslen(s);
    for (int i = 0; i < len; ++i) {
        draw_pixel(x + i * 2, y, create_pixel(s[i], color_fore, color_back));
        draw_pixel(x + i * 2 + 1, y, create_pixel_fullwidth());
    }
}

// line
void draw_line(Coord2 pos1, Coord2 pos2, Pixel pixel, bool reverse) {
	float absDeltaX = abs(pos1.x - pos2.x);
	float absDeltaY = abs(pos1.y - pos2.y);
    int direction = reverse ? -1 : 1;

	float rate = 0;
	if (absDeltaX == 0 && absDeltaY == 0) {
		draw_pixel(pos1, pixel);
	}
	else if (absDeltaX >= absDeltaY) {
		bool ordered = (pos1.x < pos2.x) ^ reverse;
		auto start = ordered ? pos1 : pos2;
		auto end = ordered ? pos2 : pos1;
		float deltaX = end.x - start.x;
		float deltaY = end.y - start.y;
		float rate = deltaY / deltaX;
		float extraY = 0;
		int drawY = start.y;
        // for (int x = start.x; x <= end.x; ++x) {
		for (int x = start.x; (!reverse && x <= end.x) || (reverse && x >= end.x); x += direction) {
			draw_pixel(Coord2{ x, drawY }, pixel);
			extraY += rate;
			if (extraY >= 1) {
			    drawY += direction;
				extraY = extraY - 1;
			}
			else if (extraY <= -1) {
			    drawY -= direction;
				extraY = extraY + 1;
			}
		}
	}
	else {
		bool ordered = (pos1.y < pos2.y) ^ reverse;
		auto start = ordered ? pos1 : pos2;
		auto end = ordered ? pos2 : pos1;
		float deltaX = end.x - start.x;
		float deltaY = end.y - start.y;
		float rate = deltaX / deltaY;
		float extraX = 0;
		int drawX = start.x;
		// for (int y = start.y; y <= end.y; ++y) {
        for (int y = start.y; (!reverse && y <= end.y) || (reverse && y >= end.y); y += direction) {
			draw_pixel(Coord2{ drawX, y}, pixel);
			extraX += rate;
			if (extraX >= 1) {
				drawX += direction;
				extraX = extraX - 1;
			}
			else if (extraX <= -1) {
				drawX -= direction;
				extraX = extraX + 1;
			}
		}
	}
}

void draw_sprite(Coord2 pos, const Sprite* sprite) {
    for (int i = 0; i < sprite->height; ++i) {
        for (int j = 0; j < sprite->width; ++j) {
            draw_pixel(
                Coord2{pos.x + j, pos.y + i},
                sprite->pixels[i * sprite->width + j]
            );
        }
    }
}

void draw_sprite_alpha(Coord2 pos, const Sprite* sprite) {
    for (int i = 0; i < sprite->height; ++i) {
        for (int j = 0; j < sprite->width; ++j) {
            draw_pixel_alpha(
                Coord2{ pos.x + j, pos.y + i },
                sprite->pixels[i * sprite->width + j]
            );
        }
    }
}


void draw_color(Coord2 pos, const Color color) {
    if (buffer_if_inside(pos)) {
        Pixel* p_target = buffer_draw + pos.y * buffer_width + pos.x;
        p_target->color_fore = color;
    }
}

void draw_sprite_color(Coord2 pos, const Sprite* sprite) {
    for (int i = 0; i < sprite->height; ++i) {
        for (int j = 0; j < sprite->width; ++j) {
            draw_color(
                Coord2{ pos.x + j, pos.y + i },
                sprite->pixels[i * sprite->width + j].color_fore
            );
        }
    }
}