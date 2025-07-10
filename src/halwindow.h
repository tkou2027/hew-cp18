#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <fcntl.h>
#include <io.h>

#if defined(_WIN64) || defined(_M_X64)
#ifndef _DEBUG
#define show 0
#pragma comment(lib, "./Library/HEW_x64_Release.lib")
#else
#define show 1
#pragma comment(lib, "./Library/HEW_x64_Debug.lib")
#endif

#elif defined(_WIN32) || defined(_M_IX86)
#ifndef _DEBUG
#define show 0
#pragma comment(lib, "./Library/HEW_x86_Release.lib")
#else
#define show 1
#pragma comment(lib, "./Library/HEW_x86_Debug.lib")
#endif
#endif

// Function prototype declarations
bool HEWStartup();
bool HEWCleanup();

struct ConsoleSetting {
    DWORD consoleMode;
    int outputMode;
};

//カーソル表示設定(0:非表示 1:表示)
void setConsoleCursorType(DWORD cursorType) {
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 100;
    cursorInfo.bVisible = TRUE;
    if (cursorType == 0) {
        cursorInfo.bVisible = FALSE;
    }
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

// コンソールウィンドウのタイトルを設定
//void setConsoleTitle(const char* title) {
//    printf("\x1b]0;%s\x07", title);
//}

// コンソールウィンドウのタイトルをデフォルト値に戻す
//void resetConsoleTitle() {
//    printf("\x1b]0;\x07");
//}

// コンソールウィンドウサイズの変更
void setConsoleWindowSize(int width, int height) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SMALL_RECT windowSize = { 0, 0, width - 1, height - 1 };
    SetConsoleWindowInfo(hOut, TRUE, &windowSize);
}

// コンソールウィンドウサイズをデフォルト値に戻す
void resetConsoleWindowSize() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hOut, &csbi);
    SMALL_RECT windowSize = { 0, 0, csbi.dwSize.X - 1, csbi.dwSize.Y - 1 };
    SetConsoleWindowInfo(hOut, TRUE, &windowSize);
}

// スクリーンバッファサイズの変更
void setScreenBufferSize(int width, int height) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD newSize;
    newSize.X = width;
    newSize.Y = height;
    SetConsoleScreenBufferSize(hOut, newSize);
}

// スクリーンバッファサイズをデフォルト値に戻す
void resetScreenBufferSize() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD defaultSize = { 80, 25 }; // デフォルトのサイズに設定
    SetConsoleScreenBufferSize(hOut, defaultSize);
}

// コンソールウィンドウを画面の中央に移動する関数
void centerConsoleWindow() {
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
void setConsoleWindowPosition(int x, int y) {
    HWND hwnd = GetConsoleWindow();
    SetWindowPos(hwnd, 0, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

// コンソールウィンドウポジションをデフォルト値に戻す
void resetConsoleWindowPosition() {
    HWND hwnd = GetConsoleWindow();
    SetWindowPos(hwnd, 0, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

// フォントタイプの変更
void setConsoleFontType(const wchar_t* fontName) {
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    GetCurrentConsoleFontEx(hOut, FALSE, &cfi);
    wcscpy_s(cfi.FaceName, LF_FACESIZE, fontName);
    SetCurrentConsoleFontEx(hOut, FALSE, &cfi);
}

// フォントタイプをデフォルト値に戻す
void resetConsoleFontType() {
    setConsoleFontType(L"Consolas");
}

// フォントサイズの変更
void setConsoleFontSize(int size) {
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    GetCurrentConsoleFontEx(hOut, FALSE, &cfi);
    cfi.dwFontSize.Y = size;
    SetCurrentConsoleFontEx(hOut, FALSE, &cfi);
}

// フォントサイズをデフォルト値に戻す
void resetConsoleFontSize() {
    setConsoleFontSize(16); // デフォルトのフォントサイズに設定
}

// 現在のコンソール設定を表示
void DisplayConsoleSettings() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hOut, &csbi);
    printf("Screen Buffer Size: %d x %d\n", csbi.dwSize.X, csbi.dwSize.Y);
    printf("Window Size: %d x %d\n", csbi.srWindow.Right - csbi.srWindow.Left + 1, csbi.srWindow.Bottom - csbi.srWindow.Top + 1);
    printf("Window Position: (%d, %d)\n", csbi.srWindow.Left, csbi.srWindow.Top);
}

void initWindow(ConsoleSetting* prevConsoleSetting, int width, int height, const wchar_t* title) {
    bool rtn;
    rtn = HEWStartup();
    if (rtn != 1) {
        perror("HEWStartup error");
        // return 0;  // FALSE
    }

    //enableVTMode(); // VTシーケンスモードを有効化
    //setcursortype(0); // カーソルを非表示に設定

    //// コンソールウィンドウのタイトルを設定
    //SetConsoleTitle(title); // consoleapi2
    //// コンソールウィンドウサイズを80x25に変更
    //setConsoleWindowSize(width, height);
    //// スクリーンバッファサイズを80x25に変更
    //setScreenBufferSize(width, height);

    // Unicode
    //SetConsoleOutputCP(65001);
    //setConsoleFontType(L"MS Gothic");
    //setConsoleFontSize(37);

    // font size
    // setConsoleWindowPosition(0, 0);
    // 文字色を白に変更
    // setTextColorRGB(0, 255, 0);
    // 背景色を黒に変更
    // setBackgroundColorRGB(0, 0, 0);

    // コンソールウィンドウポジションを変更
    // centerConsoleWindow();
    // setConsoleWindowPosition(0, 0);

    // mouse init
    HANDLE hStdin;
    DWORD fdwMode;
    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    // fdwMode = (ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT) & ~ENABLE_QUICK_EDIT_MODE;
    // SetConsoleMode(hStdin, fdwMode);
    DWORD prev_console_mode;
    GetConsoleMode(hStdin, &prev_console_mode);
    prevConsoleSetting->consoleMode = prev_console_mode;
    SetConsoleMode(hStdin, ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT | ENABLE_PROCESSED_INPUT);

    //// print init
    //// print 16bit chars;
    ////int prev_ouput_mode = _setmode(_fileno(stdout), _O_U16TEXT);
    ////prevConsoleSetting->outputMode = prev_ouput_mode;
    //// Set console font style and size
    //CONSOLE_FONT_INFOEX cfi;
    //cfi.cbSize = sizeof(cfi);
    //cfi.nFont = 0;
    //// ↓フォントサイズのみ変更可↓
    //cfi.dwFontSize.X = 12;						// Width of each character in the font
    //cfi.dwFontSize.Y = 24;						// Height
    //// ↑フォントサイズのみ変更可↑
    //cfi.FontFamily = FF_DONTCARE;
    //cfi.FontWeight = FW_NORMAL;
    //wcscpy_s(cfi.FaceName, L"MS Gothic");       // Choose your font
    //SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}

void resetWindow(const ConsoleSetting* prevcConsoleSetting) {
    //(void)_setmode(_fileno(stdout), prevcConsoleSetting->outputMode);

    HANDLE hStdin;
    DWORD fdwMode;
    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    SetConsoleMode(hStdin, prevcConsoleSetting->consoleMode);

    bool rtn = HEWCleanup();
    if (rtn != 1) {
        perror("HEWCleanup error");
        return;
        // return 0; // FALSE
    }
    /*
    resetConsoleFontSize();
    resetConsoleFontType();
    resetScreenBufferSize();
    resetConsoleWindowSize();
    // resetBackgroundColor();
    // resetTextColor();
    // resetConsoleTitle();
    //カーソルを表示に設定
    setcursortype(1);
    // VTシーケンスモードを無効化
    disableVTMode();
    */
}