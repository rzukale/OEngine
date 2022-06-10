#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Keyboard
{
public:
	static void KeyCallback(GLFWwindow* Window, int Key, int Scancode, int Action, int Mods);

	static bool Key(int Key);
	static bool KeyChanged(int Key);
	static bool KeyWentUp(int Key);
	static bool KeyWentDown(int Key);

private:
	static bool m_Keys[];
	static bool m_KeysChanged[];
};

