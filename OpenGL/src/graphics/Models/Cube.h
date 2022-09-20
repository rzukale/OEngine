#pragma once

#include "../Model.h"
#include "../Material.h"

class Cube : public Model
{
public:
	glm::vec3 m_Position;
	glm::vec3 m_Size;

	Material m_Material;

	Cube(glm::vec3 Position = glm::vec3(0.0f), glm::vec3 Size = glm::vec3(1.0f));
	~Cube();

	virtual void Init() override;
};

