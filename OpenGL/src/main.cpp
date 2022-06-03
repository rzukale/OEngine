#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void	FramebufferSizeCallback(GLFWwindow* Window, int Width, int Height);
void	ProcessInput(GLFWwindow* Window);

int main()
{
	std::cout << "Hello World!" << std::endl;

	glfwInit();

	// opengl version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COPMPAT, GL_TRUE);
#endif // __APPLE__

	GLFWwindow* Window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);
	if (Window == NULL)
	{
		std::cout << "Could not create window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(Window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glViewport(0, 0, 800, 600);

	glfwSetFramebufferSizeCallback(Window, FramebufferSizeCallback);

	while (!glfwWindowShouldClose(Window))
	{
		ProcessInput(Window);

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(Window);
		glfwPollEvents();
	}

	glfwTerminate();
	return EXIT_SUCCESS;
}

// enable resizing window
void	FramebufferSizeCallback(GLFWwindow* Window, int Width, int Height)
{
	glViewport(0, 0, Width, Height);
}

void	ProcessInput(GLFWwindow* Window)
{
	if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(Window, true);
	}
}