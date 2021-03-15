#include "input.h"


Input Input::m_instance;


Input& Input::Get() {
	return m_instance;
}
void Input::Update() {
	Input::Get().m_pressedKeysLast = Input::Get().m_pressedKeysNow;
}
bool Input::IsKeyPressed(int keycode) {
	return !Input::Get().m_pressedKeysLast[keycode] && Input::Get().m_pressedKeysNow[keycode];
}
bool Input::IsKeyHeld(int keycode) {
	return Input::Get().m_pressedKeysNow[keycode];
}
bool Input::IsKeyReleased(int keycode) {
	return Input::Get().m_pressedKeysLast[keycode] && !Input::Get().m_pressedKeysNow[keycode];
}
void Input::SetKey(int keycode, bool val) {
	Input::Get().m_pressedKeysNow[keycode] = val;
}
void Input::KeyInputCallback(GLFWwindow * window, int key, int scancode, int action, int mods) {
	if (action != GLFW_RELEASE) {
		Input::SetKey(key, true);
	}
	else {
		Input::SetKey(key, false);
	}
}