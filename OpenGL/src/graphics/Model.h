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
#include "../physics/RigidBody.h"

class Model
{
public:
	RigidBody m_RigidBody;
	glm::vec3 m_Size;
	std::vector<Mesh> m_Meshes;

	Model(glm::vec3 Position = glm::vec3(0.0f), glm::vec3 Size = glm::vec3(1.0f), bool bHasTextures = true);

	virtual void Init();
	void LoadModel(const std::string& filePath);

	void Render(Shader& shader, float DeltaTime, bool bSetModel = true, bool bDoRender = true);
	void Cleanup();
protected:
	bool m_bHasTextures;
	std::string m_Directory;
	std::vector<Texture> m_TexturesLoaded;

	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> LoadTextures(aiMaterial* material, aiTextureType type);
};

