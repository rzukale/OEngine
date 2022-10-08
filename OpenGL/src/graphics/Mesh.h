#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <glm/glm.hpp>

#include "Shader.h"
#include "Texture.h"
#include "../error.h"
#include "../algorithms/Bounds.h"
#include "models/Box.hpp"
#include "glmemory.hpp"


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

	BoundingRegion m_BoundingRegion;
	std::vector<Vertex> m_Vertices;
	std::vector<unsigned int> m_Indices;
	std::vector<Texture> m_Textures;
	aiColor4D m_Diffuse;
	aiColor4D m_Specular;

	Mesh(BoundingRegion Region, std::vector<Vertex> Vertices, std::vector<unsigned int> Indices, std::vector<Texture> Textures = {});
	Mesh(BoundingRegion Region, std::vector<Vertex> Vertices, std::vector<unsigned int> Indices, aiColor4D diffuse, aiColor4D specular);

	void Render(Shader& shader, glm::vec3 Position, glm::vec3 Size, Box* box, bool bDoRender = true);
	void Cleanup();

	ArrayObject m_VAO;
	unsigned int m_VBO, m_EBO;

private:
	bool m_bNoTextures;
	void Setup();
};

