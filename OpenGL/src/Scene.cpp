#include "Scene.h"

unsigned int Scene::ScreenWidth = 0;
unsigned int Scene::ScreenHeight = 0;

Scene::Scene() : Window(nullptr), title(nullptr), dirLight(nullptr), Background{0.0f, 0.0f, 0.0f, 0.0f},
                 activePointlights(0),
                 activeSpotlights(0), activeCamera(-1), glfwVersionMajor(0), glfwVersionMinor(0),
                 view(glm::mat4(1.0f)), projection(glm::mat4(1.0f)), cameraPosition(glm::vec3(0.0f)), bDirLightActive(false) {}

Scene::Scene(int glfwVersionMajor, int glfwVersionMinor, const char* title, unsigned int Width, unsigned int Height)
	: Window(nullptr), title(title), dirLight(nullptr), Background{0.0f, 0.0f, 0.0f, 0.0f}, activePointlights(0),
	activeSpotlights(0), activeCamera(-1), glfwVersionMajor(glfwVersionMajor), glfwVersionMinor(glfwVersionMinor),
	view(glm::mat4(1.0f)), projection(glm::mat4(1.0f)), cameraPosition(glm::vec3(0.0f)), bDirLightActive(false)
{
	Scene::ScreenWidth = Width;
	Scene::ScreenHeight = Height;

	SetWindowColor(0.1f, 0.15f, 0.15f, 1.0f);
}

bool Scene::Init()
{
	if (!glfwInit())
		return false;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glfwVersionMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glfwVersionMinor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COPMPAT, GL_TRUE);
#endif // __APPLE__

	Window = glfwCreateWindow(ScreenWidth, ScreenHeight, title, NULL, NULL);
	if (Window == nullptr)
	{
		return false;
	}

	glfwMakeContextCurrent(Window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		Cleanup();
		return false;
	}

	std::cout << glGetString(GL_VERSION) << std::endl;

	GLCall(glViewport(0, 0, ScreenWidth, ScreenHeight));

	glfwSetFramebufferSizeCallback(Window, Scene::FrameBufferSizeCallback);
	glfwSetKeyCallback(Window, Keyboard::KeyCallback);
	glfwSetMouseButtonCallback(Window, Mouse::MouseButtonCallback);
	glfwSetCursorPosCallback(Window, Mouse::CursorPositionCallback);
	glfwSetScrollCallback(Window, Mouse::MouseWheelCallback);
	glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	GLCall(glEnable(GL_DEPTH_TEST));

	return true;
}

void Scene::FrameBufferSizeCallback(GLFWwindow* Window, int Width, int Height)
{
	GLCall(glViewport(0, 0, Width, Height));

	Scene::ScreenWidth = Width;
	Scene::ScreenHeight = Height;
}

void Scene::ProcessInput(float DeltaTime)
{
	if (activeCamera >= 0 && activeCamera < Cameras.size())
	{
		if (Keyboard::Key(GLFW_KEY_W))
		{
			Cameras[activeCamera]->UpdateCameraPosition(CameraDirection::FORWARD, DeltaTime);
		}
		if (Keyboard::Key(GLFW_KEY_S))
		{
			Cameras[activeCamera]->UpdateCameraPosition(CameraDirection::BACKWARD, DeltaTime);
		}
		if (Keyboard::Key(GLFW_KEY_D))
		{
			Cameras[activeCamera]->UpdateCameraPosition(CameraDirection::RIGHT, DeltaTime);
		}
		if (Keyboard::Key(GLFW_KEY_A))
		{
			Cameras[activeCamera]->UpdateCameraPosition(CameraDirection::LEFT, DeltaTime);
		}
		if (Keyboard::Key(GLFW_KEY_SPACE))
		{
			Cameras[activeCamera]->UpdateCameraPosition(CameraDirection::UP, DeltaTime);
		}
		if (Keyboard::Key(GLFW_KEY_LEFT_SHIFT))
		{
			Cameras[activeCamera]->UpdateCameraPosition(CameraDirection::DOWN, DeltaTime);
		}

		float dx = (float)Mouse::GetDeltaX();
		float dy = (float)Mouse::GetDeltaY();
		//if (dx == 0.0f || dy != 0.0f)
		//{
			Cameras[activeCamera]->UpdateCameraDirection(dx, dy);
		//}
		float scrolldy = (float)Mouse::GetScrollDeltaY();
		if (scrolldy != 0.0f)
		{
			Cameras[activeCamera]->UpdateCameraZoom(scrolldy);
		}

		view = Cameras[activeCamera]->GetViewMatrix();
		cameraPosition = Cameras[activeCamera]->m_CameraPosition;
		projection = glm::perspective(glm::radians(Cameras[activeCamera]->GetZoom()), (float)ScreenWidth / (float)ScreenHeight, 0.1f, 100.0f);
	}
	if (Keyboard::Key(GLFW_KEY_ESCAPE))
	{
		SetShouldClose(true);
	}

}

void Scene::Update()
{
	GLCall(glClearColor(Background[0], Background[1], Background[2], Background[3]));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Scene::NewFrame()
{
	GLCall(glfwSwapBuffers(Window));
	GLCall(glfwPollEvents());
}

void Scene::Render(Shader& shader, bool bApplyLighting)
{
	// activate shader
	shader.Bind();

	shader.SetMat4("view", view);
	shader.SetMat4("projection", projection);
	shader.Set3Float("ViewPos", cameraPosition);

	if (bApplyLighting)
	{
		// point lights
		unsigned int nbrlights = pointlights.size();
		unsigned int nbractivelights = 0;
		for (unsigned int i = 0; i < nbrlights; i++)
		{
			if (States::IsActive(&activePointlights, i))
			{
				// i'th light is active
				pointlights[i]->Render(shader, nbractivelights);
				nbractivelights += 1;
			}
		}
		shader.SetInt("NumberOfPointlights", nbractivelights);

		nbrlights = spotlights.size();
		nbractivelights = 0;
		for (unsigned int i = 0; i < nbrlights; i++)
		{
			if (States::IsActive(&activeSpotlights, i))
			{
				spotlights[i]->Render(shader, nbractivelights);
				nbractivelights += 1;
			}
		}
		shader.SetInt("NumberOfSpotlights", nbractivelights);

		dirLight->Render(shader);
	}
}

void Scene::Cleanup()
{
	glfwTerminate();
}

bool Scene::ShouldClose()
{
	return glfwWindowShouldClose(Window);
}

CameraClass* Scene::GetActiveCamera()
{
	return (activeCamera >= 0 && activeCamera < (int)Cameras.size()) ? Cameras[activeCamera] : nullptr;
}

void Scene::SetShouldClose(bool bShouldClose)
{
	glfwSetWindowShouldClose(Window, bShouldClose);
}

void Scene::SetWindowColor(float r, float g, float b, float a)
{
	Background[0] = r;
	Background[1] = g;
	Background[2] = b;
	Background[3] = a;
}

void Scene::AddPointlight(PointLight* newLight)
{
	pointlights.push_back(newLight);
}

void Scene::AddSpotlight(SpotLight* newLight)
{
	spotlights.push_back(newLight);
}

void Scene::SetActivePointlightBit(int target)
{
	States::Activate(&activePointlights, target);
}

void Scene::SetActiveSpotlight()
{
	activeSpotlights = 1;
}

void Scene::SetDirectionalLight(DirectionalLight* DirLight)
{
	dirLight = DirLight;
}

void Scene::AddCamera(CameraClass* NewCamera)
{
	Cameras.push_back(NewCamera);
	activeCamera += 1;
}
