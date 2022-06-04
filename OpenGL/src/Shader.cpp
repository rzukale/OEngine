#include "Shader.h"

Shader::Shader(const char* VertexShaderPath, const char* FragmentShaderPath)
	: m_RendererID(0)
{
	int	Success;
	char InfoLog[512];

	GLuint VertexShader = CompileShader(VertexShaderPath, GL_VERTEX_SHADER);
	GLuint FragmentShader = CompileShader(FragmentShaderPath, GL_FRAGMENT_SHADER);

	m_RendererID = glCreateProgram();
	glAttachShader(m_RendererID, VertexShader);
	glAttachShader(m_RendererID, FragmentShader);
	glLinkProgram(m_RendererID);

	glGetProgramiv(m_RendererID, GL_LINK_STATUS, &Success);
	if (!Success)
	{
		glGetProgramInfoLog(m_RendererID, 512, NULL, InfoLog);
		std::cout << "Linking error:" << std::endl << InfoLog << std::endl;
	}
	glDeleteShader(VertexShader);
	glDeleteShader(FragmentShader);
}

Shader::~Shader()
{
	glDeleteProgram(m_RendererID);
}

void Shader::Activate()
{
	glUseProgram(m_RendererID);
}

std::string Shader::LoadShaderSource(const char* Filename)
{
	std::ifstream		File;
	std::stringstream	Buf;
	std::string			Ret = "";

	File.open(Filename);
	if (File.is_open())
	{
		Buf << File.rdbuf();
		Ret = Buf.str();
	}
	else
	{
		std::cout << "Failed to open " << Filename << std::endl;
	}
	File.close();
	return Ret;
}

GLuint Shader::CompileShader(const char* Filename, GLenum type)
{
	int Success;
	char InfoLog[512];
	GLuint ShaderID;

	ShaderID = glCreateShader(type);
	std::string ShaderSource = LoadShaderSource(Filename);
	const GLchar* Shader = ShaderSource.c_str();
	glShaderSource(ShaderID, 1, &Shader, NULL);
	glCompileShader(ShaderID);

	// catch error
	glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &Success);
	if (!Success)
	{
		glGetShaderInfoLog(ShaderID, 512, NULL, InfoLog);
		std::cout << "Error with shader compilation: " << std::endl << InfoLog << std::endl;
	}
	return ShaderID;
}

void Shader::SetMat4(const std::string& Name, const glm::mat4& Matrix)
{
	glUniformMatrix4fv(glGetUniformLocation(m_RendererID, Name.c_str()), 1, GL_FALSE, glm::value_ptr(Matrix));
}
