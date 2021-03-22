#include "core/input.h"


Input Input::m_instance;


Input& Input::Get() {
	return m_instance;
}
void Input::Update() {
	m_instance.m_pressedKeysLast = m_instance.m_pressedKeysNow;
	m_instance.xoffset = 0;
	m_instance.yoffset = 0;
}
bool Input::IsKeyPressed(int keycode) {
	return !m_instance.m_pressedKeysLast[keycode] && m_instance.m_pressedKeysNow[keycode];
}
bool Input::IsKeyHeld(int keycode) {
	return m_instance.m_pressedKeysNow[keycode];
}
bool Input::IsKeyReleased(int keycode) {
	return m_instance.m_pressedKeysLast[keycode] && !m_instance.m_pressedKeysNow[keycode];
}
void Input::SetKey(int keycode, bool val) {
	m_instance.m_pressedKeysNow[keycode] = val;
}
void Input::KeyInputCallback(GLFWwindow * window, int key, int scancode, int action, int mods) {
	if (action != GLFW_RELEASE) {
		Input::SetKey(key, true);
		//Log("Pressed: " << (char)key);
	}
	else {
		Input::SetKey(key, false);
		//Log("Released: " << (char)key);
	}
}
void Input::MouseCallback(GLFWwindow * window, double xpos, double ypos) {
	m_instance.xoffset += (float)xpos - m_instance.lastX;
	m_instance.yoffset += m_instance.lastY - (float)ypos;


	m_instance.lastX = (float)xpos;
	m_instance.lastY = (float)ypos;
}