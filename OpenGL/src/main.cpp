#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <sstream>
#include <streambuf>
#include <string>

#include "stb/stb_image.h"


void	FramebufferSizeCallback(GLFWwindow* Window, int Width, int Height);
void	ProcessInput(GLFWwindow* Window);
std::string	LoadShaderSrc(const char* Filename);

int main()
{
	std::cout << "Hello World!" << std::endl;

	int		Success;
	char	InfoLog[512];

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

	// shaders

	// compile vertex shader
	unsigned int VertexShader;
	VertexShader = glCreateShader(GL_VERTEX_SHADER);
	std::string VertexShaderSrc = LoadShaderSrc("assets/vertex_core.glsl");
	const GLchar* VertShader = VertexShaderSrc.c_str();
	glShaderSource(VertexShader, 1, &VertShader, NULL);
	glCompileShader(VertexShader);

	// catch error
	glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &Success);
	if (!Success)
	{
		glGetShaderInfoLog(VertexShader, 512, NULL, InfoLog);
		std::cout << "Error with vertex shader compilation: " << std::endl << InfoLog << std::endl;
	}

	// compile fragment shader
	unsigned int FragmentShaders[2];
	FragmentShaders[0] = glCreateShader(GL_FRAGMENT_SHADER);
	std::string FragShaderSrc = LoadShaderSrc("assets/fragment_core.glsl");
	const GLchar* FragShader = FragShaderSrc.c_str();
	glShaderSource(FragmentShaders[0], 1, &FragShader, NULL);
	glCompileShader(FragmentShaders[0]);

	// catch error
	glGetShaderiv(FragmentShaders[0], GL_COMPILE_STATUS, &Success);
	if (!Success)
	{
		glGetShaderInfoLog(FragmentShaders[0], 512, NULL, InfoLog);
		std::cout << "Error with fragment shader compilation: " << std::endl << InfoLog << std::endl;
	}

	FragmentShaders[1] = glCreateShader(GL_FRAGMENT_SHADER);
	FragShaderSrc = LoadShaderSrc("assets/fragment_core2.glsl");
	FragShader = FragShaderSrc.c_str();
	glShaderSource(FragmentShaders[1], 1, &FragShader, NULL);
	glCompileShader(FragmentShaders[1]);

	// catch error
	glGetShaderiv(FragmentShaders[1], GL_COMPILE_STATUS, &Success);
	if (!Success)
	{
		glGetShaderInfoLog(FragmentShaders[1], 512, NULL, InfoLog);
		std::cout << "Error with fragment shader compilation: " << std::endl << InfoLog << std::endl;
	}


	// create program
	unsigned int ShaderPrograms[2];
	ShaderPrograms[0] = glCreateProgram();
	glAttachShader(ShaderPrograms[0], VertexShader);
	glAttachShader(ShaderPrograms[0], FragmentShaders[0]);
	glLinkProgram(ShaderPrograms[0]);

	// catch errors
	glGetProgramiv(ShaderPrograms[0], GL_LINK_STATUS, &Success);
	if (!Success)
	{
		glGetProgramInfoLog(ShaderPrograms[0], 512, NULL, InfoLog);
		std::cout << "Linking error:" << std::endl << InfoLog << std::endl;
	}

	ShaderPrograms[1] = glCreateProgram();
	glAttachShader(ShaderPrograms[1], VertexShader);
	glAttachShader(ShaderPrograms[1], FragmentShaders[1]);
	glLinkProgram(ShaderPrograms[1]);

	// catch errors
	glGetProgramiv(ShaderPrograms[1], GL_LINK_STATUS, &Success);
	if (!Success)
	{
		glGetProgramInfoLog(ShaderPrograms[1], 512, NULL, InfoLog);
		std::cout << "Linking error:" << std::endl << InfoLog << std::endl;
	}


	glDeleteShader(VertexShader);
	glDeleteShader(FragmentShaders[0]);
	glDeleteShader(FragmentShaders[1]);

	// vertex array
	//float Vertices[] =
	//{
	//	0.5f, 0.5f, 0.0f, // top right
	//	-0.5f, 0.5f, 0.0f, // top left
	//	-0.5f, -0.5f, 0.0f, // bottom left
	//	0.5f, -0.5f, 0.0f // bottom right
	//};

	//unsigned int Indices[] =
	//{
	//	0, 1, 2, // 1st triangle
	//	2, 3, 0 // 2nd triangle
	//};

	float Vertices[] =
	{
		-0.5f, -0.5f, 0.0f,
		-0.25f, 0.5f, 0.0f,
		-0.1f, -0.5f, 0.0f,

		0.5f, -0.5f, 0.0f,
		0.25, 0.5f, 0.0f,
		0.1f, -0.5f, 0.0f
	};

	unsigned int Indices[] =
	{
		0, 1, 2, 3, 4, 5
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// setup EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

	while (!glfwWindowShouldClose(Window))
	{
		ProcessInput(Window);

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw triangle
		glBindVertexArray(VAO);

		glUseProgram(ShaderPrograms[0]);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, 6);

		glUseProgram(ShaderPrograms[1]);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(3 * sizeof(unsigned int)));

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

std::string	LoadShaderSrc(const char* Filename)
{
	std::ifstream		File;
	std::stringstream	Buf;
	std::string			Ret = "";

	File.open(Filename);
	if (File.is_open())
	{
		Buf << File.rdbuf();
		Ret = Buf.str();
	}
	else
	{
		std::cout << "Failed to open " << Filename << std::endl;
	}
	File.close();
	return Ret;
}