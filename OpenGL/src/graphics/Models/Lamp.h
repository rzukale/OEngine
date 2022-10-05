#pragma once

#include "Cube.h"
#include "../Light.h"
#include "ModelArray.hpp"

class Lamp : public Cube
{
public:
	glm::vec3 m_LightColor;

	PointLight m_PointLight;
	
	Lamp();
	Lamp(glm::vec3 LightColor, glm::vec4 Ambient, glm::vec4 Diffuse, glm::vec4 Specular, float k0, float k1, float k2, glm::vec3 Pos, glm::vec3 Size);
	~Lamp();

	void Render(Shader& shader, float DeltaTime, bool bSetModel = true, bool bDoRender = true);
};

class LampArray : public ModelArray<Lamp>
{
public:

	std::vector<PointLight> m_LightInstances;

	void Init()
	{
		m_Model = Lamp(glm::vec3(1.0f),
			glm::vec4(0.05f, 0.05f, 0.05f, 1.0f), glm::vec4(0.8f, 0.8f, 0.8f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
			1.0f, 0.07f, 0.032f,
			glm::vec3(0.0f), glm::vec3(0.25f));
		ModelArray::Init();
	}

	void Render(Shader& shader, float DeltaTime)
	{
		m_Positions.clear();
		m_Sizes.clear();

		for (PointLight& pointlight : m_LightInstances)
		{
			m_Positions.push_back(pointlight.m_Position);
			m_Sizes.push_back(m_Model.m_Size);
		}
		ModelArray::Render(shader, DeltaTime, false);
	}
};