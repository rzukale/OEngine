#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

class Model
{
public:
	glm::vec3 m_Position;
	glm::vec3 m_Size;

	Model(glm::vec3 Position = glm::vec3(0.0f), glm::vec3 Size = glm::vec3(1.0f), bool bHasTextures = true);

	virtual void Init();
	void LoadModel(const std::string& filePath);

	virtual void Render(Shader& shader);
	void Cleanup();
protected:
	bool m_bHasTextures;
	std::vector<Mesh> m_Meshes;
	std::string m_Directory;

	std::vector<Texture> m_TexturesLoaded;

	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> LoadTextures(aiMaterial* material, aiTextureType type);
};

