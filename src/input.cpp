#include "input.h"
#include "platform.h"
#include <windows.h> // virtual keys

static InputType g_input_type;
static KeyState g_key_states[InputKey_Num];

enum InputBtnController {
	// button
	InputBtnController_Jump = 2,
	InputBtnController_Confirm = 4,
	InputBtnController_Retry = 8,
	InputBtnController_RotateLeft = 16,
	InputBtnController_RotateRight = 32,
	// pov
	InputBtnController_Up = 0,
	InputBtnController_UpRight = 4500,
	InputBtnController_Right = 9000,
	InputBtnController_RightDown = 1350,
	InputBtnController_Down = 18000,
	InputBtnController_DownLeft = 22500,
	InputBtnController_Left = 27000,
	InputBtnController_LeftUp = 31500
};

enum InputBtnKeyboard {
	InputBtnKeyboard_Up = VK_UP,
	InputBtnKeyboard_Right = VK_RIGHT,
	InputBtnKeyboard_Down = VK_DOWN,
	InputBtnKeyboard_Left = VK_LEFT,
	InputBtnKeyboard_Jump = VK_SPACE,
	InputBtnKeyboard_RotateLeft = 'A', // 0x41
	InputBtnKeyboard_RotateRight = 'D', // 0x44
	InputBtnKeyboard_Confirm = VK_RETURN,
	InputBtnKeyboard_Retry = 'R', // 0x52
	InputBtnKeyboard_Exit = VK_ESCAPE
};

void input_set_key_press(InputKey key, bool press);
void input_update_keyboard();
void input_update_controller();
void input_update_controller_keyboard();

void input_init() {
	g_input_type = InputType_Controller;
}

void input_update() {
	switch (g_input_type) {
	case InputType_Controller: {
		// input_update_controller();
		input_update_controller_keyboard();
		// input_update_keyboard();
		break;
	}
	case InputType_Keyboard: {
		input_update_keyboard();
		break;
	}
	}
}

void input_update_keyboard() {
	input_set_key_press(InputKey_Up, platform_getkey(InputBtnKeyboard_Up));
	input_set_key_press(InputKey_Right, platform_getkey(InputBtnKeyboard_Right));
	input_set_key_press(InputKey_Down, platform_getkey(InputBtnKeyboard_Down));
	input_set_key_press(InputKey_Left, platform_getkey(InputBtnKeyboard_Left));
	input_set_key_press(InputKey_RotateLeft, platform_getkey(InputBtnKeyboard_RotateLeft));
	input_set_key_press(InputKey_RotateRight, platform_getkey(InputBtnKeyboard_RotateRight));
	input_set_key_press(InputKey_Jump, platform_getkey(InputBtnKeyboard_Jump));
	input_set_key_press(InputKey_Confirm, platform_getkey(InputBtnKeyboard_Confirm));
	input_set_key_press(InputKey_Retry, platform_getkey(InputBtnKeyboard_Retry));
	input_set_key_press(InputKey_Exit, platform_getkey(InputBtnKeyboard_Exit));
}

// keyboard or controller
void input_update_controller_keyboard() {
	// get input
	JOYINFOEX joyInfo;
	joyInfo.dwSize = sizeof(JOYINFOEX);
	joyInfo.dwFlags = JOY_RETURNALL;

	UINT joystickID = 1; // TODO

	if (joyGetPosEx(joystickID, &joyInfo) != JOYERR_NOERROR) {
		input_update_keyboard();
		return; // TODO
	}
	int btn = joyInfo.dwButtons;
	input_set_key_press(InputKey_Jump, (btn & InputBtnController_Jump) || platform_getkey(InputBtnKeyboard_Jump));
	input_set_key_press(InputKey_Confirm, (btn & InputBtnController_Confirm) || platform_getkey(InputBtnKeyboard_Confirm));
	input_set_key_press(InputKey_Retry, (btn & InputBtnController_Retry) || platform_getkey(InputBtnKeyboard_Retry));
	input_set_key_press(InputKey_RotateLeft, (btn & InputBtnController_RotateLeft) || platform_getkey(InputBtnKeyboard_RotateLeft));
	input_set_key_press(InputKey_RotateRight, (btn & InputBtnController_RotateRight) || platform_getkey(InputBtnKeyboard_RotateRight));
	input_set_key_press(InputKey_Exit, platform_getkey(InputBtnKeyboard_Exit));
	// move
	int xpos = joyInfo.dwXpos;
	int ypos = joyInfo.dwYpos;
	int pov = joyInfo.dwPOV;
	bool up = platform_getkey(InputBtnKeyboard_Up);
	bool right = platform_getkey(InputBtnKeyboard_Right);
	bool down = platform_getkey(InputBtnKeyboard_Down);
	bool left = platform_getkey(InputBtnKeyboard_Left);
	switch (pov) {
	case InputBtnController_Up: {
		up = true;
		break;
	}
	case InputBtnController_UpRight: {
		up = true;
		right = true;
		break;
	}
	case InputBtnController_Right: {
		right = true;
		break;
	}
	case InputBtnController_RightDown: {
		right = true;
		down = true;
		break;
	}
	case InputBtnController_Down: {
		down = true;
		break;
	}
	case InputBtnController_DownLeft: {
		down = true;
		left = true;
		break;
	}
	case InputBtnController_Left: {
		left = true;
		break;
	}
	case InputBtnController_LeftUp: {
		left = true;
		up = true;
		break;
	}
	default: {
		float x = (float)xpos / 65535.0f;
		float y = (float)ypos / 65535.0f;
		if (x <= 0.3f) {
			left = true;
		}
		else if (x >= 0.7f) {
			right = true;
		}
		if (y <= 0.3f) {
			up = true;
		}
		else if (y >= 0.7f) {
			down = true;
		}
		break;
	}
	}
	input_set_key_press(InputKey_Up, up);
	input_set_key_press(InputKey_Right, right);
	input_set_key_press(InputKey_Down, down);
	input_set_key_press(InputKey_Left, left);
}

void input_update_controller() {
	// get input
	JOYINFOEX joyInfo;
	joyInfo.dwSize = sizeof(JOYINFOEX);
	joyInfo.dwFlags = JOY_RETURNALL;

	UINT joystickID = 1; // TODO

	if (joyGetPosEx(joystickID, &joyInfo) != JOYERR_NOERROR) {
		return; // TODO
	}
	int btn = joyInfo.dwButtons;
	input_set_key_press(InputKey_Jump, btn & InputBtnController_Jump);
	input_set_key_press(InputKey_Confirm, btn & InputBtnController_Confirm);
	input_set_key_press(InputKey_Retry, btn & InputBtnController_Retry);
	input_set_key_press(InputKey_RotateLeft, btn & InputBtnController_RotateLeft);
	input_set_key_press(InputKey_RotateRight, btn & InputBtnController_RotateRight);
	// move
	int xpos = joyInfo.dwXpos;
	int ypos = joyInfo.dwYpos;
	int pov = joyInfo.dwPOV;
	bool up = false, right = false, down = false, left = false;
	switch (pov) {
	case InputBtnController_Up: {
		up = true;
		break;
	}
	case InputBtnController_UpRight: {
		up = true;
		right = true;
		break;
	}
	case InputBtnController_Right: {
		right = true;
		break;
	}
	case InputBtnController_RightDown: {
		right = true;
		down = true;
		break;
	}
	case InputBtnController_Down: {
		down = true;
		break;
	}
	case InputBtnController_DownLeft: {
		down = true;
		left = true;
		break;
	}
	case InputBtnController_Left: {
		left = true;
		break;
	}
	case InputBtnController_LeftUp: {
		left = true;
		up = true;
		break;
	}
	default: {
		float x = (float)xpos / 65535.0f;
		float y = (float)ypos / 65535.0f;
		if (x <= 0.3f) {
			left = true;
		}
		else if (x >= 0.7f) {
			right = true;
		}
		if (y <= 0.3f) {
			up = true;
		}
		else if (y >= 0.7f) {
			down = true;
		}
		break;
	}
	}
	input_set_key_press(InputKey_Up, up);
	input_set_key_press(InputKey_Right, right);
	input_set_key_press(InputKey_Down, down);
	input_set_key_press(InputKey_Left, left);
}

KeyState input_get_key_state(InputKey key) {
	return g_key_states[key];
}

bool input_if_key_pressed(InputKey key) {
	return  g_key_states[key].press;
}

bool input_if_key_triggered(InputKey key) {
	return  g_key_states[key].trigger;
}

void input_set_key_press(InputKey key, bool press) {
	KeyState* state = g_key_states + key;
	bool press_old = state->press;
	state->press = press;
	state->trigger = !press_old && press;
	state->release = press_old && !press;
	state->press_old = press_old;
}