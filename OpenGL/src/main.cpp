#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <streambuf>
#include <vector>
#include <stack>

#include "stb/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "graphics/Shader.h"
#include "graphics/Texture.h"
#include "graphics/models/Lamp.h"
#include "graphics/Model.h"
#include "graphics/Light.h"
#include "graphics/models/Sphere.h"
#include "graphics/models/Box.hpp"

#include "physics/Global.h"

#include "io/Keyboard.h"
#include "io/CameraClass.h"
#include "Scene.h"

SphereArray LaunchObjects; // TODO: move out of globals

void	ProcessInput(float DeltaTime, Scene* scene);

int main()
{
	std::cout << "Hello World!" << std::endl;

	Scene scene(3, 3, "OpenGL", 1280, 720);
	if (scene.Init() == false)
	{
		return (EXIT_FAILURE);
	}
	CameraClass camera;
	scene.AddCamera(&camera);

	// shaders
	Shader shader("assets/object.vs", "assets/object.fs");
	Shader lampShader("assets/instanced/instanced.vs", "assets/lamp.fs");
	Shader launchShader("assets/instanced/instanced.vs", "assets/object.fs");
	Shader boxShader("assets/instanced/box.vs", "assets/instanced/box.fs");

	glm::vec3 pointLightPositions[] =
	{
			glm::vec3(0.7f,  0.2f,  2.0f),
			glm::vec3(2.3f, -3.3f, -4.0f),
			glm::vec3(-4.0f,  2.0f, -12.0f),
			glm::vec3(0.0f,  0.0f, -3.0f)
	};

	glm::vec4 ambient = glm::vec4(0.05f, 0.05f, 0.05f, 1.0f);
	glm::vec4 diffuse = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
	glm::vec4 specular = glm::vec4(1.0f);

	float k0 = 1.0f;
	float k1 = 0.07f;
	float k2 = 0.032f;

	PointLight pointlights[4];
	LampArray Lamps;
	Lamps.Init();
	for (unsigned int i = 0; i < 4; i++)
	{
		pointlights[i] = {
			pointLightPositions[i],
			k0, k1, k2,
			ambient, diffuse, specular
		};
		Lamps.m_LightInstances.push_back(pointlights[i]);
		scene.AddPointlight(&pointlights[i]);
		scene.SetActivePointlightBit(i);
	}
	DirectionalLight dirLight = { glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec4(0.1f, 0.1f, 0.1f, 1.0f), glm::vec4(0.4f, 0.4f, 0.4f, 1.0f), glm::vec4(0.75f, 0.75f, 0.75f, 1.0f) };
	scene.SetDirectionalLight(&dirLight);
	SpotLight spotLight = { scene.GetActiveCamera()->m_CameraPosition, scene.GetActiveCamera()->m_CameraFront,
		glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(20.0f)),
		1.0f, 0.07f, 0.032f,
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) };

	scene.AddSpotlight(&spotLight);
	scene.SetActiveSpotlight();

	Model model(BoundTypes::AABB, glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.05f), true);
	model.LoadModel("assets/models/lotr_troll/scene.gltf");
	Box box;
	LaunchObjects.Init();
	box.Init();

	float LastFrame = 0.0f;
	while (!scene.ShouldClose())
	{
		box.m_Positions.clear();
		box.m_Sizes.clear();

		float CurrentTime = (float)glfwGetTime();
		float DeltaTime = CurrentTime - LastFrame;
		LastFrame = CurrentTime;
		ProcessInput(DeltaTime, &scene);

		// render
		scene.Update();

		scene.Render(shader);
		model.Render(shader, DeltaTime, &box);


		// launchobjects
		std::stack<int> RemovableObjects;
		for (int i = 0; i < (int)LaunchObjects.m_Instances.size(); i++)
		{
			if (glm::length(scene.GetActiveCamera()->m_CameraPosition - LaunchObjects.m_Instances[i].m_Position) > 250.0f)
			{
				RemovableObjects.push(i);
				continue;
			}
		}
		for (int i = 0; i < (int)RemovableObjects.size(); i++)
		{
			LaunchObjects.m_Instances.erase(LaunchObjects.m_Instances.begin() + RemovableObjects.top());
			RemovableObjects.pop();
		}
		if (LaunchObjects.m_Instances.empty() == false)
		{
			scene.Render(launchShader);
			LaunchObjects.Render(launchShader, DeltaTime, &box);
		}

		scene.Render(lampShader, false);
		Lamps.Render(lampShader, DeltaTime, &box);

		// Boundingboxes
		if (box.m_Positions.empty() == false)
		{
			scene.Render(boxShader, false);
			box.Render(boxShader);
		}
		scene.NewFrame();
	}

	LaunchObjects.Cleanup();
	Lamps.Cleanup();
	box.Cleanup();
	scene.Cleanup();
	return EXIT_SUCCESS;
}

void LaunchItem(float DeltaTime, const CameraClass* Camera)
{
	if (Camera != nullptr)
	{
		RigidBody NewBody(1.0f, Camera->m_CameraPosition);
		NewBody.TransferEnergy(100.0f, Camera->m_CameraFront);
		NewBody.ApplyAcceleration(Global::EnviromentalAcceleration);
		LaunchObjects.m_Instances.push_back(NewBody);
	}
}

void	ProcessInput(float DeltaTime, Scene* scene)
{
	scene->ProcessInput(DeltaTime);

	if (Keyboard::KeyWentDown(GLFW_KEY_F))
	{
		LaunchItem(DeltaTime, scene->GetActiveCamera());
	}
}
