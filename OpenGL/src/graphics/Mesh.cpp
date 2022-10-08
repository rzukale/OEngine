#include "Mesh.h"

std::vector<Vertex> Vertex::GenList(float* Vertices, int NumberOfVertices)
{
	std::vector<Vertex> Ret(NumberOfVertices);

	int Stride = sizeof(Vertex) / sizeof(float);

	for (int i = 0; i < NumberOfVertices; i++)
	{
		Ret[i].m_Position = glm::vec3
		(
			Vertices[i * Stride + 0],
			Vertices[i * Stride + 1],
			Vertices[i * Stride + 2]
		);

		Ret[i].m_Normal = glm::vec3
		(
			Vertices[i * Stride + 3],
			Vertices[i * Stride + 4],
			Vertices[i * Stride + 5]
		);

		Ret[i].m_TexCoord = glm::vec2
		(
			Vertices[i * Stride + 6],
			Vertices[i * Stride + 7]
		);
	}
	return Ret;
}

Mesh::Mesh(BoundingRegion Region, std::vector<Vertex> Vertices, std::vector<unsigned int> Indices, std::vector<Texture> Textures)
	: m_BoundingRegion(Region), m_Vertices(Vertices), m_Indices(Indices), m_Textures(Textures), m_bNoTextures(false)
{
	Setup();
}

Mesh::Mesh(BoundingRegion Region, std::vector<Vertex> Vertices, std::vector<unsigned int> Indices, aiColor4D diffuse, aiColor4D specular)
	: m_BoundingRegion(Region), m_Vertices(Vertices), m_Indices(Indices), m_Diffuse(diffuse), m_Specular(specular), m_bNoTextures(true)
{
	Setup();
}

void Mesh::Render(Shader& shader, glm::vec3 Position, glm::vec3 Size, Box* box, bool bDoRender)
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
	if (bDoRender)
	{
		box->AddInstance(m_BoundingRegion, Position, Size);

		m_VAO.Bind();
		m_VAO.Draw(GL_TRIANGLES, (GLuint)m_Indices.size(), GL_UNSIGNED_INT, 0);
		ArrayObject::Unbind();
	}
}

void Mesh::Setup()
{
	m_VAO.Generate();
	m_VAO.Bind();

	m_VAO["EBO"] = BufferObject(GL_ELEMENT_ARRAY_BUFFER);
	m_VAO["EBO"].Generate();
	m_VAO["EBO"].Bind();
	m_VAO["EBO"].SetData<GLuint>((GLuint)m_Indices.size(), &m_Indices[0], GL_STATIC_DRAW);

	m_VAO["VBO"] = BufferObject(GL_ARRAY_BUFFER);
	m_VAO["VBO"].Generate();
	m_VAO["VBO"].Bind();
	m_VAO["VBO"].SetData<Vertex>((GLuint)m_Vertices.size(), &m_Vertices[0], GL_STATIC_DRAW);

	m_VAO["VBO"].SetAttribPointer<GLfloat>(0, 3, GL_FLOAT, 8, 0);
	m_VAO["VBO"].SetAttribPointer<GLfloat>(1, 3, GL_FLOAT, 8, 3);
	m_VAO["VBO"].SetAttribPointer<GLfloat>(2, 2, GL_FLOAT, 8, 6);

	m_VAO["VBO"].Unbind();
	ArrayObject::Unbind();
}

void Mesh::Cleanup()
{
	m_VAO.Cleanup();
}