#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <sstream>
#include <streambuf>
#include <string>

#include "stb/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"


void	FramebufferSizeCallback(GLFWwindow* Window, int Width, int Height);
void	ProcessInput(GLFWwindow* Window);

int main()
{
	std::cout << "Hello World!" << std::endl;

	int		Success;
	char	InfoLog[512];

	if (!glfwInit())
		return EXIT_FAILURE;

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

	// shaders

	Shader shader("assets/vertex_core.glsl", "assets/fragment_core.glsl");
	Shader shader2("assets/vertex_core.glsl", "assets/fragment_core2.glsl");

	// vertex array
	float Vertices[] =
	{	// positions			// colors
		-0.25f, -0.5f,	0.0f,	1.0f, 1.0f, 0.5f,
		0.15f,	0.0f,	0.0f,	0.5f, 1.0f, 0.75f,
		0.0f,	0.5f,	0.0f,	0.6f, 1.0f, 0.2f,
		0.5f,	-0.4f,	0.0f,	1.0f, 0.2f, 1.0f	
	};

	unsigned int Indices[] =
	{
		0, 1, 2, // 1st triangle
		3, 1, 2 // 2nd triangle
	};

	// VAO, VBO
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// bind VAO
	glBindVertexArray(VAO);
	// bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	// set attribute pointer

	// positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// colors
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// setup EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

	glm::mat4 Transform = glm::mat4(1.0f);
	Transform = glm::rotate(Transform, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.Activate();
	shader.SetMat4("transform", Transform);

	glm::mat4 Transform2 = glm::mat4(1.0f);
	Transform2 = glm::scale(Transform2, glm::vec3(1.5f));
	Transform2 = glm::rotate(Transform2, glm::radians(15.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader2.Activate();
	shader2.SetMat4("transform", Transform2);
	
	while (!glfwWindowShouldClose(Window))
	{
		ProcessInput(Window);

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		Transform = glm::rotate(Transform, glm::radians((float)glfwGetTime() / 100.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		shader.Activate();
		shader.SetMat4("transform", Transform);
		
		// draw triangle
		glBindVertexArray(VAO);
		shader.Activate();
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		Transform2 = glm::rotate(Transform2, glm::radians((float)glfwGetTime() / -100.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		shader2.Activate();
		shader2.SetMat4("transform", Transform2);

		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(3 * sizeof(GLuint)));
		//glDrawArrays(GL_TRIANGLES, 0, 6);

		//glUseProgram(ShaderPrograms[1]);
		//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(3 * sizeof(unsigned int)));

		glfwSwapBuffers(Window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VAO);
	glDeleteBuffers(1, &EBO);

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
