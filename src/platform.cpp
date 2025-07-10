// 校内 Win32 コンソール I/O 拡張ヘッダ を参考
#define _CRT_SECURE_NO_WARNINGS
#include "platform.h"
#include <windows.h>
#include <malloc.h>
#include <digitalv.h> // MCI_DGV_PLAY_REPEAT

struct SoundInfo {
	int				nDevType;
	MCIDEVICEID		wDeviceID;
	char			szPath[MAX_PATH];
	int				nRepeat;
};

static DWORD g_dwKeyMap[8];

int platform_opensound(char* path)
{
	const char	szMP3Ext[] = ".mp3";
	char		szExt[_MAX_EXT];
	union {
		MCI_WAVE_OPEN_PARMSA	wop;
		MCI_OPEN_PARMSA			op;
	} m;
	DWORD		dwCmd;
	SoundInfo* psi;

	psi = (SoundInfo*)malloc(sizeof(SoundInfo));
	if (psi == NULL)
		return 0;
	ZeroMemory(psi, sizeof(*psi));

	ZeroMemory(&m, sizeof(m));
	_splitpath(path, NULL, NULL, NULL, szExt);
	_strlwr(szExt);
	dwCmd = MCI_OPEN_TYPE | MCI_OPEN_ELEMENT;
	// mp3 only
	if (strstr(szMP3Ext, szExt)) {
		psi->nDevType = MCI_DEVTYPE_DIGITAL_VIDEO;
		lstrcpynA(psi->szPath, path, MAX_PATH);
		m.op.lpstrDeviceType = "MPEGVideo";
		m.op.lpstrElementName = psi->szPath;
	}
	else {
		free(psi);
		return 0;
	}
	if (mciSendCommandA(0, MCI_OPEN, dwCmd, (DWORD)&m)) {
		free(psi);
		return 0;
	}
	psi->wDeviceID = m.op.wDeviceID;
	return (int)psi;
}

void platform_closesound(int hsound)
{
	SoundInfo* psi;

	if (!hsound)
		return;
	psi = (SoundInfo*)hsound;
	if (psi->wDeviceID) {
		mciSendCommand(psi->wDeviceID, MCI_CLOSE, 0, 0);
		psi->wDeviceID = 0;
	}
	free(psi);
}

void platform_playsound(int hsound, int repeat)
{
	SoundInfo* psi;
	DWORD					dwCmd;
	MCI_PLAY_PARMS			mpp;

	if (!hsound)
		return;
	psi = (SoundInfo*)hsound;
	if (!psi->wDeviceID)
		return;
	psi->nRepeat = repeat;
	ZeroMemory(&mpp, sizeof(mpp));
	dwCmd = 0;
	if (repeat) {
		switch (psi->nDevType) {
		case MCI_DEVTYPE_DIGITAL_VIDEO:
			dwCmd |= (MCI_FROM | MCI_DGV_PLAY_REPEAT);
			mpp.dwFrom = 0;
			break;
		case MCI_DEVTYPE_SEQUENCER:
		case MCI_DEVTYPE_WAVEFORM_AUDIO:
			break;
		default:
			break;
		}
	}
	mciSendCommand(psi->wDeviceID, MCI_SEEK, MCI_SEEK_TO_START, 0);
	mciSendCommand(psi->wDeviceID, MCI_PLAY, dwCmd, (DWORD)&mpp);
}

void platform_stopsound(int hsound)
{
	SoundInfo* psi;

	if (!hsound)
		return;
	psi = (SoundInfo*)hsound;
	if (!psi->wDeviceID)
		return;
	psi->nRepeat = 0;
	mciSendCommand(psi->wDeviceID, MCI_STOP, MCI_WAIT, 0);
	mciSendCommand(psi->wDeviceID, MCI_SEEK, MCI_SEEK_TO_START, 0);
}

int platform_getkey(int port)
{
	DWORD	dwEvent;
	HANDLE	h;

	// キーイベントチェック
	dwEvent = 0;
	h = GetStdHandle(STD_INPUT_HANDLE);
	if (GetNumberOfConsoleInputEvents(h, &dwEvent) && dwEvent) {
		DWORD			dwRead = 0;
		PINPUT_RECORD	pInpRec = (PINPUT_RECORD)_alloca(dwEvent * sizeof(INPUT_RECORD));

		if (ReadConsoleInput(h, pInpRec, dwEvent, &dwRead) && dwRead) {
			PINPUT_RECORD	pir = pInpRec;
			DWORD			dw;
			for (dw = 0; dw < dwRead; dw++, pir++) {
				switch (pir->EventType) {
				case KEY_EVENT: {
					KEY_EVENT_RECORD* pker = &pir->Event.KeyEvent;
					if (pker->wVirtualKeyCode > 0x0FF)
						break;
					if (pker->bKeyDown)
						g_dwKeyMap[pker->wVirtualKeyCode >> 5] |= (0x01 << (pker->wVirtualKeyCode & 31));
					else
						g_dwKeyMap[pker->wVirtualKeyCode >> 5] &= ~(0x01 << (pker->wVirtualKeyCode & 31));
				}
				default:
					break;
				}
			}
		}
	}
	// キー状態を返す
	return (g_dwKeyMap[(port & 0x0FF) >> 5] & (0x01 << (port & 31))) != 0;
}

// キー情報リセット
void platform_resetkey(void)
{
	int i;
	for (i = 0; i < 8; i++)
	{
		g_dwKeyMap[i] = 0;
	}
}