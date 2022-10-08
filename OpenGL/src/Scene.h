#pragma once

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "graphics/Light.h"
#include "graphics/Shader.h"
#include "io/CameraClass.h"
#include "io/Keyboard.h"
#include "io/Mouse.h"
#include "algorithms/States.hpp"
#include "error.h"

class Scene
{
public:

	static void FrameBufferSizeCallback(GLFWwindow* Window, int Width, int Height);

	Scene();
	Scene(int glfwVersionMajor, int glfwVersionMinor, const char* title, unsigned int Width, unsigned int Height);

	bool Init();
	void ProcessInput(float DeltaTime);
	void Update();
	void NewFrame();
	void Render(Shader& shader, bool bApplyLighting = true);
	void Cleanup();

	bool ShouldClose();
	CameraClass* GetActiveCamera();

	void SetShouldClose(bool bShouldClose);
	void SetWindowColor(float r, float g, float b, float a);

	/*int GetWindowWidth() const { return ScreenWidth; }
	int GetWindowHeight() const { return ScreenHeight; }*/
	void AddPointlight(PointLight* newLight);
	void AddSpotlight(SpotLight* newLight);

	void SetActivePointlightBit(int target);
	void SetActiveSpotlight();
	void SetDirectionalLight(DirectionalLight* DirLight);
	void AddCamera(CameraClass* NewCamera);

protected:
	GLFWwindow* Window;
	const char* title;
	std::vector<PointLight*> pointlights;
	std::vector<SpotLight*> spotlights;
	std::vector<CameraClass*> Cameras;
	DirectionalLight* dirLight;
	float Background[4];

	static unsigned int ScreenWidth;
	static unsigned int ScreenHeight;

	unsigned int activePointlights;
	unsigned int activeSpotlights;

	int activeCamera;
	int glfwVersionMajor;
	int glfwVersionMinor;

	glm::mat4 view;
	glm::mat4 projection;
	glm::vec3 cameraPosition;

	bool bDirLightActive;
};

