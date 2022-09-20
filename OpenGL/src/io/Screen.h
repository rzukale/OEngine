#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../error.h"

class Screen
{
public:
	static unsigned int	SCREEN_WIDTH;
	static unsigned int SCREEN_HEIGHT;

	static void FrameBufferSizeCallback(GLFWwindow* Window, int Width, int Height);

	Screen();
	~Screen();

	bool Init();

	void SetParameters();

	// main loop
	void Update();
	void NewFrame();

	// window closing functions
	bool ShouldClose();
	void SetShouldClose(bool bShouldClose);

private:
	GLFWwindow* m_Window;
};

