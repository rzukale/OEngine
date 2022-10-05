#include "Model.h"
#include "../physics/Global.h"

Model::Model(glm::vec3 Position, glm::vec3 Size, bool bHasTextures)
	: m_Size(Size), m_bHasTextures(bHasTextures)
{
	m_RigidBody.m_Position = Position;
}

void Model::Init() { }

void Model::Render(Shader& shader, float DeltaTime, bool bSetModel, bool bDoRender)
{
	m_RigidBody.Update(DeltaTime);
	if (bSetModel)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, m_RigidBody.m_Position);
		model = glm::scale(model, m_Size);
		shader.SetMat4("model", model);
	}
	shader.SetFloat("material.shininess", 0.5f);

	for (Mesh& mesh : m_Meshes)
	{
		mesh.Render(shader, bDoRender);
	}
}

void Model::Cleanup()
{
	for (Mesh& mesh : m_Meshes)
	{
		mesh.Cleanup();
	}
}

void Model::LoadModel(const std::string& filePath)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "Failed to load model at " << filePath << std::endl << importer.GetErrorString() << std::endl;
		return;
	}
	m_Directory = filePath.substr(0, filePath.find_last_of("/"));
	ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	// process meshes
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_Meshes.push_back(ProcessMesh(mesh, scene));
	}

	// pre-order process child nodes
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	// vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		// position
		vertex.m_Position = glm::vec3
		(
			mesh->mVertices[i].x,
			mesh->mVertices[i].y,
			mesh->mVertices[i].z
		);

		// normal
		vertex.m_Normal = glm::vec3
		(
			mesh->mNormals[i].x,
			mesh->mNormals[i].y,
			mesh->mNormals[i].z
		);

		// TexCoord
		if (mesh->mTextureCoords[0])
		{
			vertex.m_TexCoord = glm::vec2
			(
				mesh->mTextureCoords[0][i].x,
				mesh->mTextureCoords[0][i].y
			);
		}
		else
		{
			vertex.m_TexCoord = glm::vec2(0.0f);
		}
		vertices.push_back(vertex);
	}

	// process indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	// process material
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		if (m_bHasTextures == false)
		{
			// diffuse color
			aiColor4D diff(1.0f);
			aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diff);

			// specular color
			aiColor4D spec(1.0f);
			aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &spec);

			return Mesh(vertices, indices, diff, spec);
		}

		// diffuse maps
		std::vector<Texture> diffuseMaps = LoadTextures(material, aiTextureType_DIFFUSE);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		// specular maps
		std::vector<Texture> specularMaps = LoadTextures(material, aiTextureType_SPECULAR);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}
	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::LoadTextures(aiMaterial* material, aiTextureType type)
{
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
	{
		aiString string;
		material->GetTexture(type, i, &string);
		std::cout << string.C_Str() << std::endl;

		// prevent duplicates
		bool bSkip = false;
		for (unsigned int j = 0; j < m_TexturesLoaded.size(); j++)
		{
			if (std::strcmp(m_TexturesLoaded[j].m_Path.data(), string.C_Str()) == 0)
			{
				bSkip = true;
				textures.push_back(m_TexturesLoaded[j]);
				break;
			}
		}
		if (!bSkip)
		{
			Texture texture(m_Directory, string.C_Str(), type);
			texture.Load(false);
			textures.push_back(texture);
			m_TexturesLoaded.push_back(texture);
		}
	}
	return textures;
}
