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

#include "graphics/Shader.h"
#include "graphics/Texture.h"
#include "graphics/Models/Cube.h"
#include "graphics/Models/Lamp.h"
#include "graphics/Model.h"
#include "graphics/Light.h"

#include "io/Screen.h"
#include "io/Mouse.h"
#include "io/Keyboard.h"
#include "io/CameraClass.h"

#include "error.h"

void	ProcessInput(float DeltaTime);

float	MixValue = 0.5f;
unsigned int SCREEN_W = 800, SCREEN_H = 600;
float x, y, z;

Screen screen;

CameraClass Cameras[2] =
{
	CameraClass(glm::vec3(0.0f, 0.0f, 0.0f)),
	CameraClass(glm::vec3(10.0f, 10.0f, 10.0f))
};

int ActiveCameraIndex = 0;
//CameraClass Camera(glm::vec3(0.0f, 0.0f, 3.0f));
float DeltaTime = 0.0f;
float LastFrame = 0.0f;


#include <stdio.h>

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

	if (!screen.Init())
	{
		std::cout << "Could not create window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}
	std::cout << glGetString(GL_VERSION) << std::endl;
	screen.SetParameters();
	GLCall(glEnable(GL_DEPTH_TEST));

	// shaders
	Shader shader("assets/object.vs", "assets/object.fs");
	Shader lampShader("assets/object.vs", "assets/lamp.fs");

	glm::vec3 cubePositions[] =
	{
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

	glm::vec3 pointLightPositions[] =
	{
			glm::vec3(0.7f,  0.2f,  2.0f),
			glm::vec3(2.3f, -3.3f, -4.0f),
			glm::vec3(-4.0f,  2.0f, -12.0f),
			glm::vec3(0.0f,  0.0f, -3.0f)
	};
	Lamp lamps[4];
	for (unsigned int i = 0; i < 4; i++)
	{
		lamps[i] = Lamp(glm::vec3(1.0f),
			glm::vec4(0.05f, 0.05f, 0.05f, 1.0f), glm::vec4(0.8f, 0.8f, 0.8f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
			1.0f, 0.07f, 0.032f,
			pointLightPositions[i], glm::vec3(0.25f));
		lamps[i].Init();
	}

	DirectionalLight dirLight = { glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec4(0.1f, 0.1f, 0.1f, 1.0f), glm::vec4(0.4f, 0.4f, 0.4f, 1.0f), glm::vec4(0.75f, 0.75f, 0.75f, 1.0f) };
	SpotLight spotLight = { Cameras[ActiveCameraIndex].m_CameraPosition, Cameras[ActiveCameraIndex].m_CameraFront,
		glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(20.0f)),
		1.0f, 0.07f, 0.032f,
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) };

	Model model(glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.05f), true);
	model.LoadModel("assets/models/lotr_troll/scene.gltf");


	while (!screen.ShouldClose())
	{
		float CurrentTime = (float)glfwGetTime();
		DeltaTime = CurrentTime - LastFrame;
		LastFrame = CurrentTime;
		ProcessInput(DeltaTime);

		// render
		screen.Update();

		shader.Bind();
		//shader.Set3Float("light.Position", lamp.m_Pos);
		shader.Set3Float("ViewPos", Cameras[ActiveCameraIndex].m_CameraPosition);

		dirLight.m_Direction = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(0.5f), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::vec4(dirLight.m_Direction, 1.0f));
		dirLight.Render(shader);

		for (int i = 0; i < 4; i++)
		{
			lamps[i].pointLight.Render(shader, i);
		}
		shader.SetInt("NumberOfPointlights", 4);

		spotLight.m_Position = Cameras[ActiveCameraIndex].m_CameraPosition;
		spotLight.m_Direction = Cameras[ActiveCameraIndex].m_CameraFront;
		shader.SetInt("NumberOfSpotlights", 1);

		// create transformation for screen
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);

		view = Cameras[ActiveCameraIndex].GetViewMatrix();
		projection = glm::perspective(glm::radians(Cameras[ActiveCameraIndex].GetZoom()), (float)SCREEN_W / (float)SCREEN_H, 0.1f, 100.0f);
		
		shader.SetMat4("view", view);
		shader.SetMat4("projection", projection);

		spotLight.Render(shader, 0);

		model.Render(shader);
		
		lampShader.Bind();
		lampShader.SetMat4("view", view);
		lampShader.SetMat4("projection", projection);
		
		for (int i = 0; i < 4; i++)
		{
			lamps[i].Render(lampShader);
		}
		screen.NewFrame();
	}

	model.Cleanup();
	for (int i = 0; i < 4; i++)
	{
		lamps[i].Cleanup();
	}

	glfwTerminate();
	return EXIT_SUCCESS;
}

void	ProcessInput(float DeltaTime)
{
	if (Keyboard::Key(GLFW_KEY_ESCAPE))
	{
		screen.SetShouldClose(true);
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
