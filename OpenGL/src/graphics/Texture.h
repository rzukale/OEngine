#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <assimp/scene.h>
#include <stb/stb_image.h>
#include "../error.h"

class Texture
{
public:
	Texture();
	Texture(const std::string& directory, const std::string& path, aiTextureType type);
	~Texture();

	void Generate();
	void Load(bool bFlip = true);

	void Allocate(GLenum format, GLuint width, GLuint height, GLenum type);

	static void SetParams(GLenum texMinFilter = GL_NEAREST,
		GLenum texMagFilter = GL_NEAREST,
		GLenum wrapS = GL_REPEAT,
		GLenum wrapT = GL_REPEAT);

	void Bind();
	void Cleanup();

	unsigned int m_ID;
	aiTextureType m_TextureType;
	std::string m_Path;
private:
	std::string m_Directory;
	
};

