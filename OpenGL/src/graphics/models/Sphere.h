#pragma once

#include "ModelArray.hpp"
#include "../Model.h"

class Sphere : public Model
{
public:

	Sphere(glm::vec3 Position = glm::vec3(0.0f), glm::vec3 Size = glm::vec3(0.025f)) : Model(BoundTypes::SPHERE, Position, Size, false) {}

	void Init();
};

class SphereArray : public ModelArray<Sphere>
{
public:

	void Init()
	{
		m_Model = Sphere(glm::vec3(0.0f), glm::vec3(0.025f));
		ModelArray::Init();
	}
};
