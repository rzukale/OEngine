#include "Shader.h"

Shader::Shader()
	: m_RendererID(0)
{
}

Shader::Shader(const char* VertexShaderPath, const char* FragmentShaderPath)
{
	m_RendererID = Generate(VertexShaderPath, FragmentShaderPath);
}

unsigned int Shader::Generate(const char* VertexShaderPath, const char* FragmentShaderPath)
{
	int	Success;
	char InfoLog[512];

	GLCall(unsigned int program = glCreateProgram());
	unsigned int VertexShader = CompileShader(VertexShaderPath, GL_VERTEX_SHADER);
	unsigned int FragmentShader = CompileShader(FragmentShaderPath, GL_FRAGMENT_SHADER);

	GLCall(glAttachShader(program, VertexShader));
	GLCall(glAttachShader(program, FragmentShader));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	GLCall(glGetProgramiv(program, GL_LINK_STATUS, &Success));
	if (Success == GL_FALSE)
	{
		GLCall(glGetProgramInfoLog(program, 512, NULL, InfoLog));
		std::cout << "Linking error:" << std::endl << InfoLog << std::endl;
	}

	//GLCall(glDetachShader(m_RendererID, VertexShader));
	//GLCall(glDetachShader(m_RendererID, FragmentShader));

	GLCall(glDeleteShader(VertexShader));
	GLCall(glDeleteShader(FragmentShader));

	return program;
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind()
{
	GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind()
{
	GLCall(glUseProgram(0));
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

unsigned int Shader::CompileShader(const char* Filename, GLenum Type)
{
	int Success;

	GLCall(unsigned int id = glCreateShader(Type));
	std::string ShaderSource = LoadShaderSource(Filename);
	const char* Shader = ShaderSource.c_str();
	GLCall(glShaderSource(id, 1, &Shader, nullptr));
	GLCall(glCompileShader(id));

	// catch error
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &Success));
	if (Success == GL_FALSE)
	{
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)alloca(length * sizeof(char));
		GLCall(glGetShaderInfoLog(id, length, &length, message));
		std::cout << "Failed to compile " << (Type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;
		GLCall(glDeleteShader(id));
		return 0;
	}
	return id;
}

void Shader::SetMat4(const std::string& Name, const glm::mat4& Matrix)
{
	//glUniformMatrix4fv(glGetUniformLocation(m_RendererID, Name.c_str()), 1, GL_FALSE, glm::value_ptr(Matrix));
	GLCall(glUniformMatrix4fv(GetUniformLocation(Name), 1, GL_FALSE, glm::value_ptr(Matrix)));
}

void Shader::SetInt(const std::string& Name, int Value)
{
	//glUniform1i(glGetUniformLocation(m_RendererID, Name.c_str()), Value);
	GLCall(glUniform1i(GetUniformLocation(Name), Value));
}

void Shader::SetFloat(const std::string& Name, float Value)
{
	//glUniform1f(glGetUniformLocation(m_RendererID, Name.c_str()), Value);
	GLCall(glUniform1f(GetUniformLocation(Name), Value));
}

void Shader::Set3Float(const std::string& Name, const glm::vec3& Value)
{
	Set3Float(Name, Value.x, Value.y, Value.z);
	//GLCall(glUniform3f(GetUniformLocation(Name), Val.x, Val.y, Val.z));
}

void Shader::Set3Float(const std::string& Name, float Val1, float Val2, float Val3)
{
	//glUniform3f(glGetUniformLocation(m_RendererID, Name.c_str()), Val1, Val2, Val3);
	GLCall(glUniform3f(GetUniformLocation(Name), Val1, Val2, Val3));
}

void Shader::Set4Float(const std::string& Name, const aiColor4D& color)
{
	GLCall(glUniform4f(GetUniformLocation(Name), color.r, color.g, color.b, color.a));
}

void Shader::Set4Float(const std::string& Name, const glm::vec4& Val)
{
	GLCall(glUniform4f(GetUniformLocation(Name), Val.x, Val.y, Val.z, Val.w));
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
	{
		return m_UniformLocationCache[name];
	}
	GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	if (location == -1)
	{
		std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
	}
	m_UniformLocationCache[name] = location;
	return location;
}
