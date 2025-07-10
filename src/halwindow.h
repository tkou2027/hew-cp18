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

//�J�[�\���\���ݒ�(0:��\�� 1:�\��)
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

// �R���\�[���E�B���h�E�̃^�C�g����ݒ�
//void setConsoleTitle(const char* title) {
//    printf("\x1b]0;%s\x07", title);
//}

// �R���\�[���E�B���h�E�̃^�C�g�����f�t�H���g�l�ɖ߂�
//void resetConsoleTitle() {
//    printf("\x1b]0;\x07");
//}

// �R���\�[���E�B���h�E�T�C�Y�̕ύX
void setConsoleWindowSize(int width, int height) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SMALL_RECT windowSize = { 0, 0, width - 1, height - 1 };
    SetConsoleWindowInfo(hOut, TRUE, &windowSize);
}

// �R���\�[���E�B���h�E�T�C�Y���f�t�H���g�l�ɖ߂�
void resetConsoleWindowSize() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hOut, &csbi);
    SMALL_RECT windowSize = { 0, 0, csbi.dwSize.X - 1, csbi.dwSize.Y - 1 };
    SetConsoleWindowInfo(hOut, TRUE, &windowSize);
}

// �X�N���[���o�b�t�@�T�C�Y�̕ύX
void setScreenBufferSize(int width, int height) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD newSize;
    newSize.X = width;
    newSize.Y = height;
    SetConsoleScreenBufferSize(hOut, newSize);
}

// �X�N���[���o�b�t�@�T�C�Y���f�t�H���g�l�ɖ߂�
void resetScreenBufferSize() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD defaultSize = { 80, 25 }; // �f�t�H���g�̃T�C�Y�ɐݒ�
    SetConsoleScreenBufferSize(hOut, defaultSize);
}

// �R���\�[���E�B���h�E����ʂ̒����Ɉړ�����֐�
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

// �R���\�[���E�B���h�E�|�W�V�����̕ύX
void setConsoleWindowPosition(int x, int y) {
    HWND hwnd = GetConsoleWindow();
    SetWindowPos(hwnd, 0, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

// �R���\�[���E�B���h�E�|�W�V�������f�t�H���g�l�ɖ߂�
void resetConsoleWindowPosition() {
    HWND hwnd = GetConsoleWindow();
    SetWindowPos(hwnd, 0, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

// �t�H���g�^�C�v�̕ύX
void setConsoleFontType(const wchar_t* fontName) {
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    GetCurrentConsoleFontEx(hOut, FALSE, &cfi);
    wcscpy_s(cfi.FaceName, LF_FACESIZE, fontName);
    SetCurrentConsoleFontEx(hOut, FALSE, &cfi);
}

// �t�H���g�^�C�v���f�t�H���g�l�ɖ߂�
void resetConsoleFontType() {
    setConsoleFontType(L"Consolas");
}

// �t�H���g�T�C�Y�̕ύX
void setConsoleFontSize(int size) {
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    GetCurrentConsoleFontEx(hOut, FALSE, &cfi);
    cfi.dwFontSize.Y = size;
    SetCurrentConsoleFontEx(hOut, FALSE, &cfi);
}

// �t�H���g�T�C�Y���f�t�H���g�l�ɖ߂�
void resetConsoleFontSize() {
    setConsoleFontSize(16); // �f�t�H���g�̃t�H���g�T�C�Y�ɐݒ�
}

// ���݂̃R���\�[���ݒ��\��
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

    //enableVTMode(); // VT�V�[�P���X���[�h��L����
    //setcursortype(0); // �J�[�\�����\���ɐݒ�

    //// �R���\�[���E�B���h�E�̃^�C�g����ݒ�
    //SetConsoleTitle(title); // consoleapi2
    //// �R���\�[���E�B���h�E�T�C�Y��80x25�ɕύX
    //setConsoleWindowSize(width, height);
    //// �X�N���[���o�b�t�@�T�C�Y��80x25�ɕύX
    //setScreenBufferSize(width, height);

    // Unicode
    //SetConsoleOutputCP(65001);
    //setConsoleFontType(L"MS Gothic");
    //setConsoleFontSize(37);

    // font size
    // setConsoleWindowPosition(0, 0);
    // �����F�𔒂ɕύX
    // setTextColorRGB(0, 255, 0);
    // �w�i�F�����ɕύX
    // setBackgroundColorRGB(0, 0, 0);

    // �R���\�[���E�B���h�E�|�W�V������ύX
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
    //// ���t�H���g�T�C�Y�̂ݕύX��
    //cfi.dwFontSize.X = 12;						// Width of each character in the font
    //cfi.dwFontSize.Y = 24;						// Height
    //// ���t�H���g�T�C�Y�̂ݕύX��
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
    //�J�[�\����\���ɐݒ�
    setcursortype(1);
    // VT�V�[�P���X���[�h�𖳌���
    disableVTMode();
    */
}