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

#include "io/Mouse.h"
#include "io/Keyboard.h"
#include "io/CameraClass.h"


void	FramebufferSizeCallback(GLFWwindow* Window, int Width, int Height);
void	ProcessInput(GLFWwindow* Window, float DeltaTime);

float	MixValue = 0.5f;
unsigned int SCREEN_W = 800, SCREEN_H = 600;
float x, y, z;

CameraClass Cameras[2] =
{
	CameraClass(glm::vec3(0.0f, 0.0f, 3.0f)),
	CameraClass(glm::vec3(10.0f, 10.0f, 10.0f))
};

int ActiveCameraIndex = 0;
//CameraClass Camera(glm::vec3(0.0f, 0.0f, 3.0f));
float DeltaTime = 0.0f;
float LastFrame = 0.0f;

int main()
{
	std::cout << "Hello World!" << std::endl;

	if (!glfwInit())
		return EXIT_FAILURE;

	// opengl version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COPMPAT, GL_TRUE);
#endif // __APPLE__

	GLFWwindow* Window = glfwCreateWindow(SCREEN_W, SCREEN_H, "OpenGL", NULL, NULL);
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

	glViewport(0, 0, SCREEN_W, SCREEN_H);

	glfwSetFramebufferSizeCallback(Window, FramebufferSizeCallback);

	glfwSetKeyCallback(Window, Keyboard::KeyCallback);
	glfwSetMouseButtonCallback(Window, Mouse::MouseButtonCallback);
	glfwSetCursorPosCallback(Window, Mouse::CursorPositionCallback);
	glfwSetScrollCallback(Window, Mouse::MouseWheelCallback);

	glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_DEPTH_TEST);

	// shaders

	Shader shader("assets/vertex_core.glsl", "assets/fragment_core.glsl");
	//Shader shader2("assets/vertex_core.glsl", "assets/fragment_core2.glsl");

	// vertex array
	float vertices[] = {
		// position				//	tex coords
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	// world space positions of our cubes
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	// VAO, VBO
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	// bind VAO
	glBindVertexArray(VAO);
	// bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// setup EBO
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
	
	// set attribute pointer
	// positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// colors
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);
	// texture coordinates
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// TEXTURES
	unsigned int Texture1, Texture2;
	glGenTextures(1, &Texture1);
	glBindTexture(GL_TEXTURE_2D, Texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// load image
	int Width, Height, NumberOfChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* Data = stbi_load("assets/textures/space2.jpg", &Width, &Height, &NumberOfChannels, 0);

	if (Data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, Data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(Data);

	glGenTextures(1, &Texture2);
	glBindTexture(GL_TEXTURE_2D, Texture2);

	Data = stbi_load("assets/textures/fallout.png", &Width, &Height, &NumberOfChannels, 0);

	if (Data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(Data);

	shader.Activate();
	shader.SetInt("texture1", 0); // set uniform texture1 in fragment shader
	shader.SetInt("texture2", 1); // set uniform texture2 in fragment shader

	x = 0.0f;
	y = 0.0f;
	z = 3.0f;
	
	while (!glfwWindowShouldClose(Window))
	{
		float CurrentTime = (float)glfwGetTime();
		DeltaTime = CurrentTime - LastFrame;
		LastFrame = CurrentTime;
		ProcessInput(Window, DeltaTime);

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, Texture2);

		// draw triangle
		glBindVertexArray(VAO);

		// create transformation for screen
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);

		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(-55.0f), glm::vec3(0.5f));
		view = Cameras[ActiveCameraIndex].GetViewMatrix();
		projection = glm::perspective(glm::radians(Cameras[ActiveCameraIndex].m_Zoom), (float)SCREEN_W / (float)SCREEN_H, 0.1f, 100.0f);

		shader.Activate();
		shader.SetMat4("model", model);
		shader.SetMat4("view", view);
		shader.SetMat4("projection", projection);

		shader.SetFloat("MixValue", MixValue);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);

		glfwSwapBuffers(Window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VAO);
	//glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return EXIT_SUCCESS;
}

// enable resizing window
void	FramebufferSizeCallback(GLFWwindow* Window, int Width, int Height)
{
	glViewport(0, 0, Width, Height);
	SCREEN_W = Width;
	SCREEN_H = Height;
}

void	ProcessInput(GLFWwindow* Window, float DeltaTime)
{
	if (Keyboard::Key(GLFW_KEY_ESCAPE))
	{
		glfwSetWindowShouldClose(Window, true);
	}

	// change mix value
	if (Keyboard::Key(GLFW_KEY_UP))
	{
		MixValue += .05f;
		if (MixValue > 1.0f)
		{
			MixValue = 1.0f;
		}
	}
	if (Keyboard::Key(GLFW_KEY_DOWN))
	{
		MixValue -= .05f;
		if (MixValue < 0)
		{
			MixValue = 0.0f;
		}
	}

	if (Keyboard::KeyWentDown(GLFW_KEY_TAB))
	{
		ActiveCameraIndex = (ActiveCameraIndex == 0) ? 1 : 0;
	}

	if (Keyboard::Key(GLFW_KEY_W))
	{
		Cameras[ActiveCameraIndex].UpdateCameraPosition(CameraDirection::FORWARD, DeltaTime);
	}
	if (Keyboard::Key(GLFW_KEY_S))
	{
		Cameras[ActiveCameraIndex].UpdateCameraPosition(CameraDirection::BACKWARD, DeltaTime);
	}
	if (Keyboard::Key(GLFW_KEY_D))
	{
		Cameras[ActiveCameraIndex].UpdateCameraPosition(CameraDirection::RIGHT, DeltaTime);
	}
	if (Keyboard::Key(GLFW_KEY_A))
	{
		Cameras[ActiveCameraIndex].UpdateCameraPosition(CameraDirection::LEFT, DeltaTime);
	}
	if (Keyboard::Key(GLFW_KEY_SPACE))
	{
		Cameras[ActiveCameraIndex].UpdateCameraPosition(CameraDirection::UP, DeltaTime);
	}
	if (Keyboard::Key(GLFW_KEY_LEFT_SHIFT))
	{
		Cameras[ActiveCameraIndex].UpdateCameraPosition(CameraDirection::DOWN, DeltaTime);
	}

	float DeltaX = (float)Mouse::GetDeltaX();
	float DeltaY = (float)Mouse::GetDeltaY();
	if (DeltaX != 0 || DeltaY != 0)
	{
		Cameras[ActiveCameraIndex].UpdateCameraDirection(DeltaX, DeltaY);
	}
	float ScrollDeltaY = (float)Mouse::GetScrollDeltaY();
	if (ScrollDeltaY != 0)
	{
		Cameras[ActiveCameraIndex].UpdateCameraZoom(ScrollDeltaY);
	}
}
