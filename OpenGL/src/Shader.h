#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
private:
	unsigned int m_RendererID;
public:
	Shader(const char* VertexShaderPath, const char* FragmentShaderPath);
	~Shader();
	void Activate();

	// utility functions
	std::string	LoadShaderSource(const char* Filename);
	GLuint CompileShader(const char* Filename, GLenum Type);

	// uniform functions
	void SetMat4(const std::string& Name, const glm::mat4& Matrix);
	void SetInt(const std::string& Name, int Value);
	void SetFloat(const std::string& Name, float Value);
};