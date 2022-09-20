#include "Mesh.h"

std::vector<Vertex> Vertex::GenList(float* Vertices, int NumberOfVertices)
{
	std::vector<Vertex> Ret(NumberOfVertices);

	int Stride = sizeof(Vertex) / sizeof(float);

	for (int i = 0; i < NumberOfVertices; i++)
	{
		Ret[i].Position = glm::vec3
		(
			Vertices[i * Stride + 0],
			Vertices[i * Stride + 1],
			Vertices[i * Stride + 2]
		);

		Ret[i].Normal = glm::vec3
		(
			Vertices[i * Stride + 3],
			Vertices[i * Stride + 4],
			Vertices[i * Stride + 5]
		);

		Ret[i].TexCoord = glm::vec2
		(
			Vertices[i * Stride + 6],
			Vertices[i * Stride + 7]
		);
	}
	return Ret;
}

Mesh::Mesh()
{
}

Mesh::Mesh(std::vector<Vertex> Vertices, std::vector<unsigned int> Indices, std::vector<Texture> Textures)
	: m_Vertices(Vertices), m_Indices(Indices), m_Textures(Textures), m_bNoTextures(false)
{
	Setup();
}

Mesh::Mesh(std::vector<Vertex> Vertices, std::vector<unsigned int> Indices, aiColor4D diffuse, aiColor4D specular)
	: m_Vertices(Vertices), m_Indices(Indices), m_Diffuse(diffuse), m_Specular(specular), m_bNoTextures(true)
{
	Setup();
}

void Mesh::Render(Shader& shader)
{
	if (m_bNoTextures)
	{
		shader.Set4Float("material.diffuse", m_Diffuse);
		shader.Set4Float("material.specular", m_Specular);
		shader.SetInt("NoTex", 1);
	}
	else
	{
		// textures
		unsigned int diffuseIndex = 0;
		unsigned int specularIndex = 0;

		for (unsigned int i = 0; i < m_Textures.size(); i++)
		{
			GLCall(glActiveTexture(GL_TEXTURE0 + i));
			std::string name;
			switch (m_Textures[i].m_TextureType)
			{
			case aiTextureType_DIFFUSE:
				name = "diffuse" + std::to_string(diffuseIndex++);
				break;
			case aiTextureType_SPECULAR:
				name = "specular" + std::to_string(specularIndex++);
				break;
			}

			shader.SetInt(name, i);
			m_Textures[i].Bind();
		}
		shader.SetInt("NoTex", 0);
	}
	GLCall(glBindVertexArray(m_VAO));
	GLCall(glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0));
	GLCall(glBindVertexArray(0));
	GLCall(glActiveTexture(GL_TEXTURE0));
}

void Mesh::Cleanup()
{
	GLCall(glDeleteVertexArrays(1, &m_VAO));
	GLCall(glDeleteBuffers(1, &m_VBO));
	GLCall(glDeleteBuffers(1, &m_EBO));
}

void Mesh::Setup()
{
	GLCall(glGenVertexArrays(1, &m_VAO));
	GLCall(glGenBuffers(1, &m_VBO));
	GLCall(glGenBuffers(1, &m_EBO));

	GLCall(glBindVertexArray(m_VAO));

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
	GLCall(glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), &m_Vertices[0], GL_STATIC_DRAW));

	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), &m_Indices[0], GL_STATIC_DRAW));

	// Setting Vertex Attribute pointers
	// Vertex Position
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position)));

	GLCall(glEnableVertexAttribArray(1));
	GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal)));

	// Vertex TexCoords
	GLCall(glEnableVertexAttribArray(2));
	GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoord)));

	GLCall(glBindVertexArray(0));
}
