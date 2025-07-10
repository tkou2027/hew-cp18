#pragma once

struct KeyState {
	bool trigger;
	bool release;
	bool press;
	bool press_old;
};

enum InputType {
	InputType_Keyboard,
	InputType_Controller
};

enum InputKey {
	InputKey_Up,
	InputKey_Right,
	InputKey_Down,
	InputKey_Left,
	InputKey_Jump,
	InputKey_RotateLeft,
	InputKey_RotateRight,
	InputKey_Confirm,
	InputKey_Retry,
	InputKey_Exit,
	//
	InputKey_Num
};

void input_init();
void input_update();
KeyState input_get_key_state(InputKey key);
bool input_if_key_pressed(InputKey key);
bool input_if_key_triggered(InputKey key);