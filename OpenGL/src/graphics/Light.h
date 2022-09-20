#pragma once

#include <glm/glm.hpp>
#include "Shader.h"

struct PointLight
{
	glm::vec3 m_Position;

	// attenuation constants
	float m_k0;
	float m_k1;
	float m_k2;

	glm::vec4 m_Ambient;
	glm::vec4 m_Diffuse;
	glm::vec4 m_Specular;

	void Render(Shader& shader, int Index);
};

struct DirectionalLight
{
	glm::vec3 m_Direction;

	glm::vec4 m_Ambient;
	glm::vec4 m_Diffuse;
	glm::vec4 m_Specular;

	void Render(Shader& shader);
};

struct SpotLight
{
	glm::vec3 m_Position;
	glm::vec3 m_Direction;
	
	float m_CutOff;
	float m_OuterCutOff;

	// attenuation constants
	float m_k0;
	float m_k1;
	float m_k2;

	glm::vec4 m_Ambient;
	glm::vec4 m_Diffuse;
	glm::vec4 m_Specular;

	void Render(Shader& shader, int Index);

};

