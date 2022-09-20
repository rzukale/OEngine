#include "error.h"

#include <iostream>

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* Function, const char* File, int Line)
{
	while (GLenum Error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << Error << "): " << Function << " " << File << ":" << Line << std::endl;
		return false;
	}
	return true;
}
