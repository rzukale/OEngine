#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Mouse
{
public:
	static void CursorPositionCallback(GLFWwindow* Window, double X, double Y);
	static void MouseButtonCallback(GLFWwindow* Window, int Button, int Action, int Mods);
	static void MouseWheelCallback(GLFWwindow* Window, double DeltaX, double DeltaY);

	static double GetMouseX();
	static double GetMouseY();

	static double GetDeltaX();
	static double GetDeltaY();

	static double GetScrollDeltaX();
	static double GetScrollDeltaY();

	static bool Button(int Button);
	static bool ButtonChanged(int Button);
	static bool ButtonWentUp(int Button);
	static bool ButtonWentDown(int Button);

private:
	static double m_X;
	static double m_Y;

	static double m_LastX;
	static double m_LastY;

	static double m_DeltaX;
	static double m_DeltaY;

	static double m_ScrollDeltaX;
	static double m_ScrollDeltaY;

	static bool m_FirstMouse;

	static bool m_Buttons[];
	static bool m_ButtonsChanged[];
};

