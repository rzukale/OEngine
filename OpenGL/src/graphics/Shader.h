#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/scene.h>

#include "../error.h"

class Shader
{
public:
	Shader();
	Shader(const char* VertexShaderPath, const char* FragmentShaderPath);
	~Shader();
	
	unsigned int Generate(const char* VertexShaderPath, const char* FragmentShaderPath);
	void Bind();
	void Unbind();

	// utility functions
	std::string	LoadShaderSource(const char* Filename);
	unsigned int CompileShader(const char* Filename, GLenum Type);

	// uniform functions
	void SetMat4(const std::string& Name, const glm::mat4& Matrix);
	void SetInt(const std::string& Name, int Value);
	void SetFloat(const std::string& Name, float Value);
	void Set3Float(const std::string& Name, const glm::vec3& Value);
	void Set3Float(const std::string& Name, float Val1, float Val2, float Val3);
	void Set4Float(const std::string& Name, const aiColor4D& color);
	void Set4Float(const std::string& Name, const glm::vec4& Val);

private:
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache;

	int GetUniformLocation(const std::string& name);
};