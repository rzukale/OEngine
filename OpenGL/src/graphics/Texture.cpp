#include "Texture.h"

#include <iostream>

Texture::Texture() { }

Texture::Texture(const std::string& directory, const std::string& path, aiTextureType type)
	: m_Directory(directory), m_Path(path), m_TextureType(type)
{
	Generate();
}

Texture::~Texture()
{
}

void Texture::Generate()
{
	GLCall(glGenTextures(1, &m_ID));
}

void Texture::Load(bool bFlip)
{
	stbi_set_flip_vertically_on_load(bFlip);

	int Width, Height, NumberOfChannels;

	unsigned char* Data = stbi_load((m_Directory + "/" + m_Path).c_str(), &Width, &Height, &NumberOfChannels, 0);

	GLenum ColorMode = GL_RGB;

	switch (NumberOfChannels)
	{
		case 1:
			ColorMode = GL_RED;
			break;
		case 4:
			ColorMode = GL_RGBA;
			break;
	}
	if (Data)
	{
		GLCall(glBindTexture(GL_TEXTURE_2D, m_ID));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, ColorMode, Width, Height, 0, ColorMode, GL_UNSIGNED_BYTE, Data));
		GLCall(glGenerateMipmap(GL_TEXTURE_2D));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	}
	else
	{
		std::cout << "Image not loaded at " << m_Path << std::endl;
	}
	stbi_image_free(Data);
}

void Texture::Allocate(GLenum Format, GLuint Width, GLuint Height, GLenum Type)
{
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, Format, Width, Height, 0, Format, Type, NULL));
}

void Texture::SetParams(GLenum texMinFilter, GLenum texMagFilter, GLenum wrapS, GLenum wrapT)
{
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texMinFilter));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texMagFilter));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT));
}

void Texture::Bind()
{
	GLCall(glBindTexture(GL_TEXTURE_2D, m_ID));
}

void Texture::Cleanup()
{
	GLCall(glDeleteTextures(1, &m_ID));
}
