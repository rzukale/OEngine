#pragma once

#include <glm/glm.hpp>

class Material
{
public:
	glm::vec3 m_Ambient;
	glm::vec3 m_Diffuse;
	glm::vec3 m_Specular;
	float m_Shininess;

    static Material Emerald;
    static Material Jade;
    static Material Obsidian;
    static Material Pearl;
    static Material Ruby;
    static Material Turquoise;
    static Material Brass;
    static Material Bronze;
    static Material Chrome;
    static Material Copper;
    static Material Gold;
    static Material Silver;
    static Material Black_plastic;
    static Material Cyan_plastic;
    static Material Green_plastic;
    static Material Red_plastic;
    static Material White_plastic;
    static Material Yellow_plastic;
    static Material Black_rubber;
    static Material Cyan_rubber;
    static Material Green_rubber;
    static Material Red_rubber;
    static Material White_rubber;
    static Material Yellow_rubber;

    static Material Mix(const Material& material1, const Material& material2, float Mix = 0.5f);
};

