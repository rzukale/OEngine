#include "Mouse.h"

double Mouse::m_X = 0;
double Mouse::m_Y = 0;
double Mouse::m_LastX = 0;
double Mouse::m_LastY = 0;
double Mouse::m_DeltaX = 0;
double Mouse::m_DeltaY = 0;
double Mouse::m_ScrollDeltaX = 0;
double Mouse::m_ScrollDeltaY = 0;
bool Mouse::m_FirstMouse = true;
bool Mouse::m_Buttons[GLFW_MOUSE_BUTTON_LAST] = { 0 };
bool Mouse::m_ButtonsChanged[GLFW_MOUSE_BUTTON_LAST] = { 0 };

void Mouse::CursorPositionCallback(GLFWwindow* Window, double X, double Y)
{
	m_X = X;
	m_Y = Y;

	if (m_FirstMouse)
	{
		m_LastX = X;
		m_LastY = Y;
		m_FirstMouse = false;
	}

	m_DeltaX = m_X - m_LastX;
	m_DeltaY = m_LastY - m_Y; // inverted
	m_LastX = X;
	m_LastY = Y;
}

void Mouse::MouseButtonCallback(GLFWwindow* Window, int Button, int Action, int Mods)
{
	if (Action != GLFW_RELEASE)
	{
		if (!m_Buttons[Button])
		{
			m_Buttons[Button] = true;
		}
	}
	else
	{
		m_Buttons[Button] = false;
	}
	m_ButtonsChanged[Button] = Action != GLFW_REPEAT;
}

void Mouse::MouseWheelCallback(GLFWwindow* Window, double DeltaX, double DeltaY)
{
	m_ScrollDeltaX = DeltaX;
	m_ScrollDeltaY = DeltaY;
}

double Mouse::GetMouseX()
{
	return m_X;
}

double Mouse::GetMouseY()
{
	return m_Y;
}

double Mouse::GetDeltaX()
{
	double TempDeltaX = m_DeltaX;
	m_DeltaX = 0;
	return TempDeltaX;
}

double Mouse::GetDeltaY()
{
	double TempDeltaY = m_DeltaY;
	m_DeltaY = 0;
	return TempDeltaY;
}

double Mouse::GetScrollDeltaX()
{
	double TempScrollDX = m_ScrollDeltaX;
	m_ScrollDeltaX = 0;
	return TempScrollDX;
}

double Mouse::GetScrollDeltaY()
{
	double TempScrollDY = m_ScrollDeltaY;
	m_ScrollDeltaY = 0;
	return TempScrollDY;
}

bool Mouse::Button(int Button)
{
	return m_Buttons[Button];
}

bool Mouse::ButtonChanged(int Button)
{
	bool Ret = m_ButtonsChanged[Button];
	m_ButtonsChanged[Button] = false;
	return Ret;
}

bool Mouse::ButtonWentUp(int Button)
{
	return !m_Buttons[Button] && ButtonChanged(Button);
}

bool Mouse::ButtonWentDown(int Button)
{
	return m_Buttons[Button] && ButtonChanged(Button);
}
