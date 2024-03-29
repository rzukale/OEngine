#include "Lamp.h"
#include <iostream>

Lamp::Lamp()
{
}

Lamp::Lamp(glm::vec3 LightColor, glm::vec4 Ambient, glm::vec4 Diffuse, glm::vec4 Specular, float k0, float k1, float k2, glm::vec3 Pos, glm::vec3 Size)
	: m_LightColor(LightColor), m_PointLight({ Pos, k0, k1, k2, Ambient, Diffuse, Specular }), Cube(Pos, Size)
{
}

Lamp::~Lamp()
{
}

void Lamp::Render(Shader& shader, float DeltaTime, Box* box, bool bSetModel, bool bDoRender)
{
	shader.Set3Float("LightColor", m_LightColor);

	Cube::Render(shader, DeltaTime, box, bSetModel, bDoRender);
}
