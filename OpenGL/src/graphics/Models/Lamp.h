#pragma once

#include "Cube.h"
#include "../Light.h"

class Lamp : public Cube
{
public:
	glm::vec3 m_LightColor;

	PointLight pointLight;

	Lamp();
	Lamp(glm::vec3 LightColor, glm::vec4 Ambient, glm::vec4 Diffuse, glm::vec4 Specular, float k0, float k1, float k2, glm::vec3 Pos, glm::vec3 Size);
	~Lamp();

	virtual void Render(Shader& shader) override;
};

