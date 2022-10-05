#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <glm/glm.hpp>

#include "Shader.h"
#include "Texture.h"
#include "../error.h"

struct Vertex
{
	glm::vec3 m_Position;
	glm::vec3 m_Normal;
	glm::vec2 m_TexCoord;

	static std::vector<Vertex> GenList(float* Vertices, int NumberOfVertices);
};
typedef struct Vertex Vertex;

class Mesh
{
public:
	std::vector<Vertex> m_Vertices;
	std::vector<unsigned int> m_Indices;
	std::vector<Texture> m_Textures;
	aiColor4D m_Diffuse;
	aiColor4D m_Specular;

	Mesh();
	Mesh(std::vector<Vertex> Vertices, std::vector<unsigned int> Indices, std::vector<Texture> Textures = {});
	Mesh(std::vector<Vertex> Vertices, std::vector<unsigned int> Indices, aiColor4D diffuse, aiColor4D specular);

	void Render(Shader& shader, bool bDoRender = true);
	void Cleanup();

	unsigned int m_VAO;
	unsigned int m_VBO, m_EBO;

private:
	bool m_bNoTextures;
	void Setup();
};

